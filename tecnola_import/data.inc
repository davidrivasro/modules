<?php

/**
 * @file
 * Migration of tecnola products from CSV files
 *
 * The basic idea is
 *
 * We will use the Migrate API to import and transform this data and turn it into
 * a working Drupal system.
 */

abstract class TecnolaMigration extends DynamicMigration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    parent::__construct();

  }
}

/**
 * The TecnolaProducts use an cvs file as source
 * and creates Drupal nodes of type 'standard_product' and products in commerce as destination.
 */
class TecnolaProducts extends TecnolaMigration {
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    $this->description = t('Products to upload');


    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'timm_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Product',
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_product', 'product')
    );

    $query = db_select('tecnola_import_merge', 'timm')
      ->fields('timm', array(
        'timm_id',
        'timm_part_number',
        'timm_reference',
        'timm_provider',
        'timm_price_usd',
        'timm_price_cop',
        'timm_profit_1',
        'timm_profit_2',
        'timm_delivery_percentaje',
        'timm_delivery_fixed',
        'timm_vat',
        'timm_delivery',
        'timm_status'));

    $this->source = new MigrateSourceSQL($query);

    $this->destination = new MigrateDestinationEntityAPI('commerce_product', 'product');

    // Properties,
    $this->addFieldMapping('sku', 'timm_part_number');
    $this->addFieldMapping('type')
         ->defaultValue('product');
    $this->addFieldMapping('title', 'timm_reference');
    $this->addFieldMapping('uid')
         ->defaultValue(0);
    $this->addFieldMapping('status', 'timm_status');
    $this->addFieldMapping('commerce_price', 'price');
    $this->addFieldMapping('commerce_price:include_tax', 'vat_type');
    $this->addFieldMapping('field_product_provider', 'timm_provider');
    $this->addFieldMapping('field_product_price_usd', 'timm_price_usd');
    $this->addFieldMapping('field_product_price_cop', 'timm_price_cop');
    $this->addFieldMapping('field_product_profit_1', 'timm_profit_1');
    $this->addFieldMapping('field_product_profit_2', 'timm_profit_2');
    $this->addFieldMapping('field_product_delivery_percent', 'timm_delivery_percentaje');
    $this->addFieldMapping('field_product_delivery_fixed', 'timm_delivery_fixed');
    $this->addFieldMapping('field_product_vat', 'timm_vat');
    $this->addFieldMapping('field_product_delivery', 'timm_delivery');

  }

  public function prepareRow($row) {
    $price_usd = (int) $row->timm_price_usd;
    $price_cop = (int) $row->timm_price_cop;
    $row->vat_type = NULL;

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if($row->timm_part_number == ""){
      drupal_set_message('El producto no tiene número de parte');
      $row->timm_status = 0;
    }

    if($price_usd == 0 && $price_cop == 0){
      drupal_set_message('El producto con número de parte: '.$row->timm_part_number.' no tiene un precio válido');
      $row->timm_status = 0;
    }

    // The provider value is normalized
    $row->timm_provider = strtolower($row->timm_provider);
    $row->timm_provider = $this->get_taxonomy_tid_by_vocabulary($row->timm_provider, 'proveedor', TRUE);

    // Define the price Value
    $trm = variable_get('tecnola_trm_dolar', NULL);

    //define the VAT Tax in a number
    $row->timm_vat = (int) str_replace('%', '', $row->timm_vat);
    $row->timm_profit_1 = (int) str_replace('%', '', $row->timm_profit_1);
    $row->timm_profit_2 = (int) str_replace('%', '', $row->timm_profit_2);
    $row->timm_delivery_percentaje = (int) str_replace('%', '', $row->timm_delivery_percentaje);

    if($row->timm_profit_1 == 0 && $row->timm_profit_2 == 0){
      drupal_set_message('El producto '.$row->timm_part_number.' no tiene un porcentaje de ganancia');
      $row->timm_status = 0;
    }

    // If have both prices or only have USD Price
    if($price_cop == 0 || ($price_cop != 0 && $price_usd != 0)){
      $row->timm_price_cop = $trm * $price_usd;
      $row->price = $trm * $price_usd;
    }
    else{
      $row->price = $row->timm_price_cop;
    }

    // Add profit to the price
    if($row->timm_profit_2 != 0){
      $row->price = $row->price + round(($row->price * $row->timm_profit_2)/100);
    }
    else{
      $row->price = $row->price + round(($row->price * $row->timm_profit_1)/100);
    }

    // Add VAT
    $min_vat = variable_get('tecnola_vat_min', NULL);

    //if($row->price > $min_vat){
    /*
    if($row->timm_vat > 0){
      $row->price = $row->price + round(($row->price * $row->timm_vat)/100);
      $row->vat_type = 'iva';
    }
    */
  }

    /**
   * Get the Term ID of a Term, or create a new term.
   * @param  string  $term_name  Term name
   * @param  string  $vocabulary Vocabulary machine name
   * @param  boolean $create     TRUE if want create a new term
   * @return mixed              Term ID
   */
  public function get_taxonomy_tid_by_vocabulary($term_name, $vocabulary, $create = FALSE) {
    $terms = taxonomy_get_term_by_name($term_name, $vocabulary);

    if(count($terms) > 0){
      foreach ($terms as $key => $term) {
        return $term->name;
      }
    }
    else{
      if(count($terms) == 0 & $create == TRUE) {
        $vocabularies = taxonomy_vocabulary_get_names();

        if (isset($vocabularies[$vocabulary])) {
          $vid = $vocabularies[$vocabulary]->vid;

          $new_term = array(
            'vid' => $vid,
            'name' => $term_name,
          );

          $new_term = (object) $new_term;

          taxonomy_term_save($new_term);

          return $new_term->name;
        }
        else{
          return FALSE;
        }
      }
    }
  }
}

