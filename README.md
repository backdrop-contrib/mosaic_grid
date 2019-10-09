Mosaic Grid
===========

Mosaic Grid is a Views style plugin that allows to display images in a nice responsive justified grid.
Best works as a single large full page image gallery but can also be used as a block and for creating multiple galleries on a single page.
Here is a short list of features:

- Justified responsive grid layout for images.
- Optimised image loading (lazy loading).
- Plenty of configuration options like image row height, gap etc.
- Captions that appear on mouse hover.
- Ability to tweak caption style without writing extra CSS.
- Ability to use rich text for captions (by adding text fields in views).
- Several loader icon styles to choose from.


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules

- Visit the configuration page under Administration > Configuration > Media >
  Mosaic Grid (admin/config/media/mosaic_grid) to configure the global
  settings.

- Enable and configure the Masonry Grid in a view's format section. This plugin requires
  one image field and may optionally use one text field as a source for captions.


Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/mosaic-grid/issues

Known Issues
------------

At some circumstances a broken grid is created. In that case try some of the following:
- Clear cache: Home -> Flush all caches
- Enable 'Refit on Resize' setting in case it was disabled.
- Try different image size for grid images (i.e. change image style).
- Try different layout. For instance try Boxton Hero instead of Boxton.
- If you are using customized flexible layout then try setting the container style to maximum or full width.
- Try using a different Theme if you can.

If non of above helps then please report your case here with as much details as possible:
https://github.com/backdrop-contrib/mosaic_grid/issues/4

Current Maintainers
-------------------

- Pavel (https://github.com/korontari)
- Co-maintainers are welcome.

Credits
-------

- jQuery Mosaic plugin (https://github.com/tin-cat/jquery-mosaic/).
- jQuery Lazy Load XT plugin (http://ressio.github.io/lazy-load-xt/).

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

