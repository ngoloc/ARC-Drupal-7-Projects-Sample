<?php
// $Id$
 include_once drupal_get_path("module", "misc") . "/oeputil.inc";
/* 
 * Page callback; page callback returning a render array.
 */

/** Author Methods **/
function by_article_state($arg){
  $output = '';
	switch ($arg){
//		case 'not-assigned':
//			$output = pages_ec_article_for_new_entry();
//			break;
		case 'being-written':
			$output = pages_ec_article_for_assigned_entry();
			break;
		case 'not-started':
			$output = pages_ec_article_by_state_not_started();
            break;
		case 'review':
			$output = pages_ec_article_by_state_needs_review();
            break;
        case 'assigned-reviewers':
            $output = pages_ec_article_by_state_assigned_editors();
            break;
        case 'assigned-factcheckers':
            $output = pages_ec_article_by_state_assigned_factcheckers();
            break;
        case 'reviews-completed':
        	$output = pages_ec_article_by_state_reviews_completed();
            break;
        case 'needs-eic-review':
        	$output = pages_ec_article_by_state_needs_eic_review();
            break;
        case 'needs-me-review':
        	$output = pages_ec_article_by_state_needs_me_review();
            break;
        case 'needs-ce-review':
        	$output = pages_ec_article_by_state_needs_ce_review();
            break;
        case 'assigned-eics':
        	$output = pages_ec_article_by_state_assigned_eics();
        	break;
        case 'assigned-mes':
        	$output = pages_ec_article_by_state_assigned_mes();
        	break;
        case 'assigned-ces':
        	$output = pages_ec_article_by_state_assigned_ces();
        	break;
        case 'ce-reviews-completed':
        	$output = pages_ec_article_by_state_ce_reviews_completed();
        	break;
        case 'published':
        	$output = pages_ec_article_by_state_published();
        	break;
//      $sql = "select *
//              from (
//                select A.*, B.entity_id
//                from (
//                  select A.*, B.title, B.vid
//                  from (select nid, max(hid) as hid from {workbench_moderation_node_history} where state in ('needs_review','reviewer_assigned')  group by nid) A
//                  left join node B
//                  on A.nid=B.nid) A
//                left join (select entity_id, max(revision_id) as revision_id from {field_revision_field_editorial_review_group} group by entity_id) B
//                on A.nid=B.entity_id and A.vid=B.revision_id ) A
//              where entity_id is not null";
//      $nids = get_nids_by_sql($sql);
//      
//      $sql = "select *
//              from (
//                select A.*, B.entity_id
//                from (
//                  select A.*, B.title, B.vid
//                  from (select nid, max(hid) as hid from {workbench_moderation_node_history} where state in ('needs_review','reviewer_assigned')  group by nid) A
//                  left join node B
//                  on A.nid=B.nid) A
//                left join (select entity_id, max(revision_id) as revision_id from {field_revision_field_fact_checker} group by entity_id) B
//                on A.nid=B.entity_id and A.vid=B.revision_id ) A
//              where entity_id is not null";
//      $add_nids = get_nids_by_sql($sql);
//      foreach ( $add_nids as $id ){
//        if ( in_array($id, $nids) == false) {
//          $nids[] = $value;
//        }
//      }
//			$output .= pages_ec_article_in_review($nids);
//		case 'return-to-revision':
			//$output = pages_ec_article_by_state('return-to-revision');
      break;
		default:
			$output = ec_greeting_msg();
	}
	return $output;
}

function ec_greeting_msg()
{
	return 'EC said Hello';
}


function pages_me() {
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/ajax/ajax_layout.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/assign_me.js', 'file');
  global $user;
  $uid = $user->uid; 
  $sql = "select n.nid
		  from workbench_moderation_node_history wmnh, node n, field_data_field_managing_editor fdfme
		  where wmnh.current = 1 AND wmnh.state IN ('me_in_review') AND wmnh.nid = n.nid AND fdfme.entity_id = n.nid
				and fdfme.field_managing_editor_target_id = $uid";
  
  $results = db_query($sql)->fetchAll();
  
  $output = '<br />';
  if ( count($results) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($results as $r){
      $nid = $r->nid;
      $node = node_load($nid);
      
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($node->title), t( "mypages/article/$nid/view" ) ),
								       	l(t( 'edit' ), t( "node/$nid/edit" ) ),
								       	l(t( 'comment/discuss' ), t( "mypages/oep-comments/" . $nid . "/" . OepUtil::get_latest_revision($nid) . "/discuss" ) ),
								        t($node->workbench_moderation['current']->state . "&nbsp|&nbsp <a href='javascript:submit_to_editorial_coordinator(". $nid .")'>approve</a>"),
								      	date('Y-m-d H:i:s',$node->revision_timestamp),
										$deadline_str,
								   )
				  );
					
	  if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
	//script for sorting the table
    $output.= '
    			<div id="notice_dialog" title="Submit to EC" ></div>
    		    <script>
  				 $(document).ready(function() {
				        $("table").tablesorter({sortList: [[5,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false}, 3: {sorter: false} }
                                              });
					   });
  	 		    </script>
  			 '; 
  }

	return $output;
}

function pages_eic($arg) {
	switch($arg){
		case 'assigned':
			return eic_assigned();
		case 'browse_entries':
			return browse_entries();
	}
}

function browse_entries(){
	drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
	$oep_util = new OepUtil();
	$authors_str = $oep_util->get_authors();
	$images_dir = base_path() . drupal_get_path('module', 'misc') . '/images/';
	$html_string = '
	  <div class="form-item form-type-select form-item-field-theme-und">
	 	 Author:&nbsp;
		 <select class="form-select" id="author_select" autocomplete="off">
		    <option value="all"></option>'.
			$authors_str
		.'</select>
		  &nbsp;&nbsp;&nbsp;
		  Status:&nbsp;
		  <select class="form-select" id="status_select" autocomplete="off">
		    <option value="all"></option>
		    <option value="assigned_entry">Assigned Entry</option>
		    <option value="in_progress">In Progress</option>
		    
		    <option value="needs_review">Needs Review</option> 
		    <option value="reviewer_assigned">Reviewer Assigned</option>
		    <option value="review_completed">Review Completed</option>
		     
		    <option value="needs_eic_review">Needs EIC review</option>
		    <option value="eic_in_review">EIC In review</option>
		    
		    <option value="needs_me_review">Needs ME review</option>
		    <option value="me_in_review">ME in review</option>
		    
		    <option value="needs_ce_review">Needs CE review</option>
		    <option value="ce_in_review">CE in review</option>
		    
		    <option value="published">Published</option>
		    
		    <option value="withdrawn_from_author">Withdrawn from Author</option>
		    <option value="needs_publication">Needs Publication</option>
		    <option value="returned_for_revision">Returned for Revision</option>
		  </select>
		  <img id="loading_img" src="' . $images_dir . 'loading.gif" width=20 height=20 style="margin-bottom:-10px"></img>
	  </div>
	  <div id="wrapper">
	  </div>
	  <script>
	  	var ajax_callback_str = "' . base_path() . 'ajax_callback/";
	  </script>
	  <script type="text/javascript" src="' . base_path() . drupal_get_path('module', 'oep_pages') .'/scripts/browse_entries.js"></script>
	';
	
	return $html_string;
}

