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

    $options['max_row_height'] = array('default' => 400);
    $options['gap']            = array('default' => 2  );
    $options['caption']        = array('default' => 'none');
    $options['max_rows']       = array('default' => 0);
    $options['truncate']       = array('default' => false);

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
    
    $form['gap'] = array (
      '#title' => t('Gap'),
      '#description' => t('Amount of space between items.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 99,
      //'#field_suffix' => t('pix'),
      '#default_value' => $this->options['gap'], 
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
    
    $form['max_rows'] = array (
       '#title' => t('Maximum Rows Number'),
       '#description' => t('Maximum number of rows to display. Items exceeding this
          row are hidden.'),
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