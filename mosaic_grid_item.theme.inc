<?php
/**
 * @file
 * Theme functions for the Mosaic Grid module.
 */

/**
 * Returns HTML for a Mosaic grid single item - image with a caption.
 *
 * @param array $variables
 *   An associative array containing:
 *   - image_rendered:  Image as rendered html.
 *   - width:           Width of the image in Mosaic Grid.
 *   - height:          Height of the image in Mosaic Grid.
 *   - text_rendered:   Caption text. Can be an html or a plain text.
 *   - options:         An array with Mosaic Grid style plugin options.
 *
 * @ingroup themeable
 */
function theme_mosaic_grid_item($variables) {
  $image_rendered = $variables['image_rendered'];
  $width          = $variables['width'         ];
  $height         = $variables['height'        ];
  $text_rendered  = $variables['text_rendered' ];
  $options        = $variables['options'       ];

  // Compose html code for caption.
  $caption = '';
  if (!empty($text_rendered)) {
    if ($text_rendered == strip_tags($text_rendered)) {
      // If text is not html and no wrapper element defined then
      // place it in <p> tags. This is required for proper styling.
      $text_rendered = '<p>' . $text_rendered . '</p>';
    }

    // Compose caption html based on selected display option.
    switch ($options['captions']['display']) {
      case 'hover':
        $caption  = '<div class="mosaic-grid-overlay">';
        $caption .= '<div class="mosaic-grid-caption mosaic-grid-hide-mobile">';
        $caption .= $text_rendered . '</div></div>';
        break;
      case 'static':
      default: // Fallback to 'static' display option as default.
        $caption  = '<div class="mosaic-grid-caption">';
        $caption .= $text_rendered . '</div>';
    }
  }

  // Add loader if lazy load plugin is enabled.
  $loader = '';
  if (config_get('mosaic_grid.settings', 'lazyload_enabled')) {
    $loader = '<div class="mosaic-grid-loader">&nbsp</div>';
  }

  // Compose html for the entire mosaic grid item.
  $rendered_item  = '<div class="mosaic-grid-item" data-w=' . $width . ' data-h=' . $height . ' style="margin: ' . $options['gap'] . 'px">';
  $rendered_item .= $image_rendered . $caption . $loader;
  $rendered_item .= '</div>';

  return $rendered_item;
}
