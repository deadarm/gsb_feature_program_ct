<?php

function gsb_feature_program_ct_preprocess_gsb_feature_program_ct_program_menu(&$vars) {

  $node = menu_get_object();
  $language = $node->language;

  $program_node = null;
  if ($node->type == 'program') {
    $program_node = $node;
  }
  else if ($node->type == 'program_instance' || $node->type == 'program_panelized_page') {
    $program_nid = $node->field_program[$language][0]['target_id'];
    $program_node = node_load($program_nid);
  }
  else {
    return;
  }

  if (empty($program_node)) {
    return;
  }

  $wrapper = entity_metadata_wrapper('node', $program_node->nid);

  $program_path_alias = $program_node->path['alias'];
  $program_path_alias_title = explode('/', $program_path_alias);
  $program_path_alias_title = $program_path_alias_title[sizeof($program_path_alias_title) - 1];

  $program_category = $wrapper->field_program_category->value();
  $program_listing_page_url = $program_category->field_program_listing_page_url[$language][0]['url'];

  $path = $program_listing_page_url . '/' . $program_path_alias_title;

  $program_links = array(
    'Overview' => '',
    'Curriculum' => 'curriculum',
    'Faculty' => 'faculty',
    'Participant Profile' => 'participant-profile'
  );
  
  $links = array();

  foreach($program_links as $link_title => $path_suffix) {
    $link['text'] = $link_title;
    $link['path'] = $path . '/' . $path_suffix;
    if (stripos($node->path['alias'], $path_suffix) !== false) {
      $link['options']['attributes'] = array('class' => array('program_menu_item', 'active'));
    }
    else if ($node->type == 'program' && $link_title == 'Overview') {
      $link['options']['attributes'] = array('class' => array('program_menu_item', 'active'));
    }
    else {
      $link['options']['attributes'] = array('class' => array('program_menu_item'));
    }
    $link['options']['html'] = null;
    $links[] = theme('link', $link);
  }

  $additional_pages = $wrapper->field_additional_pages->value();

  if (!empty($additional_pages->field_page_link[$language])) {
    $link = array();
    $link['text'] = $additional_pages->field_page_link[$language][0]['title'];
    $link['path'] = $additional_pages->field_page_link[$language][0]['url'];
    if ($link['path'] == $node->path['alias']) {
      $link['options']['attributes'] = array('class' => array('program_menu_item', 'active'));
    }
    else {
      $link['options']['attributes'] = array('class' => array('program_menu_item'));
    }
    $link['options']['html'] = null;
    $links[] = theme('link', $link);
  }
  
  $vars['program_menu'] = theme('item_list', array('items' => $links));
}
