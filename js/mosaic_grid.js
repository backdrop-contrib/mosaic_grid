/**
 * @file
 * Sets options for lazyLoadXT plugin.
 */
(function($) {
  
  Backdrop.behaviors.mosaic_grid = {
    attach: function (context, settings) {
      $.extend($.lazyLoadXT, {
        edgeY:  settings.mosaic_grid.lazyload_edgeY,
        selector: 'img[lazy-src]',
        srcAttr: 'lazy-src',
        
        // Modified onload function that removes loader.
        onload: function() {
          // Basic functionality that shows image after it is loaded.
          $(this).removeClass('lazy-hidden');
          $(this).addClass('lazy-loaded');
          // Seek for item DIV that should be a parent for the image.
          var itemDiv = $(this).parents('div.mosaic-grid-item');
          if(itemDiv) {
            // Delete loader DIV that is a children of the item DIV.
            itemDiv.children('.mosaic-grid-loader').remove();
          }
        },
      });
    }      
  };
  
})(jQuery);
