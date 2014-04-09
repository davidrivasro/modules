<?php

/*
 * A collection of herpers and variable_get
 */

function factura_por_email_get_record_by_msisdn($msisdn = 0){

    $sql = "select tid, msisdn, email, tdate, status from {factura_por_email_status_log}
            where msisdn = :msisdn order by status desc, tdate desc limit 1";
    $result = db_query($sql, array(':msisdn' => $msisdn))->fetchAssoc();

    if($result == false){
        return array(
            'tid'    => 0,
            'msisdn' => 0,
            'email'  => 0,
            'tdate'  => 0,
            'status' => 0,
        );
    }
    return $result;
}

function factura_por_email_send_token($msisdn = null){
    $success = false;
    if($msisdn){
        // get token, set message an save token in session
        $token = factura_por_email_create_token(5);
        $message = t('Codigo de seguridad: ') . $token;
        $_SESSION['factura_por_email']['token'] = $token;

        // get sms sender from config
        $sender = factura_por_email_get_sms_sender();

        // send token, get result = bool
        $result = sms_send($msisdn, $message, array('sender' => $sender));

        // temporal dev assign
           //$result = true;
           //drupal_set_message('SMS: ' . serialize($result));
        if($_SERVER['HTTP_HOST'] == 'sandbox.tigocloud.net')
           drupal_set_message('Token: ' . $token . ' (se imprime para testeo en sandbox)');

        if($result){
            $success = true;
        }
    }

    return $success;
}

function factura_por_email_validate_token($token = null){
    $success = false;
    if($token){
        // get token from session
        $valid_token = @$_SESSION['factura_por_email']['token'] ? $_SESSION['factura_por_email']['token'] : false;
        // validate token return bool
        if($valid_token != false && strtoupper($token) == $valid_token){
            $success = true;
        }
    }

    return $success;
}

function factura_por_email_set_electronic_invoice_status($msisdn = null, $email = null){
    $success = false;
    if($msisdn && $email){
        // set enableDisableElectronicInvoice status
        // get status
        $status = factura_por_email_connect_electronic_invoice_ws($msisdn, $email);
        $success = $status['update'];
    }

    return $success;
}

function factura_por_email_create_token($length = 6){
    $key = '';
    $keys = array_merge(range(1, 9), range('a', 'h'), range('w', 'z')); // avoid 0O Il UV

    for ($i = 0; $i < $length; $i++) {
        $key .= $keys[array_rand($keys)];
    }

    return strtoupper($key);
}

function factura_por_email_get_step1_legend(){
    $legend = t('Para recibir la factura en tu correo electr&oacute;nico, completa el formulario '.
                'y recibir&aacute;s un mensajecon un C&oacute;digo en el m&oacute;vil ingresado. '.
                'Deber&aacute;s ingresar ese c&oacute;digo en el paso siguiente.');
    return variable_get('factura_por_email_step1_legend', $legend);
}

function factura_por_email_get_step2_legend(){
    $legend = t('Se ha enviado un mensaje de texto al m&oacute;vil indicado en el paso anterior. '.
                'Escribe el c&oacute;digo de seguridad que recibiste en el mensaje de texto.');
    return variable_get('factura_por_email_step2_legend', $legend);
}

function factura_por_email_get_token_error(){
    $legend = t('El c&oacute;digo de seguridad es incorrecto, por favor ingrese el c&oacute;digo '.
                'nuevamente o solicite uno nuevo desde el enlace <b>Reenviar</b>.');
    return variable_get('factura_por_email_token_error', $legend);
}

function factura_por_email_get_msisdn_error(){
    $legend = t('<h2>El n&uacute;mero ingresado no es un n&uacute;mero v&aacute;lido o el servicio de '.
                'validacion ha fallado, intente nuevamente en unos minutos.</h2>');
    return variable_get('factura_por_email_msisdn_error', $legend);
}

function factura_por_email_get_step1_terms(){
    $text = t('Tigo Online
ACUERDO DEL USUARIO

Términos y Condiciones para la subscripcion a factura electronica

Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec tortor elit, euismod vitae tempor ut, gravida quis metus. Quisque nec venenatis odio. Cras varius augue lorem. Donec a lobortis urna. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque elementum tempus diam, nec tincidunt tellus laoreet a. Sed quis sapien neque. Phasellus tellus lectus, luctus eget scelerisque in, pharetra sit amet libero. Nulla consectetur auctor ipsum, ut elementum tortor ullamcorper ut. Sed pulvinar condimentum vulputate. Nam pharetra, dui eu sollicitudin luctus, turpis libero tincidunt odio, non aliquam magna ipsum ac orci. Quisque dapibus mauris vel mauris semper sed iaculis urna aliquet. Maecenas mollis, mauris sed tincidunt fringilla, velit eros aliquam turpis, vitae consequat mi enim at purus. Vestibulum eu commodo urna.');

    $terms = variable_get('factura_por_email_terms', $text);
    return $terms;
}

function factura_por_email_get_error_msg(){
    $msg = t('<h2>Ocurri&oacute; un problema al realizar la subscripci&oacute;n de envios de facturas por correo electr&oacute;nico. '.
             'Por favor intentalo mas tarde.</h2>');
    return variable_get('factura_por_email_error_msg', $msg);
}

function factura_por_email_get_respuesta_title(){
    $text = t('Confirmaci&oacute;n');
    return variable_get('factura_por_email_respuesta_title', $text);
}

function factura_por_email_get_respuesta_message(){
    $text = t('Acabas de registrarte al servicio de env&iacute;o de facturas por correo electr&oacute;nico con tu correo '.
              '<b>@email</b>, te enviaremos un mensaje de texto a <b>@msisdn</b> confirmando el estado de tu transacci&oacute;n.');
    return variable_get('factura_por_email_respuesta_message', $text);
}

function factura_por_email_get_sms_sender(){
    return variable_get('factura_por_email_sms_sender', 'INFO.TIGO');
}

function factura_por_email_get_ws_invoiceOption(){
    return variable_get('factura_por_email_ws_invoiceOption', '2');
}

function factura_por_email_get_ws_summaryOption(){
    return variable_get('factura_por_email_ws_summaryOption', 'RESUMIDA');
}

function factura_por_email_get_ws_detailOption(){
    return variable_get('factura_por_email_ws_detailOption', '');
}

function factura_por_email_get_ws_action(){
    return variable_get('factura_por_email_ws_action', 'ENABLE');
}

