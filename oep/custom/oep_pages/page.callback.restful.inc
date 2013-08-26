<?php
// $Id$

/**
 *  List of Ajax callbacks for REST API
 *  An Ajax callback which will load some new text for the heading tag.
 */
function interact_unapprove_callback($nid, $uid, $sid, $field, $type = 'ajax') {

  /** TODO: Update article **/
  $node = node_load($nid);
  $article_state = $node->workbench_moderation['current']->state;
  $node->workbench_moderation_state_current = $article_state;
  $node->workbench_moderation_state_new = $article_state;
  
  switch($field) {
    case 'field_editorial_review_group';
      foreach ( $node->field_editorial_review_group['und'] as $key => $value ){
        if ( $value['target_id'] == $uid ) {
          unset($node->field_editorial_review_group['und'][$key]);
          node_save($node);
          break;
        }
      }
      break;
    case 'field_fact_checker';
      foreach ( $node->field_fact_checker['und'] as $key => $value ){
        if ( $value['target_id'] == $uid ) {
          unset($node->field_fact_checker['und'][$key]);
          node_save($node);
          break;
        }
      }
      break;
    case 'field_copy_editor';
      foreach ( $node->field_copy_editor['und'] as $key => $value ){
        if ( $value['target_id'] == $uid ) {
          unset($node->field_copy_editor['und'][$key]);
          node_save($node);
          break;
        }
      }
      break;
    default;
      break;
  }
  
	// This is being delivered as a callback.
  /** TODO: Delete the item clicked. **/
	if ($type == 'ajax') {
		$commands = array();
		//$commands[] = ajax_command_html('#changeme', "<strong>Comment out this. NID:$nid, field_name:$field, UID:$uid, selector:$sid</strong>");
		$commands[] = ajax_command_remove("#$sid");
		$page = array('#type' => 'ajax', '#commands' => $commands);
		ajax_deliver($page);
	}
	else {
		$output = t("This is what would be seen if JavaScript is not enabled.");
		return $output;
	}
}

function interact_withdraw_aruthor_callback( $uid, $nid, $sid, $field = 'field_authors', $type = 'ajax') {

  $node = node_load($nid);
  
  /** Set workbench state value to keep the current state **/
  $article_state = $node->workbench_moderation['current']->state;
  $node->workbench_moderation_state_current = $article_state;
  $node->workbench_moderation_state_new = $article_state;
  foreach ( $node->field_authors['und'] as $key => $value ){
    if ( $value['target_id'] == $uid ) {
      unset($node->field_authors['und'][$key]);
      node_save($node);
    }
  }
  
	// This is being delivered as a callback.
  /** TODO: Delete the item clicked. **/
	if ($type == 'ajax') {
		$commands = array();
		$commands[] = ajax_command_remove("#$sid");
		$page = array('#type' => 'ajax', '#commands' => $commands);
		ajax_deliver($page);
	}
	else {
		$output = t("This is what would be seen if JavaScript is not enabled.");
		return $output;
	}
}



/** Page-callback argument **/
function user_filteredByRole($form, &$form_state) {
  
  $form['filter_params'] = array(
    '#title' => 'Filters',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $sql = "select name from {role} order by weight";
  $result = db_query($sql);
  $options = array();
  foreach ( $result as $value ) {
    $options[t($value->name)] = t($value->name);
  }
  
  // Added AJAX callback.
  $form['filter_params']['user_role'] = array(
    '#title' => t('User Role'),
    '#type' => 'select',
    //'#options' => drupal_map_assoc(array(t('Not Selected'), t('administrator'), t('editor coordinator'), t('author'))),
    '#options' => $options,
    '#ajax' => array(
      'callback' => 'ajax_callback_user',
      'wrapper' => 'user_wrapper',
     ),
  );
  
  // Setting an empty element with a wrapper to be populated.
  $form['filter_params']['article_status_verify'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="input_article_status_verify_wrapper">',
    '#suffix' => '</div>',
  );
  
  $form['output'] = array(
  		'#type' => 'markup',
  		'#prefix' => '<div id="user_wrapper">',
      //'#markup'
  		'#suffix' => '</div>',
  );
  
  // When submitted, check for value of user_role, and populate article_status_verify.
  if (isset($form_state['values']['user_role'])) { 
    $sql = "select name, mail
            from {users} U
            right join (select uid 
                        from {users_roles} U
                        right join (select rid from {role} where name=:role) R
                        on U.rid = R.rid) A
            on U.uid=A.uid";
    $result = db_query($sql, array(':role' => $form_state['values']['user_role']));
    
    $output = '<br />';
    $headers = array(t('User Name'), t('Email'));
    foreach ($result as $user){
      $rows[] = array( t($user->name), t($user->mail) );
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    
    $form['output']['#type'] = 'markup';
    $form['output']['#markup'] = $output;
  }

  return $form;
}

function ajax_callback_user($form, $form_state) {
	return $form['output'];
}