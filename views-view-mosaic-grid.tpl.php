<?php
/**
 * @file
 * Template for the Mosaic Grid style.
 *
 * - $items_rendered  contains an array of images to display.
 * - $items_ratio     array containing size ratio for each image.
 * - $captions        array with captions for each image.
 *
 * @ingroup views_templates
 */
?>

<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>

<div id="mosaic-grid-gallery">
  <?php foreach ($items_rendered as $i => $item): ?>
    <div class="mosaic-grid-item" data-aspect-ratio="<?php print $items_ratio[$i] ?>">
      <div class="mosaic-grid-loader">&nbsp</div>
      <?php print $item; ?>
      <?php if(!empty($captions[$i])): ?>
        <div class="mosaic-grid-caption">
          <?php print $captions[$i] ?>
        </div>
      <?php endif;?>
    </div>
  <?php endforeach; ?>
</div>