<?php

function tol_resumen_de_tu_cuenta_page() {
  
  //Valida que el perfil en tigo este completo de lo contrario lo redirecciona a online/resumen/complete-profile
  global $user;
  
  $userinfo = user_load($user->uid, true);
  
  $mainmsisdn = !empty($userinfo->field_msisdn['und'][0]['value'])?$userinfo->field_msisdn['und'][0]['value']:"";
  $mainmail = $userinfo->mail;
  $mainnro_documento = !empty($userinfo->field_nro_documento['und'][0]['value'])?$userinfo->field_nro_documento['und'][0]['value']:"";
  $maintipo_documento = !empty($userinfo->field_tipo_documento['und'][0]['tid'])?$userinfo->field_tipo_documento['und'][0]['tid']:"";
  	
  if(empty($mainmsisdn) || empty($mainmail) || (empty($mainnro_documento) && variable_get('tol_flag_required_field_tipo_documento')) || (empty($maintipo_documento) && variable_get('tol_flag_required_field_nro_documento')))
  { 
  	drupal_goto('online/resumen/complete-profile');
  }
  
  //manejo de cambio de linea seleccionada
	if (isset($_REQUEST["numcp"])){
  	$_SESSION['my_line'] = $_REQUEST["numcp"]; ////cambia de linea seleccionada

		//reseteo variables de sesion
		$_SESSION['nrocuenta'] = '';
		$_SESSION['estado_plan'] = '';
		
	  tol_clear_cache(); //se limpia cache cuando se cambia de linea

  }
  $setVar = 'resumen-de-tu-cuenta';
  $updateDate = _tol_generic_format_fechaDDMMYYYY(date ("Y-m-d"));
  $updateTime = date ("H:i");


/**** Inicio Saldo ****/
	$saldos = _tol_obtener_saldo2(); //llamada a metodo que contiene la llamada al servicio
    $cuenta = array(
    'mensaje_status' => "Su factura se encuentra vencida. Click aqui para pagar.",
    'saldo' => $saldos['balanceAmountCall'],
    'saldo_facturacion' => "",
    'tipo_linea' => "",
    'update_saldo_date' => $updateDate,
    'update_saldo_hours' => $updateTime,
    'update_linea' => '',
    'update_facturacion_date' => "",
    'update_facturacion_hours' => "",   
    'saldo_minutos' => _tol_min_int_decimal($saldos['balanceAmountVOICE']),  
//    'saldo_minutos' => _tol_region_format_moneda($saldos['balanceAmountVOICE'], 'round'),  
    'saldo_sms' => $saldos['balanceAmountSMS'],
    'saldo_datos' => $saldos['balanceAmountDATA'],
    'milinea_mensajes' => '-',
    'milinea_mensajes2' => '-',
    'milinea_datos' => '-',
  );
	$fechas = array(
      'date_begin' => null,
      'date_end' => null);
/**** Fin Saldo ****/

/**** Inicio Linea ****/
  $nombre_plan = 'No Disponible';
  $estado_plan = 'No Disponible';
  $informacion = 'No Disponible';
  $class_estado = 'No Disponible';
  $uri = NULL;

  $plan=tol_get_plan_info();
  
  $estado_plan = $plan['state'];
  $informacion = $plan['info'];

  if ($plan['type']!='*') { //tal vez hay que quitar este if si definen en la taxonomia el termino "*"
      $term_pos = taxonomy_get_term_by_name($plan['type'], 'telephony_type');
      if (!empty($term_pos)) {
          $term_pos = current($term_pos);
          $uri = $term_pos->field_image['und'][0]['uri'];
      }
  }
  $nombre_plan=strtoupper($plan['name']);
  $nombre_plan = str_replace("PLAN", "", $nombre_plan);
  $cuenta['tipo_linea'] = $nombre_plan;
/**** Fin Linea ****/

/**** Inicio factura ****/
	  $ultimas_facturas = array();
  	if ($plan['type']!='PRE'){
      
      $facturas = _tol_obtener_facturas(); //llamada a metodo que contiene la llamada al servicio
	  
     
      if (isset($facturas->invoiceList->invoideHeader)){
      	if (is_array($facturas->invoiceList->invoideHeader)){
    		  $latestBill = $facturas->invoiceList->invoideHeader[0];
      	}else{
      		$latestBill = $facturas->invoiceList->invoideHeader;
      	}
        module_load_include('inc','tigoapi','tigoapi');
        $tigo = TigoAPI::getInstance();
        $msisdn=tol_get_current_msisdn();
        $accountDebt=$tigo->GetAccountDebt($msisdn);
		

        if (!isset($accountDebt->balance) ||$accountDebt->balance <= 0 ){ //por si el saldo es negativo o igual a cero
          $cuenta['saldo_facturacion'] = 0;
        }else{
          $cuenta['saldo_facturacion'] = _tol_region_format_moneda($accountDebt->balance,'ceil', variable_get('tol_moneda_alternativo'));
          //$cuenta['saldo_facturacion'] = _tol_region_format_moneda($accountDebt->balance);
        }

        $cuenta['date_begin'] = (isset($latestBill->periodStartDate))?_tol_generic_format_fechaDDMMYYYY($latestBill->periodStartDate):NULL;
        $cuenta['date_end'] = (isset($latestBill->periodEndDate))?_tol_generic_format_fechaDDMMYYYY($latestBill->periodEndDate):NULL;

		if (!isset($cuenta['date_begin'])){
			$fechas =tol_ciclo_facturacion($latestBill);
			$cuenta['date_end'] =  $fechas['date_end'];
			$cuenta['date_begin'] =  $fechas['date_begin'];
		}
      	if (empty($cuenta['date_end'])){ //RVS 20130701 a peticion de TIGO CO
        	$cuenta['date_end'] = date('Y-m-d', strtotime("+1 months", strtotime($latestBill->periodStartDate)));
        	$cuenta['date_end'] = date('Y-m-d', strtotime("-1 day", strtotime($cuenta['date_end'])));
        	$cuenta['date_end'] = _tol_generic_format_fechaDDMMYYYY($cuenta['date_end']);
        }
        
      	$cuenta['update_facturacion_date'] = $updateDate;
      	$cuenta['update_facturacion_hours'] = $updateTime;


    	  $_SESSION['factura_actual'] = $latestBill->invoiceNumber; //important! //chequear
  
      if (is_array($facturas->invoiceList->invoideHeader)){
    	   
    	  foreach ($facturas->invoiceList->invoideHeader as $factura){
    	  	$fact = array(
            'nro' => $factura->invoiceNumber,
            'saldo'=>_tol_region_format_moneda($factura->invoiceAmount,'ceil',variable_get('tol_moneda_alternativo') ),
            //'saldo'=>_tol_region_format_moneda($factura->invoiceAmount),
          );

          $ultimas_facturas[] = $fact;

          if (sizeof($ultimas_facturas) == 3){
          	break;
          }
      	}
		}
    	}
   }
  
/**** Fin factura ****/

  //variables de configuracion
  $showRecargar = variable_get('tol_resumen_show_reload');
  $recargarLink = variable_get('tol_resumen_reload_link');

  $showCambiarPlan = variable_get('tol_resumen_show_change_plan');
  $showPagar = variable_get('tol_resumen_show_pagar');
  $showPeriodoFact = variable_get('tol_resumen_show_periodofact', 1);
  if ($showPagar){
  	$formulario_pago = render(drupal_get_form('tol_facturacion_pago_form'));
  }else{
    $formulario_pago=null;
  }
  
  drupal_add_js(drupal_get_path('module', 'tol') . '/js/tagcrazyegg.js');
  
  return theme('resumen-de-tu-cuenta', array(
    'output' => $setVar,
    'cuenta' => $cuenta,
    'ultimas_facturas' => $ultimas_facturas,
    'showRecargar' => $showRecargar,
    'recargarLink' => $recargarLink,
    'showCambiarPlan' => $showCambiarPlan,
    'showPagar' => $showPagar,
    'formulario_pago'=>$formulario_pago,
    'planType'=>$saldos['planType'],
    'periodoFact'=>$showPeriodoFact,
    )
  );
}


