<?php
// $Id$

/**
 * @file
 */


/** Form pages and submit function **/
function approve_page() {
  $nid = arg(2);
  // Set the path to our script.
  $script = drupal_get_path('module', 'interact') . '/interact.js';
  // Include a JavaScript file.
  $js = drupal_add_js($script);
  drupal_add_library('interact', 'corner');
  drupal_add_js('jQuery(".roundme").corner();', array('type' => 'inline', 'scope' => 'footer'));
  drupal_add_js('misc/ajax.js');
  
  $approve_page = array();

  $node = node_load($nid, OepUtil::get_latest_revision($nid));

  /** Build Forms**/
  $form['title']['#markup'] = "<h2>Assign Reviewers: $node->title</h2>";
  build_editorial_review_group_list($form, $node);
  build_fact_checker_list($form, $node);
  add_min_review_form($form, $node);

  $items = field_get_items('node', $node, 'field_required_review', $node->language);
  $min_reviews = $items[0]['value'];
  
 
  
  /** Theming Assigned Group reviewers **/
  $assigned_erg_uids = array();
  $my_item = array();
  $items = field_get_items('node', $node, 'field_editorial_review_group', $node->language);
  if ( $items ) {
    foreach ( $items as $item ) {
      $uid = intval($item['target_id']);
      $assigned_erg_uids[] = $uid;
      $user = user_load($uid);
      $my_item[] = array('uid'=>$user->uid, 'name'=>$user->name);
    }
  }
  $output = theme('reviewer_list', array(
    'reviwer_type'=>'erg',
    'field_name' => 'field_editorial_review_group',
    'title' => 'Assigned Group Editors',
    'items' => $my_item,
    'type' => 'ol',
    'attributes' => array(),
    'nid' => $nid,
  ));
  
  $my_item = array();
  $items = field_get_items('node', $node, 'field_fact_checker', $node->language);
  if ( $items ) {
    foreach ($items as $item){
      $uid = intval($item['target_id']);
      $user = user_load( $uid );
      $my_item[] = array('uid'=>$user->uid, 'name'=>$user->name);
    }
  }
  $output .= theme('reviewer_list', array(
    'reviwer_type'=>'fc',
    'field_name' => 'field_fact_checker',
    'title' => 'Assigned Fact Checkers',
    'items' => $my_item,
    'type' => 'ol',
    'attributes' => array(),
    'nid' => $nid,
  ));

  $form['output']['#type'] = 'markup';
  $form['output']['#markup'] = $output;
   /** Theming minimum reviews **/
  $form['min_review']['#type'] = 'markup';
  $form['min_review']['#markup'] = "<h3>Minimum Review</h3><ul><li>$min_reviews</li></ul>";
  
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );

  return $form;
}

