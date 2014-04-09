<?php
/**
 * @file
 * ts_homepage.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function ts_homepage_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'tigostar_home';
  $context->description = 'Block placement for homepage';
  $context->tag = 'Tigo Star';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-banners-block' => array(
          'module' => 'views',
          'delta' => 'banners-block',
          'region' => 'main_prefix',
          'weight' => '-10',
        ),
        'views-banners-block_1' => array(
          'module' => 'views',
          'delta' => 'banners-block_1',
          'region' => 'front_content',
          'weight' => '1',
        ),
        'menu-menu-help' => array(
          'module' => 'menu',
          'delta' => 'menu-help',
          'region' => 'front_content',
          'weight' => '2',
        ),
        'views-movies-block' => array(
          'module' => 'views',
          'delta' => 'movies-block',
          'region' => 'front_content',
          'weight' => '3',
        ),
        'ts_program-outstanding_programming' => array(
          'module' => 'ts_program',
          'delta' => 'outstanding_programming',
          'region' => 'front_content',
          'weight' => '4',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Block placement for homepage');
  t('Tigo Star');
  $export['tigostar_home'] = $context;

  return $export;
}
