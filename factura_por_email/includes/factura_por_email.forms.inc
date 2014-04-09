<?php

/*
 * A collection of forms, hook_validate and hook_submit functions
 */

/*
 * Form factura_por_email
 */
function factura_por_email_form($form, &$form_state){

    $form['field_legend'] = array(
        '#type' => 'item',
        '#markup' => factura_por_email_get_step1_legend(),
    );

    $form['field_cellphone_number'] = array(
        '#type' => 'textfield',
        '#title' => t('N&uacute;mero del m&oacute;vil'),
        //'#required' => true,
        '#attributes' => array('placeholder' => t('Ingresa el numero')),
    );

    $form['hr1'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['field_email1'] = array(
        '#type' => 'textfield',
        '#title' => t('Correo electr&oacute;nico'),
        //'#required' => true,
        '#suffix' => '<div class="email1_status messages"> </div>',
        '#attributes' => array(
            'placeholder' => t('Ingresa el correo electronico'),
            'class' => array('test-email'),
        ),
    );

    $form['hr2'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['field_email2'] = array(
        '#type' => 'textfield',
        '#title' => t('Confirmar correo'),
        //'#required' => true,
        '#suffix' => '<div class="email2_status messages warning"> </div>',
        '#attributes' => array(
            'placeholder' => t('Ingresa el correo electronico'),
            'class' => array('email-compare'),
        ),
    );

    $form['hr3'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['actions'] = array(
        '#type' => 'actions',
        'cancel' => array(
            '#type' => 'item',
            '#markup' => '<a href="/">'.t('Cancelar').'</a>',
        ),
        'submit' => array(
            '#type' => 'item',
            '#markup' => t(''),
            '#prefix' => '<div id="factura-por-email-submit">',
            '#suffix' => '</div>',
        ),
        'continue' => array(
            '#type'     => 'submit',
            '#submit'   => array('factura_por_email_form_submit'),
        ),
    );

    $form['#validate'] = array('factura_por_email_form_validate');

    return $form;
}

function factura_por_email_form_validate(&$form, $form_state){
    $values = $form_state['values'];
    if(!is_numeric($values['field_cellphone_number'])){
        form_set_error('field_cellphone_number', t('Debe ingresar un numero.'));
    }
    if(valid_email_address($values['field_email1'])){
        if($values['field_email1'] != $values['field_email1']){
            form_set_error('field_email2', t('Las direcciones deben coincidir.'));
        }
    }else{
        form_set_error('field_email2', t('Debe ingresar una direccion de correo valida.'));
    }
    $pause = null;
}

function factura_por_email_form_submit(&$form, &$form_state){

    $request_time = $_SERVER['REQUEST_TIME'];
    $values['msisdn'] = $form_state['values']['field_cellphone_number'];
    $values['email']  = $form_state['values']['field_email1'];
    $values['tdate']  = date('Ymd H i s', $request_time);
    $values['status'] = 0; // 0: in progress - 1: complete

    // write the record in table
    $result = drupal_write_record('factura_por_email_status_log', $values);
    $redirect_path = variable_get('factura_por_email_page', 'factura-por-email');
    if($result == false){
        // if record not saved go to error page
        drupal_goto($redirect_path . '/error');
    }else{
        // if record is saved set session and continue
        $_SESSION['factura_por_email']['record'] = $values;
        $form_state['redirect'] = $redirect_path . '/token';
    }
    /* to check if proccess exists
    $record = factura_por_email_get_record_by_msisdn((int)$values['msisdn']);
    $tdate1 = explode(' ', $values['tdate']);
    $tdate2 = explode(' ', $record['tdate']);

    if(!isset($_SESSION['factura_por_email']['record'][$values['msisdn']]['tid']) || $tdate1[0] != $tdate2[0]){
        drupal_write_record('factura_por_email_status_log', $values);
        $_SESSION['factura_por_email']['record'] = $values;
        //$_SESSION['factura_por_email']['record'][$values['msisdn']] = $values;
    }else{
        $_SESSION['factura_por_email']['record'][$values['msisdn']]['email'] = $values['email'];
    }
    */
    $pause = null;
}

/*
 * Form terms & conditions factura_por_email
 */
function factura_por_email_terms_form($form){

    $form['acepto_terminos_condiciones'] = array(
        '#type' => 'checkbox',
        '#title' => t('Acepto los términos y condiciones'),
        '#default_value' => 0,
    );

    return $form;
}

/*
 * Form factura_por_email token
 */
function factura_por_email_token_form($form, &$form_state){

    $values = isset($_SESSION['factura_por_email']['record']['tid']) ? $_SESSION['factura_por_email']['record'] : false;
    if($values == false){
        drupal_set_message(t('Debe completar la solicitud de suscripci&oacute;n antes de ir al siguiente paso.'), 'warning');
        drupal_goto(variable_get('factura_por_email_page', 'factura-por-email'));
    }
    $form['field_legend'] = array(
        '#type' => 'item',
        '#markup' => factura_por_email_get_step2_legend(),
    );

    $form['field_msisdn'] = array(
        '#type' => 'textfield',
        '#title' => t('N&uacute;mero del m&oacute;vil'),
        '#value' => $values['msisdn'],
        '#attributes' => array('readonly' => 'readonly', 'disabled' => 'disabled'),
    );

    $form['hr1'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['field_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Correo electr&oacute;nico'),
        '#value' => $values['email'],
        '#attributes' => array('readonly' => 'readonly', 'disabled' => 'disabled'),
    );

    $form['hr2'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['field_token'] = array(
        '#type' => 'textfield',
        '#title' => t('C&oacute;digo de seguridad'),
        '#suffix' => '<div class="factura-por-email-reenviar-token">Reenviar</div>',
        '#attributes' => array('placeholder' => t('Ingresa el numero')),
    );

    $form['hr3'] = array(
        '#type' => 'item',
        '#markup' => '<hr>',
    );

    $form['actions'] = array(
        '#type' => 'actions',
        'cancel' => array(
            '#type' => 'item',
            '#markup' => '<a href="/">'.t('Cancelar').'</a>',
        ),
        'submit' => array(
            '#type' => 'item',
            '#markup' => t(''),
            '#prefix' => '<div id="factura-por-email-submit">',
            '#suffix' => '</div>',
        ),
        'continue' => array(
            '#type'     => 'submit',
            '#submit'   => array('factura_por_email_token_form_submit'),
        ),
    );

    $form['#validate'] = array('factura_por_email_token_form_validate');

    return $form;
}


function factura_por_email_token_form_validate(&$form, &$form_state){
    $success = false;
    $token = $form_state['values']['field_token'];
    if($token != ''){
        $success = factura_por_email_validate_token($token);
    }
    if($success == false){
        $msg = factura_por_email_get_token_error();
        form_set_error('field_token', $msg);
    }
    $pause = null;
}


function factura_por_email_token_form_submit(&$form, &$form_state){
    //$result = false;
    $_SESSION['factura_por_email']['update_email'] = false;
    $values = $_SESSION['factura_por_email']['record'];
    $redirect_path = variable_get('factura_por_email_page', 'factura-por-email');

    // todo: enableDisableElectronicInvoice
    $electronic_invoice = factura_por_email_set_electronic_invoice_status((int)$values['msisdn'], $values['email']);

    // todo: save new status in db
    // write the record in table
    if($electronic_invoice){
        $values['status'] = 1;
        $record = $values;
        unset($record['tid']);
        //$result = drupal_write_record('factura_por_email_status_log', $values);
        $result = db_update('factura_por_email_status_log')
            ->fields($record)
            ->condition('tid', $values['tid'], '=')
            ->execute();
        $_SESSION['factura_por_email']['record'] = $values;

        //if(isset($electronic_invoice['email'])){
            $_SESSION['factura_por_email']['update_email'] = true;
        //}
        $form_state['redirect'] = $redirect_path . '/confirmacion';
    }else{
        // if record not saved go to error page
        drupal_goto($redirect_path . '/error');
    }

    $pause = null;
}
