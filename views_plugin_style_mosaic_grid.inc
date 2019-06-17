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

    $options['max_row_height']                = array('default' => 400);
    $options['inner_gap']                     = array('default' => 5  );
    $options['outer_margin']                  = array('default' => 0  );
    $options['caption']                       = array('default' => 'none');
    $options['max_row_height_policy']         = array('default' => 'tail');
 
    $options['more']  = array (
      'default' => array(
        'refit_on_resize'                 => true,
        'refit_on_resize_delay'           => 0,
        'max_rows'                        => 0,
        'responsive_width_threshold'      => 0,
        'max_items_threshold_surpassed'   => 0,
       ),
    );
    
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    
    $form['max_row_height'] = array (
      '#title' => t('Maximum Row Height'),
      '#description' => t('The maximum desired height of rows.'),
      '#type' => 'number',
      '#min' => 10,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['max_row_height'], 
    );
    
    $form['inner_gap'] = array (
      '#title' => t('Inner Gap'),
      '#description' => t('A gap size in pixels to leave a space between elements.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['inner_gap'], 
    );
    
    $form['outer_margin'] = array (
      '#title' => t('Outer Margin'),
      '#description' => t('A margin size in pixels for the outher edge of the whole mosaic.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['outer_margin'], 
    );
    
    // Set options list for caption selector.
    $caption_options = array(
      'none'    => t('None'),
      'alt'     => t('Image Alt Tag'),
      'title'   => t('Image Title Tag'),
    );
    // Detect if text field is present and add it to the options list.
    $text_field = $this->get_first_non_excluded_text_field()['handler'];
    if(!empty($text_field)) {
      $caption_options['text'] = $text_field->ui_name();
    }
    // Create caption selector form.
    $form['caption'] = array(
      '#type' => 'select',
      '#title' => t('Caption Text'),
      '#options' => $caption_options,
      '#default_value' => $this->options['caption'],
      '#description' => t('Select source for image caption text. A text field can be
        used as caption text if added to the view. Note that not all HTML features
        will work in captions.'),
    );
    
    // Assign to variables so that options names can be reused in the description.
    $tail     = t('Tail');
    $skip     = t('Skip');
    $crop     = t('Crop');
    $oversize = t('Oversize');
    
    $form['max_row_height_policy'] = array(
      '#type' => 'select',
      '#title' => t('Maximum Row Height Policy'),
      '#options' => array(
        'tail'     => $tail,
        'skip'     => $skip,
        'crop'     => $crop,
        'oversize' => $oversize,
      ),
      '#default_value' => $this->options['max_row_height_policy'],
      '#description' => t('Defines behaviour for when the remaining items cannot be fitted on
        a row without surpassing the maximum row height. For those cases, choose one of the
        available settings:') . 
        '<ul>' .
        '<li>' . $tail .     ' - ' . t('Renders items respecting their aspect ratio without surpassing the specified Maximum Row Height, resulting in a last row that might not completely fit the screen horizontally.') . '</li>' .
        '<li>' . $skip .     ' - ' . t('Does not renders the unfitting items.') . '</li>' .
        '<li>' . $crop .     ' - ' . t('Caps the desired height to the specified Maximum Row Height, resulting in those items not keeping their aspect ratios.') . '</li>' .
        '<li>' . $oversize . ' - ' . t('Renders items respecting their aspect ratio but surpassing the specified Maximum Row Height.') . '</li>' .
        '</ul>',
    );
    
    $form['more'] = array(
      '#type' => 'fieldset',
      '#title' => t('More'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['more']['refit_on_resize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Refit On Resize'),
      '#description' => t("Whether to rebuild the mosaic when the window is resized or not."),
      '#default_value' => $this->options['more']['refit_on_resize'],
    );
    
    $form['more']['refit_on_resize_delay'] = array(
      '#type' => 'number',
      '#title' => t('Refit On Resize Delay'),
      '#description' => t("Milliseconds to wait after a resize event to refit
        the mosaic. Useful when creating huge mosaics that can take some CPU
        time on the user's browser. Leave it to zero to refit the mosaic in
        realtime."),
      '#min' => 0,
      '#max' => 9999,
      '#default_value' => $this->options['more']['refit_on_resize_delay'],
      '#field_suffix' => t('ms'),
      '#states' => array(
        'enabled' => array(
          ':input[name="style_options\[more\]\[refit_on_resize\]"]' => array('checked' => TRUE),
        ),
      ),
    );
    
    $form['more']['max_rows'] = array (
       '#title' => t('Maximum Rows Number'),
       '#description' => t('If specified, the mosaic will not have more than
         this number of rows. Zero value means all rows will be displayed.
         If Responsive Width Threshold is specified(see below) then
         Maximum Rows Number is not considered when the threshold has been
         reached.'),
       '#type' => 'number',
       '#min' => 0,
       '#max' => 9999,
       '#default_value' => $this->options['more']['max_rows'], 
    );
    
    $form['more']['responsive_width_threshold'] = array (
      '#title' => t('Resposive Width Threshold'),
      '#description' => t('If specified, when the width is less than this, the
        mosaic building logic is not applied, and one item per row is always
        shown. This might help you avoid resulting item sizes that are too small
        and might break complex html/css inside them.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['more']['responsive_width_threshold'],
      '#states' => array(
        'enabled' => array(
          ':input[name="style_options\[more\]\[refit_on_resize\]"]' => array('checked' => TRUE),
        ),
      ),
    );
    
    $form['more']['max_items_threshold_surpassed'] = array (
      '#title' => t('Max Items To Show When Responsive Threshold Surpassed'),
      '#description' => t('If set (and also Responsive Width Threshold is set),
        only this amount of items will be shown when the Responsive Width
        Threshold is met.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 9999,
      '#default_value' => $this->options['more']['max_items_threshold_surpassed'],
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options\[more\]\[responsive_width_threshold\]"]' => array('value' => 0),
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
  protected function get_first_non_excluded_field(string $field_type){
    $fields = $this->display->handler->get_handlers('field');
    
    foreach($fields as $field_name => $field) {
      $type = $field->field_info['type'];
      // Skip excluded fields and fields without field_info.
      if (!empty($field->options['exclude']) || empty($type)){
        continue;
      }
      // If it is of required type return field handler and field name.
      if($field_type == $type){
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
    foreach($text_types as $text_type){
      $field_data = $this->get_first_non_excluded_field($text_type);
      // Return the fist text field found.
      if(!empty($field_data)){
        return $field_data;
      }
    }
    return NULL;
  }
  
  /**
   * Returns array of text field types.
   */
  function get_text_field_types(){
    return array('text','text_long','text_with_summary');
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
    if(!$this->view->editing) {
      return $errors; 
    } 
    
    // Get a list of fields that have been added to the display.
    $fields = $this->display->handler->get_handlers('field');
  
    $image_field_count = 0;
    $text_field_count  = 0;

    foreach ($fields as $field) {
      // Ignore fields excluded from display.
      if (!empty($field->options['exclude'])) {
        continue;
      }
      
      // Check field type.
      $field_type = $field->field_info['type'];
      $is_image   = $field_type == 'image';
      $is_text    = in_array($field_type, $this->get_text_field_types());
      
      // If invalid field type found then validation failed.
      if(empty($field_type) && !$is_image && !$is_text) {
        $errors[] = t('Invalid field types found. This format requires one image field and may optionally have one text field.');
        return $errors;
      }
      
      // Count valid fields.
      if($is_image) {
        $image_field_count ++;
      }
      if($is_text){
        $text_field_count++;
      }
      
      // Check if there is no more than one of each valid field types.
      if($image_field_count > 1){
        $errors[] = t('There are more than one image field. This format can display only one image field.');
        break;
      }
      if($text_field_count > 1){
        $errors[] = t('There are more than one text field. This format can display only one text field as a caption.');
        break;
      }
    }
    
    // Check if there is at least one image field.
    if($image_field_count < 1) {
      $errors[] = t('There are no image fields. This format requires one image field.');
    }
    
    return $errors;
  }
}