function tol_facturacion_page() {
	
	//Datos de Facturacion
	$time_starts = microtime(true);
	$configFactura = _tol_obtener_config_factura(); //llamada a metodo que busca en BD
    
	
	$facturacion = array(
			'recibir_fis' => (@$configFactura->printedInvoiceStatus)? 'checked':null,
			'recibir_elec' => (@$configFactura->electronicInvoiceStatus)? 'checked':null,
			//'recibir_det_res' => (@$configFactura->printedInvoiceDetails)? 'checked':null,
			'email' => (@$configFactura->email)?$configFactura->email:t('No disponible'),
			'electronica_detalle' => (@$configFactura->electronicInvoiceDetails)?'Si':'No',
			'direccion' => (@$configFactura->email)?$configFactura->address : t('No disponible'),
			'departamento' => '',//(@$configFactura->email)?$configFactura->department : t('No disponible'),
			'ciudad' => '',//(@$configFactura->email)?$configFactura->city : t('No disponible'),
			'telefono' => '',//(@$configFactura->email)?$configFactura->phoneNumber : t('No disponible'),
			'fisica_detalle' => (@$configFactura->printedInvoiceDetails)?'Si':'No',
			'amount' => null,
			'date_begin' => null,
			'date_end' => null,
	
			'vencimiento' => null,
			'fecha_limite' => null,
			'nrofactura' => null,
			'nrocuenta' => null,
			'periodo' => null,
			'emision' => null,
	);
	
	//Manejo de descarga cuando esta activa la segunda clave
	$doFactDown = false;

	$factDescargaID = '';
	if (!empty($_GET["idDes"])){
		$factDescargaID = $_GET["idDes"];
		drupal_set_message(t('La descarga de su factura iniciara en pocos segundos.'), 'status');
		$doFactDown = true;
	}

	$setVar = 'facturacion';

	$fechas = array(
      'date_begin' => null,
      'date_end' => null);
	
	$ultimas_facturas = array();
    
    $facturas = _tol_obtener_facturas(); //llamada a metodo que contiene la llamada al servicio
    $plan=tol_get_plan_info();
        
    if (!empty($facturas->invoiceList->invoideHeader)){

		$latestBill = $facturas->invoiceList->invoideHeader[0];  //obtengo la factura mas reciente

		module_load_include('inc','tigoapi','tigoapi');
		$tigo = TigoAPI::getInstance();
		$accountDebt = $tigo->GetAccountDebt($_SESSION['my_line']);

		if (!isset($accountDebt->balance) || $accountDebt->balance <= 0 ){ //por si el saldo es negativo o igual a cero
			$facturacion['amount'] = 0;
		}else{
		  $facturacion['amount'] = _tol_region_format_moneda($accountDebt->balance,'ceil', variable_get('tol_moneda_alternativo'));
		}


		$facturacion['date_begin'] = (isset($latestBill->periodStartDate))?_tol_generic_format_fechaDDMMYYYY($latestBill->periodStartDate):NULL;
		$facturacion['date_end'] = (isset($latestBill->periodEndDate))?_tol_generic_format_fechaDDMMYYYY($latestBill->periodEndDate):NULL;
		$facturacion['vencimiento'] = (isset($latestBill->expirationDate))?_tol_generic_format_fechaDDMMYYYY($latestBill->expirationDate):NULL;
     
   
		if (!isset($facturacion['date_begin'])){
			$fechas = tol_ciclo_facturacion($latestBill);
			$facturacion['date_end'] =  $fechas['date_end'];
			$facturacion['date_begin'] =  $fechas['date_begin'];
		}
   
		if (empty($facturacion['date_end'])){ //RVS 20130701 a peticion de TIGO CO
			$facturacion['date_end'] = date('Y-m-d', strtotime("+1 months", strtotime($latestBill->periodStartDate)));
			$facturacion['date_end'] = date('Y-m-d', strtotime("-1 day", strtotime($facturacion['date_end'])));
			$facturacion['date_end'] = _tol_generic_format_fechaDDMMYYYY($facturacion['date_end']);
		}

		$facturacion['fecha_limite'] = '';

		$facturacion['nrofactura'] = $latestBill->invoiceNumber;
		$facturacion['totalfactura'] = _tol_region_format_moneda($latestBill->invoiceAmount,'ceil', variable_get('tol_moneda_alternativo'));
		if (!empty($latestBill->contract)){
			$facturacion['nrocuenta'] = $latestBill->contract;
		} else if(!empty($latestBill->ruc)){
			//$facturacion['nro_cuenta'] = $latestBill->ruc; //en CO esto es la cedula, debe ser Numero de cuenta 10 digitos y empieza con 8
			$facturacion['nro_cuenta'] = '';

		}

		if (!empty($latestBill->billPeriod)){
			$facturacion['periodo'] = $latestBill->billPeriod;
		}

		if (!empty($latestBill->printDate)){
			$facturacion['emision'] = _tol_generic_format_fechaDDMMYYYY($latestBill->printDate);
		}

		/*
		
		//Falta que estos datos se obtengan
		$facturacion['pago'] = '08/05/2013???';

		
		
		if (empty($configFactura)){
			//creo la factura y la guardo en BD

			$facturaCreada = _tol_factura_general_create(); //creo factura nueva para almacenar en BD

			//le pongo los valores de la factura recibida por el servicio
			
			$facturaCreada->field_factura_valor['und'][0]['value'] = $latestBill->invoiceAmount;
			

			
			//$facturaCreada->field_factura_correo['und'][0]['value'] = $latestBill->invoiceAmount;
			$facturaCreada->field_direccion['und'][0]['value'] = $latestBill->addressInInvoice;
			//0 = Fac. Resumida 1 = Fac. Detallada SOLO UN VALOR PARA AMBAS
			$facturaCreada->field_factura_det_res['und'][0]['value'] = 1; //valor por defecto //chequear

//				$facturaCreada->field_factura_departamento['und'][0]['value'] = $latestBill->phoneNumber; //No vienen en el servicio
//				$facturaCreada->field_factura_ciudad['und'][0]['value'] = $latestBill->phoneNumber;  //No vienen en el servicio
			$facturaCreada->field_factura_telefono['und'][0]['value'] = $latestBill->phoneNumber;

			$resultSave = _tol_factura_general_save($facturaCreada); //guardo en BD


			if (!empty($facturaCreada)){
				$configFactura = $facturaCreada;
			}


		}
		
		//1=fisica, 2=electronica, 3=both
		

		$correo = @$configFactura->field_factura_correo['und'][0]['value'];

		if (empty($correo)) {
    	$correo = 'No Disponible';
    }

		$direccion = @$configFactura->field_direccion['und'][0]['value'];

		if (empty($direccion)) {
    	$direccion = 'No Disponible';
    }

		
		//$departamento = @$configFactura->field_factura_departamento['und'][0]['value'];

		//if (empty($departamento)) {
    //	$departamento = '';
    //}
		

		$departamento = null;
	  if (!empty($configFactura->field_factura_departamento[LANGUAGE_NONE][0]['tid'])){
	    $tid=$configFactura->field_factura_departamento[LANGUAGE_NONE][0]['tid'];
	    $state=taxonomy_term_load($tid);
	    $departamento=$state->name;
	  }

		if (empty($departamento)) {
    	$departamento = '';
    }

    
    //$ciudad = @$configFactura->field_factura_ciudad['und'][0]['value'];

		//if (empty($ciudad)) {
    //	$ciudad = '';
    //}
    

    $ciudad = null;
	  if (!empty($configFactura->field_factura_ciudad[LANGUAGE_NONE][0]['tid'])){
	    $tid=$configFactura->field_factura_ciudad[LANGUAGE_NONE][0]['tid'];
	    $city=taxonomy_term_load($tid);
	    $ciudad=$city->name;
	  }

		if (empty($ciudad)) {
    	$ciudad = '';
    }


    $telefono = @$configFactura->field_factura_telefono['und'][0]['value'];

		if (empty($telefono)) {
    	$telefono = 'No Disponible';
    }

  
	$valor_defecto_fis_elec = @$configFactura->field_factura_fis_elec['und'][0]['value'];

    switch ($valor_defecto_fis_elec){
      case 1:
        $facturacion['recibir_elec'] = null;
		    $facturacion['recibir_fis'] = 'checked';

        break;
      case 3:
        $facturacion['recibir_elec'] = 'checked';
        $facturacion['recibir_fis'] = 'checked';
        break;
      case 2:
      default:
        $facturacion['recibir_elec'] = 'checked';
        $facturacion['recibir_fis'] = null;
        break;
    }

    $valor_defecto_det_res = @$configFactura->field_factura_det_res['und'][0]['value'];

    if ($valor_defecto_det_res == 1) {
    	$facturacion['recibir_det_res'] = 'checked';
    } else {
    	$facturacion['recibir_det_res'] = '';
    }

    $facturacion['email'] = $correo;
    $facturacion['electronica_detalle'] = ($facturacion['recibir_det_res']=='checked')?'Si':'No';

    $facturacion['direccion'] = $direccion;
    $facturacion['departamento'] = $departamento;
    $facturacion['ciudad'] = $ciudad;
    $facturacion['telefono'] = $telefono;
    $facturacion['fisica_detalle'] = ($facturacion['recibir_det_res']=='checked')?'Si':'No';
*/

    //Se guarda en sesion el numero de cuenta para que cuando vaya a otras seccion no se tenga que llamar de nuevo al servicio solo para obtener este dato
    if (!empty($latestBill->contract)) {
    	$_SESSION['nrocuenta']	= $latestBill->contract;
    }

	  //Seteo la factura actual
	  $_SESSION['factura_actual'] = $latestBill->invoiceNumber; //importante

      
//El historial
	  foreach ($facturas->invoiceList->invoideHeader as $factura){
	  	$fact = array(
        'total' => (isset($factura->invoiceAmount))?_tol_region_format_moneda($factura->invoiceAmount,'ceil', variable_get('tol_moneda_alternativo')):NULL,
        'id_factura' => (!empty($factura->invoiceNumber))?$factura->invoiceNumber:'',
      );

      if (isset($factura->periodStartDate))      
        $fact['periodo_begin'] = _tol_generic_format_fechaDDMMYYYY($factura->periodStartDate);
      elseif (isset($factura->billPeriod))
        $fact['periodo_begin'] = $factura->billPeriod;
      elseif (isset($factura->printDate))
        $fact['periodo_begin'] = _tol_generic_format_fechaDDMMYYYY($factura->printDate);
      else{
        $fact['periodo_begin'] = NULL;
      }
      if (isset($factura->invoiceState))      
        $fact['invoiceState'] = ($factura->invoiceState =="CA")? "Si":"No";

      if (!empty($factura->periodEndDate)) {
	      $fact['perdiodo_end'] = _tol_generic_format_fechaDDMMYYYY($factura->periodEndDate);
	    } else {
			if (variable_get('tigoapi_country_iso') == "SV") 
			{
				 $fechas = tol_ciclo_facturacion($factura);
				 $fact['perdiodo_end'] =  $fechas['date_end'];
				 $fact['periodo_begin'] =  $fechas['date_begin'];
   		    }
			else{
				$fact['perdiodo_end'] = date('Y-m-d', strtotime("+1 months", strtotime($factura->periodStartDate)));
				$fact['perdiodo_end'] = date('Y-m-d', strtotime("-1 day", strtotime($fact['perdiodo_end'])));
				$fact['perdiodo_end'] = _tol_generic_format_fechaDDMMYYYY($fact['perdiodo_end']);
				//RVS 20130705 a peticion de TIGO CO
	    	}
				
			}

      $ultimas_facturas[] = $fact;
  	}

	} else {
  	drupal_set_message(t('En este momento el servicio de consulta no esta disponible. Intentalo mas tarde.'), 'warning');
  }

  //variables de configuracion
  $showPagar = variable_get('tol_pagofac_online');
  $showDescargar = variable_get('tol_descargafac_online');
  $showPrint = variable_get('tol_imprimirfac_online');
  $useSclave = variable_get('tol_flag_sclave');
  $showDetallefac = variable_get('tol_factura_show_detalle');

  $form_historial = render(drupal_get_form('tol_facturacion_historial_form'));

  if ($showPagar){
  	$formulario_pago = render(drupal_get_form('tol_facturacion_pago_form'));
  } else {
  	$formulario_pago = null;
  }
  return theme('facturacion', array('show_factura_fisica'=>variable_get('tol_factura_envio_fis_ele',true), 'show_factura_fisica'=>variable_get('tol_factura_envio_fis_ele',true),'showDatosFactura'=>variable_get('tol_showDatosFactura',true),'showNumeroFactura'=>variable_get('tol_showNumeroFactura',true),'formulario_historial' => $form_historial, 'formulario_pago' => $formulario_pago, 'facturacion' => $facturacion, 'ultimas_facturas' => $ultimas_facturas, 'showPagar' => $showPagar, 'showDescargar' => $showDescargar, 'useSclave' => $useSclave, 'showPrint' => $showPrint, 'status'=> 'OK', 'doFactDown' => $doFactDown, 'idDesParam' => $factDescargaID, 'showDetallefac'=>$showDetallefac) );
}
    
    
function tol_ciclo_facturacion($factura)
{
	$terms=taxonomy_get_term_by_name($factura->cycle, 'ciclo_facturacion');
	$terms = current($terms);

	$facturacion = array(
      'date_begin' => null,
      'date_end' => null);
 
	if($factura->cycle == "X" || $factura->cycle == "Y")
	{
		 $mes = substr($factura->billPeriod, 5, 2);
		 if($mes == '04'|| $mes == '06' || $mes == '09'|| $mes == '11'){
			$dia = ((int)$terms->field_ciclo_fact_valor['und'][0]['value'])-1;
		 }
		 elseif($mes == '02'){
			 if(tol_yearbisiesto(substr($factura->billPeriod, 0, 4)))
			 {
				 $dia = ((int)$terms->field_ciclo_fact_valor['und'][0]['value'])-2;
			 }
			 else {
				 $dia = ((int)$terms->field_ciclo_fact_valor['und'][0]['value'])-3;
		     }
			
		 }
		 else{ 
		    $dia = $terms->field_ciclo_fact_valor['und'][0]['value'];
		 }
		 $facturacion['date_end'] =  $factura->billPeriod."/".$dia;	     	 
		 $facturacion['date_end'] = _tol_generic_format_fechaDDMMYYYY($facturacion['date_end']);
		 $facturacion['date_begin'] = $factura->billPeriod."/01";
		 $facturacion['date_begin'] = _tol_generic_format_fechaDDMMYYYY($facturacion['date_begin']);
	}
	if($factura->cycle == "C" || $factura->cycle == "H" || $factura->cycle == "K" || $factura->cycle == "P"|| $factura->cycle == "U")
	{
		  $digito = str_pad($terms->field_ciclo_fact_valor['und'][0]['value'], 2, "0", STR_PAD_LEFT); 
		  $facturacion['date_end'] =  $factura->billPeriod."/".$digito;        
		  $facturacion['date_begin']=$facturacion['date_end'];
		  $facturacion['date_begin'] = date('Y-m-d', strtotime("-1 months", strtotime($facturacion['date_begin'])));
		  $facturacion['date_begin'] = date('Y-m-d', strtotime("+1 day", strtotime($facturacion['date_begin'])));
          $facturacion['date_end'] = _tol_generic_format_fechaDDMMYYYY($facturacion['date_end']);
		  $facturacion['date_begin'] = _tol_generic_format_fechaDDMMYYYY($facturacion['date_begin']);
	}
	
	return $facturacion;
}	