function eic_assigned(){
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid; 
  $sql = "select title, n.nid, wmnh.vid, state, created
		  from workbench_moderation_node_history wmnh, node n, field_data_field_chief_in_editor
		  where wmnh.current = 1 AND wmnh.state IN ('eic_in_review') AND wmnh.nid = n.nid AND entity_id = n.nid AND field_chief_in_editor_target_id = $uid";
  
  $results = db_query($sql);
  
  $output = '<br />';
  $headers = array(
    t('Title'),
    t('Comment'),
    t('Current State'),
    t('Last Update'),
    t('Deadline'),
  );

  foreach ($results as $r){
  	  $nid = $r->nid;
  	  $node = node_load($nid);
  	  $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      
      $row = array( 'data' => array(
								      l( t($r->title), t( "mypages/article/$r->nid/view" ) ),
								      l(t('comment/discuss'), "mypages/oep-comments/$r->nid/$r->vid/discuss" ),
								      t($r->state),
									  date('Y-m-d H:i:s',$node->revision_timestamp),
									  $deadline_str,
								   )
    			   );
    if($interval/(3600*24)>=10)
	      $rows[] = $row;
	else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
  }

  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
   //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

//function pages_ec_article_for_new_entry() {
//  $output = '<br />';
//	$nodes = get_nodes_by_article_state('new_entry');
//  
//  if ( count($nodes) > 0 ) {
//    $headers = array(
//      t('Title'),
//      t(''), 
//      t('Current State')
//    );
//    $rows = array();
//    foreach ($nodes as $node){
//      $nid = $node->nid;
//      $rows[] = array( 
//        l( t($node->title), t( "mypages/article/$nid/view" ) ),
//        l( t( 'edit author' ),  t( "mypages/article/$nid/edit_author" )),
//        t($node->workbench_moderation['current']->state)
//      );
//    }
//    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//  }
//
//	return $output;
//}
//
//function sort_by_title($nodes,$sort){
//	
//	$new_nodes = array();
//	foreach($nodes as $key => $node)
//		$new_nodes[] = $node;
//		
//	if($sort=='asc')
//	{
//		for($i=0;$i<count($new_nodes)-1;$i++)
//			for($j=$i+1;$j<count($new_nodes);$j++)
//				if($new_nodes[$i]->title>$new_nodes[$j]->title)
//				{
//					$tmp = $new_nodes[$i];
//					$new_nodes[$i] = $new_nodes[$j];
//					$new_nodes[$j] = $tmp;
//				}
//	}
//	else //desc
//	{
//		for($i=0;$i<count($new_nodes)-1;$i++)
//			for($j=$i+1;$j<count($new_nodes);$j++)
//				if($new_nodes[$i]->title<$new_nodes[$j]->title)
//				{
//					$tmp = $new_nodes[$i];
//					$new_nodes[$i] = $new_nodes[$j];
//					$new_nodes[$j] = $tmp;
//				}
//	}
//	return $new_nodes;
//}
//
//function sort_by_created($nodes,$sort){
//	
//	$new_nodes = array();
//	foreach($nodes as $key => $node)
//		$new_nodes[] = $node;
//		
//	if($sort=='asc')
//	{
//		for($i=0;$i<count($new_nodes)-1;$i++)
//			for($j=$i+1;$j<count($new_nodes);$j++)
//				if($new_nodes[$i]->created>$new_nodes[$j]->created)
//				{
//					$tmp = $new_nodes[$i];
//					$new_nodes[$i] = $new_nodes[$j];
//					$new_nodes[$j] = $tmp;
//				}
//	}
//	else //desc
//	{
//		for($i=0;$i<count($new_nodes)-1;$i++)
//			for($j=$i+1;$j<count($new_nodes);$j++)
//				if($new_nodes[$i]->created<$new_nodes[$j]->created)
//				{
//					$tmp = $new_nodes[$i];
//					$new_nodes[$i] = $new_nodes[$j];
//					$new_nodes[$j] = $tmp;
//				}
//	}
//	return $new_nodes;
//}


function pages_ec_article_for_assigned_entry() {

  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  $output = '<br />';
  $nodes = get_nodes_by_article_state('in_progress');
  // ordering the nodes by $node->field_deadline['und'][0]['value']
  // $ordered_nodes = array();
//  for($i = 0; $i < count($nodes) ; $i++){
//  	$nodes[$i]->field_deadline[LANGUAGE_NONE][0]['value'];
//  }

  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t(''), 
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($node->title), t( "mypages/article/$nid/view" ) ),
								        l( t( 'edit author' ),  t( "mypages/article/$nid/edit-author" )),
								        t($node->workbench_moderation['current']->state),
								      	date('Y-m-d H:i:s',$node->revision_timestamp),
										$deadline_str,
								   )
				  );
					
	  if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
	//script for sorting the table
    $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  	 		  </script>
  			 '; 
  }

	return $output;
}

function pages_ec_article_by_state_not_started() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  /** SQL: List of nid with having less than 2 words **/
  $sql = "select n.title, n.nid, wmnh.state, n.created
		  from workbench_moderation_node_history wmnh, node n
		  where wmnh.current = 1 AND wmnh.state IN ('assigned_entry') AND wmnh.nid = n.nid ";
  $r = db_query($sql);
  
  $output = '<br />';
  $headers = array(
    t('Title'), 
    t('Edit'),
    t('Current State'),
    t('Last Update'),
    t('Deadline'),
  );

  foreach ($r as $row){
  	$node = node_load($row->nid);
  	$deadline = OepUtil::get_deadline($row->nid);
  	
  	// if deadline field is not set, output empty and not highlighted
  	if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	}
    else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
    }
    
    $row = array( 'data' => array(
							      l( t($row->title), t( "mypages/article/$row->nid/view" ) ),
							      array('data' => l( t( 'edit author' ),  t( "mypages/article/$row->nid/edit-author" )), 'width' => '100px'),
							      t($row->state),
							      date('Y-m-d H:i:s',$node->revision_timestamp),
							      $deadline_str,
							     )
    			);

    if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  
  return $output;
}

