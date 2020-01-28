<?php
/**
 * @file
 * Provides views data and handlers for mosaic_grid.module.
 */

/**
 * Implements hook_views_plugins().
 */
function mosaic_grid_views_plugins() {
  return array(
    'style' => array(
      'mosaic_grid' => array(
      'title' => t('Mosaic Grid'),
      'help' => t("Displays images in a mosaic grid layout."),
      'handler' => 'views_plugin_style_mosaic_grid',
      'type' => 'normal',
      'theme' => 'views_view_mosaic_grid',
      'path' => backdrop_get_path('module', 'mosaic_grid'),
      'theme path' => backdrop_get_path('module', 'mosaic_grid'),
      'uses fields' => TRUE,
      'uses row plugin' => FALSE,
      'uses row class' => TRUE,
      'uses options' => TRUE,
      'uses grouping' => TRUE,
    ),
    ),
  );
}
