<?php

function topic_management() {
	global $user;
	$oep_util = new OepUtil();
	
    if(!isset($_GET["topic_id"]) || !$oep_util->verify_topic_with_author_set($_GET["topic_id"]))
    {
    	$form['error'] = array(
    	  '#type' => 'markup',
	      '#markup' => '<p>The form is not ready.</p>',
    	);
    }
    else
    {
    	$images_root = base_path() . drupal_get_path('module', 'topicmgr') . '/images/';
    	$topic_id = $_GET["topic_id"];
    	
    	$topic_name = $oep_util->get_topic_name($topic_id);
    	
		$form['topic'] = array(
		  '#type' => 'fieldset',
	      '#title' => t('TOPIC'),
	      '#weight' => 5,
	      '#collapsible' => TRUE, 
	      '#collapsed' => FALSE,
		);
		$article_node_id = $oep_util->get_article_id($topic_id);
		$article_node = node_load($article_node_id);
		$topic_node = node_load($topic_id);
		if(count($topic_node->field_source)==0)
		  $source = "none";
		else {
		  $term = taxonomy_term_load($topic_node->field_source["und"][0]["tid"]);
		  $source = $term->name;
		}
	    $form['topic']['topic_info'] = array (
	      '#type' => 'markup',
	      '#markup' => '<h2>' . $topic_name . '</h2>
	      				<br/>
	      				<p><b>Category</b>: ' . $oep_util->get_topic_category($topic_id) . '</p>
	                    <p><b>County</b>: ' . $oep_util->get_topic_county($topic_id) . '</p>
	                    <p><b>Article Maximum Length</b>: ' . $article_node->field_submission_length["und"][0]["value"] . '</p>
	                    <p><b>Source</b>: ' . $source . '</p>
	                    <p><b>Date Created</b>: ' . date('Y-m-d H:i:s', $topic_node->created) . '</p>',
	                   
	    );
	    
	    $form['topic_timeline'] = array(
		  '#type' => 'fieldset',
	      '#title' => t('TOPIC TIMELINE'),
	      '#weight' => 10,
	      '#collapsible' => TRUE, 
	      '#collapsed' => FALSE,
		);
	    
	    $form['topic_timeline']['authors'] = array (
	      '#type' => 'markup',
	      '#markup' => $oep_util->insert_libs() .
	                    '
						<link href="' . base_path() . drupal_get_path('module', 'topicmgr') . '/libs/yui/build/fonts/fonts-min.css" type="text/css" rel="stylesheet">
						<link href="' . base_path() . drupal_get_path('module', 'topicmgr') . '/libs/yui/build/tabview/assets/skins/sam/tabview.css" type="text/css" rel="stylesheet">
						<script src="' . base_path() . drupal_get_path('module', 'topicmgr') . '/libs/yui/build/yahoo-dom-event/yahoo-dom-event.js" type="text/javascript"></script>
						<script src="' . base_path() . drupal_get_path('module', 'topicmgr') . '/libs/yui/build/element/element-min.js" type="text/javascript"></script>
						<script src="' . base_path() . drupal_get_path('module', 'topicmgr') . '/libs/yui/build/tabview/tabview-min.js" type="text/javascript"></script>
  						
						<div class="yui-skin-sam">
							<div class="yui-navset yui-navset-top" id="authors">
							    <img id="add_tab1" height="30" width="33" style="cursor: pointer; margin-bottom:-35px; visibility: hidden;" src="' . $images_root .'add.gif"></img>
							    <ul class="yui-nav"> 
							    </ul>
							    <div class="yui-content">
							    </div>
							</div>
						</div>
	    				<div id="dialog" title="Add New Author" ></div>
	    				<div id="add_note_dialog" title="Add New Note"></div>
	    		 		<div id="edit_note_dialog" title="Edit Note"></div>
	    		 		<div id="caution_dialog" title="Notice"></div>
	    		 		
						<script type="text/javascript">
						  var tabView = new YAHOO.widget.TabView("authors");
						  var topic_id = ' . $topic_id . ';
						  var images_root = "' . $images_root .'";
						  var userid = "' . $user->uid .'";
						  var username = "' . $user->name .'";
	    				  var ajax_callback_str = "'. base_path() .'ajax_callback";
					   </script>
					   <script src="' . base_path() . drupal_get_path('module', 'topicmgr')  .'/scripts/topicmgr_author_topic_assgmt.js"></script>',);
    }
	return $form;
}

?>