//function pages_ec_article_in_review($nids) {
//  $num = count($nids);
//  $output = '<br />';
//  $headers = array(t('Title'), t('Edit'), t('Current State'));
//  if ( count($nids) == 1 ) {
//    $node = node_load($nids[0]);
//    $nid = $node->nid;
//    $rows[] = array(
//      t($node->title), 
//      l( t('view'), t( "mypages/article/$nid/view" ) ),
//      l( t('edit reviewers'), t( "mypages/article/$nid/approve" ) ), 
//      t( $node->workbench_moderation['current']->state) 
//    );
//    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//  } elseif ( count($nids) > 1 ) {
//    $nodes = node_load_multiple($nids);
//    foreach ($nodes as $node){
//      $nid = $node->nid;
//      $rows[] = array(
//        l( t($node->title), t( "mypages/article/$nid/view" ) ),
//        l( t('edit reviewers'), t( "mypages/article/$nid/approve" ) ), 
//        t( $node->workbench_moderation['current']->state) 
//      );
//    }
//    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//  }
//
//	return $output;
//}

//function pages_ec_article_in_revision() {
//  
//  $sql = "select title, nid
//          from node
//          where nid in (select distinct nid
//            from workbench_moderation_node_history
//            where current=1 and state = 'needs_revision')";
//
//  $r = db_query($sql);
//	$result = array();
//	foreach ($r as $row) {
//		$result[] = $row;
//	}
//  
//  $output = '<br />';
//  if ( count($result) > 0 ) {
//    $headers = array(
//      t('Article Title'), 
//      t(''), 
//    );
//    $rows = array();
//    foreach ($result as $row){
//      $nid = $row->nid;
//      $rows[] = array(
//        t($row->title), 
//        l( t('view'), t( "mypages/article/$nid/view" ) ),
//      );
//    }
//    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//  }
//
//	return $output;
//}


function article_view_page($arg){

  // Example 1: Loading a single node and displaying it in full and teaser mode.
  $build = '';
  if ($node = node_load($arg)) {
    $build['node_display'] = node_view($node, 'oep_article_view_page');
    
    $output = array('node'=>$node);
  }
  return $build;
}

//function pages_unassigned_articles() {
//	$sql = "select C.nid
//					from (
//					  select A.nid, B.state
//					  from (select nid, max(stamp) as stamp from {workbench_moderation_node_history} group by nid) A
//					  left join {workbench_moderation_node_history} B
//					  on A.nid=B.nid and A.stamp=B.stamp) C
//					where C.state=:state";
//	$r = db_query($sql, array(':state' => 'new_entry'));
//	
//	$ids = array();
//	foreach ($r as $row) {
//		$ids[] = $row->nid;
//	}
//	
//	$nodes = node_load_multiple($ids);
//	
//	$output = '<br />';
//	$headers = array(t('Title'), t('From State'), t('Current State'));
//	foreach ($nodes as $node){
//		$rows[] = array(l(t($node->title),t($node->path['source'])), t($node->workbench_moderation['current']->from_state), t($node->workbench_moderation['current']->state));
//	}
//	$output .= theme('table', array('header' => $headers, 'rows' => $rows));
//	
//  $build = array(
//    'string_paragraph' => array(
//      '#type' => 'markup',
//      '#markup' => '<p>Pages can be returned as strings.</p>',
//    ),
//    'render_array_paragraph' => array(
//      '#type' => 'markup',
//      '#markup' => '<p>Pages can be returned as <em>render arrays<em>.</p>',
//    ),
//    'why_render_arrays' => array(
//      '#items' => array('They allow content to be modified as an array.', 'Arrays are a lot easier to modify than HTML.'),
//      '#title' => 'Render arrays are better because...',
//      '#theme' => 'item_list',
//    ),
//  );

//  return $output;
//}

function pages_ec_article_by_state_needs_review() {
  	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
	drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');
	drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/need_reviews.js', 'file');
	
  $output = '<br />';
  $nodes = get_nodes_by_article_state('needs_review');

  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($node->title), t( "mypages/article/$nid/view" ) ),
								        l(t('edit reviewers'),"mypages/article/$nid/approve" ), 
								        t($node->workbench_moderation['current']->state . "&nbsp|&nbsp <a href='javascript:submit_assigned_reviewers(". $nid .")'>submit</a>"),
								        date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
								      )
      			  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));

    $build = array(
        'string_paragraph' => array(
            '#type' => 'markup',
            '#markup' => '<p>Pages can be returned as strings.</p>',
        ),
        'render_array_paragraph' => array(
            '#type' => 'markup',
            '#markup' => '<p>Pages can be returned as <em>render arrays<em>.</p>',
        ),
        'why_render_arrays' => array(
            '#items' => array('They allow content to be modified as an array.', 'Arrays are a lot easier to modify than HTML.'),
            '#title' => 'Render arrays are better because...',
            '#theme' => 'item_list',
        ),
    );
     
  }
  //script for sorting the table
  $output.= '
  			 <div id="submit_dialog" title="Assign to Reviewers"></div>
  			 <script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false}}
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}


/** Author Methods **/
function pages_author_article($arg){

  $output = '';
	switch ($arg){
	case 'assigned':
			$output = author_article_for_assigned_entry();
			break;
    case 'submitted':
			$output = author_article_for_submitted_entry();
			break;
//    case 'published':
//			$output = author_article_for_published_entry();
//			break;
		default:
			$output = ec_greeting_msg();
	}
	return $output;
}

function author_article_for_assigned_entry() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid;
  $sql = "select A.nid, B.title
          from (select distinct entity_id as nid from field_data_field_authors where field_authors_target_id=$uid) A
          left join (select * from node where nid in (select nid from workbench_moderation_node_history where current=1 and state in ('assigned_entry', 'in_progress')) )B
          on A.nid=B.nid
          where title is not null";

  $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
	}
  
  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    
    $rows = array();
    foreach ($result as $r){
      $nid = $r->nid;
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
      // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
							         l( t($r->title), t( "mypages/article/$nid/view" ) ),
							         l( t( 'edit article' ),  t( "node/$nid/edit" )),
							         t($node->workbench_moderation['current']->state),
							         date('Y-m-d H:i:s',$node->revision_timestamp),
							         $deadline_str,
							       )
                  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
      //script for sorting the table
	  $output.= '<script>
	  				$(document).ready(function() {
					        $("table").tablesorter({sortList: [[4,0]],
													headers: { 1: { sorter: false}, 2: {sorter: false} }
	                                              }); 
					    }
					); 
	  			 </script>
	  			';
	  }

	return $output;
}

// state list: 'assigned_entry','needs_review','reviewer_assigned','review_completed','me_in_review','needs_cie_review','cie_in_review','needs_ce_review','ce_in_review','needs_revision','needs_publication','published'

