<?php
/**
 * @file
 * Contains the Mosaic Grid style plugin.
 */
class views_plugin_style_mosaic_grid extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['max_row_height']       = array('default' => 400);
    $options['gap']                  = array('default' => 2);
    $options['max_rows']             = array('default' => 0);
    $options['truncate']             = array('default' => FALSE);
    $options['hide_empty']           = array('default' => TRUE);
    $options['captions']             = array(
      'default' => array(
        'source'       => '',
        'display'      => 'hover',
        'height'       => array (
          'option' => 'auto',
          'value'  => 2.0,
        ),
        'no_padding_h' => FALSE,
      ),
    );

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {

    $form['max_row_height'] = array(
      '#title' => t('Maximum row height'),
      '#type' => 'number',
      '#min' => 10,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['max_row_height'],
    );

    $form['gap'] = array(
      '#title' => t('Gap'),
      '#description' => t('Amount of space between items. To build a grid without gaps, a value of <em>-1</em> is recommended. This will prevent irregular gaps of one-pixel width.'),
      '#type' => 'number',
      '#min' => -1,
      '#max' => 99,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['gap'],
    );

    $form['max_rows'] = array(
      '#title' => t('Maximum rows number'),
      '#description' => t('Items exceeding this limit will be hidden. Set to zero to display all rows.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#default_value' => $this->options['max_rows'],
    );

    $form['truncate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Truncate'),
      '#description' => t("Hide last row of items, if incomplete."),
      '#default_value' => $this->options['truncate'],
    );

    $form['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty images'),
      '#description' => t("Ignore images with missing files."),
      '#default_value' => $this->options['hide_empty'],
    );

    // Caption options fieldset.
    $form['captions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Caption settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Set options list for caption selector.
    $caption_options = array(
      'alt'        => t('Image alt tag'),
      'title'      => t('Image title tag'),
    );
    // Detect if text or title field is present, and add it to the options list.
    $text_field = $this->get_first_non_excluded_text_field()['handler'];
    if (!empty($text_field)) {
      $caption_options['text'] = $text_field->ui_name();
    }
    // Create caption selector form.
    $form['captions']['source'] = array(
      '#type' => 'select',
      '#title' => t('Text source'),
      '#options' => $caption_options,
      '#empty_option' => t('None'),
      '#default_value' => $this->options['captions']['source'],
      '#description' => t('A text or title field can be added to the view, to be used as caption.'),
    );

    $caption_display = array(
      'hover'  => t('Hidden initially, shown on mouse hover'),
      'static' => t('Always visible, at the bottom of the image'),
    );

    $form['captions']['display'] = array(
      '#type' => 'select',
      '#title' => t('Caption display mode'),
      '#default_value' => $this->options['captions']['display'],
      '#options' => $caption_display,
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );

    // Title and description are made as markup elements. This is needed for
    // pretty looking inline placement of the 'select' and 'number' fields below.
    $form['captions']['height_title'] = array(
      '#prefix' => '<label style="margin-top: 10px; margin-bottom: -5px">',
      '#markup' => t('Height'),
      '#suffix' => '</label>',
    );
    
    // Container for inline placement of the next two fields.
    $form['captions']['height'] = array(
      '#type' => 'container',
      '#attributes' => array (
        'class' => 'container-inline',
      ),
    );
    
    $caption_height_options = array(
      'auto'  => t('Automatic'),
      'limit' => t('Automatic, up to specific limit'),
      'fixed' => t('Specific, fixed height'),
    );
    
    $form['captions']['height']['option'] = array(
      '#type' => 'select',
      '#options' => $caption_height_options,
      '#default_value' => $this->options['captions']['height']['option'],
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['captions']['height']['value'] = array(
      '#type' => 'number',
      '#min' => 0.1,
      '#max' => 10,
      '#step' => 0.1,
      '#size' => 2,
      '#field_suffix' => t('em'),
      '#default_value' => $this->options['captions']['height']['value'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[captions][height][option]"]' => array('value' => 'auto'),
        ),
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['captions']['description'] = array(
      '#prefix' => '<div class="description">',
      '#markup' => t('Sets caption height. Units (<a href="@link" target="_blank">em</a>) are relative to the font size of the caption.', array('@link' => url('https://en.wikipedia.org/wiki/Em_(typography)'))),
      '#suffix' => '</div>',
    );

    $form['captions']['no_padding_h'] = array(
      '#type' => 'checkbox',
      '#title' => t('No horisontal padding'),
      '#default_value' => $this->options['captions']['no_padding_h'],
      '#description' => t('Horizontally align the caption with the grid item. This overrides the padding setting in the <a href="@link" target="_blank">configuration page</a> (currently set to <em>@global_padding_setting em</em>).', array('@link' => url('admin/config/media/mosaic_grid'), '@global_padding_setting' => config_get('mosaic_grid.settings', 'caption_padding'))),
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );
  }

  /**
   * Returns first non-excluded field by type.
   * Returned value is an array containing handler and field's name. Works only
   * for views_handler_field_field types that have field_info and for title fields.
   */
  protected function get_first_non_excluded_field(string $field_type) {
    $fields = $this->view->display_handler->get_handlers('field');

    foreach ($fields as $field_name => $field) {
      // Ignore excluded fields.
      if (!empty($field->options['exclude']))
        continue;

      if (isset($field->field_info) || $field_type == 'title') {
        $type = FALSE;
        if (!empty($field->field_info['type'])) {
          $type = $field->field_info['type'];
        }
        // If it is of required type, return field handler and field name.
        // Title has no field_info, so add a second condition.
        if ($field_type == $type || ($field_type == 'title' && $field_name == 'title')) {
          return array('handler' => $field, 'field_name' => $field_name);
        }
      }

    }
    return NULL;
  }

  /*
   * Returns first non excluded image field.
   */
  function get_first_non_excluded_image_field() {
    return $this->get_first_non_excluded_field('image');
  }

  /*
   * Returns first non excluded text field.
   */
  function get_first_non_excluded_text_field() {
    // Set possible text field types.
    $text_types = $this->get_text_field_types();
    // Search for each possible type of text field.
    foreach ($text_types as $text_type) {
      $field_data = $this->get_first_non_excluded_field($text_type);
      // Return the fist text field found.
      if (!empty($field_data)) {
        return $field_data;
      }
    }
    return NULL;
  }

  /**
   * Returns array of text field types.
   */
  function get_text_field_types() {
    return array('title', 'text', 'text_long', 'text_with_summary');
  }

  /**
   * Validates the view configuration.
   * Fails if there is a non-image or non-text field, or there are more
   * than one image and text fields that are not excluded from display.
   */
  function validate() {
    $errors = parent::validate();

    // Do not validate when view is being created as there is no image field
    // by default, and validation will fail.
    if (!$this->view->editing) {
      return $errors;
    }

    // Get a list of fields that have been added to the display.
    $fields = $this->display->handler->get_handlers('field');

    $image_field_count = 0;
    $text_field_count  = 0;

    foreach ($fields as $field_name => $field) {
      // Ignore fields excluded from display.
      if (!empty($field->options['exclude'])) {
        continue;
      }

      // Check field type.
      if (isset($field->field_info['type'])) {
        $field_type = $field->field_info['type'];
      }
      elseif ($field->field == 'title') {
        $field_type = 'title';
      }
      $is_image = $field_type == 'image';
      $is_text = in_array($field_type, $this->get_text_field_types());
      $is_title = $field_name == 'title';

      // If invalid field type found then validation failed.
      if (!$is_image && !$is_text && !$is_title) {
        $errors[] = t('Invalid field types found. This format requires one image field, and may optionally have one text or title field.');
        return $errors;
      }

      // Count valid fields.
      if ($is_image) {
        $image_field_count ++;
      }
      if ($is_text  || $is_title) {
        $text_field_count++;
      }

      // Check if there is no more than one of each valid field types.
      if ($image_field_count > 1) {
        $errors[] = t('There is more than one image field. This format can display only one image field.');
        break;
      }
      if ($text_field_count > 1) {
        $errors[] = t('There is more than one text field. This format can display only one text or title field.');
        break;
      }
    }

    // Check if there is at least one image field.
    if ($image_field_count < 1) {
      $errors[] = t('There are no image fields. This format requires at least one image field.');
    }

    return $errors;
  }
}
