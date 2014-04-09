<?php

function factura_por_email_check_msisdn_ws($msisdn){

    $msisdn_array = array(
        '3013383387', //charles.romestant
        '3012408453', //ivan.olivos
        '3012393773', //cristian.sparacino
        '3002018356',
        '3003899731',
        '3003333333', // test false
    );

    //$response = unserialize('O:8:"stdClass":7:{s:14:"expirationDate";s:16:"2013-08-02-05:00";s:7:"balance";s:1:"0";s:10:"lastAmount";s:5:"20001";s:8:"contract";s:10:"8840084705";s:17:"lastInvoiceNumber";s:12:"B1-575166777";s:11:"unpaidBills";i:1;s:16:"additionalResult";O:8:"stdClass":1:{s:13:"ParameterType";O:8:"stdClass":3:{s:13:"parameterName";s:7:"message";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:75:"No tienes pagos pendientes en el momento. Tu Referencia pago es: 8840084705";}}}');
//    $response = false;

    module_load_include('inc','tigoapi');
    $tigo = TigoAPI::getInstance();
    $response = $tigo->GetAccountDebt($msisdn);
    //drupal_set_message('msisdn: ' . serialize($response));

    return $response;
}

function factura_por_email_connect_electronic_invoice_ws($msisdn, $email){

    $dev = false;
    $invoiceOption = factura_por_email_get_ws_invoiceOption();
    $summaryOption = factura_por_email_get_ws_summaryOption();
    $detailOption  = factura_por_email_get_ws_detailOption();
    $action        = factura_por_email_get_ws_action();

    if(!$dev){
        //Para enableDisableElectronicInvoice se creo una librería especial ya que este servicio se implemento fuera del stack de Tigo API:
        module_load_include('inc', 'tigoapi', 'enableDisableElectronicInvoice');
        $tigo1 = enableDisableElectronicInvoiceClient::getInstance();
        $enable = $tigo1->EnableDisableElectronicInvoice($msisdn, $invoiceOption, $summaryOption, $detailOption, $action);
        //$enable = unserialize('O:8:"stdClass":1:{s:7:"success";b:1;}');

        module_load_include('inc','tigoapi', 'tigoapi');
        $tigo2 = TigoAPI::getInstance();
        $data   = $tigo2->GetClientAccountGeneralInfo($msisdn, 'MSISDN', 1, 1);
        //$data   = unserialize('O:8:"stdClass":3:{s:10:"ClientType";O:8:"stdClass":9:{s:13:"documenNumber";s:8:"72286582";s:12:"documentType";s:2:"CC";s:4:"name";s:17:"IVAN OLIVOS AARON";s:13:"firstLastName";s:12:"OLIVOS AARON";s:11:"phoneNumber";s:10:"3012408453";s:5:"email";s:21:"olivos.ivan@gmail.com";s:3:"fax";s:9:"PLCON0191";s:12:"businessName";s:18:"IVAN  OLIVOS AARON";s:7:"scoring";s:1:"P";}s:9:"contracts";O:8:"stdClass":1:{s:12:"ContractType";O:8:"stdClass":3:{s:5:"state";s:6:"Activo";s:8:"accounts";O:8:"stdClass":1:{s:9:"AssetType";O:8:"stdClass":6:{s:6:"msisdn";s:10:"3012408453";s:4:"imsi";s:15:"732103056295421";s:4:"imei";s:15:"356936052777732";s:10:"nominative";b:0;s:5:"plans";O:8:"stdClass":1:{s:8:"PlanType";O:8:"stdClass":5:{s:8:"planName";s:30:"Plan Tigo Empleados Control AN";s:8:"planType";s:3:"HIB";s:8:"planCode";s:9:"PLCON0191";s:15:"planDescription";s:30:"Plan Tigo Empleados Control AN";s:12:"dataPlanOnly";b:0;}}s:8:"services";O:8:"stdClass":1:{s:16:"AssetServiceType";O:8:"stdClass":2:{s:7:"service";s:13:"TELEFONIA_POS";s:5:"state";s:6:"Activo";}}}}s:8:"profiles";O:8:"stdClass":1:{s:19:"ContractProfileType";O:8:"stdClass":4:{s:12:"billingCycle";s:2:"15";s:14:"billingAddress";s:24:"CL 105 # 17 - 46 AP 301 ";s:4:"city";s:6:"BOGOTA";s:10:"department";s:10:"BOGOTA D.E";}}}}s:17:"additionalResults";a:6:{i:0;O:8:"stdClass":3:{s:13:"parameterName";s:3:"Dsn";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:10:"8840084705";}i:1;O:8:"stdClass":3:{s:13:"parameterName";s:16:"CollectionsLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:5:"CA+LA";}i:2;O:8:"stdClass":3:{s:13:"parameterName";s:10:"FraudLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:1:"A";}i:3;O:8:"stdClass":3:{s:13:"parameterName";s:11:"SiebelLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:8:"SA+CA+FA";}i:4;O:8:"stdClass":3:{s:13:"parameterName";s:11:"PrepayLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:1:"A";}i:5;O:8:"stdClass":3:{s:13:"parameterName";s:7:"PartNum";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:9:"PLCON0191";}}}');

        $items = array(
            'msisdn' => $msisdn,
            'documentType' => $data->ClientType->documentType,
            'documenNumber' => $data->ClientType->documenNumber,
            'documentExpeditionDate' => '',
            'nationality' => '',
            'clientCode' => '',
            'name' => '',
            'firstLastName' => '',
            'secondLastName' => '',
            'phoneNumber' => '',
            'email' => $email,
            'fax' => '',
            'ruc' => '',
            'businessName' => '',
            'clientType' => '',
            'subType' => '',
            'economicActivity' => '',
            'admissionDate' => '',
            'segmentation' => '',
            'scoring' => '',
            'baseCurrency' => '',
            'urlClient' => '',
            'guaranteesNumber' => '',
            'subClientState' => '',
            'clientState' => '',
            'genre' => '',
            'address' => $data->contracts->ContractType->profiles->ContractProfileType->billingAddress,
        );

        module_load_include('inc', 'tigoapi', 'tigoapi');
        $tigo3 = TigoAPI::getInstance();
        $update = $tigo3->UpdateClientInfo($items);

        //drupal_set_message('enable_invoice: ' . serialize($enable)); // N;
        //drupal_set_message('account_data:   ' . serialize($data));   // b:0;
        //drupal_set_message('update_data: ' . serialize($update)); // b:0;

    }else{
        $enable = unserialize('O:8:"stdClass":1:{s:7:"success";b:1;}');
        $data   = unserialize('O:8:"stdClass":3:{s:10:"ClientType";O:8:"stdClass":9:{s:13:"documenNumber";s:8:"72286582";s:12:"documentType";s:2:"CC";s:4:"name";s:17:"IVAN OLIVOS AARON";s:13:"firstLastName";s:12:"OLIVOS AARON";s:11:"phoneNumber";s:10:"3012408453";s:5:"email";s:21:"olivos.ivan@gmail.com";s:3:"fax";s:9:"PLCON0191";s:12:"businessName";s:18:"IVAN  OLIVOS AARON";s:7:"scoring";s:1:"P";}s:9:"contracts";O:8:"stdClass":1:{s:12:"ContractType";O:8:"stdClass":3:{s:5:"state";s:6:"Activo";s:8:"accounts";O:8:"stdClass":1:{s:9:"AssetType";O:8:"stdClass":6:{s:6:"msisdn";s:10:"3012408453";s:4:"imsi";s:15:"732103056295421";s:4:"imei";s:15:"356936052777732";s:10:"nominative";b:0;s:5:"plans";O:8:"stdClass":1:{s:8:"PlanType";O:8:"stdClass":5:{s:8:"planName";s:30:"Plan Tigo Empleados Control AN";s:8:"planType";s:3:"HIB";s:8:"planCode";s:9:"PLCON0191";s:15:"planDescription";s:30:"Plan Tigo Empleados Control AN";s:12:"dataPlanOnly";b:0;}}s:8:"services";O:8:"stdClass":1:{s:16:"AssetServiceType";O:8:"stdClass":2:{s:7:"service";s:13:"TELEFONIA_POS";s:5:"state";s:6:"Activo";}}}}s:8:"profiles";O:8:"stdClass":1:{s:19:"ContractProfileType";O:8:"stdClass":4:{s:12:"billingCycle";s:2:"15";s:14:"billingAddress";s:24:"CL 105 # 17 - 46 AP 301 ";s:4:"city";s:6:"BOGOTA";s:10:"department";s:10:"BOGOTA D.E";}}}}s:17:"additionalResults";a:6:{i:0;O:8:"stdClass":3:{s:13:"parameterName";s:3:"Dsn";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:10:"8840084705";}i:1;O:8:"stdClass":3:{s:13:"parameterName";s:16:"CollectionsLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:5:"CA+LA";}i:2;O:8:"stdClass":3:{s:13:"parameterName";s:10:"FraudLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:1:"A";}i:3;O:8:"stdClass":3:{s:13:"parameterName";s:11:"SiebelLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:8:"SA+CA+FA";}i:4;O:8:"stdClass":3:{s:13:"parameterName";s:11:"PrepayLayer";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:1:"A";}i:5;O:8:"stdClass":3:{s:13:"parameterName";s:7:"PartNum";s:13:"parameterType";s:6:"STRING";s:14:"parameterValue";s:9:"PLCON0191";}}}');
        //error
        //$update = unserialize('O:8:"stdClass":2:{s:7:"success";b:0;s:5:"Error";O:8:"stdClass":3:{s:9:"errorType";s:3:"NEG";s:6:"reason";s:27:"Datos de entrada inválidos";s:11:"description";s:27:"Datos de entrada inválidos";}}');
        //success
        $update = unserialize('O:8:"stdClass":1:{s:7:"success";b:1;}');
    }

    return array(
        'enable' => $enable->success,
        'data'   => isset($data->ClientType) ? true : false,
        'update' => $update->success,
    );
}