function author_article_for_submitted_entry() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid;
  $sql = "select A.nid, B.title
          from (select distinct entity_id as nid from field_data_field_authors where field_authors_target_id=$uid) A
          left join (select * from node where nid in (select nid from workbench_moderation_node_history where current=1 and state in ('needs_review')) )B
          on A.nid=B.nid
          where title is not null";
 
  $result = db_query($sql)->fetchAll();
  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'), 
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($result as $r){
      $nid = $r->nid;
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array( 
								        l( t($r->title), t( "mypages/article/$nid/view" ) ),
								        t($node->workbench_moderation['current']->state),
								        date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
								   )
                  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
     
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    //script for sorting the table
    $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[3,0]],
												headers: { 1: { sorter: false} }
                                              }); 
				    }
				); 
  			  </script>
  			 ';
  }

	return $output;
}

//function author_article_for_published_entry() {
//  global $user;
//  $uid = $user->uid;
//  
//  $sql = "select title, nid
//          from node
//          where nid in 
//            (select distinct entity_id as nid 
//            from field_data_field_authors 
//            where field_authors_target_id=$uid and entity_id in (select nid from workbench_moderation_node_history where current=1 and state='published'))";
//
//  $r = db_query($sql);
//	$result = array();
//	foreach ($r as $row) {
//		$result[] = $row;
//	}
//  
//  $output = '<br />';
//  if ( count($result) > 0 ) {
//    $headers = array(
//      t('Title'), 
//      t('NID'), 
//      t(''), 
//    );
//    $rows = array();
//    foreach ($result as $row){
//      $nid = $row->nid;
//      $rows[] = array(
//        t($row->title), 
//        t($row->nid), 
//        l( t('view'), t( "mypages/article/$nid/view" ) ),
//      );
//    }
//    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//  }
//
//	return $output;
//}

/** Copy Editor Methods **/
function pages_copy_editor($arg){

//   $output = '';
// 	switch ($arg){
// 		case 'assigned':
// 			$output = assigned_review_for_copy_editor();
// 			break;
//     case 'reviewed':
// 			$output = submitted_review_for_copy_editor();
// 			break;
// 	}
// 	return $output;
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/ajax/ajax_layout.css', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
	drupal_add_css(drupal_get_path('module', 'oep_comments') . '/oep_comments.css', 'file');
	drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var base_url = "'. base_path() .'"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');
	drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/view_comments.js', 'file');
	drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/assign_ce.js', 'file');
	global $user;
	$uid = $user->uid;
	$sql = "select n.nid
			from workbench_moderation_node_history wmnh, node n, field_data_field_copy_editor fdfme
			where wmnh.current = 1 AND wmnh.state IN ('ce_in_review') AND wmnh.nid = n.nid AND fdfme.entity_id = n.nid
			and fdfme.field_copy_editor_target_id = $uid";
	
	$results = db_query($sql)->fetchAll();
	
	$output = '<br />';
	if ( count($results) > 0 ) {
		$headers = array(
				t('Title'),
				t(''),
				t('Comments'),
				t('Current State'),
				t('Last Update'),
				t('Deadline'),
		);
		$rows = array();
		foreach ($results as $r){
			$nid = $r->nid;
			$node = node_load($nid);
	
			$oep_util = new OepUtil();
			$deadline = OepUtil::get_deadline($nid);
			 
			// if deadline field is not set, output empty and not highlighted
			if($deadline=="" || $deadline==0){
				$deadline_str = "";
				$interval = 10000000000;// set interval maximum for not initialized deadline
			}
			else{
				$deadline_str = date('Y-m-d H:i:s',$deadline);
				$interval = $deadline - time();
			}
			$row = array( 'data' => array(
											l( t($node->title), t( "mypages/article/$nid/view" ) ),
											l(t( 'edit' ), t( "node/$nid/edit" ) ),
											t('<a href="javascript:view_comments(' . $nid . ',' . $oep_util->get_latest_revision($nid) . ');">view</a>'),
											t($node->workbench_moderation['current']->state . "&nbsp|&nbsp <a href='javascript:submit_to_editorial_coordinator(". $nid .")'>approve</a>"),
											date('Y-m-d H:i:s',$node->revision_timestamp),
											$deadline_str,
										 )
			       		);
				
			if($interval/(3600*24)>=10)
				$rows[] = $row;
			else
				$rows[] = $row + array(
										'no_striping' => true,
										'class' => array('deadline_row'),
									  );
		}
	
		$output .= theme('table', array('header' => $headers, 'rows' => $rows));
		$output .= '<div id="view_dialog" title="View Commented Article" ></div>
  	          		<div id="ajax_layout" class="act" style="visibility:hidden"></div>';
		//script for sorting the table
		$output.= '
					<div id="notice_dialog" title="Submit to EC" ></div>
					<script>
						$(document).ready(function() {
							$("table").tablesorter({sortList: [[5,0]],
													headers: { 1: { sorter: false}, 2: {sorter: false}, 3: {sorter: false} }
												  });
						});
					</script>
				  ';
	}
	
	return $output;
	
}

// function assigned_review_for_copy_editor() {
//   global $user;
//   $uid = $user->uid;
//   $uid = '14';
  
//   $sql = "select title, nid
//           from node
//           where nid in (
//             select distinct entity_id as nid 
//             from field_data_field_copy_editor 
//             where field_copy_editor_target_id=14 and entity_id in (
//               select nid from workbench_moderation_node_history where state='ce_in_review'))";

//   $r = db_query($sql);
// 	$result = array();
// 	foreach ($r as $row) {
// 		$result[] = $row;
// 	}
  
//   $output = '<br />';
//   if ( count($result) > 0 ) {
//     $headers = array(
//       t('Title'), 
//       t(''), 
//     );
//     $rows = array();
//     foreach ($result as $row){
//       $nid = $row->nid;
//       $rows[] = array(
//         t($row->title), 
//         l( t('view'), t( "mypages/article/$nid/view" ) ),
//       );
//     }
//     $output .= theme('table', array('header' => $headers, 'rows' => $rows));
//   }

// 	return $output;
// }

// function submitted_review_for_copy_editor() {
//   $output = '<br />TODO: it require to look at review submition';

// 	return $output;
// }

/** Fact Checker Methods **/
function pages_fact_checker($arg){

  $output = '';
	switch ($arg){
		case 'assigned':
			$output = assigned_review_for_fact_checker();
			break;
//    	case 'reviewed':
//			$output = submitted_review_for_fact_checker();
//			break;
	}
	return $output;
}


function assigned_review_for_fact_checker() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid;

  $sql = "SELECT title, entity_id nid, revision_id vid
		  FROM {field_data_field_fact_checker} fdffc, {node} n, {workbench_moderation_node_history} wmnh 
          WHERE fdffc.field_fact_checker_target_id = $uid and entity_id = n.nid and fdffc.revision_id = wmnh.vid
 				and wmnh.state = 'reviewer_assigned' and wmnh.current = 1";

  $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
	}

  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($result as $row){
      $nid = $row->nid;
      $node = node_load($nid);
      $version = $row->vid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($row->title), t( "mypages/article/$nid/view" ) ),
								        l(t('comment/discuss'), "mypages/oep-comments/$nid/$version/discuss" ),
								        t($node->workbench_moderation['current']->state),
								        date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
								   )
                  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function submitted_review_for_fact_checker() {
  $output = '<br />TODO: it require to look at review submit';

	return $output;
}

