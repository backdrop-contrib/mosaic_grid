<?php
/**
 * @file
 * Administration page callbacks for the Mosaic Grid module.
 */

/**
 * Form constructor for the Mosaic Grid settings form.
 */
function mosaic_grid_admin_form($form, &$form_state) {
  $config = config('mosaic_grid.settings');

  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['appearance']['loader_style'] = array(
    '#type' => 'select',
    '#title' => t('Loader Style'),
    '#options' => array(
      'spinning_ring'      => t('Spinning Ring'),
      'spinning_ring_thin' => t('Spinning Ring Thin'),
      'floating_balls'     => t('Floating Balls'),
      'three_dots'         => t('Three Dots'),
      'square_blocks'      => t('Square Blocks'),
      'vertical_blocks'    => t('Vertical Blocks'),
    ),
    '#empty_option' => t('None'),
    '#default_value' => $config->get('loader_style'),
    '#description' => t('Sets style for animated loader icon that is displayed while image is being loaded.'),
  );

  $form['appearance']['loader_color'] = array(
    '#type' => 'color',
    '#title' => t('Loader Color'),
    '#default_value' => $config->get('loader_color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="loader_style"]' => array('value' => ''),
      ),
    ),
  );

  $form['appearance']['caption_style_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Customize caption style'),
    '#description' => t("Enables changing caption size and colors. This will
      override style provided by the theme. Uncheck if you would like the theme
      to control caption style."),
    '#default_value' => $config->get('caption_style_override'),
  );

  $form['appearance']['caption_font_color'] = array(
    '#type' => 'color',
    '#title' => t('Caption font color'),
    '#default_value' => $config->get('caption_font_color'),
    '#states' => array(
      'visible' => array(
        ':input[name="caption_style_override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['appearance']['caption_background_color'] = array(
    '#type' => 'color',
    '#title' => t('Caption background color'),
    '#default_value' => $config->get('caption_background_color'),
    '#states' => array(
      'visible' => array(
        ':input[name="caption_style_override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['appearance']['caption_opacity'] = array(
    '#type' => 'number',
    '#title' => t('Caption background opacity'),
    '#default_value' => $config->get('caption_opacity'),
    '#min' => 0,
    '#max' => 100,
    '#field_suffix' => t('%'),
    '#states' => array(
      'visible' => array(
        ':input[name="caption_style_override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['appearance']['caption_font_size'] = array(
    '#type' => 'number',
    '#title' => t('Caption font size'),
    '#description' => t(
      'Relative (<a href="@link" target="_blank">em</a>) units are used. <em>1em</em> means caption font size is equal to the font size defined by the theme.',
       array('@link' => url('https://en.wikipedia.org/wiki/Em_(typography)'))
    ),
    '#default_value' => $config->get('caption_font_size'),
    '#min'  => 0.1,
    '#max'  => 10,
    '#step' => 0.1,
    '#field_suffix' => t('em'),
    '#states' => array(
      'visible' => array(
        ':input[name="caption_style_override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['appearance']['caption_padding'] = array(
    '#type' => 'number',
    '#title' => t('Caption padding'),
    '#description' => t(
      'Sets the size of the space around the caption text. Relative (<a href="@link" target="_blank">em</a>) units are used, which means that the actual padding size depends on the font size, as defined in the field above.',
      array('@link' => url('https://en.wikipedia.org/wiki/Em_(typography)'))
    ),
    '#default_value' => $config->get('caption_padding'),
    '#min'  => 0,
    '#max'  => 10,
    '#step' => 0.1,
    '#field_suffix' => t('em'),
    '#states' => array(
      'visible' => array(
        ':input[name="caption_style_override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['lazyload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lazy loading'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['lazyload']['lazyload_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable lazy loading'),
    '#description' => t("Images are loaded only when they get within a visible page area. This considerably optimises the loading time of pages that contain many images. If you use some other module to lazy-load images on your site, then you may want to disable this option."),
    '#default_value' => $config->get('lazyload_enabled'),
  );

  $form['lazyload']['lazyload_edgeY'] = array(
    '#type' => 'number',
    '#title' => t('Offset'),
    '#description' => t("The offset controls how early you want the elements to be loaded before they are visible. For example, value 100 means 100px before an element is visible it will start loading. Zero value means only images that are inside visible area will be loaded."),
    '#default_value' => $config->get('lazyload_edgeY'),
    '#min' => 0,
    '#max' => 9999,
    '#field_suffix' => t('pix'),
    '#states' => array(
      'visible' => array(
        ':input[name="lazyload_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Saves configuration settings when the form is submitted.
 */
function mosaic_grid_admin_form_submit($form, &$form_state) {
  $config = config('mosaic_grid.settings');

  $config->set('loader_style', $form_state['values']['loader_style']);
  $config->set('loader_color', $form_state['values']['loader_color']);
  $config->set('caption_style_override', $form_state['values']['caption_style_override']);
  $config->set('caption_font_color', $form_state['values']['caption_font_color']);
  $config->set('caption_background_color', $form_state['values']['caption_background_color']);
  $config->set('caption_opacity', $form_state['values']['caption_opacity']);
  $config->set('caption_padding', $form_state['values']['caption_padding']);
  $config->set('caption_font_size', $form_state['values']['caption_font_size']);
  $config->set('lazyload_enabled', $form_state['values']['lazyload_enabled']);
  $config->set('lazyload_edgeY', $form_state['values']['lazyload_edgeY']);

  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
}
