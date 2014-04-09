<?php


function factura_por_email_reports($form, &$form_state){
    $form = array();

    if (!db_table_exists('factura_por_email_status_log')){
        $form['update'] = array(
            '#type'     => 'item',
            '#markup'   => t('Se produjo un error durante la instalacion del modulo, por favor instalelo nuevamente.'),
        );
        return $form;
    }

    $page = factura_por_email_get_status_reports();
    $values = @$_SESSION['factura_por_email']['reports'] ? $_SESSION['factura_por_email']['reports'] : array();

    $form['factura_por_email_search'] = array(
        '#type'         => 'fieldset',
        '#title'        => t('Search'),
        '#collapsible'  => true,
        '#collapsed'    => false,

        'msisdn' => array(
            '#type'     => 'textfield',
            '#title'    => t('MSISDN'),
            '#default_value' => @$values['msisdn'],
        ),

        'start_date' => array(
            '#type'     => 'date',
            '#title'    => t('Start date'),
            '#default_value' => @$values['start_date'],
            '#after_build' => array('factura_por_email_set_year_range'),
        ),

        'end_date' => array(
            '#type'     => 'date',
            '#title'    => t('End date'),
            '#default_value' => @$values['end_date'],
            '#after_build' => array('factura_por_email_set_year_range'),
        ),

        'email' => array(
            '#type'     => 'textfield',
            '#title'    => t('Email'),
            '#default_value' => @$values['email'],
        ),

        'status' => array(
            '#type'     => 'select',
            '#title'    => t('Status'),
            '#default_value' => array(@$values['status'] => @$values['status']),
            '#options'  => array(
                '_none' => t('Select all'),
                '1' => t('Complete'),
                '0' => t('Incomplete'),
            ),
        ),

        'results' => array(
            '#type'     => 'textfield',
            '#title'    => t('Results'),
            '#default_value' => (int)$page['total'],
            '#disabled' => true,
        ),
    );

    $form['factura_por_email_search']['submit'] = array(
        '#type'     => 'submit',
        '#value'    => 'Search',
        '#submit'   => array('factura_por_email_reports_submit'),
    );

    $form['factura_por_email_search']['export'] = array(
        '#type'     => 'submit',
        //'#name'     => 'export',
        '#value'    => 'Export to CSV',
        '#submit'   => array('factura_por_email_reports_export'),
    );

    $form['factura_por_email_pager_top'] = array(
        '#type'     => 'item',
        '#markup'   => $page['pager'],
    );

    $form['factura_por_email_table'] = array(
        '#type'     => 'item',
        '#markup'   => $page['table'],
    );

    $form['factura_por_email_pager_bottom'] = array(
        '#type'     => 'item',
        '#markup'   => $page['pager'],
    );

    return $form;
}

function factura_por_email_set_year_range($form_element, $form_values) {
    $form_element['year']['#options'] = drupal_map_assoc(range(2013, 2030));
    return $form_element;
}

function factura_por_email_reports_submit(&$form, &$form_state){
    $_SESSION['factura_por_email']['reports'] = $form_state['values'];
    $form_state['redirect'] = @$_GET['q'];
    $pause = null;
}

function factura_por_email_reports_export(&$form, &$form_state){
    $data = factura_por_email_get_status_list($step = -1, $count = false);
    factura_por_email_output_csv($data);
    $pause = null;
}

function factura_por_email_output_csv($data) {

    // send response headers to the browser
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment; filename=factura_por_email_report.csv');
    drupal_add_http_header('Pragma', 'no-cache');
    drupal_add_http_header('Expires', '0');

    $delimiter = ';';
    $outputBuffer = fopen("php://output", 'w');
    if(isset($data[0])){
        $keys = array_keys((array)$data[0]);
        fputcsv($outputBuffer, $keys, $delimiter);
    }
    foreach($data as $val) {
        fputcsv($outputBuffer, (array)$val, $delimiter);
    }
    fclose($outputBuffer);
    drupal_exit();
}

/*
 * Pages
 */
function factura_por_email_get_status_reports(){

    $limit = FACTURA_POR_EMAIL_REPORTS_PAGER_LIMIT;
    $step = @$_GET['page'] ? $_GET['page'] : '0';
    $step = intval($step);
    $reports = factura_por_email_get_status_list($step);
    $total = factura_por_email_get_status_list($step = 0, $count = true);

    $table = factura_por_email_status_table($reports);
    pager_default_initialize($total, $limit, $element = 0);
    $pager = theme('pager', array('quantity' => $total));

    return array('table' => $table, 'pager' => $pager, 'total' => $total);
}

/*
 * Get data
 */
function factura_por_email_get_status_list($step = 0, $count = false){
    //$result = 5;
    $limit = FACTURA_POR_EMAIL_REPORTS_PAGER_LIMIT;
    $start = $step * $limit;
    $values = @$_SESSION['factura_por_email']['reports'] ? $_SESSION['factura_por_email']['reports'] : array();
    $query = db_select('factura_por_email_status_log', 'fe');

    // query values
    $vars = new stdClass();
    $vars->msisdn       = @$values['msisdn']        ? $values['msisdn'] : '';
    $vars->start_date   = @$values['start_date']    ? $values['start_date'] : '';
    $vars->end_date     = @$values['end_date']      ? $values['end_date'] : '';
    $vars->email        = @$values['email']         ? $values['email'] : '';
    $vars->status       = @$values['status'];

    // get fields
    $query->fields('fe');

    // msisdn
    $query->condition('fe.msisdn', '%' . $vars->msisdn . '%', 'LIKE');

    // dates range
    if(isset($vars->start_date['year'])){
        $d = $vars->start_date;
        $start_date = $d['year'] . sprintf("%02s", $d['month']) . sprintf("%02s", ($d['day'] - 1));
        $d = $vars->end_date;
        $end_date   = $d['year'] . sprintf("%02s", $d['month']) . sprintf("%02s", $d['day']);
        $query->condition('fe.tdate', array($start_date.'%', $end_date.'%'), 'BETWEEN');
    }

    // email
    $query->condition('fe.email', '%' . $vars->email . '%', 'LIKE');

    if(in_array($vars->status, array('0', '1'))){
        $query->condition('fe.status', $vars->status, '=');
    }

    if($count == true){
        // only count rows
        $rows = $query->execute();
        $items = $rows->rowCount();
    }else{
        $query->orderBy('fe.tid', 'DESC');
        if($step >= 0){
            $query->range($start, $limit);
        }
        $result = $query->execute();
        $items = array();
        foreach($result as $value){
            $items[] = $value;
        }
    }


    return $items;
}

/*
 * Get table
 */
function factura_por_email_status_table($records){

    $rows = array();
    $header = array(
        //'tid'       => 'TID',
        'terms_date'=> t('Fecha: YYYYMMDD HH MM SS'),
        'msisdn'    => t('msisdn'),
        'email'     => t('Email'),
        'status'    => t('Status'),
    );

    foreach($records as $key => $record){

        $rows[] = array(
            //'tid'       => $record->tid,
            'terms_date'=> $record->tdate,
            'msisdn'    => $record->msisdn,
            'email'     => $record->email,
            'status'    => $record->status,
        );
    }

    return theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'id' => 'table-account-status',
        'class' => 'articles',
    ));
}
