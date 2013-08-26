<?php
	
	function from_machine_name($machine_name)
	{
		switch ($machine_name){
			case "assigned_entry":
				return "Assigned Entry";
			case "in_progress":
				return "In Progress";
			case "needs_review":
				return "Needs Review";
			case "reviewer_assigned":
				return "Reviewer Assigned";
			case "review_completed":
				return "Review Completed";
			case "needs_eic_review":
				return "Needs EIC review";
			case "eic_in_review":
				return "EIC In review";
			case "needs_me_review":
				return "Needs ME review";
			case "me_in_review":
				return "ME in review";
			case "needs_ce_review":
				return "Needs CE review";
			case "ce_in_review":
				return "CE in review";
			case "published":
				return "Published";
			case "withdrawn_from_author":
				return "Withdrawn from Author";
			case "needs_publication":
				return "Needs Publication";
			case "returned_for_revision":
				return "Returned for Revision";
			default:
				return $machine_name;
		}	
	}
	
	function oep_ajax_callback(){
		if(!isset($_POST["func_no"])){
			exit;
		}
		$oep_util = new OepUtil();
		$func_no = $_POST["func_no"];
		switch($func_no){
			case 1:
				$category = $_POST["category"];
				$status = $_POST["status"];
				$topics = $oep_util->filter_topics($category,$status,"all","all");
				echo json_encode($topics);
				break;
			case 2:
				$topic_titles = $oep_util->get_topic_titles();
				echo json_encode($topic_titles);
				break;
			case 3:
				$keyword = $_POST["keyword"];
				$topics = $oep_util->filter_topics("all","all","all","all");
				$result_topics = array();
				foreach($topics as $topic){ 
					if(match($topic["topic"],$keyword))
						array_push($result_topics,$topic);
				}
				echo json_encode($result_topics);
				break;
			case 4:
				$author = $_POST["author"];
				$county = $_POST["county"];
				$status = $_POST["status"];
				$topics = $oep_util->filter_topics("all",$status,$author,$county);
				echo json_encode($topics);
				break;
			case 5:
			    $topic_id = $_POST["topic_id"];
			    $author_id = $_POST["author_id"];
			    $status = $_POST["status"];
			    $oep_util->update_state($topic_id,$author_id,$status);
			    //send an email to thank the author for accepting/rejecting to write the article
			    $article_id = $oep_util->get_article_id($topic_id);
		    	$author_user = user_load($author_id);
		    	if ($status == "Accepted")
		    		pet_send_mail(variable_get('agree_to_write_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
		    	else
		    		pet_send_mail(variable_get('declined_rejected_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
				break;
			case 6:
				$topic_id = $_POST["topic_id"];
				$arr = $oep_util->get_topic_authors($topic_id);
				echo json_encode($arr);
				break; 
			case 7:
				$topic_id = $_POST["topic_id"];
				$arr = $oep_util->get_avail_authors($topic_id);
				echo json_encode($arr);
				break;
			case 8:
				$topic_id = $_POST["topic_id"];
				$author_id = $_POST["author_id"];
				$oep_util->add_topic_author($topic_id,$author_id);
				break;
			case 9:
			    $topic_id = $_POST["topic_id"];
				$reminder = $_POST["reminder"];
			    $date = $_POST["date"];
				$author_id = $_POST["author_id"];
				
				if($reminder=="w9onfile"){
					$oep_util->update_w9onfile($topic_id,$date,$author_id);
					$u = user_load((int)$author_id);
					if($date!='')
						$u->field_w9_on_file['und'][0]['value'] = '1';
					else
						$u->field_w9_on_file['und'][0]['value'] = '0';
					user_save($u);
				}
				else {
					$oep_util->update_reminder($topic_id,$reminder,$date,$author_id);
					//send an email to remind the author
					if($date != '') {
						$author_user = user_load($author_id);
						switch($reminder){
							case 'reminder1':
								pet_send_mail(variable_get('1st_reminder_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
								break;
							case 'reminder2':
								pet_send_mail(variable_get('2nd_reminder_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
								break;
							case 'reminder3':
								pet_send_mail(variable_get('3rd_reminder_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
								break;
						}
						
					}
				}
				
				break;
		    case 10:
		    	$topic_id = $_POST["topic_id"];
		    	$author_id = $_POST["author_id"];
		    	$note_content = filter_xss($_POST["note_content"], array('a', 'em', 'strong', 'cite', 'blockquote', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd'));
		    	$date_taken = $_POST["date_taken"];
		    	$commenter = $_POST["commenter"];
		    	echo json_encode(array('note_content' => $note_content, 'data' => $oep_util->insert_topic_author_note($topic_id,$author_id,$note_content,strtotime($date_taken),$commenter)));
		    	break;
		    case 11:
		    	$topic_id = $_POST["topic_id"];
		    	$author_id = $_POST["author_id"];
		    	$arr = $oep_util->get_author_notes($topic_id,$author_id);
				echo json_encode($arr);
		    	break;
		    case 12:
		    	$note_id = $_POST["note_id"];
		    	$note = $oep_util->get_author_note($note_id);
				echo $note["note_content"];
		    	break;
		    case 13:
		    	$note_id = $_POST["note_id"];
		    	$note_content = filter_xss($_POST["note_content"], array('a', 'em', 'strong', 'cite', 'blockquote', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd'));
		    	$arr = $oep_util->update_topic_author_note($note_id,$note_content);
		    	echo json_encode(array('note_content' => $note_content, 'data' => $arr));
		    	break;
		    case 14:
		    	$note_id = $_POST["note_id"];
		    	$oep_util->delete_author_note($note_id);
		    	break;
		   	case 15:
		   		$topic_id = $_POST["topic_id"];
		   		$author_id = $_POST["author_id"];
		   		$invited = $_POST["invited"];
		    	$oep_util->set_invited_author($topic_id,$author_id,$invited);
		    	//send an email to invite the author to write the article
		    	if ($invited == true){
		    		$article_id = $oep_util->get_article_id($topic_id);
		    		$author_user = user_load($author_id);
		    		pet_send_mail(variable_get('author_invited_pet',''), array(array('mail' => $author_user->mail, 'uid' => $author_id)), array('nid'=>$topic_id));
		    	}
		   		break;
		   	case 16:
		   		echo json_encode($oep_util->get_eic_users());
		   		break;
		   	case 17:
		   		$nid = $_POST["nid"];
		   		$vid = $_POST["vid"];
		   		$start = $_POST["start"];
		   		$stop = $_POST["stop"];
		   		echo $oep_util->add_new_thread($nid,$vid,$start,$stop);
		   		break;
		   	case 18:
		   		$nid = $_POST["nid"];
		   		$vid = $_POST["vid"];
		   		echo json_encode($oep_util->get_all_comments($nid,$vid));
		   		break;
		   	case 19://get all authors
		   		echo json_encode($oep_util->get_authors_autocomplete());
		   		break;
		   	case 20:
		   		$arr = $oep_util->get_article_list();
		    	echo json_encode($arr);
		   		break;
		   	case 21:
		   		$article_id = $_POST["article_id"];
		   		$article = node_load($article_id,OepUtil::get_latest_revision($nid));
		   		echo json_encode(array("title" => $article->title, "body_content" => $article->body["und"][0]["value"]));
		   		break;
		   	case 22:
		   		$thid = $_POST["thid"];
		   		echo json_encode($oep_util->get_comments($thid));
		   		break;
		   	case 23:
		   		$thid = $_POST["thid"];
		   		$comment = filter_xss($_POST["comment"], array('a', 'em', 'strong', 'cite', 'blockquote', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd'));
		   		global $user;
		   		$uid = $user->uid;
		   		$created = time();
		   		$vid = $_POST["vid"];
		   		$oep_util->insert_comment($thid,$comment,$uid,$created,$vid);
		   		break;
		   	case 24:
		   		$nid = $_POST["nid"];
		   		$vid = $_POST["vid"];
		   		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		   		$revisions = _oep_comments_load_body_revisions($node , $vid, true);
		   		echo json_encode(array('revision_body'=>$revisions, 'revision_index' => OepUtil::get_revision_index($nid,$vid)));
		   		break;
		   	case 25://remove author for a topic
		   		$topic_id = $_POST["topic_id"];
		   		$author_id = $_POST["author_id"];
		   		$result = $oep_util->remove_author($topic_id,$author_id);
		   		echo $result;
		   		break;
		   	case 26:
		   		$state = $_POST['state'];
		   		$nid = $_POST['nid'];
		    	$node = node_load($nid,OepUtil::get_latest_revision($nid));
		   		$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = $state;
				node_save($node);
		   		break;
		   	case 27://assign editor in chief
		   		$nid = $_POST['nid'];
		   		$uid = $_POST['uid'];
		   		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		   		$node->field_chief_in_editor['und'][0]['target_id'] = $uid;
		   		$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = 'eic_in_review';
		   		node_save($node);
		   		break;
		   	case 28:
		   		echo json_encode($oep_util->get_me_users());
		   		break;
		   	case 29://assign managing editor
		   		$nid = $_POST['nid'];
		   		$uid = $_POST['uid'];
		   		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		   		$node->field_managing_editor['und'][0]['target_id'] = $uid;
		   		$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = 'me_in_review';
		   		node_save($node);
		   		break;
		   	case 30:
		   		$author = $_POST["author"];
				$status = $_POST["status"];
				$results = $oep_util->filter_articles($author,$status);
				$headers = array(
						t('Title'),
						t('Authors'),
						t('Reviewers'),
						t('Factcheckers'),
						t('State'),
						t('Created'),
				);
				
				$rows = array();
				foreach ($results as $r){
					$authors = $r['authors'];
					$authors_str = '';
					foreach($authors as $author){
						if($authors_str != '')
							$authors_str .= '<br/>';
						$authors_str .= $author;
					}
					
					$reviewers = $r['reviewers'];
					$reviewers_str = '';
					foreach($reviewers as $reviewer){
						if($reviewers_str != '')
							$reviewers_str .= '<br/>';
						$reviewers_str .= $reviewer;
					}
					
					$factcheckers = $r['factcheckers'];
					$factcheckers_str = '';
					foreach($factcheckers as $factchecker){
						if($factcheckers_str != '')
							$factcheckers_str .= '<br/>';
						$factcheckers_str .= $factchecker;
					}

					$rows[] = array(
									l(t($r['title']), t("mypages/article/" . $r['article_id'] ."/view")),
								    t($authors_str),
									t($reviewers_str),
									t($factcheckers_str),
								    t(from_machine_name($r['state'])),
									$r['created'],
								  );
				}
				
				$output = theme('table', array('header' => $headers, 'rows' => $rows));
				echo $output;
		   		break;
		   	case 31:
		   		$nid = $_POST['nid'];
		  		$node = node_load($nid,OepUtil::get_latest_revision($nid));
				$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = 'needs_ce_review';
				node_save($node);
				drupal_set_message('"' . $node->title . '" was submitted to editorial coordinators');
				break;
		   	case 32:
		   		echo json_encode($oep_util->get_ce_users());
		   		break;
		   	case 33: //assign copy editor
		   		$nid = $_POST['nid'];
		   		$uid = $_POST['uid'];
		   		$ce = user_load($uid);
		   		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		   		$node->field_copy_editor['und'][0]['target_id'] = $uid;
		   		$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = 'ce_in_review';
		   		node_save($node);
		   		drupal_set_message('"' . $node->title . '" was assigned to copy editor "' . $ce->field_first_name['und'][0]['value'] . " " . $ce->field_last_name['und'][0]['value'] . '"');
		   		break;
		   	case 34:
		   		$nid = $_POST['nid'];
		  		$node = node_load($nid,OepUtil::get_latest_revision($nid));
				$article_state = $node->workbench_moderation['current']->state;
				$node->workbench_moderation_state_current = $article_state;
				$node->workbench_moderation_state_new = 'ce_review_completed';
				node_save($node);
				drupal_set_message('"' . $node->title . '" was submitted to editorial coordinators');
		   		break;
		   	case 35:
		   		$nid = $_POST['nid'];
		  		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		  		$reviewers = array();
		  		foreach($node->field_editorial_review_group['und'] as $reviewer) {	  		
		  			$user = user_load($reviewer['target_id']);
		  			$reviewers[] = $user->name;
		  		}
		  		$fact_checkers = array();
		  		foreach($node->field_fact_checker['und'] as $fact_checker){		  		
		  			$user = user_load($fact_checker['target_id']);
		  			$fact_checkers[] = $user->name;
		  		}
		  			
		  		$reviews_number = $node->field_required_review['und'][0]['value'];

		  		echo json_encode(array('reviewers' => $reviewers, 'fact_checkers' => $fact_checkers, 'reviews_number' => $reviews_number));
				break;
		   	case 36:
		   		$nid = $_POST['nid'];
		   		$node = node_load($nid,OepUtil::get_latest_revision($nid));
		  		if(!isset($node->field_editorial_review_group['und'])) {
		  			echo 'No editorial reviewer is assigned';
		  			break;
		  		}
		  			
		  		if(!isset($node->field_required_review['und'])) {
		  			echo 'Missing required reviews number';
		  			break;
		  		}

		  		if(count($node->field_editorial_review_group['und']) < intval($node->field_required_review['und'][0]['value'])){
		   			echo 'Number of reviewers is not satisfied';
		  			break;
		  		}
		  		$article_state = $node->workbench_moderation['current']->state;
		  		$node->workbench_moderation_state_current = $article_state;
		  		$node->workbench_moderation_state_new = 'reviewer_assigned';
		  		node_save($node);
		  		drupal_set_message("Article was assigned for reviewing successfully.");
		  		echo '';
		   		break;
		   	case 37: //save comments for the whole article
		   		$nid = $_POST['nid'];
		   		$vid = $_POST['vid'];
		   		$content = $_POST['content'];
		   		global $user;
		   		$uid = $user->uid;
		   		$created = time();
		   		$oep_util->add_article_comment($nid,$vid,$uid,$content,$created);
		    	$all_comments = OepUtil::get_article_comments($nid,$vid); 
		    	echo json_encode($all_comments);
		   		break;
		   	case 38:
		   		$nid = $_POST['nid'];
		   		$vid = $_POST['vid'];
		   		$all_comments = OepUtil::get_article_comments($nid,$vid);
		   		echo json_encode($all_comments);
		   		break;
		   	case 39: //remove a comment for the whole article
		   		$nid = $_POST['nid'];
		   		$vid = $_POST['vid'];
		   		$comment_id = $_POST['comment_id'];
		   		OepUtil::remove_comment($comment_id);
		   		$all_comments = OepUtil::get_article_comments($nid,$vid);
		   		
		   		echo json_encode($all_comments);
		   		break;
		}
	}