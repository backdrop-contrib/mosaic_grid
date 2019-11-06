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
        'source'       => array('default' => '' ),
        'display'      => array('default' => 'hover'),
        'height'       => array('default' => 'auto' ),
        'height_value' => array('default' => 2),
        'no_padding_h' => array('default' => FALSE  ),
      ),
    );

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {

    $form['max_row_height'] = array(
      '#title' => t('Maximum Row Height'),
      '#description' => t('The maximum desired height of rows.'),
      '#type' => 'number',
      '#min' => 10,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['max_row_height'],
    );

    $form['gap'] = array(
      '#title' => t('Gap'),
      '#description' => t(
        'Amount of space between items. To build a grid without gaps a value of <em>-1</em> is recommended.
        This will prevent appearing of irregular one pixel width gaps.'
      ),
      '#type' => 'number',
      '#min' => -1,
      '#max' => 99,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['gap'],
    );

    $form['max_rows'] = array(
      '#title' => t('Maximum Rows Number'),
      '#description' => t('Maximum number of rows to display. Items exceeding this
         row are hidden. Zero value means all rows will be displayed.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#default_value' => $this->options['max_rows'],
    );

    $form['truncate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Truncate'),
      '#description' => t("Hide incomplete last row of items."),
      '#default_value' => $this->options['truncate'],
    );

    $form['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty images'),
      '#description' => t("If checked images with missing files are ignored."),
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
      'alt'        => t('Image Alt Tag'),
      'title'      => t('Image Title Tag'),
    );
    // Detect if text or title field is present and add it to the options list.
    $text_field = $this->get_first_non_excluded_text_field()['handler'];
    if (!empty($text_field)) {
      $caption_options['text'] = $text_field->ui_name();
    }
    // Create caption selector form.
    $form['captions']['source'] = array(
      '#type' => 'select',
      '#title' => t('Text Source'),
      '#options' => $caption_options,
      '#empty_option' => t('None'),
      '#default_value' => $this->options['captions']['source'],
      '#description' => t('Sets source for caption text. A text or title field
        can be added to the view and used as caption.'),
    );

    $caption_display = array(
      'hover'  => t('Appear on mouse over'),
      'static' => t('Placed over image bottom'),
    );

    $form['captions']['display'] = array(
      '#type' => 'select',
      '#title' => t('Display'),
      '#default_value' => $this->options['captions']['display'],
      '#options' => $caption_display,
      '#description' => t('Defines how captions should be displayed.'),
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );

    // Title and description are made as markup elements.
    // This is needed for pretty looking inline placement
    // of the 'select' and 'number' fields below.
    $form['captions']['height_title'] = array(
      '#prefix' => '<label style="margin-bottom: -5px">',
      '#markup' => t('Height'),
      '#suffix' => '</label>',
    );

    $caption_height = array(
      'auto'  => t('Auto'),
      'limit' => t('Set height limit'),
      'fixed' => t('Set constant height'),
    );

    $form['captions']['height'] = array(
      // Start of inline placement.
      '#prefix' => '<div class="container-inline">',
      '#type' => 'select',
      '#options' => $caption_height,
      '#default_value' => $this->options['captions']['height'],
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['captions']['height_value'] = array(
      '#type' => 'number',
      '#min' => 0.1,
      '#max' => 10,
      '#step' => 0.1,
      '#size' => 2,
      '#field_suffix' => t('em'),
      '#default_value' => $this->options['captions']['height_value'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[captions][height]"]' => array('value' => 'auto'),
        ),
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
      // End of inline placement.
      '#suffix' => '</div>',
    );

    $form['captions']['description'] = array(
      '#prefix' => '<div class="description">',
      '#markup' => t(
        'Sets caption height. Units (<a href="@link" target="_blank">em</a>) are relative to capion font size.',
        array('@link' => url('https://en.wikipedia.org/wiki/Em_(typography)'))
      ),
      '#suffix' => '</div>',
    );

    $form['captions']['no_padding_h'] = array(
      '#type' => 'checkbox',
      '#title' => t('No horisontal padding'),
      '#default_value' => $this->options['captions']['no_padding_h'],
      '#description' => t('If checked caption text will be horisontally aligned with grid item.
        This overrides padding set on <a href="@link" target="_blank">configuration page</a>.',
        array('@link' => url('admin/config/media/mosaic_grid'))
      ),
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[captions][source]"]' => array('value' => ''),
        ),
      ),
    );
  }

  /**
   * Returns first non excluded field by type.
   * Returned value is an array containing handler and field's name.
   * Works only for views_handler_field_field types
   * that have field_info.
   */
  protected function get_first_non_excluded_field(string $field_type) {
    $fields = $this->view->display_handler->get_handlers('field');

    foreach ($fields as $field_name => $field) {
      // Ignore excluded fields.
      if (!empty($field->options['exclude']))
        continue;
      $type = $field->field_info['type'];
      // If it is of required type return field handler and field name.
      // Title has no field_info so a second condition added.
      if ($field_type == $type || ($field_type == 'title' && $field_name == 'title')) {
        return array('handler' => $field, 'field_name' => $field_name);
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

    //Do not validate when view is being created as there is no
    //image field by default and validation will fail.
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
      $field_type = $field->field_info['type'];
      $is_image   = $field_type == 'image';
      $is_text    = in_array($field_type, $this->get_text_field_types());
      $is_title   = $field_name == 'title';

      // If invalid field type found then validation failed.
      if (!$is_image && !$is_text && !$is_title) {
        $errors[] = t('Invalid field types found. This format requires one image field and may optionally have one text or title field.');
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
      $errors[] = t('There are no image fields. This format requires one image field.');
    }

    return $errors;
  }
}