/**
 * The TecnolaProducts use an cvs file as source
 * and creates Drupal nodes of type 'standard_product' and products in commerce as destination.
 */
class TecnolaProductsPage extends TecnolaMigration {
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    $this->description = t('Pages for products uploaded');
    $this->dependencies = array('TecnolaProducts');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'timm_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Product Page',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('tecnola_import_merge', 'timm')
      ->fields('timm', array(
        'timm_id',
        'timm_part_number',
        'timm_name',
        'timm_description',
        'timm_technical_description',
        'timm_trademark',
        'timm_group',
        'timm_subgroup',
        'timm_division',
        'timm_family',
        'timm_reference',
        'timm_tags',
        'timm_video',
        'timm_newness',
        'timm_status',
        'timm_promotion',
        ));

    $this->source = new MigrateSourceSQL($query);

    $this->destination = new MigrateDestinationNode('standard_product');

    // Properties
    $this->addFieldMapping('title', 'timm_reference');
    $this->addFieldMapping('field_product_name', 'timm_name');
    $this->addFieldMapping('body', 'timm_description');
    $this->addFieldMapping('field_technical_info', 'timm_technical_description');
    $this->addFieldMapping('uid')
         ->defaultValue(1);
    $this->addFieldMapping('status', 'timm_status');
    $this->addFieldMapping('promote', 'promote');
    $this->addFieldMapping('field_product_part_number', 'timm_part_number');
    $this->addFieldMapping('field_product_trademark', 'timm_trademark');
    $this->addFieldMapping('field_product_group', 'timm_group');
    $this->addFieldMapping('field_product_group:source_type')
         ->defaultValue('tid');
    $this->addFieldMapping('field_product_family', 'timm_family');
    $this->addFieldMapping('field_product_reference', 'timm_reference');
    $this->addFieldMapping('field_tags', 'timm_tags')
          ->separator(',');
    $this->addFieldMapping('field_video', 'timm_video');
    $this->addFieldMapping('field_products_variations', 'product_id');
    // We will get the image data from a related table in prepareRow()
    $this->addFieldMapping('field_product_images', 'url');
    // Indicate that we want each file to maintain its name, replacing any
    // previous file of the same name (as opposed to being renamed to avoid
    // collisions, which is the default).
    $this->addFieldMapping('field_product_images:file_replace')
         ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_product_images:source_dir')
      ->defaultValue('/home2/riaom3/public_html/tecnolat/sites/tecnola/files/import_images');
    $this->addFieldMapping('field_product_images:alt', 'image_alt');
    $this->addFieldMapping('field_product_images:title', 'image_title');
    $this->addFieldMapping('field_product_images:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_newness', 'timm_newness');
    $this->addFieldMapping('field_in_promotion', 'timm_promotion');
    $this->addFieldMapping('field_old_price', 'timm_past_price');
  }

  public function prepareRow($row) {

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Relate product
    $result = db_select('commerce_product', 'cp')
    ->fields('cp', array('product_id', 'status'))
    ->condition('sku', trim($row->timm_part_number), '=')
    ->execute()
    ->fetchObject();

    if(!$result){
      drupal_set_message('La página del producto con número de parte: '.$row->timm_part_number.' no tiene un producto relacionado, puede que el producto no se haya podido importar por no cumplir con los requisitos mínimos');
      $row->timm_status = 0;
    }
    else{
      $row->product_id = $result->product_id;
      $row->timm_status = $result->status;
    }



    // Define name field

    $row->timm_name = $row->timm_name;

    $row->timm_group = trim($row->timm_group, " \t\n\r\0\x0B ");
    $row->timm_subgroup = trim($row->timm_subgroup, " \t\n\r\0\x0B ");
    $row->timm_division = trim($row->timm_division, " \t\n\r\0\x0B ");
    $row->timm_family = trim($row->timm_family, " \t\n\r\0\x0B ");
    $row->timm_trademark = trim($row->timm_trademark, " \t\n\r\0\x0B ");

    $row->promote = 0;

    $tags = array();

    // Convert term names to tid
    if($row->timm_group != ""){
      $group_term = $this->get_taxonomy_tid_by_vocabulary($row->timm_group, 'grupo', TRUE);
      $row->timm_group = $group_term->tid;
    }

    if($row->timm_subgroup != ""){
      $subgroup_term = $this->get_taxonomy_tid_by_vocabulary($row->timm_subgroup, 'grupo', TRUE, $group_term->tid);
      $row->timm_subgroup = $subgroup_term->tid;
    }

    if($row->timm_division != ""){
      $division_term = $this->get_taxonomy_tid_by_vocabulary($row->timm_division, 'grupo', TRUE, $subgroup_term->tid);
      $row->timm_division = $division_term->tid;
    }

    if($row->timm_family != ""){
      $family_term = $this->get_taxonomy_tid_by_vocabulary($row->timm_family, 'grupo', TRUE);
      $row->timm_family = $family_term->name;
    }

    if($row->timm_trademark != ""){
      $trademark_term = $this->get_taxonomy_tid_by_vocabulary($row->timm_trademark, 'trademark', TRUE);
      $row->timm_trademark = $trademark_term->name;
    }

    // Create Tags
    $tags_array = explode(',', $row->timm_tags);

    foreach ($tags_array as $value) {
      $term = $this->get_taxonomy_tid_by_vocabulary($value, 'tags', TRUE);
      $tags[] = $term->name;
    }

    $row->timm_tags = implode(',', $tags);

    // Associate groups
	  $row->timm_group = (int) $row->timm_group;
	  $row->timm_subgroup = (int) $row->timm_subgroup;
	  $row->timm_division = (int) $row->timm_division;

    $groups = array();
	  if($row->timm_group != 0): $groups[] = $row->timm_group; endif;
	  if($row->timm_subgroup != 0): $groups[] = $row->timm_subgroup; endif;
	  if($row->timm_division != 0): $groups[] = $row->timm_division; endif;

    $row->timm_group = $groups;

    // We scan the directory to find the images related to SKU product
    $files = file_scan_directory('public://import_images', '/^'.preg_quote(trim($row->timm_part_number), '/').'.*\.jpg$/');

    if(count($files) > 0){
      foreach ($files as $value) {
        $row->url[] = $value->filename;
        $row->image_alt[] = $row->timm_name;
        $row->image_title[] = $row->timm_name;
      }

      $row->promote = 1;
    }
  }

  /**
   * Get the Term ID of a Term, or create a new term.
   * @param  string  $term_name  Term name
   * @param  string  $vocabulary Vocabulary machine name
   * @param  boolean $create     TRUE if want create a new term
   * @return mixed              Term ID
   */
  public function get_taxonomy_tid_by_vocabulary($term_name, $vocabulary, $create = FALSE, $parent = NULL) {
    $vocabularies = taxonomy_vocabulary_get_names();

    if (isset($vocabularies[$vocabulary])) {
          $vid = $vocabularies[$vocabulary]->vid;
    }
    else{
      return FALSE;
    }

    if($parent != NULL) {
      $query = db_select('taxonomy_term_data', 'ttd');
      $query->fields('ttd', array('name', 'tid'));
      $query->join('taxonomy_term_hierarchy', 'tth', 'tth.parent = :parent AND tth.tid=ttd.tid', array(':parent' => $parent));

      $result = $query->execute();

      foreach($result as $record){
        if($record->name == $term_name){
          return $record;
        }
      }
    }

    if($parent == NULL) {
      $terms = taxonomy_get_term_by_name($term_name, $vocabulary);

      if(count($terms) > 0){
        foreach ($terms as $key => $term) {
          return $term;
        }
      }
    }

    if($create == TRUE) {
      if($parent == NULL){
        $new_term = array(
          'vid' => $vid,
          'name' => $term_name,
        );
      }
      else{
        $new_term = array(
          'vid' => $vid,
          'name' => $term_name,
          'parent' => $parent,
        );

      }

      $new_term = (object) $new_term;

      taxonomy_term_save($new_term);
      return $new_term;
    }
  }
}
