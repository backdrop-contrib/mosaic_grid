/**
 * @file
 * Sets options for lazyLoadXT plugin.
 */
(function($) {
  
  Backdrop.behaviors.mosaic_grid = {
    attach: function (context, settings) {
      $.extend($.lazyLoadXT, {
        edgeY:  settings.mosaic_grid.lazyload_edgeY,
        
        // Modified onload function that removes loader.
        onload: function() {
          // Basic functionality that shows image after it is loaded.
          $(this).removeClass('lazy-hidden');
          $(this).addClass('lazy-loaded');
          // Seek for item DIV that should be a parent for the image.
          var itemDiv = $(this).parent();
          if(!itemDiv.hasClass('mosaic-grid-item')) {
            // In case image(IMG) is wrapped in a link(A) we need 
            // to move one step higher in hierarchy.
            itemDiv = itemDiv.parent();
          }
          // Delete loader DIV that is a children of the item.
          itemDiv.children('.mosaic-grid-loader').remove();
        },
      });
    }      
  };
  
})(jQuery);