function tol_yearbisiesto($year){
   if((!($year%4) && ($year%100)) || !($year%400)){
      return true;
   }else{
      return false;
   }
}
function tol_facturacion_editar_page() {
		$configFactura = null;
		$configFactura = _tol_obtener_config_factura(); //llamada a metodo que busca en BD
		error_log("GetClientAccountGeneralInfo configFactura : ".print_r($configFactura, TRUE)."\n",3,'/mnt/logs/drupal/servicios.log');

		$facturacionC = array(
				'recibir_elec' => @$configFactura->electronicInvoiceStatus,
				'email' => @$configFactura->email,
				'elec_detalle' => @$configFactura->electronicInvoiceDetails,
				'recibir_fis' => @$configFactura->printedInvoiceStatus,
				'direccion' => @$configFactura->address,
				'departamento' => @$configFactura->department,
				'ciudad' => @$configFactura->city,
				'telefono' => @$configFactura->phoneNumber,
				'formato' => (@$configFactura->printedInvoiceDetails)?'1':'0',
				'show_factura_fisica'=>variable_get('tol_factura_envio_fis_ele',true),
				'showFormatoFactura'=>variable_get('tol_showFormatoFactura',true),
				
		//  'showfielddepartamento'=>variable_get('tol_show_modifiable_field_departamento',true),
		);
		
/*
		$correo = @$configFactura->email['und'][0]['value'];


		if (empty($correo)) {
    	$correo = '';
        }

		$direccion = @$configFactura->field_direccion['und'][0]['value'];

		if (empty($direccion)) {
    	$direccion = '';
    }



		//NO
		//$departamento = @$configFactura->field_factura_departamento['und'][0]['value'];

		$departamento = null;
	  if (!empty($configFactura->field_factura_departamento[LANGUAGE_NONE][0]['tid'])){
	    $tid=$configFactura->field_factura_departamento[LANGUAGE_NONE][0]['tid'];
	    $state=taxonomy_term_load($tid);
	    $departamento=$state->name;
	  }

		if (empty($departamento)) {
    	$departamento = '';
    }

		//NO
    //$ciudad = @$configFactura->field_factura_ciudad['und'][0]['value'];

	  $ciudad = null;
	  if (!empty($configFactura->field_factura_ciudad[LANGUAGE_NONE][0]['tid'])){
	    $tid=$configFactura->field_factura_ciudad[LANGUAGE_NONE][0]['tid'];
	    $city=taxonomy_term_load($tid);
	    $ciudad=$city->name;
	  }


		if (empty($ciudad)) {
    	$ciudad = '';
    }

		$telefono = @$configFactura->field_factura_telefono['und'][0]['value'];

		if (empty($telefono)) {
    	$telefono = '';
    }


    if (variable_get('tol_is_fact_fisica_editable', TRUE)){	
    	if ($configFactura->field_factura_fis_elec['und'][0]['value']==2 || $configFactura->field_factura_fis_elec['und'][0]['value']==3){
    		$configFactura->field_factura_fis_elec['und'][0]['value'] = 3; //Ya que electronica estaba marcada
    	}else{
    		$configFactura->field_factura_fis_elec['und'][0]['value'] = 1; //solo física
    	}
    } 
   
    $valor_defecto_fis_elec = @$configFactura->field_factura_fis_elec['und'][0]['value'];
    
    switch ($valor_defecto_fis_elec){
      case 1:
        $cf_recibir_elec = 0;
        $cf_recibir_fis = 1;
        break;
      case 3:
        $cf_recibir_elec = 1;
        $cf_recibir_fis = 1;
        break;
      case 2:
      default:
        $cf_recibir_elec = 1;
        $cf_recibir_fis = 0;
        break;
    }

   $valor_defecto_det_res = @$configFactura->field_factura_det_res['und'][0]['value'];

	 if ($valor_defecto_det_res == 1) {
	  	$cf_det_res = 1;
	 } else {
	  	$cf_det_res = 0;
	 }

  $facturacionC = array(
    'recibir_elec' => $cf_recibir_elec,
    'email' => $correo,
    //'elec_detalle' => $cf_det_res,
	'recibir_fis' => $cf_recibir_fis,
    'direccion' => $direccion,
    'departamento' => $departamento,
    'ciudad' => $ciudad,
    'telefono' => $telefono,
    'formato' => $cf_det_res,
  //  'showfielddepartamento'=>variable_get('tol_show_modifiable_field_departamento',true),
   );
*/
  
  $form = render(drupal_get_form('tol_facturacion_form',$facturacionC));
  return  theme('facturacion-editar', array('formulario_facturacion' => $form));

}


