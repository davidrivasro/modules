<?php
/**
 * @file
 * premium.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function premium_ctools_plugin_api() {
  return array("version" => "1");
}

/**
 * Implements hook_node_info().
 */
function premium_node_info() {
  $items = array(
    'premium' => array(
      'name' => t('Premium'),
      'base' => 'node_content',
      'description' => t('Backtones, Ringontes, Wallpapers, Subscripciones SMS, Aplicaciones, etc'),
      'has_title' => '1',
      'title_label' => t('Titulo'),
      'help' => '',
    ),
  );
  return $items;
}
