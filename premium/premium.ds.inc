<?php
/**
 * @file
 * premium.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function premium_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'comment|comment_node_premium|default';
  $ds_fieldsetting->entity_type = 'comment';
  $ds_fieldsetting->bundle = 'comment_node_premium';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'post_date' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'ds_post_date_medium',
    ),
    'author' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'ds_user_picture' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'ds_picture_square_thumbnail',
    ),
  );
  $export['comment|comment_node_premium|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|premium|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'premium';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'rating_stars' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'title' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h2',
        'class' => '',
      ),
    ),
    'links' => array(
      'weight' => '11',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'comments' => array(
      'weight' => '17',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|premium|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|premium|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'premium';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h2',
        'class' => '',
      ),
    ),
  );
  $export['node|premium|teaser'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function premium_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'comment|comment_node_premium|default';
  $ds_layout->entity_type = 'comment';
  $ds_layout->bundle = 'comment_node_premium';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 0,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'ds_user_picture',
        1 => 'author',
      ),
      'right' => array(
        0 => 'comment_body',
        1 => 'post_date',
      ),
    ),
    'fields' => array(
      'ds_user_picture' => 'left',
      'author' => 'left',
      'comment_body' => 'right',
      'post_date' => 'right',
    ),
    'classes' => array(),
  );
  $export['comment|comment_node_premium|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|premium|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'premium';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked';
  $ds_layout->settings = array(
    'hide_empty_regions' => 0,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'field_image',
        1 => 'rating_stars',
      ),
      'right' => array(
        0 => 'title',
        1 => 'field_premium_type',
        2 => 'field_premium_author',
        3 => 'field_premium_album',
        4 => 'field_premium_label',
        5 => 'field_premium_price',
        6 => 'field_premium_renewal',
        7 => 'field_premium_url',
        8 => 'field_premium_gift',
        9 => 'links',
      ),
      'footer' => array(
        0 => 'field_premium_preview',
        1 => 'field_premium_keyword',
        2 => 'field_premium_shortcode',
        3 => 'body',
        4 => 'field_tags',
        5 => 'comments',
      ),
    ),
    'fields' => array(
      'field_image' => 'left',
      'rating_stars' => 'left',
      'title' => 'right',
      'field_premium_type' => 'right',
      'field_premium_author' => 'right',
      'field_premium_album' => 'right',
      'field_premium_label' => 'right',
      'field_premium_price' => 'right',
      'field_premium_renewal' => 'right',
      'field_premium_url' => 'right',
      'field_premium_gift' => 'right',
      'links' => 'right',
      'field_premium_preview' => 'footer',
      'field_premium_keyword' => 'footer',
      'field_premium_shortcode' => 'footer',
      'body' => 'footer',
      'field_tags' => 'footer',
      'comments' => 'footer',
    ),
    'classes' => array(),
  );
  $export['node|premium|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|premium|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'premium';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 0,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'field_image',
      ),
      'right' => array(
        0 => 'title',
        1 => 'field_premium_author',
        2 => 'field_premium_type',
        3 => 'field_premium_price',
      ),
    ),
    'fields' => array(
      'field_image' => 'left',
      'title' => 'right',
      'field_premium_author' => 'right',
      'field_premium_type' => 'right',
      'field_premium_price' => 'right',
    ),
    'limit' => array(
      'field_image' => '1',
    ),
    'classes' => array(),
  );
  $export['node|premium|teaser'] = $ds_layout;

  return $export;
}