function approve_page_submit($form, &$form_state) {
  $nid = intval($form['nid']['#value']);
  $node = node_load($nid, OepUtil::get_latest_revision($nid));
  $node_state = $node->workbench_moderation['current']->state;
  $node->workbench_moderation_state_current = $node_state;
  $node->workbench_moderation_state_new = $node_state;
  //update node vid to latest revision
//  $sql = "select max(vid) vid
//  		  from {node_revision}
//  		  where nid = $nid";
//  $result = db_query($sql);
//  $record = $result->fetchObject();
//  $node->vid = $record->vid;

//  $article_state = $node->workbench_moderation['current']->state;
//  $node->workbench_moderation_state_current = $article_state;
//  $node->workbench_moderation_state_new = 'needs_review';
  
  
//new button handler function//////////////////////////
//just rewrote so any of 3 button calls all 3 of myeonwoo's submit functions

//minimum button
 if($form_state['clicked_button']['#value'] == $form_state['values']['erg_button']) 
   { 
     // Editorial Review Group
    /** Get assgined reviwers **/
      $assigned_reviewer_ids = array();
      $group_id = $form_state['values']['erg_list'];
      $list = $node->field_authors['und'];
      //$group_ok = 1;
     
      $currentGroup = $node->field_editorial_review_group['und'];
	  foreach($currentGroup as $key=>$value){
	         $current[] = $value['target_id'];}
	 
      foreach($list as $key=>$value){
      $authors[] = $value['target_id'];}
       //dsm($authors);
      $sql = "SELECT u.name, u.uid from {user_group} ug
                JOIN {users} u 
                ON u.uid = ug.uid
                WHERE ug.group_id = :gid";
      $r = db_query($sql, array(':gid' => $group_id));
     
	  //$users is array of editorial group members' uids
	  foreach($r as $element=>$user){
              $users[] = $user->uid;}

       //check whether author of article is member of editorial group  
      foreach ($users as $id){
          if(in_array($id, $authors)){
	          drupal_set_message("Author of article is in the assigned editorial group, please remember to remove author from assigned group");
	   //       $group_ok = 0;   
          }
       }
		
	 // if($group_ok == 1) {
//	   		unset ($node->field_editorial_review_group['und']);
	        foreach ($users as $id){
	        	//add new member not in current editorial review group
	        	$found = false;
	        	foreach ($node->field_editorial_review_group['und'] as $member)
	        		if($member['target_id'] == $id)
	        		{
	        			$found =true;
	        			break;
	        		}
	        	if(!$found)
	        		$node->field_editorial_review_group['und'][] = array("target_id" => "$id");
	        }
	  // }
//	   if(count($node->field_editorial_review_group['und']) >= $node->field_required_review['und'][0]['value'] ){
//	     	$node->workbench_moderation_state_new = 'reviewer_assigned';
//	      	drupal_set_message("Article was assigned for reviewing successfully.");
//	   }
	   node_save($node);
}

elseif ($form_state['clicked_button']['#value'] == $form_state['values']['fact_checker_button'] )
   {
    // 'Fact Checker' 
    /** Get assgined reviwers **/
    $assigned_reviewer_ids = array();
    if ( isset( $node->field_fact_checker['und'] ) ){
      foreach ( $node->field_fact_checker['und'] as $row ){
        $assigned_reviewer_ids[] = $row['target_id'];
      }
    }
    
    /** Get requested reviewers **/
    $requested_uid = array($form['fact_checker']['fact_checker_list']['#value']);

    /** Remove uid pre-assigned **/
    $add_ids = array_diff($requested_uid, $assigned_reviewer_ids);

    /** update reviewers **/
    if ( count($add_ids) > 0) {
      foreach ( $add_ids as $id ){
        $node->field_fact_checker['und'][] = array("target_id" => "$id");
      }
     
    }

    node_save($node);
  }

 elseif ($form_state['clicked_button']['#value'] == $form_state['values']['min_reviews_button'])  
    {/** Get requested reviwers **/
    	$requested_num = $form['min_reviews']['min_reviews_list']['#value'];
    //@todo handle the db entry for review group 
    
    
//    $items = field_get_items('node', $node, 'field_required_review', $node->language);
//    $min_current = $items ? $items[0]['value'] : false;
//    
//    if ( $min_current != $requested_num ) {
//      $node->field_required_review['und'][0] = array("value" => "$requested_num");
//      node_save($node);
//       }
		$node->field_required_review['und'][0] = array("value" => "$requested_num");
		node_save($node);
     }
    
}









//
//
////**Edit Author Form **///
//function edit_author(&$form, $form_state, $nid) {
//  // Set the path to our script.
//  $script = drupal_get_path('module', 'interact') . '/interact.js';
//  // Include a JavaScript file.
//  $js = drupal_add_js($script);
//  drupal_add_library('interact', 'corner');
//  drupal_add_js('jQuery(".roundme").corner();', array('type' => 'inline', 'scope' => 'footer'));
//  drupal_add_js('misc/ajax.js');
//  
//  /** Load the node **/
//  $node = node_load($nid);
//  
//  // Get assigned uids for each field
//  $assigned_author_uids = array();
//  $my_item = array();
//  $assigned_erg_users = array();
//  $assigned_author_items = array();
//  foreach ($node->field_authors as $row){
//    foreach ( $row as $key => $value){
//      $uid = intval($value['target_id']);
//      $user = user_load( $uid );
//      $assigned_author_uids[] = $uid;
//      $assigned_erg_users[] = $user;
//      $my_item[] = array(
//        'uid'=>$user->uid, 
//        'name'=>$user->name
//      );
//      $assigned_author_items[] = $user->name; 
//    }
//  }
//
//  /** Build list of selectable authors **/
//  $fc_list = get_author_names();
//  //$fc_list = array(9 => t('limm_cooridinator'), 10 => t('limm_author'));
//
//  /** Add the list of authors and build submit button **/
//  $form['author'] = array (
//    '#type' => 'fieldset',
//    '#title' => 'List of Authors',
//    '#collapsible' => TRUE,
//    '#collapsed' => TRUE,
//  );
//  $form['author']['author_list'] = array(
//    '#type' => 'select',
//    '#options' => $fc_list,
//  );
//  $form['author']['author_button'] = array (
//    '#value' => t('Assign Author'),
//    '#type' => 'submit',
//  );
//  
//  $variables = array(
//      'field_name' => 'field_authors',
//      'title' => 'List of assigned authors',
//      'items' => $my_item,
//      'type' => 'ol',
//      'attributes' => array(),
//      'nid' => $nid
//  );
//  $output = theme('oep_article_author_list', $variables);
//  
//  $form['output']['#type'] = 'markup';
//  $form['output']['#markup'] = $output;
//  
//  $form['nid'] = array(
//    '#type' => 'value',
//    '#value' => $nid,
//  );
//
//  return $form;
//}
//
////**Edit Author Form Submit Function
//function edit_author_submit($form, &$form_state) {
//  
//  $nid = intval($form['nid']['#value']);
//  $node = node_load($nid);
//  /** Keep moderation state **/
//  $article_state = $node->workbench_moderation['current']->state;
//  $node->workbench_moderation_state_current = $article_state;
//  $node->workbench_moderation_state_new = $article_state;
//  
//  /** Get assigned authors **/
//  $author_ids = array();
//  if ( isset( $node->field_authors['und'] ) ){
//    foreach ( $node->field_authors['und'] as $row ){
//      $author_ids[] = $row['target_id'];
//    }
//  }
//  
//  /** Get requested reviwers **/
//  $requested_uid = array($form['author']['author_list']['#value']);
//
//  /** Remove uid pre-assigned author **/
//  $add_ids = array_diff($requested_uid, $author_ids);
//  /** update reviewers **/
//  if ( count($add_ids) > 0) {
//    foreach ( $add_ids as $id ){
//      $node->field_authors['und'][] = array("target_id" => "$id");
//    }
//    node_save($node);
//  }
//  
//}