function tol_factura_detalle_page() {
	$factID = $_GET["id"];
  $_SESSION['factura_actual'] = $factID; //important!

  $factura_arra = array(
    'valor' => null,
    'vencimiento' => null,
    'fecha_limite' => '',
    'nro_factura' => null,
    'nro_cuenta' => null,
    'date_first' => null,
    'date_last' => null,
    'periodo' => null,//'08/05/2013 al 06/07/2014', //no tiene sentido
    'emision' => null,
  );

  $factura = _tol_obtener_datos_factura(); //llamo al servicio
  $plan=tol_get_plan_info();


 	$factura_arra['valor'] = _tol_region_format_moneda($factura->invoiceAmount,'ceil', variable_get('tol_moneda_alternativo'));

	if (!empty($factura->expirationDate)){
  	$factura_arra['vencimiento'] = _tol_generic_format_fechaDDMMYYYY($factura->expirationDate);
  }

	$factura_arra['nro_factura'] = $factura->invoiceNumber;

  if (!empty($factura->contract)){
  	$factura_arra['nro_cuenta'] = $factura->contract;
  } else if(!empty($factura->ruc)){
  	//$factura_arra['nro_cuenta'] = $factura->ruc; //en CO esto es la cedula, debe ser Numero de cuenta 10 digitos y empieza con 8
  	$factura_arra['nro_cuenta'] = '';
  }
  if (isset($factura->billPeriod)) {$factura_arra['periodo'] = $factura->billPeriod; }
  
  if (!empty($factura->periodEndDate)){
  	$factura_arra['date_last'] = _tol_generic_format_fechaDDMMYYYY($factura->periodEndDate);
  } else { //RVS 20130705 a peticion de TIGO CO
  	$factura_arra['date_last'] = date('Y-m-d', strtotime("+1 months", strtotime($factura->periodStartDate)));
  	$factura_arra['date_last'] = date('Y-m-d', strtotime("-1 day", strtotime($factura_arra['date_last'])));
  	$factura_arra['date_last'] = _tol_generic_format_fechaDDMMYYYY($factura_arra['date_last']);
  }	
  
  if (isset($factura->periodStartDate)) //OJO ESTO NO ESTA ASI EN LA PAG DE FACTURA
    $factura_arra['date_first'] = _tol_generic_format_fechaDDMMYYYY($factura->periodStartDate);
    /*
  elseif (isset($factura->billPeriod))
    $factura_arra['date_first'] = $factura->billPeriod;
  elseif (isset($factura->printDate))
    $factura_arra['date_first'] = _tol_generic_format_fechaDDMMYYYY($factura->printDate);
  else{
    $factura_arra['date_first'] = NULL;
  }*/
  $fechas = array(
      'date_begin' => null,
      'date_end' => null);
  if (!isset($factura_arra['date_first'])){
			$fechas = tol_ciclo_facturacion($factura);
			$factura_arra['date_last']  =  $fechas['date_end'];
			$factura_arra['date_first'] =  $fechas['date_begin'];
		}

  if (!empty($factura->printDate)){
  	$factura_arra['emision'] = _tol_generic_format_fechaDDMMYYYY($factura->printDate);
  }

  //variables de configuracion
  $tipoPago = variable_get('tol_tipo_pagofac_online');
  $showDescargar = variable_get('tol_descargafac_online_detalle_fac');
  $showPrint = variable_get('tol_imprimirfac_online');
  $showNumeroFactura = variable_get('tol_showNumeroFactura');
  $useSclave = variable_get('tol_flag_sclave');
  $form = render(drupal_get_form('tol_factura_detalle_form'));
  return theme('factura-detalle', array('formulario_pago' => $form, 'factura' => $factura_arra,'showNumeroFactura' => $showDescargar, 'showDescargar' => $showDescargar,'useSclave' => $useSclave, 'showPrint' => $showPrint,'tipoPago' => $tipoPago) );
}


function tol_factura_pre_descarga_page() {

  $factDestino = "descargarf";
	if (!empty($_GET["dest"])){
		$factDestino = $_GET["dest"];
	}

  $_SESSION['factDestino'] = $factDestino; //se guarda en sesion que accion disparo el color-box
	if ($factDestino == 'descargarf'){
	  //id de la factura a descargar
	  $factDescargaID = $_GET["idDes"];

	  $_SESSION['factDescargaID'] = $factDescargaID; //se guarda en sesion
	}

  $form = render(drupal_get_form('tol_into_segunda_contrasena_form', $factDestino));
	return theme('segunda-contrasena', array('formulario_into_segunda_contrasena' => $form));
}



function tol_factura_descarga_page() {
	$factDescargaID = $_GET["idDes"];

  $_SESSION['factura_actual'] = $factDescargaID; //important!

  $result = FALSE;

  $factura = _tol_obtener_datos_factura(); //obtengo los datos de la factura en cuestion


  if (empty($factura)) {
    return;
  } else {
    $cycle = trim($factura->cycle);
    $invoiceSerial = trim(@$factura->invoiceSerial);
    
    $invoiceNumber = trim(@trim($factura->invoiceNumber)); 
    $printDate = trim(@$factura->printDate);
    


    /*if (empty($printDate)){
      $printDate = trim(@$factura->periodStartDate);
    }*/
    $invoiceRequestedType = 'Factura';
    $serviceType = 'Telefonia';

    $result = _tol_obtener_factura_pdf($cycle, $invoiceSerial, $invoiceNumber, $invoiceRequestedType, $serviceType, $printDate);
    //if ($result['status'] == 'ERROR'){
    if ($result){
    	//drupal_set_message(t($result->reason), 'error');
    	drupal_set_message(t('La factura aun no se encuentra disponible en formato electrónico'), 'error');
    	drupal_goto('online/facturacion');
    }

  }

}


function tol_mis_lineas_page() {
  $setVar = 'mis-lineas';
  
  
//Aqui se maneja la seleccion de linea a usar para las operaciones
//Tambien se maneja la seleccion de linea predeterminada en la seccion Linea

//manejo de cambio de linea seleccionada

	if (isset($_REQUEST["numcp"])){
  	$_SESSION['my_line'] = $_REQUEST["numcp"]; ////cambia de linea seleccionada

		//reseteo variables de sesion
		$_SESSION['nrocuenta'] = '';
		$_SESSION['estado_plan'] = '';
  }

//manejo de cambio de linea predeterminada
  $linea_numero = '';

  if (isset($_REQUEST["lineaNum"])){
  	$linea_numero = $_REQUEST["lineaNum"];
  
  }

  if (!empty($linea_numero)){
  	$_SESSION['my_predetermi_line'] = $linea_numero; //cambia de linea predeterminada
  	$_SESSION['my_line'] = $linea_numero; ////cambia de linea seleccionada
  	tol_update_linea_predeterminado($linea_numero);
  	tol_clear_cache();
  
  }

  global $user;

  if ($user->uid != 0) {

    //Obtengo los id de las lineas asociadas al usuario
    module_load_include('inc','tol','lib/lines');
	$lineDB = new TOL_lines();
  	$obj_lineas = $lineDB->get($user->uid);
    /*
  	if (empty($obj_lineas)){ //si no hay lineas activas voy a online?
			drupal_goto('online/resumen');
		}*/


  	if (isset($_SESSION['my_line'])) {
      $msisdn = $_SESSION['my_line'];
    }
    else{
      $msisdn = _tol_get_linea_first($user->uid);
      $_SESSION['my_line'] = $msisdn;
     } 
    
  }
 
  $lineas = array();

	if (isset($obj_lineas)) {
	  foreach($obj_lineas as $obj_linea){

	  	$linea_id = $obj_linea->id;
	  	$linea_num = $obj_linea->line;
	    $linea_nombre = $obj_linea->name;
	    $linea_estado = ($_SESSION['my_line']==$obj_linea->line) ? 'predeterminado':'';

	    if ($linea_nombre == ''){
	  		$linea_nombre = 'Mi linea Tigo';
	  	}

	  	//linea predeterminada
	  	$active = '';

	  	if (empty($_SESSION['my_predetermi_line'])){
	  		if ($msisdn == $linea_num){
		  		$active = 'predeterminado';
		  		$_SESSION['my_predetermi_line'] = $linea_num;
		  	}
		  } else {
		  	if ($_SESSION['my_predetermi_line'] == $linea_num){
		  		$active = 'predeterminado';
		  		$_SESSION['my_predetermi_line'] = $linea_num;
		  	}
		  }

	  	$linea = array(
		    'active'=> $active,
		    'lineaId'=> $linea_id,
		    'nombre'=> $linea_nombre,
		    'numero'=> $linea_num,

		    'userid'=> $user->uid,
	    );

	    //Se hace un render del form para cada row (linea)
	     //module_load_include('inc','tol','tol.forms');
	    $form = render(drupal_get_form('tol_mis_lineas_form', $linea));

	  	$linea['ediform'] = $form;

	  	$lineas[] = $linea;

	  }
  }

  return theme('mis-lineas', array('showAsociarLinea'=>variable_get('tol_showAsociarLinea',true),'output' => $setVar, 'lineas' => $lineas));
}

