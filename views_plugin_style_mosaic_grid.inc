<?php
/**
 * @file
 * Contains the Mosaic Grid style plugin.
 */
class views_plugin_style_mosaic_grid extends views_plugin_style {
  public $rendered_mosaic_items = array();
  public $rendered_field_values = array();
  public $image_styles = array();
  public $field_now_rendered = NULL;

  /**
   * Sets default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['max_row_height']       = array('default' => 400);
    $options['gap']                  = array('default' => 2);
    $options['max_rows']             = array('default' => 0);
    $options['truncate']             = array('default' => FALSE);
    $options['captions']             = array(
      'default' => array(
        'source'       => array(),
        'display'      => 'hover',
        'height'       => array(
          'option' => 'auto',
          'value'  => 2.0,
        ),
        'no_padding_h' => FALSE,
      ),
    );

    return $options;
  }

  /**
   * Renders options form.
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

    // Caption options fieldset.
    $form['captions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Caption settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Title and description for the list of text source selectors.
    $form['captions']['source']['title'] = array(
      '#prefix' => '<label>',
      '#markup' => t('Text source'),
      '#suffix' => '</label>',
    );

    $form['captions']['source']['description'] = array(
      '#prefix' => '<div class="description">',
      '#markup' => t('A text or title field can be added to the view, to be used as caption.'),
      '#suffix' => '</div>',
    );

    // Set options list for text source selector.
    $caption_options = array(
      'alt_tag'      => t('Image alt tag'),
      'title_tag'    => t('Image title tag'),

    );
    // Get all text and title fields and add them to the options list.
    $text_fields = $this->get_all_text_fields(TRUE);
    if (!empty($text_fields)) {
      foreach ($text_fields as $text_field) {
        $text_field_id = $text_field->options['id'];
        $text_field_name = $text_field->ui_name();
        $caption_options[$text_field_id] = $text_field_name;
      }
    }

    // Create caption selector form and related state conditions for
    // all available image fields.
    $image_fields = $this->get_all_image_fields();
    $state_conditions = array();
    if (!empty($image_fields)) {
      foreach ($image_fields as $image_field) {
        $image_field_id = $image_field->options['id'];
        $default_value = '';
        if (isset($this->options['captions']['source'][$image_field_id])) {
          $default_value = $this->options['captions']['source'][$image_field_id];
        }

        // Create a label with the field name above(!) the select control.
        // We could use a #description param for that, but placing
        // the label above the control reads better.
        $label = 'label_' . $image_field_id;
        $form['captions']['source'][$label] = array(
          '#prefix' => '<div class="description" style="margin-top: 12px; margin-bottom: -5px">',
          '#markup' => $image_field->ui_name(),
          '#suffix' => '</div>',
        );

        // Create dropdow selector with text source options.
        $form['captions']['source'][$image_field_id] = array(
          '#type' => 'select',
          '#options' => $caption_options,
          '#empty_option' => t('None'),
          '#default_value' => $default_value,
        );

        // Add state condition that disables related form elements if all image selectors are set to None.
        $state_conditions[':input[name="style_options[captions][source][' . $image_field_id . ']"]'] = array('value' => '');
      }
    }

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
        'disabled' => $state_conditions,
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
      '#attributes' => array(
        'class' => array('container-inline'),
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
        'disabled' => $state_conditions,
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
        'disabled' => $state_conditions,
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
        'disabled' => $state_conditions,
      ),
    );
  }


  /**
   * Tries to guess and store image style used on each of the view's image fields.
   * Image style can change image proportions and this preliminary check
   * is needed to set image width and height inside Mosaic Grid more correctly.
   * Some lightbox formatters used in Backdrop CMS are taken into account.
   */
  function pre_render($result) {
    parent::pre_render($result);

    $image_fields = $this->get_all_image_fields();
    foreach ($image_fields as $image_field) {
      $image_field_formatter = $image_field->options['type'];
      $image_field_settings  = $image_field->options['settings'];
      $image_field_id        = $image_field->options['id'];
      $image_style           = '';

      // Try to guess image field style based on formatter type.
      if (isset($image_field_formatter)) {
        switch ($image_field_formatter) {
          case 'image':
            $image_style = $image_field_settings['image_style'];
            break;
          case 'glightbox':
            $image_style = $image_field_settings['glightbox_image_style_content'];
            break;
          case 'featherlight':
            $image_style = $image_field_settings['featherlight_image_style_content'];
            break;
          case 'photoswipe':
            $image_style = $image_field_settings['photoswipe_node_style'];
            break;
          case 'colorbox':
            $image_style = $image_field_settings['colorbox_node_style'];
            break;
        }
      }
      $this->image_styles[$image_field_id] = $image_style;
    }
  }

