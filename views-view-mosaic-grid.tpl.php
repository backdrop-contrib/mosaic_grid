<?php
/**
 * @file
 * Template for the Mosaic Grid style.
 *
 * - $items_rendered  array of images to display.
 * - $items_ratio     array containing size ratio for each image.
 * - $captions        array with captions for images.
 * - $mosaic_id       unique id for this mosaic grid instance.
 * - $settings        array with Mosaic plugin settings.
 *
 * @ingroup views_templates
 */
?>

<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>

<div <?php print
      // This long string passes settings to Mosaic jquery plugin.
      'id="'                              . $mosaic_id                                 . '" ' .                                 
      'data-max-row-height="'             . $settings['max_row_height']                . '" ' .
      'data-inner-gap="'                  . $settings['inner_gap']                     . '" ' .
      'data-outer-margin="'               . $settings['outer_margin']                  . '" ' .
      'data-max-row-height-policy="'      . $settings['max_row_height_policy']         . '" ' .
      'data-refit-on-resize="'            . $settings['refit_on_resize']               . '" ' .
      'data-refit-on-resize-delay="'      . $settings['refit_on_resize_delay']         . '" ' .
      'data-max-rows="'                   . $settings['max_rows']                      . '" ' .
      'data-responsive-width-threshold="' . $settings['responsive_width_threshold']    . '" ' .
      'data-max-items-to-show-when-responsive-threshold-surpassed="'
                                          . $settings['max_items_threshold_surpassed'] . '" ';
     ?>>
  <?php foreach ($items_rendered as $i => $item): ?>
    <div class="mosaic-grid-item" data-aspect-ratio="<?php print $items_ratio[$i] ?>">
      <?php if($item): ?>
        <div class="mosaic-grid-loader">&nbsp</div>
        <?php print $item; ?>
      <?php else:?>
         <div class="mosaic-grid-empty"><?php print t('Image file is missing.');?></div>
      <?php endif;?>
      <?php if(!empty($captions[$i])): ?>
        <div class="mosaic-grid-overlay">
          <div class="mosaic-grid-caption">
            <?php print $captions[$i] ?>
          </div>
        </div>
      <?php endif;?>
    </div>
  <?php endforeach; ?>
</div>

<?php
  /* 
   * The code below is required for views preview to work.
   * In preview mode module's scripts are not added so we need to do it manually.
   */
  if($view -> editing):
    if($settings['caption_style_override']):
      backdrop_set_message(t('Mosaic Grid caption style customization is not shown on the preview.'), 'warning');
    endif;
?> 
<script>
  $(window).lazyLoadXT({show: true});
  $('#<?php print $mosaic_id ?>').Mosaic();
</script>
<?php endif; ?>