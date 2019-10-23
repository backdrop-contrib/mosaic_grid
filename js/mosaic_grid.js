/**
 * @file
 * Sets options and initiates jquery plugins.
 */
(function($) {
  
  Backdrop.behaviors.mosaic_grid = {
    attach: function (context, settings) {

      for(var i in settings.mosaic_grid) {
        if(i == 'lazyload') {
          // In views preview force start lazyload.
          if(settings.mosaic_grid.lazyload.editing){
            $(window).lazyLoadXT();
          }
          // Configure lazyLoadXT.
          $.extend($.lazyLoadXT, {
            edgeY:    settings.mosaic_grid.lazyload.edgeY,
            selector: 'img[lazy-src]',
            srcAttr:  'lazy-src',
            // Modified onload function that removes loader icon.
            onload: function() {
              // Basic functionality that shows image after it is loaded.
              $(this).removeClass('lazy-hidden');
              $(this).addClass('lazy-loaded');
              // Find and remove loader icon.
              var itemDiv = $(this).parents('div.mosaic-grid-item');
              if(itemDiv) {
                itemDiv.children('.mosaic-grid-loader').remove();
              }
            },
          }); 
        }
        else {
          // Initiate grid on a page.
          var grid_settings = settings.mosaic_grid[i];
          $('#' + i).flexImages({
            container: '.mosaic-grid-item',
            object: 'img',
            rowHeight: Number(grid_settings.max_row_height),
            maxRows:   Number(grid_settings.max_rows),
            truncate:  grid_settings.truncate,
          });
        }
      }

    }      
  };
  
})(jQuery);