/** Fact Checker Methods **/
function pages_editor($arg){

  $output = '';
	switch ($arg){
		case 'assigned':
			$output = assigned_review_for_editor();
			break;
        case 'reviewed':
			$output = submitted_review_for_editor();
			break;
	}
	return $output;
}

function oep_review_complete($vid,$uid){
	$sql = "SELECT *
		    FROM {oep_review}
		    WHERE uid = $uid AND vid = $vid AND state = 'complete'";
	
	$r = db_query($sql)->fetchAll();
	if(count($r)>0)
		return true;
	return false;
}

function get_oep_review_submitted_count($vid){
	
	$sql = "SELECT *
		    FROM {oep_review}
		    WHERE vid = $vid AND state = 'complete'";
	
	$r = db_query($sql)->fetchAll();
	return count($r);
}

function assigned_review_for_editor() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid;
  
  $sql = "SELECT title, wmnh.nid, fdferg.revision_id
		  FROM {workbench_moderation_node_history} wmnh, {field_data_field_editorial_review_group fdferg}, {node} n
		  WHERE current = 1 AND state = 'reviewer_assigned' AND wmnh.vid = fdferg.revision_id AND field_editorial_review_group_target_id = $uid AND wmnh.nid = n.nid
		  ORDER BY title";

  
    $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		//just get entry <> complete
		if(!oep_review_complete($row->revision_id,$uid))
			$result[] = $row;
	}
  
  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'), 
      t('Comment'),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    
    foreach ($result as $r){
      $nid = $r->nid;
      $node = node_load($nid);
      $version = $r->revision_id;
      $deadline = OepUtil::get_deadline($nid);

  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
									        l( t($r->title), "mypages/article/$nid/view"),
									        l(t('comment/discuss'), "mypages/oep-comments/$nid/$version/discuss" ),
									        t($node->workbench_moderation['current']->state),
									        date('Y-m-d H:i:s',$node->revision_timestamp),
											$deadline_str,
								   )
      			  );
      
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    //script for sorting the table
 	$output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			  </script>
  			 ';
  }

	return $output;
}

function submitted_review_for_editor() {
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $uid = $user->uid;
  $sql =   "SELECT title, wmnh.nid, wmnh.vid, n.created
			FROM {workbench_moderation_node_history} wmnh
			JOIN {field_data_field_editorial_review_group} fdferg ON wmnh.vid = fdferg.revision_id
			JOIN {node} n ON wmnh.nid = n.nid
			JOIN {oep_review} oepr ON fdferg.field_editorial_review_group_target_id = oepr.uid AND fdferg.revision_id = oepr.vid
			WHERE wmnh.current = 1 AND wmnh.state = 'reviewer_assigned' AND fdferg.field_editorial_review_group_target_id = $uid
				  AND oepr.state = 'complete'";
  
  $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$row->submitted_count = get_oep_review_submitted_count($row->vid);
		$node = node_load($row->nid);
		$row->needed_reviewers_count = $node->field_required_review['und'][0]['value'] - $row->submitted_count;
		$result[] = $row;
	}

  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'),
      t('Submitted'),
      t('Needed'),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    
    foreach ($result as $r){
      $nid = $r->nid;
      $node = node_load($nid);
      $version = $r->vid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($r->title), "mypages/article/$nid/view"),
								        $r->submitted_count,
								        $r->needed_reviewers_count,
								        t($node->workbench_moderation['current']->state),
									    date('Y-m-d H:i:s',$node->revision_timestamp),
										$deadline_str,
								   )
                  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    //script for sorting the table
    $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[5,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false}, 3: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  }

	return $output;
}


/**
 * Page callback argument for (drupal_get_form)
 */
function pages_articles_with_form($form, &$form_state) {
  
  $form['filter_params'] = array(
    '#title' => 'Filters',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  // Added AJAX callback.
  $form['filter_params']['article_status'] = array(
    '#title' => t('Article Status'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('', t('new_entry'), t('assigned_entry'), t('in_process'))),
    '#ajax' => array(
      'callback' => 'ajax_callback',
      'wrapper' => 'input_article_status_verify_wrapper_edit',
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
  		'#prefix' => '<div id="input_article_status_verify_wrapper_edit">',
      //'#markup'
  		'#suffix' => '</div>',
  );
  
  // When submitted, check for value of article_status, and populate article_status_verify.
  if (isset($form_state['values']['article_status'])) { // Fixed this line, was throwing an error in the original video code.
    
    $sql = "select C.nid
            from (
              select A.nid, B.state
              from (select nid, max(stamp) as stamp from {workbench_moderation_node_history} group by nid) A
              left join {workbench_moderation_node_history} B
              on A.nid=B.nid and A.stamp=B.stamp) C
            where C.state=:state";
    $r = db_query($sql, array(':state' => $form_state['values']['article_status']));
    
    $ids = array();
    foreach ($r as $row) {
      $ids[] = $row->nid;
    }
    
    $nodes = node_load_multiple($ids);
    
    $output = '<br />';
    $headers = array(t('Title'), t('From State'), t('Current State'));
    foreach ($nodes as $node){
      $rows[] = array(l(t($node->title),t($node->path['source'])), t($node->workbench_moderation['current']->from_state), t($node->workbench_moderation['current']->state));
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
    
    $form['output']['#type'] = 'markup';
    //$form['output']['#markup'] = t('You selected @article_status', array('@article_status' => $form_state['values']['article_status']));
    $form['output']['#markup'] = $output;
  }

  return $form;
}

function pages_ec_article_by_state_assigned_editors(){
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  $oep_util = new OepUtil();
    //@todo amin
    // search through the oep_review_states and workbench_moderation_node_history
    // for all needs review and what state they are in
    
    // output all those that have assigned editors or both (two tables
    
  
  $sql = "select n.title, n.nid, n.created, ferg.field_editorial_review_group_target_id as fertid, ferg.revision_id
          from {field_data_field_editorial_review_group} ferg
               join {node} n on n.nid = ferg.entity_id
               join {workbench_moderation_node_history} wmnh on n.nid = wmnh.nid
          where wmnh.state in ('reviewer_assigned') and wmnh.current = 1 and wmnh.vid = ferg.revision_id";
  
  $uids = array();
  $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
                $uids[] = $row->fertid;
	}
  $users = user_load_multiple($uids);
  global $user;
  $uid = $user->uid;
  
  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'),
      t('Editor'),
      t('Comment'),
      t('Submitted'),
      t('Needed'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->revision_id;
      $usid = $row->fertid;
      
      $node = node_load($nid);
      $authorized_users = field_get_items('node', $node, 'field_editorial_review_group');
      $submitted_count = get_oep_review_submitted_count($version);
      $deadline = OepUtil::get_deadline($nid);
	  // if deadline field is not set, output empty and not highlighted
	  if($deadline=="" || $deadline==0){
	  		$deadline_str = "";
	      	$interval = 10000000000;// set interval maximum for not initialized deadline
	  }
	  else{
	    	$deadline_str = date('Y-m-d H:i:s',$deadline);
	      	$interval = $deadline - time();
	  }
      
      $arr =  array(
						l( t($row->title), "mypages/article/$nid/view"),
						$users[$usid]->name,
				   );
     
      if (validate_user($uid, $authorized_users) && $uid == $usid && !$oep_util->oep_review_completed_state($nid,$uid))
      	$arr = array_merge($arr,array(l(t('comment'), "mypages/oep-comments/$nid/$version/discuss")));
      else //disable link if current user cannot access to the commenting page
      	if ($usid && $oep_util->oep_review_completed_state($nid,$usid))
      		$arr = array_merge($arr, array(t('<span style="color:green">Submitted<span>')));
      	else
	     	$arr = array_merge($arr, array(t('N/A')));
	  $arr = array_merge($arr, array(
	  					$submitted_count,
						t($node->field_required_review['und'][0]['value'] - $submitted_count),
						date('Y-m-d H:i:s',$node->revision_timestamp),
						$deadline_str,
	      		   ));
	  $row = array( 'data' => $arr);
	  if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[6,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false}, 3: {sorter: false}, 4: {sorter: false}}
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_assigned_factcheckers(){
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  $sql = "select n.title, n.nid, n.created, fdfc.field_fact_checker_target_id as fertid, fdfc.revision_id
          from {field_data_field_fact_checker} fdfc
               join {node} n on n.nid = fdfc.entity_id
               join {workbench_moderation_node_history} wmnh on n.nid = wmnh.nid
          where wmnh.state IN ('reviewer_assigned') and wmnh.current = 1";  
  
  $uids = array();
  $r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
                $uids[] = $row->fertid;
	}
  $users = user_load_multiple($uids);
  
  
  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'),
      t('Fact Checker'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
//     dpm($results, 'RESULTS');
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->revision_id;
      $usid = $row->fertid;
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($row->title), "mypages/article/$nid/view"),
								        $users[$usid]->name,
								      	date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
							      	)
      			   );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[3,0]],
												headers: { 1: { sorter: false}}
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}



