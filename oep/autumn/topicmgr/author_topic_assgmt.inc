<?php

function author_topic_assgmt() {
	$oep_util = new OepUtil();
	$tmp_authors = $oep_util->get_all_authors();
	$authors = array();
	
	foreach($tmp_authors as $tmp_author)
	  $authors[$tmp_author['uid']] = $tmp_author['name'];
	
	if(!isset($_GET['topic_id']))
		$form['topic_title'] = array (
				'#type' => 'markup',
				'#markup' => '<h3>The form is not ready</h3>',
		);
	else{
		$topic = node_load($_GET['topic_id']);
		
		$form['topic_title'] = array (
		      '#type' => 'markup',
		      '#markup' => '<h2 style="margin-bottom:10px;">Topic: ' . $topic->title .'</h2>',
		);
		
		$form['author_radios'] = array(
	          '#type' => 'radios',
			  '#title' => t('Choose an author'),
			  '#options' => $authors,
		);
	
		$form['submit'] = array(
	          '#type' => 'submit',
	          '#value' => t('Apply'),
		);
	}
	return $form;
}

function author_topic_assgmt_submit($form, $form_state) {
	global $user;
	$author_id = $form_state['values']['author_radios'];
	
	if(!isset($_GET['topic_id']))
	  drupal_set_message('Topic ID is not recognized','error');
	else {
		$topic_id = $_GET['topic_id'];
		$oep_util = new OepUtil();
		$oep_util->assign_topic_to_author($topic_id,$author_id);
		//create new associated article for this topic
			
		$node = new StdClass();
		//creating a bare node
		
		$node->uid = $user->uid;
		//submitter
			
		$node->type = 'article';
		//giving it type
			
		$node->status = 0;
		//give it a published staus
			
		$node->title = $oep_util->get_topic_title($topic_id);
		//gives title
		
		$node->body = "";
		//gives body
			
		$node->language = "und";
		//gives body
			
		$node->field_authors['und'][0]['target_id'] = $author_id;
		//add author field to this article
			
		$node->field_associated_topic['und'][0]['target_id'] = $topic_id;
		//add topic field to this article
			
		node_save($node);
		//save it and give it the rest of the attributes

		$topic = node_load($topic_id);
		$topic->field_associated_article['und'][0]['target_id'] = $node->nid;
		node_save($topic);
		
//		$oep_util->bind_article($topic_id,$node->nid);
		//bind article to this topic
			
		drupal_goto("mypages/topics");
	}
}

/**
 * Validate Author submission.
 */
function author_topic_assgmt_validate($form, &$form_state) {
	if (!isset($form_state['values']['author_radios'])) {
		form_set_error('author_radios', t('An author must be chosen'));
	}
}

?>