/** Assign Reviewers Form(s) building methods *******/
function build_editorial_review_group_list(&$form){
  $sql = "SELECT group_name, group_id from groups ";//where type='ed_group'
  $r = db_query($sql);
  $ids = array();
  
  foreach ($r as $row) {
    $ids[$row->group_id] = $row->group_name;
  }
 
//  
//  $erg_list = array();
//  foreach ($nodes as $node) {
//    $erg_list[intval($node->nid)] = $node->title;
//  }
  
  $form['erg'] = array (
    '#type' => 'fieldset',
    '#title' => "Editorial Review Group",
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['erg']['erg_list'] = array(
    '#type' => 'select',
    '#options' => $ids,
	
  );
  $form['erg']['erg_button'] = array (
    '#value' => t('Assign editorial review group'),
    '#type' => 'submit',
  );

 }

function build_fact_checker_list(&$form, $node){
  $fc_list = get_factchecker_names();
  //$fc_list = array(9 => t('limm_cooridinator'), 10 => t('limm_author'));
  $default = $node->field_fact_checker['und'][0]['target_id'];
  
  
  $form['fact_checker'] = array (
    '#type' => 'fieldset',
    '#title' => 'Fact Checker',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fact_checker']['fact_checker_list'] = array(
    '#type' => 'select',
    '#options' => $fc_list,
	'#default_value' => $default,
  );
  $form['fact_checker']['fact_checker_button'] = array (
    '#value' => t('Assign fact checker'),
    '#type' => 'submit',
  );

}

function add_min_review_form(&$form, $node){
  $min_list = range(0, 10);
  if(isset($node->field_required_review['und'][0]['value']))
	{$default = $node->field_required_review['und'][0]['value'];}
  else $default = '3';
  $form['min_reviews'] = array (
    '#type' => 'fieldset',
    '#title' => 'Minimum Reviews',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['min_reviews']['min_reviews_list'] = array(
    '#type' => 'select',
    '#options' => $min_list,
	'#default_value' => $default,
  );
  $form['min_reviews']['min_reviews_button'] = array (
    '#value' => t('Assign minimum reviews'),
    '#type' => 'submit',
  );

  }

//function build_copy_editor_list(&$form){
//  //$copy_editor_list = array(9 => t('limm_cooridinator'), 10 => t('limm_author'));
//  $copy_editor_list = get_copy_editor_names();
//
//  //button 3
//  $form['copy_editor'] = array (
//    '#type' => 'fieldset',
//    '#title' => 'Copy Editor',
//   '#collapsible' => TRUE,
//    '#collapsed' => TRUE,
//  );
//  $form['copy_editor']['copy_editor_list'] = array(
//    '#type' => 'select',
//    '#options' => $copy_editor_list,
//  );
//  $form['copy_editor']['copy_editor_button'] = array (
//    '#value' => t('Assign copy editor'),
//    '#type' => 'submit',
//  );
//}


/** function to get name arrays from uid queries **/
//function get_author_names(){
//  $sql = "select A.uid, U.name
//          from (select uid from users_roles where rid in (4,12) ) A
//          left join users U
//          on A.uid = U.uid";
//  $r = db_query($sql);
//  $authors = array();
//  foreach ($r as $row) {
//    $authors[$row->uid] = $row->name;
//  }
//  return $authors;
//}
//
function get_factchecker_names(){
  $sql = "select A.uid, U.name
          from (select uid from users_roles where rid in (7) ) A
          left join users U
          on A.uid = U.uid";
  $r = db_query($sql);
  $authors = array();
  foreach ($r as $row) {
    $authors[$row->uid] = $row->name;
  }
  return $authors;
}
//
//function get_copy_editor_names(){
//  $sql = "select A.uid, U.name
//          from (select uid from users_roles where rid in (14) ) A
//          left join users U
//          on A.uid = U.uid";
//  $r = db_query($sql);
//  $authors = array();
//  foreach ($r as $row) {
//    $authors[$row->uid] = $row->name;
//  }
//  return $authors;
//}


//Deprecated///////////

// function approve_page_old(&$form, $form_state, $nid) {
//   // Set the path to our script.
//   $script = drupal_get_path('module', 'interact') . '/interact.js';
//   // Include a JavaScript file.
//   $js = drupal_add_js($script);
//   drupal_add_library('interact', 'corner');
//   drupal_add_js('jQuery(".roundme").corner();', array('type' => 'inline', 'scope' => 'footer'));
//   drupal_add_js('misc/ajax.js');
//   
//   $approve_page = array();
//   
//   $node = node_load($nid);
//   
//   /** Build Assigned Group reviewers Theme **/
//   // Get assigned uids for each field
//   $assigned_erg_uids = array();
//   foreach ($node->field_editorial_review_group as $row){
//     foreach ( $row as $key => $value){
//       $assigned_erg_uids[] = intval($value['target_id']);
//     }
//   }
//   
//   /** Get User Nodes **/
//   $assigned_erg_users = user_load_multiple($assigned_erg_uids);
//   //$assigned_ce_users = user_load_multiple($assigned_ce_uid);
//   
//   // Get itmes for custom_item_list
//   $assigned_erg_items = array();
//   foreach ( $assigned_erg_users as $key => $value ){
//     $assigned_erg_items[] = $value->name; 
//   }
//   
//   
//   
//   /** Build Assigned Reviewer List**/
//   $my_item = array();
//   foreach ($node->field_editorial_review_group as $row){
//     foreach ( $row as $key => $value){
//       $user = user_load(intval($value['target_id']));
//       $my_item[] = array('uid'=>$user->uid, 'name'=>$user->name);
//     }
//   }
// 
//   /** Build Form Buttons**/
//   build_editorial_review_group_list($form);
//   build_fact_checker_list($form);
//   build_copy_editor_list($form);
//   
//   $output = theme('reviewer_list', array(
//     'reviwer_type'=>'erg',
//     'field_name' => 'field_editorial_review_group',
//     'title' => 'Assigned Group Editors',
//     'items' => $my_item,
//     //'items' => $assigned_erg_items,
//     'type' => 'ol',
//     'attributes' => array(),
//     'nid' => $nid,
//     //'uids' => $assigned_erg_uids,
//   ));
//   
//   $my_item = array();
//   foreach ($node->field_fact_checker as $row){
//     foreach ( $row as $key => $value){
//       $user = user_load(intval($value['target_id']));
//       $my_item[] = array('uid'=>$user->uid, 'name'=>$user->name);
//     }
//   }
//   $output .= theme('reviewer_list', array(
//     'reviwer_type'=>'fc',
//     'field_name' => 'field_fact_checker',
//     'title' => 'Assigned Fact Checkers',
//     'items' => $my_item,
//     'type' => 'ol',
//     'attributes' => array(),
//     'nid' => $nid,
//   ));
//   
//   $my_item = array();
//   foreach ($node->field_copy_editor as $row){
//     foreach ( $row as $key => $value){
//       $user = user_load(intval($value['target_id']));
//       $my_item[] = array('uid'=>$user->uid, 'name'=>$user->name);
//     }
//   }
//   $output .= theme('reviewer_list', array(
//     'reviwer_type'=>'ce',
//     'field_name' => 'field_copy_editor',
//     'title' => 'Assigned Copy Editor',
//     'items' => $my_item,
//     'type' => 'ol',
//     'attributes' => array(),
//     'nid' => $nid,
//   ));
// 
//   $form['output']['#type'] = 'markup';
//   $form['output']['#markup'] = $output;
//   
//   $form['nid'] = array(
//     '#type' => 'value',
//     '#value' => $nid,
//   );
// 
//   return $form;
// }


//old approve form submit handler
//$form['#next_state']
  //$node->workbench_moderation_state_new = $form['#next_state'];
  
  //  **commenting out old submit button handling////////////////////////////////////////////
  
  //if minimum reviews button is clicked
//   if($form_state['clicked_button']['#value'] == $form_state['values']['min_reviews_button']){	  
//     /** Get requested reviwers **/
//     $requested_num = $form['min_reviews']['min_reviews_list']['#value'];
//     //@todo handle the db entry for review group 
//     
//     
//     $items = field_get_items('node', $node, 'field_required_review', $node->language);
//     $min_current = $items ? $items[0]['value'] : false;
//     
//     if ( $min_current != $requested_num ) {
//       $node->field_required_review['und'][0] = array("value" => "$requested_num");
//       node_save($node);
//     }
    
  
//      //if erg button 1 is clicked
//   } else if($form_state['clicked_button']['#value'] == $form_state['values']['erg_button']){	 
//     /** Get assgined reviwers **/
//       $assigned_reviewer_ids = array();
//       $group_id = $form_state['values']['erg_list'];
//       $sql = "SELECT u.name, u.uid from {user_group} ug
//                 JOIN {users} u 
//                 ON u.uid = ug.uid
//                 WHERE ug.group_id = :gid";
//       $r = db_query($sql, array(':gid' => $group_id));
//       foreach ($r as $id){          
//           $node->field_editorial_review_group['und'][] = array("target_id" => "$id->uid");
//       }
//       if(count($node->field_editorial_review_group['und']) > $node->field_required_review['und'][0]['value'] ){
//           $node->workbench_moderation_state_new = 'reviewer_assigned';
//       }
//       node_save($node);
//     /** Get requested reviwers **/
// //    $review_group_nid = $form['erg']['erg_list']['#value'];
// //    dpm($form['erg']['erg_list']['#value'], 'form erg values');
// //    $group_review_node = node_load($review_group_nid);
// //    $review_group_ids = array();
// //    foreach ( $group_review_node->field_group_member['und'] as $row ){
// //      $review_group_ids[] = $row['target_id'];
// //    }
// //    
// //    $add_ids = array_diff($review_group_ids, $assigned_reviewer_ids);
// //    
// //    /** update reviewers **/
// //    if ( count($add_ids) > 0) {
// //      foreach ( $add_ids as $id ){
// //        $node->field_editorial_review_group['und'][] = array("target_id" => "$id");
// //      }
// //      node_save($node);
// //    }
//     }
//   // when 'Fact Checker' is clicked
//   else if($form_state['clicked_button']['#value'] == $form_state['values']['fact_checker_button']){  
//     
//     /** Get assgined reviwers **/
//     $assigned_reviewer_ids = array();
//     if ( isset( $node->field_fact_checker['und'] ) ){
//       foreach ( $node->field_fact_checker['und'] as $row ){
//         $assigned_reviewer_ids[] = $row['target_id'];
//       }
//     }
//     
//     /** Get requested reviwers **/
//     $requested_uid = array($form['fact_checker']['fact_checker_list']['#value']);
// 
//     /** Remove uid pre-assigned **/
//     $add_ids = array_diff($requested_uid, $assigned_reviewer_ids);
// 
//     /** update reviewers **/
//     if ( count($add_ids) > 0) {
//       foreach ( $add_ids as $id ){
//         $node->field_fact_checker['und'][] = array("target_id" => "$id");
//       }
//       node_save($node);
//     }
//   }
// }


// /**
 // * Builds and returns the scaffolding settings form.
 // */
// function build_page() {
  // $form['input_example_setting'] = array(
    // '#type' => 'textarea',
    // '#value' => 'fdf',
    // '#title' => t('Example setting'),
    // '#description' => t('This is an example setting.'),
    // '#required' => TRUE,
  // );
  
  // $active = array(0 => t('Closed'), 1 => t('Active'));
  // $form['computer'] = array(
    // '#title' => t('Setting Editorial Review Group'),
    // '#type' => 'radios',
    // '#options' => $active,
    // '#required' => TRUE,
  // );

  // return ($form);
// }

// function build_page1(){
	// $output = '<p>hello</p>';
	// return $output;
// }
