<?php

function tol_admin_form($form, &$form_state) {
  $form = array();

  $form['region'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración / Caracteristicas de la Region'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Activar'), 0 => t('Desactivar'));

  $form['region']['tol_debug']  = array(
    '#type' => 'radios',
    '#title' => t('Activa el debug de la app'),
    '#default_value' => variable_get('tol_debug', 0),
    '#options' => $active,
  );


  $form['region']['tol_moneda'] = array(
    '#type' => 'textfield',
    '#title' => t('Símbolo Moneda'),
    '#default_value' => variable_get('tol_moneda', '$'),
    '#description' => t('$, $b, Gs'),
    '#maxlength' => 5,
    '#size'=> 5,
  );

  $form['region']['tol_pmoneda'] = array(
    '#type' => 'radios',
    '#title' => t('Posición símbolo de moneda'),
    '#default_value' => variable_get('tol_pmoneda', 1),
    '#options' => array(t('Izquierda - ($ 100)'), t('Derecha - (100 $)')),
  );

  $form['region']['tol_decimales'] = array(
    '#type' => 'select',
    '#title' => t('Decimales'),
    '#description' => t('Número de decimales'),
    '#options' => drupal_map_assoc(array('0', '1', '2')),
    '#default_value' => variable_get('tol_decimales', 0),
    '#required' => TRUE,
    '#maxlength' => 5,
  );

  $form['region']['tol_nlineas'] = array(
    '#type' => 'select',
    '#title' => t('Numero de lineas'),
    '#description' => t('Para definir el numero Maximo de lineas que puede asociar un usuario'),
    '#options' => drupal_map_assoc(array('1', '2', '4', '6', '8', '10','25')),
    '#default_value' => variable_get('tol_nlineas', '2'),
    '#required' => TRUE,
  );


  $form['region']['tol_moneda_alternativo'] = array(
    '#type' => 'textfield',
    '#title' => t('Símbolo Moneda Alternativa para Facturación'),
    '#default_value' => variable_get('tol_moneda_alternativo', ''),
    '#description' => t('$, $b, Gs, L'),
    '#maxlength' => 5,
    '#size'=> 5,
  );
  /*
  $form['region']['tol_typeplan_alternativo'] = array(
    '#type' => 'textfield',
    '#title' => t('Tipo de Plan para Moneda Alternativa'),
    '#default_value' => variable_get('tol_typeplan_alternativo', ''),
    '#description' => t('PRE, POS'),
    '#maxlength' => 10,
    '#size'=> 10,
  );
*/
  /* Configuración cuenta Pagos Online */

	//$user_pagos = variable_get('pagos_online_user');
  //$pass_pagos = variable_get('pagos_online_pass');
  //$moneda_pagos = variable_get('pagos_online_moneda');

  $form['pagos_online'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configuración cuenta Pagos Online'),
      '#collapsible' => TRUE,
    	'#collapsed' => TRUE,
  );

  $form['pagos_online']['tol_pagos_online_user'] = array(
      '#type' => 'textfield',
      '#title' => t('Usuario PagosOnline'),
      //'#default_value' => isset($user_pagos) ? $user_pagos : '',
      '#default_value' => variable_get('tol_pagos_online_user', ''),
      '#description' => t('Usuario ID en pagosOnline. Ejemplo: usuarioId = 59527'),
      '#maxlength' => 40,
      '#size' => 40,
  );

  $form['pagos_online']['tol_pagos_online_pass'] = array(
      '#type' => 'textfield',
      '#title' => t('Contraseña PagosOnline'),
      //'#default_value' => isset($pass_pagos) ? $pass_pagos : '',
      '#default_value' => variable_get('tol_pagos_online_pass', ''),
      '#description' => t('Contrasena en pagosOnline. Ejemplo: llave_encripcion = 1291e846533'),
      '#maxlength' => 40,
      '#size' => 40,
  );

  $form['pagos_online']['tol_pagos_online_moneda'] = array(
      '#type' => 'textfield',
      '#title' => t('Moneda'),
      //'#default_value' => isset($moneda_pagos) ? $moneda_pagos : '',
      '#default_value' => variable_get('tol_pagos_online_moneda', ''),
      '#description' => t('Moneda de la transacción en pagosOnline. Ejemplo: moneda = COP'),
      '#maxlength' => 10,
      '#size' => 10,
  );




  /* Cuenta */
  $active = array(1 => t('Mostrar'), 0 => t('Ocultar'));

  $form['micuenta']['misservicios'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración tab Mis Servicios'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['micuenta']['misservicios']['tol_misservicios_buzon'] = array(
    '#type' => 'radios',
    '#title' => t('Servicios de mi línea'),
    '#default_value' => variable_get('tol_misservicios_buzon', 0),
    '#options' => $active,
  );

  $form['micuenta']['misservicios']['tol_misservicios_multimedia'] = array(
    '#type' => 'radios',
    '#title' => t('Servicios Multimedia'),
    '#default_value' => variable_get('tol_misservicios_multimedia', 0),
    '#options' => $active,
  );

  $form['micuenta']['misservicios']['tol_misservicios_factura'] = array(
    '#type' => 'radios',
    '#title' => t('Notificación de factura'),
    '#default_value' => variable_get('tol_misservicios_factura', 0),
    '#options' => $active,
  );