/** useful methods **/
function get_nids_by_sql($sql) {
  $r = db_query($sql);
	$ids = array();
	foreach ($r as $row) {
    $ids[] = $row->nid;
	}
  return $ids;
}

function pages_ec_article_by_state_reviews_completed()
{
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/ajax/ajax_layout.css', 'file');
  drupal_add_css(drupal_get_path('module', 'topicmgr') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_css(drupal_get_path('module', 'oep_comments') . '/oep_comments.css', 'file');
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var base_url = "'. base_path() .'"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');

  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/review_completed_change_state.js', 'file');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/view_comments.js', 'file');
  $sql = "select n.title, n.nid, n.created, wmnh.vid
          from {workbench_moderation_node_history} wmnh
               join {node} n on n.nid = wmnh.nid
          where wmnh.state IN ('review_completed') and wmnh.current = 1";
	  
	$uids = array();
	$r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
	}
	$users = user_load_multiple($uids);
	$output = '<br />';

  	if ( count($result) > 0 ) {
    	$headers = array(
	      t('Title'),
	      t('Comments'),
	      t('Current State'),
	      t('Last Update'),
      	  t('Deadline'),
    	);
  	}
  	
    $rows = array();
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->vid;
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($row->title), t( "mypages/article/$nid/view" ) ),
								        t('<a href="javascript:view_comments(' . $nid . ',' . $version . ');">view</a>'),
								        t($node->workbench_moderation['current']->state .'&nbsp|&nbsp<a href="javascript:change_state(' . $nid . ')">change</a>'),
								      	date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
			      		           )
                  );
      
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= '<div id="view_dialog" title="View Commented Article" ></div>
  	          <div id="change_state_dialog" title="Change Article State" ></div>
  	          <div id="ajax_layout" class="act" style="visibility:hidden"></div>';
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_needs_eic_review()
{
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  drupal_add_css(drupal_get_path('module', 'topicmgr') . '/css/base/jquery.ui.all.css', 'file');
  
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/";','inline');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/assign_eic.js', 'file');
  $output = '<br />';
  $output .= '<div id="dialog" title="Assign Editor in Chief" ></div>';
  $nodes = get_nodes_by_article_state('needs_eic_review');
  
  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t(''), 
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($node->title), t( "mypages/article/$nid/view" ) ),
								        t('<a href="javascript:assign_eic(' . $nid . ')">Assign EIC</a>'), 
								        t($node->workbench_moderation['current']->state),
								        date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
      						       )
      			  );
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_assigned_eics()
{ 
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  $output = '<br />';
  
  $nodes = get_nodes_by_article_state('eic_in_review');
  
  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t('Editor in Chief'),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $eic = user_load($node->field_chief_in_editor['und'][0]['target_id']);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      } 
      $row = array( 'data' => array(
								        l( t($node->title), t( "mypages/article/$nid/view" ) ),
								        t($eic->field_first_name['und'][0]['value']." ".$eic->field_last_name['und'][0]['value']),
								        t($node->workbench_moderation['current']->state),
								        date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
								    )
				  );
	     
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_published()
{
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/ajax/ajax_layout.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_css(drupal_get_path('module', 'oep_comments') . '/oep_comments.css', 'file'); 
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var base_url = "'. base_path() .'"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');

  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/published_change_state.js', 'file');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/view_comments.js', 'file');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/rowcollapse.js', 'file');  