function tol_linea_detalle_page() {
  $movil = array(
    'imagen'=>'movil.jpg',
    'marca'=>'No disponible',
    'modelo'=>'No disponible',
    'imei'=>'No disponible',
    'beneficiario'=>'No disponible',
    'valor_deducible'=>'No disponible',
    'contacto'=>'No disponible',
  );

  $personal = array(
    'nombre'=>'No disponible',
    'apellido'=>'No disponible',
    'tipo_documento'=>'-',
    'nro_documento'=>'-',
    'direccion'=>'No disponible',
    'departamento'=>'No disponible',
    'ciudad'=>'No disponible',

  );

  $linea = array(
    'numero'=>null,
    'pin'=>'No disponible',
    'puk'=>'No disponible',
    'winmax'=>'',
    'estado'=>'No disponible',
  );


/**** Inicio Movil ****/
  //$imei = _tol_obtener_Imeis(); // OJO: NO USAR ESTE METODO

  $resultINFO = _tol_obtener_info_equipo();

	global $base_url;

  $image = $base_url .'/'. $resultINFO['uri'];

  $movil['imagen'] = $image;

  if (!empty($resultINFO['imei']) && $resultINFO['imei'] != 'tac_key')
  {
  	$movil['marca'] = $resultINFO['marca'];
  	$movil['modelo'] = $resultINFO['modelo'];
  	$movil['imei'] = $resultINFO['imei'];
  }


/**** Fin Movil ****/


  $data=tol_get_client_info();
    $personal['nombre'] = $data['name'];
	$personal['apellido'] = $data['lastName'];
	$personal['nro_documento'] = $data['documenNumber'];
	$personal['direccion'] = $data['address'];
	$personal['departamento'] = $data['state'];
	$personal['ciudad'] = $data['city'];

	if (!is_numeric($data['documentType'])){
	  $personal['tipo_documento'] = $data['documentType'];
	}else{
	  $term = taxonomy_term_load($data['documentType']);
	  $personal['tipo_documento'] = $term->name;
	}
/**** Fin Personal ****/



/**** Inicio Linea ****/

  $PinPuk = _tol_obtener_PinPuk();

  if (!empty($PinPuk->pin1)) {
		$linea['pin'] = $PinPuk->pin1;
  }
  if (isset($PinPuk->puk1)) {
			$linea['puk'] = $PinPuk->puk1;
  }
  if (isset($PinPuk->pin2)) {
			$linea['pin2'] = $PinPuk->pin2;
  }
  if (isset($PinPuk->puk2)) {
			$linea['puk2'] = $PinPuk->puk2;
  }

  $nrocuenta = null;

  if (!empty($_SESSION['nrocuenta'])) {
    $nrocuenta = $_SESSION['nrocuenta'];
  } else {
  	$facturas = _tol_obtener_facturas(); //llamada a metodo que contiene la llamada al servicio
	
  	if (!empty($facturas->invoiceList->invoideHeader)){
		if (is_array($facturas->invoiceList->invoideHeader)){
    		  $latestBill = $facturas->invoiceList->invoideHeader[0];
      	}else{
      		$latestBill = $facturas->invoiceList->invoideHeader;
		}

	  	if (!empty($latestBill->contract)) {
	  		$nrocuenta = $latestBill->contract;
	  	} else {
	  		$nrocuenta = $latestBill->ruc;
	  	}
    }
  }

  $linea['numero'] = $nrocuenta;
  $info = tol_get_plan_info();
  
  
  $linea['estado'] = $info['state'];
  
  return theme('linea-detalle', array('showBloquearEquipo'=>variable_get('tol_showBloquearEquipo',true),'showBloquearLine'=>variable_get('tol_linea_show_block_line',true),'showStatusLine'=>variable_get('tol_linea_show_estatus_line',true),'show_pin2_punk2'=>variable_get('tol_linea_show_pin2_punk2',true),'show_nrocuenta'=>variable_get('tol_linea_show_numcuenta',true),'showInformacionPersonal'=>variable_get('tol_showInformacionPersonal',true), 'showEditarDatosCliente'=>variable_get('tol_showEditarDatosCliente',true),'output' => 'linea-detalle', 'movil' => $movil, 'personal' => $personal, 'linea' => $linea));
}



function tol_saldo_page() {
  $setVar = 'saldo';

  $SMSarray = array();
  $VOICEarray = array();
  $DATAarray = array();

  $datos_saldo = array(
	    'saldo_actual'=> 'No disponible',
	    'saldo_update'=> 'No disponible',
	    'saldo_update_hs'=> '',
	    'minutes'=> '-',
	    'segINFO'=> $VOICEarray,
	    'sms'=> '-',
	    'smsINFO'=> $SMSarray,
	    'datas'=> '-',
	    'dataINFO'=> $DATAarray,
	    'status'=> 'Error',
	 );

	$saldos = _tol_obtener_saldo2();
     
    $updateDate = _tol_generic_format_fechaDDMMYYYY(date ("Y-m-d"));
	$updateTime = date ("H:i");

  $datos_saldo = array(
    'saldo_actual'=> $saldos['balanceAmountCall'],
    'saldo_expirationDate'=> $saldos['expirationDateCall'],
    'saldo_update'=> $updateDate, //esto no esta en el servicio
    'saldo_update_hs'=> $updateTime, //esto no esta en el servicio

    'minutes'=> $saldos['balanceAmountVOICE'],
     
   // 'minutes' => substr(_tol_region_format_moneda($saldos['balanceAmountVOICE'], 'round'), 1,strlen(_tol_region_format_moneda($saldos['balanceAmountVOICE'], 'round'))-1),
    'segINFO'=> $saldos['VOICEarray'],

    'sms'=> $saldos['balanceAmountSMS'],
    'smsINFO'=> $saldos['SMSarray'],

    'datas'=> $saldos['balanceAmountDATA'],
    'dataINFO'=> $saldos['DATAarray'],
    'status'=> $saldos['status'],
  );
	//variables de configuracion
  $showRecargar = variable_get('tol_saldo_show_reload');
  $recargarLink = variable_get('tol_saldo_reload_link');
  $showRecargarForEachWallet = variable_get('tol_saldo_show_reload_for_each_wallet');
  $showDetalle = variable_get('tol_saldo_show_details');
  $showExpirationDate = variable_get('tol_saldo_show_expiration_date');
  $showTextoSaldo = variable_get('tol_saldo_show_textosaldo');

 
  return theme('saldo', array('output' => $setVar, 'saldo' => $datos_saldo, 'showRecargar' => $showRecargar, 'recargarLink' => $recargarLink, 'showRecargarForEachWallet' => $showRecargarForEachWallet, 'showDetalle' => $showDetalle, 'showExpirationDate' => $showExpirationDate, 'showTextoSaldo' => $showTextoSaldo) );
}


function tol_mis_datos_page() {
  $setVar = 'mis-datos';

  global $user;

  $data=tol_get_client_info();


  //a solicitud de HN el tipo de documento de la taxonomia es diferente a lo que envian en la respuesta del WS
  if(variable_get("tigoapi_country_iso") == "HN"){
	$v = taxonomy_vocabulary_machine_name_load('type_document');
    $terms = taxonomy_get_tree($v->vid);
    
    foreach ($terms as $key => $value) {
		if(in_array($data['documentType'],explode(" ", $value->name))){
		 $data['documentType']=$value->name;
			}
    }
	}
  	
  if (is_numeric($data['state'])){
  	$term = taxonomy_term_load($data['state']);
  	$data['state']= $term->name;
  } 
  if (is_numeric($data['city'])){
  	$term = taxonomy_term_load($data['city']);
  	$data['city']= $term->name;
  }
   if (is_numeric($data['documentType'])){
  	$term = taxonomy_term_load($data['documentType']);
  	$data['documentType']= $term->name;
  }
  
  $variables = array(
    'nombre'=>$data['name'], //CHECK
    'apellido'=>$data['lastName'], //CHECK
    'tipo_dni'=>$data['documentType'],
    'nro_dni'=>$data['documenNumber'],
    'bithday'=>$data['lastName'],
    'direccion'=>$data['address'],
    'departamento'=>$data['state'],
    'ciudad'=>$data['city'],
    'telefono'=>$data['phoneNumber'],
    'correo'=>$data['email'],
    'uid'=>$user->uid,
    'showSegundaContrasena'=>variable_get('tol_showSegundaContrasena',true),
    'showEditarDatosCliente'=>variable_get('tol_showEditarDatosCliente',true),
  /*  'showfielddepartamento'=>variable_get('tol_show_modifiable_field_departamento',true),*/
  );


  //$_SESSION['confirmation-code-msisdn']=''; //Limpia la varaible en caso de cambio de linea
  unset($_SESSION['confirmation-code-msisdn']);

  return theme('mis-datos', $variables);
}

