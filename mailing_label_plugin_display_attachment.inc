<?php
// $Id: mailing_label_plugin_display_attachment.inc,v 1.3.2.3 2009/10/25 00:53:53 acouch Exp $

/**
 * The plugin that handles mailing label attachments.
 * 
 * Creates a special attachment for this purpose only. 
 */
class mailing_label_plugin_display_attachment extends views_plugin_display_attachment {
  
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('row_options', 'row_plugin', 'items_per_page'))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

  function options(&$display) {
    parent::options($display);
    $display['display_options']['items_per_page'] = 0;
    $display['display_options']['row_plugin'] = '';
    $display['display_options']['defaults']['style_options'] = FALSE;
    $display['display_options']['defaults']['items_per_page'] = FALSE;
    $display['display_options']['defaults']['row_plugin'] = FALSE;
    $display['display_options']['defaults']['row_options'] = FALSE;
  } 

  function render() {
    $current_display = $this->view->current_display;
    $fields = $this->view->display[$current_display]->handler->handlers['field'];
    $view=$this->view;
      //catch the inline fields from the defualt view
   $row_options = $this->view->display[$current_display]->handler->options['row_options'];
   
   //if the row options are left as defualts the variable may be null from above so check that and grab default value
   if (empty($row_options)){
     $row_options = $this->view->display['default']->handler->options['row_options'];
   }
   $allFields = array_keys($fields); //grab labels for all the fields - stuff them in a number indexed array


   $inlines= (empty($row_options) ?  Array() : $row_options['inline']) ;

    $i=0;

    foreach ($view->result as $result) {
        foreach ($view->field as $id => $field) {
          $x = check_plain(strip_tags(($view->field[$id]->theme($result))));
          if ($fields[$id]->options['exclude']==0){ //only populate the result is the field is not excluded
            $prepend = '';
            if ($i > 0) {
              $prepend = ( in_array($id,$inlines) ? ' ' : "\n" );
            } 
            $results[$i][$id] = $prepend . $allFields[$id].$x;
          }
        }
    $i++;
    }
    return mailing_label_pdf_form($results,$current_display);
  }


}

