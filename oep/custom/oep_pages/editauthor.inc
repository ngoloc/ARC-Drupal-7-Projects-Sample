<?php

function edit_author(){
  $nid = arg(2);
  $article = node_load($nid,OepUtil::get_latest_revision($nid));

  // Set the path to our script.
  $script = drupal_get_path('module', 'interact') . '/interact.js';
  // Include a JavaScript file.
  $js = drupal_add_js($script);
  drupal_add_library('interact', 'corner');
  drupal_add_js('jQuery(".roundme").corner();', array('type' => 'inline', 'scope' => 'footer'));
  drupal_add_js('misc/ajax.js');
  drupal_add_js('misc/jquery.js');
  /** Load the node **/


  /** Build list of selectable authors **/
  $fc_list = get_author_names($article);


  /** Add the list of authors and build submit button **/
  $form['nid'] = array(
  		'#type' => 'value',
  		'#value' => $article->nid,
  );
  $form['article_title'] = array (
  	'#type' => 'markup',
  	'#markup' => t('<h2>' . $article->title . '</h2>'),
  );
  $form['author'] = array (
    '#type' => 'fieldset',
    '#title' => 'List of Authors',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['author']['author_list'] = array(
    '#type' => 'select',
    '#options' => $fc_list,
  );
  $form['author']['author_button'] = array (
    '#value' => t('Assign Author'),
    '#type' => 'submit',
  );
  $assigned_authors = get_assigned_authors($article);
  $author_items = '';
  foreach ($assigned_authors as $uid => $au_name)
  	$author_items .= '<li>' . $au_name . ' | <a href="#" onclick="remove_author('. $uid . ')">remove</a></li>';
  $form['output']['#type'] = 'markup';
  $form['output']['#markup'] = '
  									<h3>List of Assigned Authors</h3>
  									<ul>'.
									  $author_items
									.'  
									</ul>
									<input type="hidden" id="remove_author_id" name="remove_author_id"/>
									<script>
										function remove_author(uid){
											$("#remove_author_id").val(uid);
											document.forms["edit-author"].submit();
										}
									</script>
  							   ';

  return $form;
}

function remove_author($article,$author_id){
	if(count($article->field_authors['und'])<=1)
	{
		drupal_set_message("At least one author must be assigned for this article","error");
		return;
	}
	for($i=0; $i<count($article->field_authors['und']); $i++){
		if($article->field_authors['und'][$i]['target_id'] == $author_id)
		{
			unset($article->field_authors['und'][$i]);
			field_attach_update('node',$article);
			break;
		}
	}
}

function get_assigned_authors($article){
	$assigned_authors = array();
	foreach($article->field_authors['und'] as $author){
		$au = user_load($author['target_id']);
		$assigned_authors[$au->uid] = $au->name;
	}
	return $assigned_authors;
}

function edit_author_submit($form, &$form_state) {
	$nid = intval($form['nid']['#value']);
	
	$article = node_load($nid,OepUtil::get_latest_revision($nid));
	$article_state = $article->workbench_moderation['current']->state;
	$article->workbench_moderation_state_current = $article_state;
	$article->workbench_moderation_state_new = $article_state;
	
	if($_POST["remove_author_id"]!="")
		remove_author($article,$_POST["remove_author_id"]);
	else
	{
		$uid = $form['author']['author_list']['#value'];
	  	$article->field_authors['und'][] = array("target_id"=>$uid);
	  	node_save($article);
	}
}

/** mics methods **/

function contain_author($authors,$uid){
	foreach($authors as $author){
		if($author['target_id']==$uid)
			return true;
	}
	return false;
}

function get_author_names($article){
	//get available authors
	$sql = "select A.uid, U.name
	        from (select uid from users_roles where rid = 4) A
	        	  left join users U
	              on A.uid = U.uid";
	$r = db_query($sql);
	$authors = array();
	foreach ($r as $row) {
		if(!contain_author($article->field_authors['und'],$row->uid))
			$authors[$row->uid] = $row->name;
	}
	return $authors;
}