function tol_mis_datos_editar_page() {
  global $user;
  $data = tol_get_client_info();
  $dataPlan = tol_get_plan_info();
  
  $variables = array(
    'nombre'=>$data['name'],
    'apellido'=>$data['lastName'],
    'tipo_dni'=>$data['documentType'],
    'nro_dni'=>$data['documenNumber'],
    'bithday'=>null, //$data['birthday'],//CHECK
    'direccion'=>$data['address'],
    'departamento'=>$data['state'],
    'ciudad'=>$data['city'],
    'telefono'=>$data['phoneNumber'],
    'correo'=>$data['email'],
    'uid'=>$user->uid,
    'dataPlan'=>$dataPlan['type'],
    'editablefieldnombre'=>variable_get('tol_flag_modifiable_field_nombre', true),
    'editablefieldapellido'=>variable_get('tol_flag_modifiable_field_apellido', true),
    'editablefieldtipodocumento'=>variable_get('tol_flag_modifiable_field_tipo_documento', true),
    'editablefieldnumerodocumento'=>variable_get('tol_flag_modifiable_field_numero_documento', true),
    'editablefielddireccion'=>variable_get('tol_flag_modifiable_field_direccion', true),
    'editablefieldtelefono'=>variable_get('tol_flag_modifiable_field_telefono', true),
    'editablefieldcorreo'=>variable_get('tol_flag_modifiable_field_correo', true),
  );

  $form = render(drupal_get_form('tol_mis_datos_form', $variables));
  return theme('mis-datos-editar', array('formulario_mis_datos' => $form));
}


function tol_editar_documento_page() {
	$form = render(drupal_get_form('tol_documento_form'));
  return  theme('editar-documento', array('formulario_documento' => $form));
}

function tol_editar_preguntas_page() {

  $form = render(drupal_get_form('tol_preguntas_form'));
  return  theme('editar-preguntas', array('formulario_preguntas' => $form));
}


function tol_editar_codigo_seguridad_page() {

	$destination = '';
	if (!empty($_GET["dest"])){
		$destination = $_GET["dest"];
	}

	if (empty($destination)){
		$destination = "linea";
	} else if (empty($_SESSION['confirmation-code-msisdn'])){ //No el mejor codigo, buscar solucion alternativa

		$msisdn = $_SESSION['my_line'];

	 	$_SESSION['confirmation-code-msisdn'] = $msisdn; //se guarda en sesion el msisdn

	 	_tol_sms_msisdn_confirmation($msisdn); //servicio que manda el sms

	}

	$form = render(drupal_get_form('tol_codigo_seguridad_form',$destination));
  return theme('editar-codigo-seguridad', array('formulario_codigo_seguridad' => $form, 'destination' => $destination));
}


function tol_editar_codigo_seguridad_page_reenviar() {

	$destination = '';
	if (!empty($_GET["dest"])){
		$destination = $_GET["dest"];
	}

	$msisdn = $_SESSION['confirmation-code-msisdn'];

	if(!empty($msisdn)){
		_tol_sms_msisdn_confirmation($msisdn); //servicio que manda el sms
	}

	$form = render(drupal_get_form('tol_codigo_seguridad_form',$destination));
  return theme('editar-codigo-seguridad', array('formulario_codigo_seguridad' => $form, 'destination' => $destination));
}


function tol_editar_segunda_contrasena_page() {

  $form = render(drupal_get_form('tol_segunda_contrasena_form'));
  return  theme('editar-segunda-contrasena', array('formulario_segunda_contrasena' => $form));
}


function tol_editar_asociar_linea_page() {

$variables = array(
    'showTerminosCondiciones'=>variable_get('tol_showTerminosCondiciones', false),
  );

  $form = render(drupal_get_form('tol_asociar_linea_form', $variables));
  return  theme('editar-asociar-linea', array('formulario' => $form));
}


function tol_eliminar_linea_page() {

  $lineaID = $_GET["id"];
  $lineaName = $_GET["name"];
  $lineaNum = $_GET["num"];

  $lineaInfo = array(
      'lineaID' =>  $lineaID,
      'lineaName' => $lineaName,
      'lineaNum' => $lineaNum,
  );

  $form = render(drupal_get_form('tol_eliminar_linea_form', $lineaInfo));
  return theme('eliminar-linea', array('formulario_eliminar_linea' => $form));
}

function tol_linea_detalle_validacion_page() {

  $form_linea_validacion = render(drupal_get_form('tol_linea_detalle_validacion_form'));
  return theme('linea_detalle_validacion', array('formulario_linea_validacion' => $form_linea_validacion));
}

function tol_linea_bloquear_equipo_page() {

  $form_linea_bloquear_equipo = render(drupal_get_form('tol_linea_bloquear_equipo_form'));
  return theme('linea_bloquear_equipo', array('formulario_linea_bloquear_equipo' => $form_linea_bloquear_equipo));
}

function tol_linea_bloquear_linea_page() {

  $form_linea_bloquear_linea = render(drupal_get_form('tol_linea_bloquear_linea_form'));
  return theme('linea_bloquear_linea', array('formulario_linea_bloquear_linea' => $form_linea_bloquear_linea));
}


