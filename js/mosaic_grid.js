/**
 * @file
 * Sets options for lazyLoadXT plugin.
 */
(function($) {
  
  Backdrop.behaviors.mosaic_grid = {
    attach: function (context, settings) {
      $.extend($.lazyLoadXT, {
        edgeY:  settings.mosaic_grid.lazyload_edgeY,
      });
    }      
  };
  
})(jQuery);