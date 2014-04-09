<?php
/**
 * @file
 * ts_program_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function ts_program_feature_node_info() {
  $items = array(
    'program' => array(
      'name' => t('Program'),
      'base' => 'node_content',
      'description' => t('Program of Tigo Star'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
