<?php
/*
 * SummaSolutions.net
 */

/*
 * TIGO factura_por_email admin settings
 */
function factura_por_email_admin(){
    $form = array();

    $form['factura_por_email_group'] = array(
        '#type' => 'fieldset',
        '#title' => t('Factura por Email Settings'),
        '#collapsible' => false,
        '#collapsed' => false,

        'factura_por_email_url_group' => array(
            '#type' => 'fieldset',
            '#title' => t('Configuracion de URL&#39;s(paths)'),
            '#collapsible' => true,
            '#collapsed' => true,

            'factura_por_email_web' => array(
                '#type'          => 'textfield',
                '#title'         => t('Url de subscripcion a factura por email'),
                '#default_value' => variable_get('factura_por_email_page', 'factura-por-email'),
                '#description'   => t('Url de la pagina de subscripcion a factura por email. Por defecto: factura-por-email'),
                '#required'      => FALSE,
            ),

            'factura_por_email_sms_sender' => array(
                '#type'          => 'textfield',
                '#title'         => t('SMS Sender'),
                '#default_value' => factura_por_email_get_sms_sender(),
                '#description'   => t('Configuracion del sms sender'),
                '#required'      => FALSE,
            ),
        ),

        'factura_por_email_ws_group' => array(
            '#type' => 'fieldset',
            '#title' => t('Configuracion de WS electronic invoice'),
            '#collapsible' => true,
            '#collapsed' => true,

            'factura_por_email_ws_invoiceOption' => array(
                '#type'          => 'textfield',
                '#title'         => t('Parametro invoiceOption'),
                '#default_value' => factura_por_email_get_ws_invoiceOption(),
                '#description'   => t('Configuracion del parametro <b>invoiceOption</b> para ws electronic invoice'),
                '#required'      => FALSE,
            ),

            'factura_por_email_ws_summaryOption' => array(
                '#type'          => 'textfield',
                '#title'         => t('Parametro summaryOption'),
                '#default_value' => factura_por_email_get_ws_summaryOption(),
                '#description'   => t('Configuracion del parametro <b>summaryOption</b> para ws electronic invoice'),
                '#required'      => FALSE,
            ),

            'factura_por_email_ws_detailOption' => array(
                '#type'          => 'textfield',
                '#title'         => t('Parametro detailOption'),
                '#default_value' => factura_por_email_get_ws_detailOption(),
                '#description'   => t('Configuracion del parametro <b>detailOption</b> para ws electronic invoice'),
                '#required'      => FALSE,
            ),

            'factura_por_email_ws_action' => array(
                '#type'          => 'textfield',
                '#title'         => t('Parametro action'),
                '#default_value' => factura_por_email_get_ws_action(),
                '#description'   => t('Configuracion del parametro <b>action</b> para ws electronic invoice'),
                '#required'      => FALSE,
            ),
        ),

        'factura_por_email_msg_group' => array(
            '#type' => 'fieldset',
            '#title' => t('Configuracion de mensajes'),
            '#collapsible' => true,
            '#collapsed' => true,

            'factura_por_email_step1_legend' => array(
                '#type'          => 'textarea',
                '#title'         => t('Descripcion formulario de subscripcion'),
                '#default_value' => factura_por_email_get_step1_legend(),
                '#description'   => t('Texto que aparece como indicacion en la pagina de subscripcion.'),
                '#required'      => FALSE,
            ),

            'factura_por_email_step2_legend' => array(
                '#type'          => 'textarea',
                '#title'         => t('Descripcion formulario de ingreso de token.'),
                '#default_value' => factura_por_email_get_step2_legend(),
                '#description'   => t('Texto que aparece como indicacion en la pagina de ingreso de token.'),
                '#required'      => FALSE,
            ),

            'factura_por_email_token_error' => array(
                '#type'          => 'textarea',
                '#title'         => t('Mensaje de token erroneo.'),
                '#default_value' => factura_por_email_get_token_error(),
                '#description'   => t('Mensaje de error al ingresar un codigo de seguridad erroneo.'),
                '#required'      => FALSE,
            ),

            'factura_por_email_msisdn_error' => array(
                '#type'          => 'textarea',
                '#title'         => t('Mensaje de msisdn erroneo.'),
                '#default_value' => factura_por_email_get_msisdn_error(),
                '#description'   => t('Mensaje de error al ingresar un msisdn erroneo o falla del servicio para la validacion.'),
                '#required'      => FALSE,
            ),

            'factura_por_email_page_error' => array(
                '#type'          => 'textarea',
                '#title'         => t('Mensage pagina de error.'),
                '#default_value' => factura_por_email_get_error_msg(),
                '#description'   => t('Mensaje de error al obtener un error en el flujo.'),
                '#required'      => FALSE,
            ),
        ),

        'factura_por_email_terms' => array(
            '#type'          => 'textarea',
            '#title'         => t('Terminos y Condiciones'),
            '#default_value' => factura_por_email_get_step1_terms(),
            '#description'   => t('Texto de los terminos y condiciones.'),
            '#required'      => FALSE,
        ),

        'factura_por_email_respuesta_group' => array(
            '#type' => 'fieldset',
            '#title' => t('Pagina de Respuesta (Confirmacion al usuario)'),
            '#collapsible' => true,
            '#collapsed' => true,

            'factura_por_email_respuesta_title' => array(
                '#type'          => 'textfield',
                '#title'         => t('Titulo'),
                '#default_value' => factura_por_email_get_respuesta_title(),
                //'#description'   => t(''),
                '#required'      => FALSE,
            ),

            'factura_por_email_respuesta_message' => array(
                '#type'          => 'textarea',
                '#title'         => t('Mensaje'),
                '#default_value' => factura_por_email_get_respuesta_message(),
                '#description'   => t('Cuerpo de la pagina de confirmacion, puede utilizar los tokens @email y @msisdn para imprimir la direccion de correo y el numero de movil respectivamente.'),
                '#required'      => FALSE,
            ),
        ),
    );

    $form['#validate'] = array('factura_por_email_admin_validate');
    $form['#submit'] = array('factura_por_email_admin_submit');

    return system_settings_form($form);
}

/*
 * Hook validate
 */
function factura_por_email_admin_validate(&$form, &$form_state){}

/*
 * Hook submit
 */
function factura_por_email_admin_submit(&$form, &$form_state){
    $old_path = variable_get('factura_por_email_web', 'factura-por-email');
    $new_path = $form_state['values']['factura_por_email_web'];
    if($new_path != $old_path){
        variable_set('factura_por_email_web', $new_path);
        menu_rebuild();
        //$pause = null;
    }
    $pause = null;
}

