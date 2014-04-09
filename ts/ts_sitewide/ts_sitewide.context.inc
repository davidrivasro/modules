<?php
/**
 * @file
 * ts_sitewide.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function ts_sitewide_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'tigostar_sitewide_context';
  $context->description = 'Context for the site wide elements';
  $context->tag = 'Tigo Star';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-secondary-menu' => array(
          'module' => 'menu',
          'delta' => 'secondary-menu',
          'region' => 'header_top',
          'weight' => '-10',
        ),
        'menu-menu-top-right-corner' => array(
          'module' => 'menu',
          'delta' => 'menu-top-right-corner',
          'region' => 'header_top',
          'weight' => '-9',
        ),
        'blockify-blockify-logo' => array(
          'module' => 'blockify',
          'delta' => 'blockify-logo',
          'region' => 'header',
          'weight' => '-10',
        ),
        'system-main-menu' => array(
          'module' => 'system',
          'delta' => 'main-menu',
          'region' => 'header',
          'weight' => '-9',
        ),
        'ts-other_products_link' => array(
          'module' => 'ts',
          'delta' => 'other_products_link',
          'region' => 'header',
          'weight' => '-7',
        ),
        'menu-menu-mundo-tigo' => array(
          'module' => 'menu',
          'delta' => 'menu-mundo-tigo',
          'region' => 'footer',
          'weight' => '-10',
        ),
        'block-1' => array(
          'module' => 'block',
          'delta' => '1',
          'region' => 'footer',
          'weight' => '-9',
        ),
        'simplenews-1' => array(
          'module' => 'simplenews',
          'delta' => '1',
          'region' => 'footer',
          'weight' => '-8',
        ),
        'tigo_redes_sociales-social_media_block_view' => array(
          'module' => 'tigo_redes_sociales',
          'delta' => 'social_media_block_view',
          'region' => 'footer',
          'weight' => '-7',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the site wide elements');
  t('Tigo Star');
  $export['tigostar_sitewide_context'] = $context;

  return $export;
}