  /**
   * Render all of the fields for a given style and store them on the object.
   * 
   * This is a slight modification of the parent views_plugin_style:render_fields().
   * It tracks what field is being rendered and the field's id can be accessed
   * via $field_now_rendered public variable. That allows theme_item_list() hook
   * to associate rendered value with a field id where value comes from. Once render
   * is done the result is stored in array where all field values are kept separate
   * and not in an html list.
   */
  function render_fields($result) {
    if (!$this->uses_fields()) {
      return;
    }

    if (!isset($this->rendered_fields)) {
      $this->rendered_fields = array();
      $this->view->row_index = 0;
      $keys = array_keys($this->view->field);

      // If all fields have a field::access FALSE, there might be no fields; so
      // there is no reason to execute this code.
      if (!empty($keys)) {
        foreach ($result as $count => $row) {
          $this->view->row_index = $count;
          foreach ($keys as $id) {
            // Set variable that contains currently rendered field id.
            $this->field_now_rendered = $id;
            // This is where theme_item_list hook is called and separate values of
            // multi value fields are collected and stored in rendered_field_values.
            $this->rendered_fields[$count][$id] = $this->view->field[$id]->theme($row);
            unset($this->field_now_rendered);
          }

          $this->row_tokens[$count] = $this->view->field[$id]->get_render_tokens(array());
        }
      }
      unset($this->view->row_index);
    }

    // Merge/union the array with rendered fields and the array with mutivalue fields.
    // The first one is the result of this function. The second array contains separate
    // (not rendered as a list) values captured by the theme_item_list hook.
    foreach ($this->rendered_fields as $row_index => $row_data) {
      if (!empty($this->rendered_field_values[$row_index])) {
        $rendered_merged[$row_index] = $this->rendered_field_values[$row_index] + $row_data;
      }
      else {
        $rendered_merged[$row_index] = $row_data;
      }
    }
    // Now the array should contain all field values, both single and multiple.
    $this->rendered_field_values = $rendered_merged;

    return $this->rendered_fields;
  }


  /**
   * Returns all field handlers of specified type.
   * Works only for views_handler_field_field types that have field_info and for title fields.
   */
  protected function get_all_fields_of_type(string $field_type, bool $excluded = FALSE) {
    $fields = $this->view->display_handler->get_handlers('field');

    $result = array();

    foreach ($fields as $field) {
      // Ignore excluded fields if not specified otherwise.
      if (!empty($field->options['exclude']) && !$excluded)
        continue;

      if (isset($field->field_info) || $field_type == 'title' || $field_type == 'nothing') {
        $type = FALSE;
        if (!empty($field->field_info['type'])) {
          $type = $field->field_info['type'];
        }
        // If it is of required type, return field handler.
        // Title and custom text has no field_info, so check options['field'] instead.
        $is_title       = $field_type == 'title'   && $field->options['field'] == 'title';
        $is_custom_text = $field_type == 'nothing' && $field->options['field'] == 'nothing';
        if ($field_type == $type || $is_title || $is_custom_text) {
          $result[] = $field;
        }
      }
    }
    return (count($result) > 0) ? $result : NULL;
  }


  /*
   * Returns all image field handlers.
   */
  function get_all_image_fields(bool $excluded = FALSE) {
    return $this->get_all_fields_of_type('image', $excluded);
  }


  /*
   * Returns all acceptable text field handlers.
   */
  function get_all_text_fields(bool $excluded = FALSE) {
    $result = [];
    // Get possible text field types.
    $text_types = $this->get_text_field_types();
    // Search for each possible type of text field.
    foreach ($text_types as $text_type) {
      $fields = $this->get_all_fields_of_type($text_type, $excluded);
      // Save found text fields to array.
      if (!empty($fields)) {
        //array_push($result, ...$fields);
        $result = array_merge($result, $fields);
      }
    }
    return (count($result) > 0) ? $result : NULL;
  }


  /**
   * Returns array of text field types.
   * 'nothing' stands for Custom text field.
   */
  function get_text_field_types() {
    return array('nothing', 'title', 'text', 'text_long', 'text_with_summary');
  }


  /**
   * Validates the view configuration.
   * Fails if current display has no image field.
   */
  function validate() {
    $errors = parent::validate();

    // Do not validate when view is being created as there is no image field
    // by default, and validation will fail.
    if (!$this->view->editing) {
      return $errors;
    }

    // Check if any image fields are present and report an error if none.
    $image_fields = $this->get_all_image_fields();
    if (empty($image_fields)) {
      $errors[] = t('No image fields are attached to %display or they are hidden. %mosaic_grid format requires at least one exposed image field.', array('%mosaic_grid' => $this->plugin_title(), '%display' => $this->display->display_title));
    }

    return $errors;
  }
}