/***  wmnh.current does not work for showing published as a new unpublished draft may be 'current' 

  $sql = "select n.title, n.nid, n.created, wmnh.vid, field_featured_value
          from {workbench_moderation_node_history} wmnh
               join {node} n on n.nid = wmnh.nid
            left join field_data_field_featured fdff
            on n.nid = fdff.entity_id
            where wmnh.state IN ('published') and wmnh.current = 1";

*/
$sql = "select * from 
       (select n.title, n.nid, n.created, wmnh.state, wmnh.vid, wmnh.stamp, field_featured_value 
       from workbench_moderation_node_history wmnh 
       left join node n on wmnh.nid = n.nid
       left join field_data_field_featured fdff
       on n.nid = fdff.entity_id 
       where wmnh.state = 'published' 
       order by n.nid, stamp DESC) d 
       group by d.nid";  
	  
	$uids = array();
	$r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
	}
	$users = user_load_multiple($uids);
	$output = '<br />';

  	if ( count($result) > 0 ) {
    	$headers = array(
	      t('Title'),
	      t('Comments'),
	      t('Current State'),
	      t('Last Update'),
      	  t('Deadline'),
          t('Featured'),
    	);
  	}
  	
    $rows = array();
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->vid;
      $checked =$row->field_featured_value;
      if($checked == 1) $checked = "checked='true'";
      else $checked = '';
      
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($row->title), t( "mypages/article/$nid/view" ) ),
								        t('<a href="javascript:view_comments(' . $nid . ',' . $version . ');">view</a>'),
								        //t($node->workbench_moderation['current']->state .'&nbsp|&nbsp<a href="javascript:change_state(' . $nid . ')">change</a>'),
								        t($row->state .'&nbsp|&nbsp<a href="javascript:change_state(' . $nid . ')">change</a>'),
                                                                      	date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
        							        //$checked,
 									"<input type='checkbox' value='" . $nid ."'" . $checked. "></input>"
			      		           )
                  );
      
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= '<div id="view_dialog" title="View Commented Article" ></div>
  	          <div id="change_state_dialog" title="Change Article State" ></div>
  	          <div id="ajax_layout" class="act" style="visibility:hidden"></div>';
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_ce_reviews_completed()
{
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/ajax/ajax_layout.css', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_css(drupal_get_path('module', 'oep_comments') . '/oep_comments.css', 'file');
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/"; var base_url = "'. base_path() .'"; var misc_url = "' . drupal_get_path('module', 'misc') . '";','inline');

  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/ce_review_completed_change_state.js', 'file');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/view_comments.js', 'file');
    
  $sql = "select n.title, n.nid, n.created, wmnh.vid
          from {workbench_moderation_node_history} wmnh
               join {node} n on n.nid = wmnh.nid
          where wmnh.state IN ('ce_review_completed') and wmnh.current = 1";
	  
	$uids = array();
	$r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
		$result[] = $row;
	}
	$users = user_load_multiple($uids);
	$output = '<br />';

  	if ( count($result) > 0 ) {
    	$headers = array(
	      t('Title'),
	      t('Comments'),
	      t('Current State'),
	      t('Last Update'),
      	  t('Deadline'),
    	);
  	}
  	
    $rows = array();
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->vid;
      $node = node_load($nid);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
								        l( t($row->title), t( "mypages/article/$nid/view" ) ),
								        t('<a href="javascript:view_comments(' . $nid . ',' . $version . ');">view</a>'),
								        t($node->workbench_moderation['current']->state .'&nbsp|&nbsp<a href="javascript:change_state(' . $nid . ')">change</a>'),
								      	date('Y-m-d H:i:s',$node->revision_timestamp),
							         	$deadline_str,
			      		           )
                  );
      
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= '<div id="view_dialog" title="View Commented Article" ></div>
  	          <div id="change_state_dialog" title="Change Article State" ></div>
  	          <div id="ajax_layout" class="act" style="visibility:hidden"></div>';
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_assigned_ces()
{
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
	drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');

	$output = '<br />';

	$nodes = get_nodes_by_article_state('ce_in_review');

	if ( count($nodes) > 0 ) {
		$headers = array(
				t('Title'),
				t('Copy Editor'),
				t('Current State'),
				t('Last Update'),
				t('Deadline'),
		);
		$rows = array();
		foreach ($nodes as $node){
			$nid = $node->nid;
			$ce = user_load($node->field_copy_editor['und'][0]['target_id']);
			$deadline = OepUtil::get_deadline($nid);
			 
			// if deadline field is not set, output empty and not highlighted
			if($deadline=="" || $deadline==0){
				$deadline_str = "";
				$interval = 10000000000;// set interval maximum for not initialized deadline
			}
			else{
				$deadline_str = date('Y-m-d H:i:s',$deadline);
				$interval = $deadline - time();
			}
			$row = array( 'data' => array(
					l( t($node->title), t( "mypages/article/$nid/view" ) ),
					t($ce->field_first_name['und'][0]['value']." ".$ce->field_last_name['und'][0]['value']),
					t($node->workbench_moderation['current']->state),
					date('Y-m-d H:i:s',$node->revision_timestamp),
					$deadline_str,
			)
			);
	   
			if($interval/(3600*24)>=10)
				$rows[] = $row;
	  else
	  	$rows[] = $row + array(
	  			'no_striping' => true,
	  			'class' => array('deadline_row'),
	  	);
		}
		$output .= theme('table', array('header' => $headers, 'rows' => $rows));
	}
	//script for sorting the table
	$output.= '<script>
	$(document).ready(function() {
	$("table").tablesorter({sortList: [[4,0]],
	headers: { 1: { sorter: false}, 2: {sorter: false} }
});
}
);
</script>
';
	return $output;
}

