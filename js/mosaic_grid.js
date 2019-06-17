/**
 * @file
 * Sets options and starts Mosaic Grid script.
 */
(function($) {
  
  Backdrop.behaviors.mosaic_grid = {
    attach: function (context, settings) {
      
      $.extend($.lazyLoadXT, {
        edgeY:  settings.mosaic_grid.lazyload_edgeY,
      });
      
      $('#mosaic-grid-gallery').Mosaic({
        maxRowHeightPolicy: settings.mosaic_grid.max_row_height_policy,
        maxRowHeight: Number(settings.mosaic_grid.max_row_height),
        innerGap: Number(settings.mosaic_grid.inner_gap),
        outerMargin: Number(settings.mosaic_grid.outer_margin),
        maxRows: Number(settings.mosaic_grid.max_rows),
        refitOnResize: settings.mosaic_grid.refit_on_resize,
        refitOnResizeDelay: Number(settings.mosaic_grid.refit_on_resize_delay),
        responsiveWidthThreshold: Number(settings.mosaic_grid.responsive_width_threshold),
        maxItemsToShowWhenResponsiveThresholdSurpassed: Number(settings.mosaic_grid.max_items_threshold_surpassed),
      });
      
    }      
  };
  
})(jQuery);