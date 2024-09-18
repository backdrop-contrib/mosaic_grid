<?php
/**
 * @file
 * Template for the Mosaic Grid style.
 *
 * - rendered_mosaic_items: An array of rendered items(image and caption) to display.
 *
 * - $mosaic_id:            A unique id for this grid instance.
 * - $custom_css:           Generated styles for customized captions. Only required for preview mode.
 *
 * @ingroup views_templates
 */
?>

<?php if(isset($view->preview)): ?>
<style>
<?php print $custom_css;?>
</style>
<?php endif; ?>

<?php if (!empty($title)) : ?>
<h3><?php print $title; ?></h3>
<?php endif; ?>

<div id="<?php print $mosaic_id ?>" class="flex-images">
  <?php foreach ($rendered_mosaic_items as $item): ?>
    <?php if(!empty($item)): ?>
      <?php print $item; ?>
    <?php endif ?>
  <?php endforeach; ?>
</div>
