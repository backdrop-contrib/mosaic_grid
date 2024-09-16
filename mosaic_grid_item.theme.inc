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
 *   - image_item:      An array containing rendered array and raw data
 *                      (output of views_handler_field_field::get_items).
 *   - image_field_id:  Id of the image field which data was provided in image_data.
 *   - image_style:     Name of an image style used by the image field.
 *   - text_item:       An array with an output similar to image_item parameter but
 *                      from a text field, or a string with a text or html. Not required
 *                      if alt tag or title tag are used for caption text source.
 *   - text_field:      Text field handler. Required for rendering of text_item data with alterations.
 *   - options:         An array with Mosaic Grid style plugin options.
 *
 * @ingroup themeable
 */
function theme_mosaic_grid_item($variables) {
  $image_item         = $variables['image_item'    ];
  $image_field_id     = $variables['image_field_id'];
  $image_style        = $variables['image_style'   ];
  $text_item          = $variables['text_item'     ];
  $text_field         = $variables['text_field'    ];
  $options            = $variables['options'       ];

  $width  = $image_item['raw']['width' ];
  $height = $image_item['raw']['height'];

  // Calculate image dimensions with regard of image style.
  if (!empty($image_style)) {
    $dimentions = array('width' => $width, 'height' => $height);
    image_style_transform_dimensions($image_style, $dimentions);
    $width  = (int)$dimentions['width'];
    $height = (int)$dimentions['height'];
  }

  // Image width and height should not be zero.
  if ($width <= 0 || $height <= 0) {
    return '';
  }

  // Get text for the caption based on selected source option.
  if (!empty($options['captions']['source'][$image_field_id])) {
    switch ($options['captions']['source'][$image_field_id]) {
      case 'alt_tag':
        if (!empty($image_item['rendered']['#item']['alt'])) {
          // Take alt value from 'rendered' to have tokens rendered.
          $caption_text = $image_item['rendered']['#item']['alt'];
        }
        break;
      case 'title_tag':
        if (!empty($image_item['rendered']['#item']['title'])) {
          // Take title value from 'rendered' to have tokens rendered.
          $caption_text = $image_item['rendered']['#item']['title'];
        }
        break;
      default:
        // Render text or title field value.
        if (!empty($text_item) && !empty($text_field)) {
          if (is_array($text_item)) {
            // Render rich text field from render array to html.
            $text_field->last_render = backdrop_render($text_item['rendered']);
          }
          else {
            // Take title text as is.
            $text_field->last_render = $text_item;
          }
          // Apply token replacements and other alterations.
          $text_field->original_value = $text_field->last_render;
          $alter = $text_field->options['alter'];
          $alter['phase'] = VIEWS_HANDLER_RENDER_TEXT_PHASE_SINGLE_ITEM;
          $caption_text = $text_field->render_text($alter);
          // Add wrapper tag if defined.
          $tag = $text_field->options['element_type'];
          if (!empty($tag)) {
            $caption_text = '<' . $tag . '>' . $caption_text . '</' . $tag . '>';
          }
        }
    }
  }

  // Compose html code for caption.
  $caption_rendered = '';
  if (!empty($caption_text)) {
    if ($caption_text == strip_tags($caption_text)) {
      // If text is not html and no wrapper element defined then
      // place it in <p> tags. This is required for proper styling.
      $caption_text = '<p>' . $caption_text . '</p>';
    }

    // Compose caption html based on selected display option.
    switch ($options['captions']['display']) {
      case 'hover':
        $caption_rendered  = '<div class="mosaic-grid-overlay">';
        $caption_rendered .= '<div class="mosaic-grid-caption mosaic-grid-hide-mobile">';
        $caption_rendered .= $caption_text . '</div></div>';
        break;
      case 'static':
      default: // Fallback to 'static' display option as default.
        $caption_rendered  = '<div class="mosaic-grid-caption">';
        $caption_rendered .= $caption_text . '</div>';
    }
  }

  // Render image html from the image render array.
  $image_rendered = backdrop_render($image_item['rendered']);

  // Add loader if lazy load plugin is enabled.
  $loader = '';
  if (config_get('mosaic_grid.settings', 'lazyload_enabled')) {
    $loader = '<div class="mosaic-grid-loader">&nbsp</div>';
  }

  // Compose html for the entire mosaic grid item.
  $rendered_item  = '<div class="mosaic-grid-item" data-w=' . $width . ' data-h=' . $height . ' style="margin: ' . $options['gap'] . 'px">';
  $rendered_item .= $image_rendered . $caption_rendered . $loader;
  $rendered_item .= '</div>';

  return $rendered_item;
}
