<?php
/**
 * Administrative Page
 *
 * @return string
 */
function tecnola_import_page() {
  $content = "";

  $content .= drupal_render(drupal_get_form('tecnola_import_form'));
  $content .= drupal_render(drupal_get_form('tecnola_import_merge_form'));
  $content .= "<h3>Paso 3: Migrar productos a la tienda</h3>";
  $content .= "<p>Ahora está listo para generar los productos para la tienda, tenga en cuenta lo siguiente:</p>
    <ul>
      <li>Esta tarea se realiza desde una sección distinta.</li>
      <li>Desde allí deberá migrar primero los productos y posteriormente las páginas de producto</li>
      <li>Si ya hay productos y páginas de productos migrados, primero deberá hacer Rollback de las páginas de los productos y los productos en ese orden</li>
      <li>Ahora debe estar listo para hacer la migración</li>
    </ul>

    <p>".l('Ir a la página de migración', 'admin/content/migrate')."</p>";

  return $content;
}

/**
 * Builds a form that will allow users to upload csv files
 *   hook_menu()
 */
function tecnola_import_form($form, $form_state) {
  $form['notes'] = array(
    '#type' => 'markup',
    '#markup' => '
      <h3>Paso 1: Importar los archivos de productos</h3>
      <div class="import-notes">Algunas notas a tener en cuenta cuando esté subiendo los archivos.
        <ul>
          <li>Asegúrese de que el archivo se encuentre en formato .csv (Separados por comas).</li>
          <li>Las columnas deberían estar en el orden y formato correcto</li>
          <li>Asegúrese de hacer click en el botón "Importar" después de seleccionar sus archivos CSV.</li>
        </ul>
      </div>',
  );

  $form['import'] = array(
    '#title' => t('Import'),
    '#type' => 'plupload',
    '#description' => t('The uploaded csv will be imported and temporarily saved.'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
    ),
  );

  $form['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit handler for tecnola_import_form()
 */
function tecnola_import_form_submit($form, $form_state){

  $saved_files = array();
  $scheme = variable_get('file_default_scheme', 'public') . '://';
  $dir_name = 'tecnola_imports';
  $imports_path = $scheme . $dir_name;
  $operations = array();

  file_prepare_directory($imports_path, FILE_CREATE_DIRECTORY);

  // We can't use file_save_upload() because of
  // http://www.jacobsingh.name/content/tight-coupling-no-not
  // file_uri_to_object();
  foreach ($form_state['values']['import'] as $uploaded_file) {
    if ($uploaded_file['status'] == 'done') {
      $source = $uploaded_file['tmppath'];
      $destination = file_stream_wrapper_uri_normalize($imports_path . '/' . $uploaded_file['name']);
      // Rename it to its original name, and put it in its final home.
      // Note - not using file_move here because if we call file_get_mime
      // (in file_uri_to_object) while it has a .tmp extension, it horks.
      $destination = file_unmanaged_move($source, $destination, FILE_EXISTS_RENAME);
      $file = plupload_file_uri_to_object($destination);

      file_save($file);
      $saved_files[] = $file;
    }
    else {
      // @todo: move this to element validate or something and clean up t().
      form_set_error('import', "Upload of {$uploaded_file['name']} failed");
    }
  }

  // If the files was uploded, is time to process.

  if(count($saved_files) != 0){

    //Firts, the table should be truncated
    $result = db_truncate('tecnola_import')->execute();

    foreach ($saved_files as $value) {
      // Open the CSV File
      $handle = fopen(drupal_realpath($value->uri), "r");
      $i = 0;

      while(($data = fgetcsv($handle, 0, ',', '"')) != FALSE ) {
        // Create the operations for each Row
        if($i > 0) {
          $operations[] = array(
            'tecnola_import_process_batch_insert',
            array($data),
          );
        }
        $i++;
      }
      fclose($handle);
    }

    $batch = array(
      'title' => t('Importing CSV'),
      'operations' => $operations,
      'finished' => 'tecnola_import_finish_batch_insert',
      'progress_message' => t('Imported @current of @total products.'),
      'error_message' => t('An error was encountered processing the file.'),
      'file' => drupal_get_path('module', 'tecnola_import') . '/tecnola_import.admin.inc',
    );

    batch_set($batch);
  }
  else{
    drupal_set_message(t('There was an error uploading your file. Please contact a System administator.'), 'error');
  }
}

function tecnola_import_process_batch_insert($data, &$context) {

  $newness = (int) $data[32];
  $promotion = (int) $data[33];

  $query = db_insert('tecnola_import')
            ->fields(array(
                'tim_part_number' => trim($data[1], " \t\n\r\0\x0B "),
                'tim_name' => $infodata[3],
                'tim_trademark' => $data[4],
                'tim_group' => $data[5],
                'tim_subgroup' => $data[6],
                'tim_division' => $data[7],
                'tim_subdivision' => $data[8],
                'tim_family' => $data[9],
                'tim_reference' => $data[10],
                'tim_description' => $data[11],
                'tim_vat' => $data[13],
                'tim_provider' => $data[14],
                'tim_price_cop' => $data[15],
                'tim_price_usd' => $data[16],
                'tim_delivery' => $data[17],
                'tim_tags' => $data[18],
                'tim_profit_1' => $data[19],
                'tim_profit_2' => $data[20],
                'tim_delivery_percentaje' => $data[21],
                'tim_delivery_fixed' => $data[22],
                'tim_technical_description' => $data[30],
                'tim_video' => $data[31],
                'tim_newness' => $newness,
                'tim_promotion' => $promotion,
                'tim_past_price' => $data[34],
                )
              );
  $result = $query->execute();

}

/**
 * Form to merge CSV info in a new table
 *
 * @return array Array form
 */
function tecnola_import_merge_form($form, $form_state) {

  $form['notes'] = array(
    '#type' => 'markup',
    '#markup' => '
      <h3>Paso 2: Combinar Listas</h3>
      <div class="import-notes">Esta operación creará una sola lista con la información disponible en las listas subidas en el paso anterior.
      </div>',
  );

  $form['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Merge Lists'),
  );

  return $form;
}

/**
  * Implements form_submit
  */
function tecnola_import_merge_form_submit($form, $form_state) {
  $count = 0;
  $num_operations = 5;
  $operations = array();

  $results = db_select('tecnola_import', 'tim')
    ->fields('tim', array('tim_part_number'))
    ->distinct()
    ->execute()
    ->fetchAll();

  if (count($results) > 0) {

    // Put all in status 0
    $results_product = db_select('tecnola_import_merge', 'timm')
      ->fields('timm', array('timm_id'))
      ->execute()
      ->fetchAll();

    if(count($results_product) > 0){
      foreach ($results_product as $key => $value) {
        db_update('tecnola_import_merge')
        ->fields(array('timm_status'=> 0))
        ->condition('timm_id', $value->timm_id)
        ->execute();
      }
    }

    // Put all migrations to update
    $results_migrate_product = db_select('migrate_map_tecnolaproducts', 'mmtp')
      ->fields('mmtp', array('sourceid1'))
      ->execute()
      ->fetchAll();

    if(count($results_migrate_product) > 0){
      foreach ($results_migrate_product as $key => $value) {
        db_update('migrate_map_tecnolaproducts')
        ->fields(array('needs_update'=> 1))
        ->execute();
      }
    }

    $results_migrate_product_page = db_select('migrate_map_tecnolaproductspage', 'mmtpp')
      ->fields('mmtpp', array('sourceid1'))
      ->execute()
      ->fetchAll();

    if(count($results_migrate_product_page) > 0){
      foreach ($results_migrate_product_page as $key => $value) {
        db_update('migrate_map_tecnolaproductspage')
        ->fields(array('needs_update'=> 1))
        ->execute();
      }
    }

    // End - Migratios updated

    // Build batch to merge
    foreach ($results as $data) {
      $operations[] = array(
        'tecnola_import_process_batch_merge',
        array($data),
      );
    }

    $batch = array(
      'title' => t('Merging Info'),
      'operations' => $operations,
      'finished' => 'tecnola_import_finish_batch_merge',
      'progress_message' => t('Imported @current of @total products.'),
      'error_message' => t('An error was encountered processing the file.'),
      'file' => drupal_get_path('module', 'tecnola_import') . '/tecnola_import.admin.inc',
    );

    batch_set($batch);
  }
}

/**
 * Merge operations
 * 1. Select all products with the same SKU (Part Number) and select the best info and price
 **/
function tecnola_import_process_batch_merge($data, &$context) {

  $info = array();
  $i = 0;
  $exists = 0;

  // Start - Compare with the info in the lists
  $results = db_select('tecnola_import', 'tim')
    ->fields('tim')
    ->condition('tim.tim_part_number', $data->tim_part_number)
    ->execute()
    ->fetchAll();

  foreach ($results as $value) {
    if($i == 0){
      $info = $value;
    }
    else{
      // Choose the best value by lenght
      if(strlen($value->tim_name) > strlen($info->tim_name)): $info->tim_name = $value->tim_name; endif;
      if(strlen($value->tim_description) > strlen($info->tim_description)): $info->tim_description = $value->tim_description; endif;
      if(strlen($value->tim_tags) > strlen($info->tim_tags)): $info->tim_tags = $value->tim_tags; endif;
      if(strlen($value->tim_technical_description) > strlen($info->tim_technical_description)): $info->tim_technical_description = $value->tim_technical_description; endif;
      if(strlen($value->tim_video) > strlen($info->tim_video)): $info->tim_video = $value->tim_video; endif;

      // Select the second best price
      if($value->tim_price_cop == NULL){
        $value->tim_price_cop = variable_get('tecnola_trm_dolar', NULL) * $value->tim_price_usd;
      }
    }

    $i++;
  }
  // End - Compare with the info in the lists

  // Start - Compare with the current merge  lists
  $result = db_select('tecnola_import_merge', 'timm')
    ->fields('timm')
    ->condition('timm.timm_part_number', $data->tim_part_number)
    ->execute()
    ->fetchObject();

  if($result){
    // The register exists
    $exists = 1;

    db_update('tecnola_import_merge')
        ->fields(array('timm_status' => 1))
        ->condition('timm_id', $result->timm_id)
        ->execute();

    // Choose the best value by lenght
      if(strlen($result->timm_name) > strlen($info->tim_name)): $info->tim_name = $result->timm_name; endif;
      if(strlen($result->timm_description) > strlen($info->tim_description)): $info->tim_description = $result->timm_description; endif;
      if(strlen($result->timm_tags) > strlen($info->tim_tags)): $info->tim_tags = $result->timm_tags; endif;
      if(strlen($result->timm_technical_description) > strlen($info->tim_technical_description)): $info->tim_technical_description = $result->timm_technical_description; endif;
      if(strlen($result->timm_video) > strlen($info->tim_video)): $info->tim_video = $result->timm_video; endif;
  }


  // End - Check if exists the SKU and compare the info

  // Save or edit the final info

  $info->tim_newness = (int) $info->tim_newness;

  $fields = array(
    'timm_part_number' => $info->tim_part_number,
    'timm_name' => $info->tim_name,
    'timm_trademark' => $info->tim_trademark,
    'timm_group' => $info->tim_group,
    'timm_subgroup' => $info->tim_subgroup,
    'timm_division' => $info->tim_division,
    'timm_subdivision' => $info->tim_subdivision,
    'timm_family' => $info->tim_family,
    'timm_reference' => $info->tim_reference,
    'timm_description' => $info->tim_description,
    'timm_vat' => $info->tim_vat,
    'timm_provider' => $info->tim_provider,
    'timm_price_cop' => $info->tim_price_cop,
    'timm_price_usd' => $info->tim_price_usd,
    'timm_delivery' => $info->tim_delivery,
    'timm_tags' => $info->tim_tags,
    'timm_profit_1' => $info->tim_profit_1,
    'timm_profit_2' => $info->tim_profit_2,
    'timm_delivery_percentaje' => $info->tim_delivery_percentaje,
    'timm_delivery_fixed' => $info->tim_delivery_fixed,
    'timm_technical_description' => $info->tim_technical_description,
    'timm_video' => $info->tim_video,
    'timm_newness' => $info->tim_newness,
    'timm_promotion' => $info->tim_promotion,
    'timm_past_price' => $info->tim_past_price,
    'timm_status' => 1,
    );

  if($exists == 0){
    $query = db_insert('tecnola_import_merge')
      ->fields($fields);
  }
  else{
    $query = db_update('tecnola_import_merge')
      ->fields($fields)
      ->condition('timm_part_number', $data->tim_part_number);

    $message = (string) $query;
  }

  $result = $query->execute();
}

/**
 * Finish Action after Bash process
 *
 */
function tecnola_import_finish_batch_insert($success, $results, $operations) {
  if($success) {
    drupal_set_message(t('import finished'));
  }
  else{
    drupal_set_message(t('An error has ocurred'));
  }
}

/**
 * Finish Action after Bash process
 *
 */
function tecnola_import_finish_batch_merge($success, $results, $operations) {
  if($success) {
    drupal_set_message(t('Merge finished'));
  }
  else{
    drupal_set_message(t('An error has ocurred'));
  }
}

/**
 * Utility function to count the HTTP requests in a session variable.
 */
function _tecnola_import_update_http_request() {
  $_SESSION['http_request_count']++;
}