function tol_factura_factura_confirmacion_pagosonline_page() {

	//OJO AQUI TODAVIA HAY SERVICIOS QUE SON DE TIGO ONLINE VIEJO


$today = date("Y-m-d, g:i:s a");
error_log("\n\ntol_factura_factura_confirmacion_pagosonline_page $today \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['transaccion_id'] =  " . $_REQUEST['transaccion_id'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['usuario_id'] =  " . $_REQUEST['usuario_id'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['descripcion'] =  " . $_REQUEST['descripcion'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['ref_venta'] =  " . $_REQUEST['ref_venta'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['valor'] =  " . $_REQUEST['valor'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['moneda'] =  " . $_REQUEST['moneda'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['estado_pol'] =  " . $_REQUEST['estado_pol'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['ciudadCobro'] =  " . $_REQUEST['ciudadCobro'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['fecha_transaccion'] =  " . $_REQUEST['fecha_transaccion'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['codigo_respuesta_pol'] =  " . $_REQUEST['codigo_respuesta_pol'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['email_comprador'] =  " . $_REQUEST['email_comprador'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['mensaje_respuesta_pol'] =  " . $_REQUEST['mensaje_respuesta_pol'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['ref_pol'] =  " . $_REQUEST['ref_pol'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['extra1'] =  " . $_REQUEST['extra1'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['extra2'] =  " . $_REQUEST['extra2'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['firma'] =  " . $_REQUEST['firma'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

//NEW

error_log("tol_factura_factura_confirmacion_pagosonline_page _REQUEST['medio_pago'] =  " . $_REQUEST['medio_pago'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');



    /* RVS 20130215 Esta es la tabla inutil de IMG, se puede eliminar */
    /* RVS 20130828 comentado */
    /*
    if (isset($_REQUEST['transaccion_id'])) {
        try {
            $nid = db_insert('tigo_transaccion_pagos_online')
                      ->fields(array(
                          'id_transaccion' => $_REQUEST['transaccion_id'],
                      ))->execute();
        } catch (Exception $e) {
            return;
        }
    }
	*/

    // LO QUE SE RECIBE DE PAGO ONLINE
    try {
    		$now = REQUEST_TIME;
    		$datetimeNow = date("Y-m-d H:i:s", $now);
    		
        //added RVS 20130829
        if (isset($_REQUEST['extra2'])) {
        	$uid_extra2 = (int) $_REQUEST['extra2'];
        } else {
        	$uid_extra2 = 0;
        }
        
        $nid = db_insert('tigo_transaccion_pagos_online_full_info')->fields(array(
            'id_transaccion' => $_REQUEST['transaccion_id'],
            'fecha_insercion' => $datetimeNow,
            //'uid' => $_REQUEST['extra2'],
            'uid' => $uid_extra2,
            'pago_fac' => null,
            'descripcion' => $_REQUEST['descripcion'],
            'usuario_id' => $_REQUEST['usuario_id'],
            'ref_venta' => $_REQUEST['ref_venta'],
            'valor' => $_REQUEST['valor'],
            'moneda' => $_REQUEST['moneda'],
            'estado_pol' => $_REQUEST['estado_pol'],
            'ciudadCobro' => $_REQUEST['ciudadCobro'],
            'fecha_transaccion' => $_REQUEST['fecha_transaccion'],
            'email_comprador' => $_REQUEST['email_comprador'],
            'mensaje_respuesta_pol' => $_REQUEST['mensaje_respuesta_pol'],
            'codigo_respuesta_pol' => $_REQUEST['codigo_respuesta_pol'],
            'telefono' => $_REQUEST['extra1'],
            'estado' => "Recibido desde pago online",
            'medio_pago' => $_REQUEST['medio_pago'],
        ))->execute();
        
    } catch (Exception $e) {
        $today = date("Y-m-d, g:i:s a");
        error_log("tol_factura_factura_confirmacion_pagosonline_page $today -- CATCH NO SE PUDO INSERTAR EN BASE DE DATOS: ". $e->message . " \n", 3, '/mnt/logs/drupal/pago.log');
    }


    if (isset($_REQUEST['usuario_id'])) {
    	
	    //ldenis 04/03/2013
	    // $datos_user = explode('|', @$_REQUEST['descripcion']);
	    // funcion para validar la Duplicidad de codigo.
	    //$tel = @$datos_user[1];
	    
	    $datos = array(); //added RVS 20130829
	   
	    $llave = variable_get('tol_pagos_online_pass'); //llave = 1291e846533
 			$llave_express = variable_get('pago_express_pol_llave'); //llave pago express 13f3a9764c5 
 			
 			$pago_fac = false;
	    
			$usuario_id = isset($_REQUEST['usuario_id']) ? $_REQUEST['usuario_id'] : NULL;
	    $ref_venta = isset($_REQUEST['ref_venta']) ? $_REQUEST['ref_venta'] : NULL;
	    $valor = isset($_REQUEST['valor']) ? $_REQUEST['valor'] : NULL;
	
	    /* if ($valor){
	      $entero = explode('.', $valor);
	      $valor = $entero[0];
	      } */
	      
	    $moneda = isset($_REQUEST['moneda']) ? $_REQUEST['moneda'] : NULL;
	    $estado_pol = isset($_REQUEST['estado_pol']) ? $_REQUEST['estado_pol'] : NULL;
	    
	    $codigoRespuestaPol = isset($_REQUEST['codigo_respuesta_pol']) ? $_REQUEST['codigo_respuesta_pol'] : NULL;
	    
	    $firma_cadena = "$llave~$usuario_id~$ref_venta~$valor~$moneda~$estado_pol";
		  $firmacreada = md5($firma_cadena); //firma que generaron ustedes
		
		  $firma_cadena_express = "$llave_express~$usuario_id~$ref_venta~$valor~$moneda~$estado_pol";
		  $firmacreada_express = md5($firma_cadena_express); //firma que generaron ustedes
	    
	    $firma =  isset($_REQUEST['firma']) ? $_REQUEST['firma'] : NULL;  //firma que envia nuestro sistema
	
			$datos['medio_pago'] = $_REQUEST['medio_pago'];
			
			/*
			10 VISA
			11 MASTERCARD
			12 AMEX
			22 DINERS
			24 Verified by VISA
			25 PSE
			27 VISA Debito
			30 Efecty
			31 Pago referenciado
		*/
		
			if (!isset($datos['medio_pago'])) {
		    $datos['medio_pago'] = "25";
		  }
		  
		  switch ($datos['medio_pago']) {
		    case "22":
		       $datos['medio_pago'] = "054"; 
		       $medio_de_pago = '22 DINERS'; 
		        break;
		    case "11":
		       $datos['medio_pago'] = "056";
		       $medio_de_pago = '11 MASTERCARD';
		        break;
		    case "10":
		       $datos['medio_pago'] = "057";
		       $medio_de_pago = '10 VISA';
		        break;
		    case "12":
		        $datos['medio_pago'] = "058";
		        $medio_de_pago = '12 AMEX';
		        break;
		   case "25":
		        $datos['medio_pago'] = "059";
		        $medio_de_pago = '25 PSE';
		        break;
		   default: 
		        $datos['medio_pago'] = "059";
		        $medio_de_pago = 'D 25 PSE';
		 break;
		 }
			
			
			if ((strtoupper($firma) == strtoupper($firmacreada)) || (strtoupper($firma) == strtoupper($firmacreada_express))) {
	    //if (strtoupper($firma) == strtoupper($firmacreada)) {
	
	       if ($estado_pol == 4 && $codigoRespuestaPol == 1) {
	       			
	            /* ------------Datos to tigo_online_services_pagoFact ------------  */
	            $datos['transaccion_id'] = isset($_REQUEST['transaccion_id']) ? @$_REQUEST['transaccion_id'] : NULL;
	            //$datos['fac_ref'] = $datos_user[0];
	            $datos['fac_ref'] = $ref_venta;
	            $datos['tel'] = isset($_REQUEST['extra1']) ? @$_REQUEST['extra1'] : NULL;
	            //$datos['uid'] = @$_REQUEST['ref_venta'];
	            $datos['uid'] = isset($_REQUEST['transaccion_id']) ? @$_REQUEST['transaccion_id'] : NULL;//ESTA RARO ESTO PERO BUENO
	            $datos['valor'] = $valor;
	            $datos['ciudadCobro'] = isset($_REQUEST['ciudadCobro']) ? @$_REQUEST['ciudadCobro'] : NULL;
	            $mensajeRespuestaPol = isset($_REQUEST['mensaje_respuesta_pol']) ? @$_REQUEST['mensaje_respuesta_pol'] : NULL;
	            $email = isset($_REQUEST['email_comprador']) ? @$_REQUEST['email_comprador'] : NULL;
	            $fechaTransaccion = isset($_REQUEST['fecha_transaccion']) ? @$_REQUEST['fecha_transaccion'] : NULL;
	            $datos['id_usuario']  = isset($_REQUEST['extra2']) ? @$_REQUEST['extra2'] : NULL;
	            
	            //Formating fecha y hora de transaccion fecha_transaccion=2012-08-02+10%3A22%3A21
	            $fecha_hora_transaccion = explode(' ', $fechaTransaccion); //Fecha de registro de pago
	            if (!isset($fecha_hora_transaccion)) {
	                $fecha_hora_transaccion = NULL;
	            }
	            $fecha_transaccion = explode('-', $fecha_hora_transaccion[0]);
	            $datos['dia_pago'] = @$fecha_transaccion[0] . @$fecha_transaccion[1] . @$fecha_transaccion[2];
	            $hora_transaccion = explode(':', @$fecha_hora_transaccion[1]);
	            $datos['hora_pago'] = @$hora_transaccion[0] . @$hora_transaccion[1] . @$hora_transaccion[2];
	            //watchdog('pago_online', serialize($_REQUEST), array(), WATCHDOG_NOTICE, 'link');
	            //$datos['uid'] = $_REQUEST['transaccion_id'];
	            $datos['fecha'] = $fechaTransaccion;
	            $datos['correo'] = $email;
	            $datos['telefono'] = $datos['tel'];
	            $datos['state'] = $mensajeRespuestaPol;
	            $datos['descripcion'] = isset($_REQUEST['descripcion']) ? @$_REQUEST['descripcion'] : NULL;
	            $state = 'aprobado';
	            $params = array(
	                'correo' => $email,
	                'state' => $mensajeRespuestaPol,
	                'transaccion_id' => $datos['transaccion_id'],
	                'valor' => $valor,
	                'fecha' => $fechaTransaccion ,
	                'telefono' => $datos['tel'],
	            );
	
	            /* RVS 20130208 CACHE NO FACTIBLE */
	            //global $user;
	            //cache_set('tigo_facturacion_pago_'.$user->uid,$_REQUEST['transaccion_id'], $bin = 'cache', 3600); //BLVE
	
	            $mensaje_texto = 'Estado de tu transaccion: ' . $mensajeRespuestaPol . ".\n  ID Transaccion: " . $datos['transaccion_id'] . ".\nEL PAGO DE TU FACTURA SE PODR� PROCESAR EN UN TIEMPO MAXIMO DE 24 HORAS.";
	            
	            //Se envia correo de confirmacion de pago RVS 20130802
	            $envio = drupal_mail('tol', 'confirmacion_pago', $email, 'ESP', $params);
	
	
	            //tigo_online_services_sendSMS($datos['tel'], $mensaje_texto);
	
	            // Send SMS to the respective msisdn tigo_online_services_sendSMS($msisdn , $message)
	            module_load_include('inc', 'tigoapi', 'tigoapi');
	            $tigo = TigoAPI::getInstance();
	            $result = $tigo->SendNotification($datos['tel'], t('Mensaje: @name', array('@name' => $mensaje_texto)));
	            $flag = TRUE;
	            if(!$result){
	              module_load_include('module', 'smsframework', 'sms');
	              $sender=variable_get('msisdn_verify_sms_sender', "");
	              $result = sms_send($datos['tel'], $mensaje_texto, array('sender'=>$sender));
	              if(!$result){
	                $flag = FALSE;
	              }
	            }
	
	             // Confirmation data
	            $datos['wsa_confirmacion'] = array(
	                'email' => $email,
	                'tel' => $datos['tel'],
	                'state' => $state,
	                'valor' => $valor,
	                'ref_venta' => $ref_venta,
	            );
	            
	            try {
	            
		            module_load_include('inc', 'tigo_api_services', 'tigo_api_services_api');
		            $tigo2 = new TigoAPIService();
		
		            $pago_fac = @$tigo2->tigo_online_services_pagoFact($datos); //llamada al servicio
		            
		            if ($pago_fac) {
		                // Register the process without queue system
		                _tigo_online_ws_asincrono_push('tigo_online_services_pagoFact', $datos, FALSE);
		                $estado = "Enviado al CRM.";
		                $repuesta = "true";
		            } else {
										// Register the process with queue system
		                _tigo_online_ws_asincrono_push('tigo_online_services_pagoFact', $datos, TRUE);
		                $estado = "En cola..";
		                $repuesta = "false";
		            }
		            
							} catch (Exception $e) {
	                $today = date("Y-m-d, g:i:s a");
	                error_log("tol_factura_factura_confirmacion_pagosonline_page $today -- CATCH Fallo algo con pagoFact o asincrono_push:". $e->message . "\n", 3, '/mnt/logs/drupal/pago.log');
	            }
	            
	            
	            //Proceso a hacer el insert
	            try {
	            		$now = REQUEST_TIME;
	            		$datetimeNow = date("Y-m-d H:i:s", $now);
	            	
			            //added RVS 20130830
			            //se cambia el valor y tipo de $datos['id_usuario'] para insertarlo en la BD
					        if (isset($datos['id_usuario'])) {
					        	$datos['id_usuario'] = (int) $datos['id_usuario'];
					        } else {
					        	$datos['id_usuario'] = 0;
					        }
			            
	                $nid = db_insert('tigo_transaccion_pagos_online_full_info')->fields(array(
	                        'id_transaccion' => $datos['transaccion_id'],
	                        'fecha_insercion' => $datetimeNow,
	                        'uid' => $datos['id_usuario'],
	                        'pago_fac' => $repuesta,
	                        'descripcion' => $datos['descripcion'],
	                        'usuario_id' => $usuario_id,
	                        'ref_venta' => $ref_venta,
	                        'valor' => $valor,
	                        'moneda' => $moneda,
	                        'estado_pol' => $estado_pol,
	                        'ciudadCobro' => $datos['ciudadCobro'],
	                        'fecha_transaccion' => $fechaTransaccion,
	                        'email_comprador' => $datos['correo'],
	                        'mensaje_respuesta_pol' => $mensajeRespuestaPol,
	                        'codigo_respuesta_pol' => $codigoRespuestaPol,
	                        'telefono' => $datos['telefono'],
	                        'estado' => $estado,
	                        'medio_pago' => $medio_de_pago,
	                    ))->execute();
	            } catch (Exception $e) {
	                $today = date("Y-m-d, g:i:s a");
	                error_log("tol_factura_factura_confirmacion_pagosonline_page $today --  CATCH NO SE PUDO INSERTAR EN BASE DE DATOS: ". $e->message ."\n", 3, '/mnt/logs/drupal/pago.log');
	            }
	
	        } else {
	            $email = isset($_REQUEST['email_comprador']) ? @$_REQUEST['email_comprador'] : NULL;
	            $mensajeRespuestaPol = isset($_REQUEST['mensaje_respuesta_pol']) ? @$_REQUEST['mensaje_respuesta_pol'] : NULL;
	            $datos['transaccion_id'] = isset($_REQUEST['transaccion_id']) ? @$_REQUEST['transaccion_id'] : NULL;
	            $fechaTransaccion = isset($_REQUEST['fecha_transaccion']) ? @$_REQUEST['fecha_transaccion'] : NULL;
	            $tel = isset($_REQUEST['extra1']) ? @$_REQUEST['extra1'] : NULL;
	            $state = 'rechazado';
	
	            $params = array(
	                'correo' => $email,
	                'state' => $mensajeRespuestaPol,
	                'transaccion_id' => $datos['transaccion_id'],
	                'valor' => $valor,
	                'fecha' => $fechaTransaccion ,
	                'telefono' =>  $tel,
	                'fac_ref' => $ref_venta,
	            );
	
	            tigo_online_services_sendSMS($tel, ': Estimado(a)  Usuario(a), la factura de tu l�nea ' . $tel . ' con referencia de pago No. ' .  $ref_venta . ' por valor de ' . $valor . ' no ha podido ser pagada exitosamente a trav�s de Pagos Online. Por favor int�ntalo Nuevamente');
	            
	            // Mail parameters
	            //Se envia correo RVS 20130802
	            $envio = drupal_mail('tol', 'confirmacion_pago', $email, 'ESP', $params);
	            
	        }
	    } else {
	
	        $datos['email'] = $_REQUEST['email_comprador'];
	        $datos['fac_ref'] = isset($_REQUEST['ref_venta']) ? $_REQUEST['ref_venta'] : NULL;
	        $datos['firma'] = isset($_REQUEST['firma']);
	        $datos['error_title'] = "NO CORRESPONDE LA FIRMA!!";
	
	        $state = 'Fallo la autenticacion de las firmas';
	        
	        watchdog('Pagos Online', 'Fallo la autenticacion de las firmas');
	        
	        $today = date("Y-m-d, g:i:s a");
	        error_log("tol_factura_factura_confirmacion_pagosonline_page $today -- $state \n", 3, '/mnt/logs/drupal/pago.log');
	        
	        // Mail parameters
	        $envio = drupal_mail('tigo_online_factura', 'error_pago', "pedro.lopez@blve.com, reinaldo.velasquez@blve.com, Ivan.olivos@tigo.com.co, amilkar.medina@mail.tigo.com.co, cristian.sparacino@tigo.com.co, Vladimir.Mancera@tigo.com.co, Alvaro.Araujo@tigo.com.co" , 'ESP', $datos);
	    }

		} else { // pagos online
				watchdog('LOG en BD para Pagos Online', 'No exiten parametros');
        
        $today = date("Y-m-d, g:i:s a");
        error_log("tol_factura_factura_confirmacion_pagosonline_page $today  -- No exiten parametros  \n", 3, '/mnt/logs/drupal/pago.log');
        
        //exit();
    }
}


function tol_factura_factura_respuesta_pagosonline_page() {
	 //Segun Ivan Olivos (20130730) en esta pagina no se recibe ningun parametro enviado por pagos online
	 
	 
	 //if (empty($_REQUEST['estado_pol']))drupal_goto('online/facturacion');
		
		$llave = variable_get('tol_pagos_online_pass');
    $moneda_pagos = variable_get('tol_pagos_online_moneda');
    $usuario_id = isset($_REQUEST['usuario_id']) ? $_REQUEST['usuario_id'] : NULL;
    $ref_venta = isset($_REQUEST['ref_venta']) ? $_REQUEST['ref_venta'] : NULL;
    $moneda = isset($_REQUEST['moneda']) ? $_REQUEST['moneda'] : NULL;
    $estado_pol = isset($_REQUEST['estado_pol']) ? $_REQUEST['estado_pol'] : NULL;
    $valor = isset($_REQUEST['valor']) ? $_REQUEST['valor'] : NULL;
    /* if ($valor){
      $entero = explode('.', $valor);
      $valor = $entero[0];
      } */
    $firma_cadena = "$llave~$usuario_id~$ref_venta~$valor~$moneda~$estado_pol";
    $firmacreada = md5($firma_cadena); //firma que generaron ustedes
    $firma = isset($_REQUEST['firma']) ? $_REQUEST['firma'] : NULL; //firma que envia nuestro sistema


//$today = date("Y-m-d, g:i:s a");
//error_log("\n\ntol_factura_factura_respuesta_pagosonline_page $today \n", 3, '/mnt/logs/drupal/zz_new_pago.log');

/*
error_log("tol_factura_factura_respuesta_pagosonline_page _REQUEST['usuario_id'] =  " . $_REQUEST['usuario_id'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_respuesta_pagosonline_page _REQUEST['ref_venta'] =  " . $_REQUEST['ref_venta'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_respuesta_pagosonline_page _REQUEST['valor'] =  " . $_REQUEST['valor'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_respuesta_pagosonline_page _REQUEST['moneda'] =  " . $_REQUEST['moneda'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
error_log("tol_factura_factura_respuesta_pagosonline_page _REQUEST['estado_pol'] =  " . $_REQUEST['estado_pol'] . " \n", 3, '/mnt/logs/drupal/zz_new_pago.log');
*/

    if (strtoupper($firma) == strtoupper($firmacreada)) {
        if ($_REQUEST['estado_pol'] == 4 && $_REQUEST['codigo_respuesta_pol'] == 1) {
            //watchdog('pago_online', serialize($_REQUEST), array(), WATCHDOG_NOTICE, 'link');
            $state = 'Transacci�n aprobada';
        } else {
            //watchdog('pago_online', serialize($_REQUEST), array(), WATCHDOG_NOTICE, 'link');
            $state = 'Transacci�n rechazada';
        }
    } else {
        //watchdog('pago_online', serialize($_REQUEST), array(), WATCHDOG_NOTICE, 'link');
        $state = 'Fallo la autenticacion de las firmas';
    }

    //drupal_set_message(t('@state', array('@state' => $state)));
    //drupal_set_message(t('Gracias por usar el Servicio de pago de factura Tigo'), 'status');
    
    drupal_set_message(t('Gracias por usar el servicio de pago de factura de Tigo.'), 'status');
    drupal_set_message(t('Recibiras un correo y un mensaje de texto con el estado de la solicitud.'), 'status');
    
    drupal_goto('online/facturacion');

//    $valores = array('state' => $state, 'ref_venta' => $ref_venta, 'ref_pol' => @$_REQUEST['ref_pol'], 'valor' => $valor, 'moneda' => $moneda,);
//    $markup = theme('tigo_online_factura_respuesta', $valores);
//    return $markup;

}



function tol_complete_profile_page() {
	$form = render(drupal_get_form('tol_complete_profile_form'));
  return  theme('complete-profile', array('formulario_complete_profile' => $form));
}
