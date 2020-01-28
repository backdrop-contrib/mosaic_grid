<?php
/**
 * @file
 * Template for the Mosaic Grid style.
 *
 * - $items_rendered     array of images to display.
 * - $width              array containing width of each image.
 * - $height             array containing height of each image.
 * - $captions           array with captions for images or FALSE if no captions set.
 * - $mosaic_id          unique id for this grid instance.
 * - $custom_css         generated styles for customized captions. Only required for preview mode.
 *
 * @ingroup views_templates
 */
?>

<?php if($view->preview): ?>
<style>
<?php print $custom_css;?>
</style>
<?php endif; ?>

<?php if (!empty($title)) : ?>
<h3><?php print $title; ?></h3>
<?php endif; ?>

<div id="<?php print $mosaic_id ?>" class="flex-images">
  <?php foreach ($items_rendered as $i => $item): ?>
    <div class="mosaic-grid-item" data-w="<?php print $width[$i] ?>" data-h="<?php print $height[$i] ?>" style="margin: <?php print $options['gap']; ?>px">

      <?php if($item):?>
        <?php print $item; ?>
        <?php if(!empty($captions[$i])): ?>

          <?php switch($options['captions']['display']):
                  case 'hover': ?>
              <div class="mosaic-grid-overlay">
                <div class="mosaic-grid-caption mosaic-grid-hide-mobile">
                  <?php print $captions[$i] ?>
                </div>
              </div>
              <?php break; ?>
            <?php case 'static': ?>
              <div class="mosaic-grid-caption">
                  <?php print $captions[$i] ?>
              </div>
            <?php break; ?>
          <?php endswitch ?>

        <?php endif ?>
        <div class="mosaic-grid-loader">&nbsp</div>
      <?php else:?>
        <div class="mosaic-grid-empty"><?php print t('Image file is missing.');?></div>
      <?php endif ?>

    </div>
  <?php endforeach; ?>
</div>