function pages_ec_article_by_state_needs_ce_review()
{
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/";','inline');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/assign_ce.js', 'file');
  
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  
  $output = '<br />';
  $output .= '<div id="dialog" title="Assign Managing Editor" ></div>';
  $nodes = get_nodes_by_article_state('needs_ce_review');
  
  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
        							 l( t($node->title), t( "mypages/article/$nid/view" ) ),
        							 t('<a href="javascript:assign_ce(' . $nid . ')">Assign CE</a>'), 
        						     t($node->workbench_moderation['current']->state),
        							 date('Y-m-d H:i:s',$node->revision_timestamp),
							         $deadline_str,
								   )
				  );
	  if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_assigned_mes()
{
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  
  $output = '<br />';
  
  $nodes = get_nodes_by_article_state('me_in_review');
  
  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t('Managing Editor'),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $eic = user_load($node->field_managing_editor['und'][0]['target_id']);
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }  
      $row = array( 'data' => array(
							         l( t($node->title), t( "mypages/article/$nid/view" ) ),
							         t($eic->field_first_name['und'][0]['value']." ".$eic->field_last_name['und'][0]['value']),
							         t($node->workbench_moderation['current']->state),
							         date('Y-m-d H:i:s',$node->revision_timestamp),
							         $deadline_str,
								   )
				  );
	      
      if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function pages_ec_article_by_state_needs_me_review()
{
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_js(drupal_get_path('module', 'topicmgr') . '/scripts/ajax.googleapis.com/jquery-ui.min.js', 'file');
  drupal_add_css(drupal_get_path('module', 'topicmgr') . '/css/base/jquery.ui.all.css', 'file');
  drupal_add_js('var ajax_url = "'. base_path() .'ajax_callback/";','inline');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/scripts/assign_me.js', 'file');
  
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  
  $output = '<br />';
  $output .= '<div id="dialog" title="Assign Managing Editor" ></div>';
  $nodes = get_nodes_by_article_state('needs_me_review');
  
  if ( count($nodes) > 0 ) {
    $headers = array(
      t('Title'),
      t(''),
      t('Current State'),
      t('Last Update'),
      t('Deadline'),
    );
    $rows = array();
    foreach ($nodes as $node){
      $nid = $node->nid;
      $deadline = OepUtil::get_deadline($nid);
  	
  	  // if deadline field is not set, output empty and not highlighted
  	  if($deadline=="" || $deadline==0){
  		$deadline_str = "";
      	$interval = 10000000000;// set interval maximum for not initialized deadline
  	  }
      else{
    	$deadline_str = date('Y-m-d H:i:s',$deadline);
      	$interval = $deadline - time();
      }
      $row = array( 'data' => array(
        							 l( t($node->title), t( "mypages/article/$nid/view" ) ),
        							 t('<a href="javascript:assign_me(' . $nid . ')">Assign ME</a>'), 
        						     t($node->workbench_moderation['current']->state),
        							 date('Y-m-d H:i:s',$node->revision_timestamp),
							         $deadline_str,
								   )
				  );
	  if($interval/(3600*24)>=10)
	      $rows[] = $row;
	  else
	  	  $rows[] = $row + array(
    		               			'no_striping' => true,
    								'class' => array('deadline_row'),
	      						);
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  }
  //script for sorting the table
  $output.= '<script>
  				$(document).ready(function() {
				        $("table").tablesorter({sortList: [[4,0]],
												headers: { 1: { sorter: false}, 2: {sorter: false} }
                                              }); 
				    }
				); 
  			 </script>
  			';
  return $output;
}

function get_nodes_by_article_state($state){

//	$sql = "select C.nid
//					from (
//            select A.nid, B.state
//            from (select nid, max(stamp) as stamp from {workbench_moderation_node_history} group by nid) A
//            left join {workbench_moderation_node_history} B
//            on A.nid=B.nid and A.stamp=B.stamp) C
//            where C.state='$state' ";

	$sql = "select nid
			from {workbench_moderation_node_history}
            where state='$state' and current = 1";
	$r = db_query($sql);
	
	$nids = array();
	foreach ($r as $row) {
		$nids[] = $row->nid;
	}

	$nodes = array();

  $num = count($nids);
  switch ( $num ) {
    case 0:
      return array();
    case 1:
      return array(node_load($nids[0]));
    default:
      return node_load_multiple($nids);
      break;
  }
	return $nodes;
}


function pages_deadline(){
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/deadline.js', 'file');
  drupal_add_js(drupal_get_path('module', 'oep_pages') . '/tablesorter.js', 'file');
  drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
  drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
  global $user;
  $roles = $user->roles;
  $uid = $user->uid;
  
 
  
  /*
  $sql = " 
       
      SELECT workbench_moderation_node_history.nid, state, stamp, title, field_deadline_value FROM (

          SELECT nid, MAX(stamp) as max_stamp FROM 
          (
                SELECT nid, state, stamp  FROM workbench_moderation_node_history WHERE state != 'published'
            ) valid_states

            GROUP BY nid
        ) max_stamp_per_node 
        INNER JOIN workbench_moderation_node_history 
        ON workbench_moderation_node_history.stamp = max_stamp_per_node.max_stamp 
        AND workbench_moderation_node_history.nid = max_stamp_per_node.nid
        INNER JOIN node ON node.nid = workbench_moderation_node_history.nid
        INNER JOIN field_data_field_deadline ON field_data_field_deadline.entity_id =  workbench_moderation_node_history.nid
       ORDER BY field_deadline_value 
        ";
   */

/*
  $sql = " 
SELECT * FROM
(  
SELECT max(node.vid) as max_vid, `changed`, workbench_moderation_node_history.nid, state, stamp, title, field_deadline_value FROM (

          SELECT nid, MAX(stamp) as max_stamp FROM
          (
                SELECT nid, state, stamp  FROM workbench_moderation_node_history WHERE state != 'published'
            ) valid_states

            GROUP BY nid
        ) max_stamp_per_node
        INNER JOIN workbench_moderation_node_history
        ON workbench_moderation_node_history.stamp = max_stamp_per_node.max_stamp
        AND workbench_moderation_node_history.nid = max_stamp_per_node.nid
        INNER JOIN node ON node.nid = workbench_moderation_node_history.nid
        INNER JOIN field_data_field_deadline ON field_data_field_deadline.entity_id =  workbench_moderation_node_history.nid
GROUP BY nid       
ORDER BY field_deadline_value
) node_info

INNER JOIN (
SELECT * FROM 
(
(SELECT entity_id as nid, field_chief_in_editor_target_id as uid FROM field_data_field_chief_in_editor)
UNION
(SELECT entity_id, field_authors_target_id FROM field_data_field_authors)
UNION
(SELECT entity_id, field_copy_editor_target_id FROM field_data_field_copy_editor)
UNION
(SELECT entity_id, field_managing_editor_target_id FROM field_data_field_managing_editor)
UNION
(SELECT entity_id, field_fact_checker_target_id FROM field_data_field_fact_checker)
UNION
(SELECT entity_id, uid FROM field_data_field_editorial_review_group 
INNER JOIN user_group 
on field_data_field_editorial_review_group.field_editorial_review_group_target_id = user_group.group_id
)
) k
) user_to_node ON user_to_node.nid = node_info.nid 
WHERE uid = $uid
ORDER BY user_to_node.uid" ;
*/
$sql = "select node.nid, node.vid, title, field_deadline_value, changed, state from node join workbench_moderation_node_history wmnh on node.nid = wmnh.nid left join field_data_field_deadline fdfd on wmnh.nid = fdfd.entity_id where current = 1 and state != 'published' order by field_deadline_value";
 
$r = db_query($sql);
	$result = array();
	foreach ($r as $row) {
        $result[] = $row;
	}

  $output = '<br />';
  if ( count($result) > 0 ) {
    $headers = array(
      t('Title'), 
      t('Deadline'), 
      t('Current State'),
      t('Last Modified'), 
      t('Edit'), 
      t('Comment'), 
    );
    $rows = array();
    foreach ($result as $row){
      $nid = $row->nid;
      $version = $row->vid;
      if($row->field_deadline_value == 0 || $row->field_deadline_value == NULL){
         $deadline = "Deadline Not Set";}
      else{$deadline = date('Y-m-d H:i:s',$row->field_deadline_value);}

      $rows[] = array(
        l( t($row->title), t("/node/$nid") ), 
        $deadline,
        t($row->state), 
        date('Y-m-d H:i:s',$row->changed),   
        l( t('edit'), t( "node/$nid/edit" ) ),
        OepUtil::exist($nid,$version) ? t('<a href = "oep-comments/' . $nid .'/' . $version . '/discuss?coor_view=1">comment/discuss</a>') : '',  
      );
    }
    $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'deadline_table') ));
  }

	return $output;

}