//Configuración Seccion Resumen

	$form['micuenta']['resumen'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración Seccion Resumen'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['micuenta']['resumen']['tol_resumen_show_reload'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para recargar saldo'),
    '#default_value' => variable_get('tol_resumen_show_reload', 0),
    '#options' => $active,
  );

  $form['micuenta']['resumen']['tol_resumen_reload_link'] = array(
    '#type' => 'textfield',
    '#size' => 70,
    '#title' => t('Enlace para el boton de recarga'),
    '#default_value' => variable_get('tol_resumen_reload_link'),
    '#description' => t('URL para el boton de recarga. Ejemplo: http://www.cnn.com'),
  );

  $form['micuenta']['resumen']['tol_resumen_show_change_plan'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para cambiar de plan'),
    '#default_value' => variable_get('tol_resumen_show_change_plan', 0),
    '#options' => $active,
  );

  $form['micuenta']['resumen']['tol_resumen_show_pagar'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para pagar'),
    '#default_value' => variable_get('tol_resumen_show_pagar', 0),
    '#options' => $active,
  );

//Configuración Seccion Saldo

	$form['micuenta']['saldo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración Sección Saldo'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['micuenta']['saldo']['tol_saldo_wallet_saldo'] = array(
    '#type' => 'textfield',
    '#size' => 70,
    '#title' => t('id de la Billetera del Saldo Core'),
    '#default_value' => variable_get('tol_saldo_wallet_saldo','Saldo_principal'),
   );

	$form['micuenta']['saldo']['tol_saldo_show_reload'] = array(
	    '#type' => 'radios',
	    '#title' => t('Mostrar opcion para recargar saldo'),
	    '#default_value' => variable_get('tol_saldo_show_reload', 0),
	    '#options' => $active,
	);

	$form['micuenta']['saldo']['tol_showTimeInExpirationDate'] = array(
	    '#type' => 'radios',
	    '#title' => t('Mostrar hora en las fechas de expiración de saldo'),
	    '#default_value' => variable_get('tol_showTimeInExpirationDate', 0),
	    '#options' => $active,
	);



  $form['micuenta']['saldo']['tol_saldo_reload_link'] = array(
    '#type' => 'textfield',
    '#size' => 70,
    '#title' => t('Enlace para el boton de recarga'),
    '#default_value' => variable_get('tol_saldo_reload_link'),
    '#description' => t('URL para el boton de recarga. Ejemplo: http://www.cnn.com'),
  );

  $form['micuenta']['saldo']['tol_saldo_show_reload_for_each_wallet'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para recargar saldo para cada billetera'),
    '#default_value' => variable_get('tol_saldo_show_reload_for_each_wallet', 0),
    '#options' => $active,
  );

  $form['micuenta']['saldo']['tol_saldo_show_details'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para ver detalles de consumo'),
    '#default_value' => variable_get('tol_saldo_show_details', 0),
    '#options' => $active,
  );

  $form['micuenta']['saldo']['tol_saldo_show_expiration_date'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar opcion para ver fecha de vencimiento'),
    '#default_value' => variable_get('tol_saldo_show_expiration_date', 0),
    '#options' => $active,
  );
  
  $form['micuenta']['saldo']['tol_saldo_show_textosaldo'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar Incluye saldo principal y promocionales Actualizado'),
    '#default_value' => variable_get('tol_saldo_show_textosaldo', 0),
    '#options' => $active,
  );

//Configuración Seccion de Facturación

  $form['micuenta']['pagofac'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración Sección de Facturación'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['micuenta']['pagofac']['tol_pagofac_online'] = array(
    '#type' => 'radios',
    '#title' => t('Pagar factura Online'),
    '#default_value' => variable_get('tol_pagofac_online', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_showFormatoFactura'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Formato de Factura Detallado/Resumido'),
      '#default_value' => variable_get('tol_showFormatoFactura', true),
      '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_showNumeroFactura'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Número de Factura'),
      '#default_value' => variable_get('tol_showNumeroFactura', true),
      '#options' => $active,
  );
   $form['micuenta']['pagofac']['tol_showEditarDatosCliente'] = array(
      '#type' => 'radios',
      '#title' => t('Editar Inf. Personal / Mis Datos'),
      '#default_value' => variable_get('tol_showEditarDatosCliente', true),
      '#options' => $active,
  );

  $tipo_flagfacturafisica = array(0 => t('Editable'), 1=> t('No Editable'));

  $form['micuenta']['pagofac']['tol_is_fact_fisica_editable'] = array(
    '#type' => 'radios',
    '#title' => t('Factura Física Editable/No Editable'),
    '#default_value' => variable_get('tol_is_fact_fisica_editable', 0),
    '#options' => $tipo_flagfacturafisica,
  );

  $form['micuenta']['pagofac']['tol_showDatosFactura'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Datos de Facturación'),
      '#default_value' => variable_get('tol_showDatosFactura', true),
      '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_imprimirfac_online'] = array(
    '#type' => 'radios',
    '#title' => t('Imprimir factura Online'),
    '#default_value' => variable_get('tol_imprimirfac_online', 0),
    '#options' => $active,
  );

  $tipo_pagofac = array(2=> t('Completo'), 1 => t('Parcial'), 0 => t('No mostrar'));

  $form['micuenta']['pagofac']['tol_tipo_pagofac_online'] = array(
    '#type' => 'radios',
    '#title' => t('Tipo de pago Online en detalle de factura'),
    '#default_value' => variable_get('tol_tipo_pagofac_online', 0),
    '#options' => $tipo_pagofac,
  );

  $form['micuenta']['pagofac']['tol_factura_envio_fis_ele'] = array(
    '#type' => 'radios',
    '#title' => t('Factura física'),
    '#default_value' => variable_get('tol_factura_envio_fis_ele', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_factura_fecha_corte'] = array(
    '#type' => 'radios',
    '#title' => t('Fecha de Corte'),
    '#default_value' => variable_get('tol_factura_fecha_corte', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_factura_fecha_limite_pago'] = array(
    '#type' => 'radios',
    '#title' => t('Fecha Limite de pago'),
    '#default_value' => variable_get('tol_factura_fecha_limite_pago', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_factura_codigo_pago'] = array(
    '#type' => 'radios',
    '#title' => t('Código de referencia'),
    '#default_value' => variable_get('tol_factura_codigo_pago', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_descargafac_online'] = array(
    '#type' => 'radios',
    '#title' => t('Descargar PDF factura Online'),
    '#default_value' => variable_get('tol_descargafac_online', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_descargafac_online_detalle_fac'] = array(
    '#type' => 'radios',
    '#title' => t('Descargar PDF factura Online en detalle de factura'),
    '#default_value' => variable_get('tol_descargafac_online_detalle_fac', 0),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_factura_show_detalle'] = array(
    '#type' => 'radios',
    '#title' => t('Detalle de Factura'),
    '#default_value' => variable_get('tol_factura_show_detalle', 1),
    '#options' => $active,
  );

  $form['micuenta']['pagofac']['tol_factura_intentos_pdf'] = array(
    '#type' => 'select',
    '#title' => t('Número de Intentos Consulta PDF'),
    '#default_value' => variable_get('tol_factura_intentos_pdf', 0),
    '#options' => drupal_map_assoc(range(1, 5)),
  );

  $form['micuenta']['pagofac']['tol_factura_url_de_pago'] = array(
    '#type' => 'textfield',
    '#title' => t('URL de Pago'),
    '#default_value' => variable_get('tol_factura_url_de_pago', 'https://gateway.pagosonline.net/apps/gateway/index.html'),
    '#description' => t('URL de pasarela pago'),
    '#maxlength' => 100,
    '#size'=> 100,
  );



//Configuración Seccion de Linea

	$form['micuenta']['linea'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuración Seccion Mi Linea'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['micuenta']['linea']['tol_consumo_fecha'] = array(
    '#type' => 'radios',
    '#title' => t('Ver mi consumo a la fechas'),
    '#default_value' => variable_get('tol_consumo_fecha', 0),
    '#options' => $active,
  );
  $form['micuenta']['linea']['tol_showAsociarLinea'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Botón Asociar Linea'),
      '#default_value' => variable_get('tol_showAsociarLinea', true),
      '#options' => $active,
  );
  $form['micuenta']['linea']['tol_showTerminosCondiciones'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Términos y Condiciones'),
      '#default_value' => variable_get('tol_showTerminosCondiciones', true),
      '#options' => $active,
  );
   $form['micuenta']['linea']['tol_showSegundaContrasena'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Botón Generar Segunda Contraseña'),
      '#default_value' => variable_get('tol_showSegundaContrasena', true),
      '#options' => $active,
  );
  $form['micuenta']['linea']['tol_showInformacionPersonal'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Información Personal'),
      '#default_value' => variable_get('tol_showInformacionPersonal', true),
      '#options' => $active,
  );
  $form['micuenta']['linea']['tol_showBloquearEquipo'] = array(
      '#type' => 'radios',
      '#title' => t('Mostrar Botón Bloquear Equipo'),
      '#default_value' => variable_get('tol_showBloquearEquipo', true),
      '#options' => $active,
  );
  $form['micuenta']['linea']['tol_micuenta_saldo'] = array(
    '#type' => 'radios',
    '#title' => t('Consulta saldos'),
    '#default_value' => variable_get('tol_micuenta_saldo', 0),
    '#options' => $active,
  );

  $form['micuenta']['linea']['tol_micuenta_imsi_imei'] = array(
    '#type' => 'radios',
    '#title' => t('Consulta imsi - imei'),
    '#default_value' => variable_get('tol_micuenta_imsi_imei', 0),
    '#options' => $active,
  );

  $form['micuenta']['linea']['tol_micuenta_crm'] = array(
    '#type' => 'radios',
    '#title' => t('Actualizar tu información'),
    '#default_value' => variable_get('tol_micuenta_crm', 0),
    '#options' => $active,
  );
  $form['micuenta']['linea']['tol_linea_show_estatus_line'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar estado de linea'),
    '#default_value' => variable_get('tol_linea_show_estatus_line', 1),
    '#options' => $active,
  );
  $form['micuenta']['linea']['tol_linea_show_block_device'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar bloqueo de equipo'),
    '#default_value' => variable_get('tol_linea_show_block_device', 0),
    '#options' => $active,
  );

  $form['micuenta']['linea']['tol_linea_show_block_line'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar bloqueo de linea'),
    '#default_value' => variable_get('tol_linea_show_block_line', 0),
    '#options' => $active,
  );

  $form['micuenta']['linea']['tol_linea_show_pin2_punk2'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar PIN2 / PUK2'),
    '#default_value' => variable_get('tol_linea_show_pin2_punk2', 0),
    '#options' => $active,
  );
  $form['micuenta']['linea']['tol_linea_show_numcuenta'] = array(
    '#type' => 'radios',
    '#title' => t('Mostrar Número de cuenta'),
    '#default_value' => variable_get('tol_linea_show_numcuenta', 0),
    '#options' => $active,
  );
  
  
//Configuración campos requeridos en los datos del cliente

$form['required_field_ClientInfo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Campos requeridos para completar datos del cliente'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $active = array(1 => t('Requerido'), 0 => t('No Requerido'));
  $form['required_field_ClientInfo']['tol_flag_required_field_nombre'] = array(
	  '#type' => 'radios',
	  '#title' => t('Nombre'),
	  '#default_value' => variable_get('tol_flag_required_field_nombre', 0),
	  '#options' => $active,
	);
   $form['required_field_ClientInfo']['tol_flag_required_field_apellido'] = array(
	  '#type' => 'radios',
	  '#title' => t('Apellido'),
	  '#default_value' => variable_get('tol_flag_required_field_apellido', 0),
	  '#options' => $active,
	);
   $form['required_field_ClientInfo']['tol_flag_required_field_tipo_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Tipo Documento'),
	  '#default_value' => variable_get('tol_flag_required_field_tipo_documento', 0),
	  '#options' => $active,
	);
	$form['required_field_ClientInfo']['tol_flag_required_field_numero_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Numero Documento'),
	  '#default_value' => variable_get('tol_flag_required_field_numero_documento', 0),
	  '#options' => $active,
	);
	$form['required_field_ClientInfo']['tol_flag_required_field_direccion'] = array(
	  '#type' => 'radios',
	  '#title' => t('Dirección'),
	  '#default_value' => variable_get('tol_flag_required_field_direccion', 0),
	  '#options' => $active,
	);
	$form['required_field_ClientInfo']['tol_flag_required_field_telefono'] = array(
	  '#type' => 'radios',
	  '#title' => t('Teléfono'),
	  '#default_value' => variable_get('tol_flag_required_field_telefono', 0),
	  '#options' => $active,
	);
		$form['required_field_ClientInfo']['tol_flag_required_field_correo'] = array(
	  '#type' => 'radios',
	  '#title' => t('Correo'),
	  '#default_value' => variable_get('tol_flag_required_field_correo', 0),
	  '#options' => $active,
	);


  $form['tigo']['extras'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuraciónes extras'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
/*
  $form['tigo']['extras']['tol_extras_piepagina'] = array(
    '#type' => 'textfield',
    '#title' => t('Pie de pagina, Correos'),
    '#default_value' => variable_get('tol_extras_piepagina', 'Copyright© Tigo. Colombia Móvil S.A E.S.P. Todos los derechos reservados'),
    '#description' => t('Copyright© Tigo. Colombia Móvil S.A E.S.P. Todos los derechos reservados'),
  );
*/
/*
  $form['tigo']['extras']['tol_extras_sms_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Mensaje de Confirmación'),
    '#description' => t('Mensaje de Confirmación al SMS'),
    '#default_value' => variable_get('tol_extras_sms_message'),
  );
*/
  $form['tigo']['extras']['tol_wsdl_ultimasfact'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Mostrar Ultimas n facturas'),
    '#default_value' => variable_get('tol_wsdl_ultimasfact'),
  );

  $form['tigo']['extras']['tol_wsdl_updatefact'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Tiempo para actualizar factura'),
    '#default_value' => variable_get('tol_wsdl_updatefact'),
    '#description' => t('Tiempo en segundos para actualizar la factura'),
  );

  $form['tigo']['extras']['tol_div_poli_father'] = array(
    '#type' => 'textfield',
    '#title' => t('División politica Padre'),
    '#default_value' => variable_get('tol_div_poli_father', 'Departamento'),
    '#description' => t('División politica padre ejemplo: Departamento'),
  );

  $form['tigo']['extras']['tol_div_poli_son'] = array(
    '#type' => 'textfield',
    '#title' => t('División politica Hijo'),
    '#default_value' => variable_get('tol_div_poli_son', 'Ciudad'),
    '#description' => t('División politica hijo ejemplo: Ciudad'),
  );

  $form['correo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Correo'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['correo']['tol_footer_face'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook'),
    '#default_value' => variable_get('tol_footer_face'),
    '#description' => t('Face correos de activación'),
  );

  $form['correo']['tol_footer_twit'] = array(
    '#type' => 'textfield',
    '#title' => t('twitter'),
    '#default_value' => variable_get('tol_footer_twit'),
    '#description' => t('twitter correos de activación'),
  );

  $form['Reportar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reportar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['Reportar']['tol_correo_sugerencia'] = array(
    '#type' => 'textfield',
    '#title' => t('Correo de sugerencias'),
    '#default_value' => variable_get('tol_correo_sugerencia', ''),
    '#description' => t('Correo de sugerencias que esta en getiona tu linea'),
  );


  $form['Reportar']['tol_titulo_reportar'] = array(
    '#type' => 'textfield',
    '#title' => t('Titulo Para reportar linea o equipo'),
    '#default_value' => variable_get('tol_titulo_reportar'),
    '#description' => t('Titulo de validación de reportar linea o equipo'),
  );

  $form['Reportar']['tol_linea_equipo'] = array(
    '#type' => 'textfield',
    '#title' => t('Reportar Linea/equipo'),
    '#default_value' => variable_get('tol_linea_equipo'),
    '#description' => t('Reportar equipo'),
  );

  $form['Reportar']['tol_report_línea'] = array(
    '#type' => 'textfield',
    '#title' => t('Reportar linea'),
    '#default_value' => variable_get('tol_report_línea'),
    '#description' => t('Reportar linea'),
  );

  $form['Reportar']['tol_report_equipo'] = array(
    '#type' => 'textfield',
    '#title' => t('Reportar equipo'),
    '#default_value' => variable_get('tol_report_equipo'),
    '#description' => t('Reportar linea'),
  );

  $form['saldo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Consulta saldo'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['saldo']['tol_consulta_saldo'] = array(
    '#type' => 'textfield',
    '#title' => t('Consulta Saldo'),
    '#default_value' => variable_get('tol_consulta_saldo'),
    '#description' => t('Consulta saldo'),
  );

  $form['saldo']['tol_mostrar_msj'] = array(
    '#type' => 'radios',
    '#title' => t('Etiquetas Recargas'),
    '#default_value' => variable_get('tol_mostrar_msj'),
    '#options' => $active,
  );

  $form['saldo']['tol_etiqueta_saldo_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Nid Etiqueta Saldo'),
    '#default_value' => variable_get('tol_etiqueta_saldo_nid'),
    '#size' => 5,
  );

  $form['saldo']['tol_etiqueta_recarga_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Nid Etiqueta Recarga'),
    '#default_value' => variable_get('tol_etiqueta_recarga_nid'),
    '#size' => 5,
  );

  $form['consumo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Consumo'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['consumo']['tol_consumo_text1'] = array(
    '#type' => 'textfield',
    '#title' => t('Titulo Minutos'),
    '#default_value' => variable_get('tol_consumo_text1', 'Minutos'),
    '#size' => 30,
  );

  $form['consumo']['tol_consumo_text2'] = array(
    '#type' => 'textfield',
    '#title' => t('Titulo Mensajes'),
    '#default_value' => variable_get('tol_consumo_text2', 'Mensajes'),
    '#size' => 30,
  );

  $form['consumo']['tol_consumo_text3'] = array(
    '#type' => 'textfield',
    '#title' => t('Titulo Kilobytes'),
    '#default_value' => variable_get('tol_consumo_text3', 'Kilobytes'),
    '#size' => 30,
  );

  $form['consumo']['tol_consumo_text_footer'] = array(
    '#type' => 'textfield',
    '#title' => t('Footer'),
    '#default_value' => variable_get('tol_consumo_text_footer', 'comunicarse al *611'),
    '#size' => 30,
  );

  $form['sclave'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activar o desactivar segunda clave'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Activar'), 0 => t('Desactivar'));

  $form['sclave']['tol_flag_sclave'] = array(
  '#type' => 'radios',
  '#title' => t('Activar o desactivar'),
  '#default_value' => variable_get('tol_flag_sclave', 0),
  '#options' => $active,
);

  $form['sms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activar o desactivar SMS de confirmacion'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Activar'), 0 => t('Desactivar'));
  $form['sms']['tol_flag_sms'] = array(
	  '#type' => 'radios',
	  '#title' => t('Activar o desactivar complemento'),
	  '#default_value' => variable_get('tol_flag_sms', 0),
	  '#options' => $active,
	);

  $active = array(1 => t('Activar'), 0 => t('Desactivar'));
  $form['sms']['tol_flag_sms_registro'] = array(
    '#type' => 'radios',
    '#title' => t('Activar o desactivar registro'),
    '#default_value' => variable_get('tol_flag_sms_registro', 0),
    '#options' => $active,
  );


	//added RVS 20130403
	$form['required_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Campos requeridos para completar datos de perfil'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Requerido'), 0 => t('No Requerido'));

  $form['required_field']['tol_flag_required_field_tipo_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Tipo Documento'),
	  '#default_value' => variable_get('tol_flag_required_field_tipo_documento', 0),
	  '#options' => $active,
	);

	$form['required_field']['tol_flag_required_field_nro_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Numero Documento'),
	  '#default_value' => variable_get('tol_flag_required_field_nro_documento', 0),
	  '#options' => $active,
	);


	$active = array(1 => t('Si'), 0 => t('No'));

	$form['required_field']['tol_flag_required_service_validation'] = array(
	  '#type' => 'radios',
	  '#title' => t('Se requiere validacion (MSISDN con Num. Docum) con servicio de Tigo?'),
	  '#default_value' => variable_get('tol_flag_required_service_validation', 0),
	  '#options' => $active,
	);



	//added RVS 20130505
	//Asociar linea
	$form['required_steps'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pasos requeridos para agregar una linea'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Requerido'), 0 => t('No Requerido'));

  $form['required_steps']['tol_flag_required_step_security_code'] = array(
	  '#type' => 'radios',
	  '#title' => t('Codigo de Seguridad'),
	  '#default_value' => variable_get('tol_flag_required_step_security_code', 1),
	  '#options' => $active,
	);

	$form['required_steps']['tol_flag_required_step_enter_document_info'] = array(
	  '#type' => 'radios',
	  '#title' => t('Ingresar informacion de documento de identidad'),
	  '#default_value' => variable_get('tol_flag_required_step_enter_document_info', 1),
	  '#options' => $active,
	);

	$form['required_steps']['tol_flag_required_step_security_questions'] = array(
	  '#type' => 'radios',
	  '#title' => t('Responder preguntas de Seguridad'),
	  '#default_value' => variable_get('tol_flag_required_step_security_questions', 1),
	  '#options' => $active,
	);

	$active = array(1 => t('Si'), 0 => t('No'));

	$form['required_steps']['tol_flag_required_validation_document'] = array(
	  '#type' => 'radios',
	  '#title' => t('Se requiere validacion (MSISDN con Num. Docum) ?'),
	  '#default_value' => variable_get('tol_flag_required_validation_document', 0),
	  '#options' => $active,
	);
	$form['required_steps']['tol_plan_validation_document'] = array(
    '#type' => 'textfield',
    '#title' => t('Tipo de plan que requiere la validación (MSISDN con Num. Docum)'),
    '#default_value' => variable_get('tol_plan_validation_document', ''),
    '#description' => t('POS, PRE, HIB'),
    '#maxlength' => 15,
    '#size'=> 15,
   );
  
	//added RVS 20130522
	//Bloquear linea
	$form['required_steps_block_line'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pasos requeridos para Bloquear una linea'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $active = array(1 => t('Requerido'), 0 => t('No Requerido'));

  $form['required_steps_block_line']['tol_flag_required_steps_block_line_form'] = array(
	  '#type' => 'radios',
	  '#title' => t('Formulario de datos de usuario'),
	  '#default_value' => variable_get('tol_flag_required_steps_block_line_form', 1),
	  '#options' => $active,
	);



	//Configuración de campos Editables en los datos del cliente

	$form['modifiable_field_ClientInfo_prepaid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Campos editables para completar datos del cliente PREPAGO'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $active = array(1 => t('No Editable'), 0 => t('Editable'));
  $form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_nombre'] = array(
	  '#type' => 'radios',
	  '#title' => t('Nombre'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_nombre', 0),
	  '#options' => $active,
	);
   $form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_apellido'] = array(
	  '#type' => 'radios',
	  '#title' => t('Apellido'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_apellido', 0),
	  '#options' => $active,
	);
   $form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_tipo_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Tipo Documento'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_tipo_documento', 0),
	  '#options' => $active,
	);
	$form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_numero_documento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Numero Documento'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_numero_documento', 0),
	  '#options' => $active,
	);
	$form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_direccion'] = array(
	  '#type' => 'radios',
	  '#title' => t('Dirección'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_direccion', 0),
	  '#options' => $active,
	);
	$form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_telefono'] = array(
	  '#type' => 'radios',
	  '#title' => t('Teléfono'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_telefono', 0),
	  '#options' => $active,
	);
	$form['modifiable_field_ClientInfo_prepaid']['tol_flag_modifiable_field_correo'] = array(
	  '#type' => 'radios',
	  '#title' => t('Correo'),
	  '#default_value' => variable_get('tol_flag_modifiable_field_correo', 0),
	  '#options' => $active,
	);
	$active = array(1 => t('Mostrar'), 0 => t('Ocultar'));
	$form['modifiable_field_ClientInfo_prepaid']['tol_show_modifiable_field_departamento'] = array(
	  '#type' => 'radios',
	  '#title' => t('Departamento y Ciudad'),
	  '#default_value' => variable_get('tol_show_modifiable_field_departamento', 0),
	  '#options' => $active,
	);
	


  return system_settings_form($form);
}
