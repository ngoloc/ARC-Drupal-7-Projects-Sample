<?php 

class OepUtil {
  private $dbh;

  public function __construct() {
           $this->dbh =  new PDO("mysql:host=mysql.rc.pdx.edu;dbname=oehc_dev", "oehc_l", "CofjedsOk0");
         }

  public function init_state($topic_id) {
           $stmt = $this->dbh->prepare('INSERT INTO topicmgr_final_state(entity_id,state)
                                        VALUES(:topic_id,"New")');
           $stmt->bindParam(':topic_id', $topic_id);
           $stmt->execute();
         }
         
  public function get_state($topic_id) {
  	       $stmt = $this->dbh->prepare('SELECT state
                                        FROM topicmgr_state
                                        WHERE entity_id = :topic_id');
           $stmt->bindParam(':topic_id', $topic_id);
           $stmt->execute();
           $r = $stmt->fetch();
           return $r[0];
         }
  
  public function get_final_state($topic_id){
  	$stmt = $this->dbh->prepare('SELECT state
                                 FROM topicmgr_final_state
                                 WHERE entity_id = :topic_id');
    $stmt->bindParam(':topic_id', $topic_id);
    $stmt->execute();
    $r = $stmt->fetch();
    return $r[0];
  }
  
  public function assign_topic_to_author($topic_id, $author_id) {
  	$stmt = $this->dbh->prepare('INSERT INTO topicmgr_states(entity_id,state,author)
  	                             VALUES(:topic_id,"Assigned",:author_id)');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->execute();
  	
    //also update the copied entry in topicmgr_final_state
    $stmt = $this->dbh->prepare('UPDATE topicmgr_final_state
                                 SET state = "Assigned", first_author = :author_id 
                                 WHERE entity_id = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->execute();
    return true;
  }
  
  public function get_topic_title($topic_id){
  	$stmt = $this->dbh->prepare('SELECT title
  	                             FROM node
  	                             WHERE nid = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$topic_arr = $stmt->fetchAll();
    return $topic_arr[0]['title'];
  }
  
//  public function bind_article($topic_id, $article_id){
//  	$stmt = $this->dbh->prepare('UPDATE topicmgr_final_state
//                                 SET article_id = :article_id
//                                 WHERE entity_id = :topic_id');
//  	$stmt->bindParam(':article_id', $article_id);
//  	$stmt->bindParam(':topic_id', $topic_id);
//  	$stmt->execute();
//  }
  
  public function verify_topic_with_author_set($topic_id){
    $stmt = $this->dbh->prepare('SELECT *
								 FROM topicmgr_final_state
								 WHERE entity_id = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	if($stmt->rowCount()==0)
  	  return false;
  	return true;
  }
  
  public function get_topic_name($topic_id){
    $stmt = $this->dbh->prepare('SELECT *
								 FROM node
								 WHERE nid = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$topic_arr = $stmt->fetchAll();
  	return $topic_arr[0]['title'] ;
  }
  
  public function get_topic_category($topic_id){
    $stmt = $this->dbh->prepare('SELECT name category
                                 FROM node, field_data_field_category f, taxonomy_term_data
								 WHERE nid = entity_id AND f.field_category_tid = tid AND nid = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$topic_arr = $stmt->fetchAll();
  	return $topic_arr[0]['category'] ;
  }
  
  public function get_topic_county($topic_id){
    $stmt = $this->dbh->prepare('SELECT name county
    							 FROM node, field_data_field_county f, taxonomy_term_data
								 WHERE nid = entity_id AND f.field_county_tid = tid AND nid = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	if($stmt->rowCount()==0)
  		return "N/A";
  	$topic_arr = $stmt->fetchAll();
  	return $topic_arr[0]['county'] ;
  }
  
  public function get_reminder_style($topic_id,$reminder){
  	$stmt_string = 'SELECT *
  	                FROM topicmgr_state
  	                WHERE entity_id = :topic_id AND ' . $reminder . ' IS NOT NULL' ;
  	$stmt = $this->dbh->prepare($stmt_string);
  	
  	$stmt->bindParam(':topic_id', $topic_id);;
  	$stmt->execute();
  	if($stmt->rowCount()==0)
  	{
  	  switch($reminder){
  	  	case 'reminder1':
  	  		return 'style="color: rgb(59, 59, 59); background-color:white"';		
  	  	case 'reminder2':
  	  		return 'style="color: rgb(59, 59, 59); background-color:white; margin-left:150px; margin-top:-87px"';
  	  	case 'reminder3':
  	  		return 'style="color: rgb(59, 59, 59); background-color:white; margin-left:300px; margin-top:-87px"';		
  	  }
  	}
  	switch($reminder){
  	  	case 'reminder1':
  	  		return 'style="color: white; background-color:navy"';
  	  	case 'reminder2':
  	  		return 'style="color: white; background-color:navy; margin-left:150px; margin-top:-87px"';
  	  	case 'reminder3':
  	  		return 'style="color: white; background-color:navy; margin-left:300px; margin-top:-87px"';		
  	  }
  }
  
  public function get_reminder_date($topic_id,$reminder)
  {
  	$stmt_string = 'SELECT ' . $reminder .
  	              ' FROM topicmgr_state
  	                WHERE entity_id = :topic_id AND ' . $reminder . ' IS NOT NULL';
  	$stmt = $this->dbh->prepare($stmt_string);
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	if($stmt->rowCount()==0)
  	  return "";
  	$reminders_arr = $stmt->fetchAll();
  	return date('Y-m-d H:i:s',$reminders_arr[0][0]);
  }
  
  public function update_w9onfile($topic_id,$date,$author_id)
  {
  	$time = ($date == "") ? NULL : strtotime($date);
  	 
  	$stmt_string = 'UPDATE topicmgr_states
  					SET w9onfile = :time
  					WHERE entity_id = :topic_id AND author = :author_id';
  	 
  	$stmt = $this->dbh->prepare($stmt_string);
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->bindParam(':time', $time);
  	$stmt->execute();
  }
  
  public function update_reminder($topic_id,$reminder,$date,$author_id)
  {
  	$time = ($date == "") ? NULL : strtotime($date);
  	
  	$stmt_string = 'UPDATE topicmgr_states
  					SET '. $reminder . ' = :time
  					WHERE entity_id = :topic_id AND author = :author_id';
  	 
  	$stmt = $this->dbh->prepare($stmt_string);
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->bindParam(':time', $time);
  	$stmt->execute();
  }
  
  public function add_topic_author($topic_id,$author)
  {
  	$stmt_string = 'INSERT INTO topicmgr_states(entity_id,author)
  					VALUES(:topic_id,:author)';
  	$stmt = $this->dbh->prepare($stmt_string);
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author', $author);
  	$stmt->execute();
  }
  
  //get the first author id of the topic
  public function get_author($topic_id)
  {
  	$stmt = $this->dbh->prepare('SELECT uid
								 FROM topicmgr_state, users
								 WHERE author = uid AND entity_id = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetchAll(); 
  	return $arr[0]["uid"];
  }
  
  //get the first author name of the topic
  public function get_author_name($topic_id)
  {
  	$stmt = $this->dbh->prepare('SELECT name
								 FROM topicmgr_state, users
								 WHERE author = uid AND entity_id = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetchAll(); 
  	return $arr[0]["name"];
  }
  
  public function get_all_authors() {
  	$stmt = $this->dbh->prepare('SELECT u.uid, u.name
  	                             FROM users u, users_roles ur, role rl
                                 WHERE u.uid = ur.uid AND rl.rid = ur.rid AND rl.name = "author"');
    $stmt->execute();
    return $stmt->fetchAll();
  }
  
  public function get_all_authors_json()
  {
  	$stmt = $this->dbh->prepare('SELECT u.uid, u.name
								 FROM users u, users_roles ur, role r
								 WHERE u.uid = ur.uid AND r.rid = ur.rid AND r.name = "author"');
	$stmt->execute();
	$authors = $stmt->fetchAll();
	$jsonStr = "{";
	$first_time = true;
	foreach($authors as $author){
		if(!$first_time)
			$jsonStr .= ',';
		$jsonStr .= '"' . $author["uid"] . '":"' . $author["name"] . '"';
		$first_time = false; 
	} 
	$jsonStr .= "}";
  	return $jsonStr;
  }
  
  public function get_avail_authors($topic_id){
  	$stmt = $this->dbh->prepare('SELECT u.uid author_id, u.name author_name
								 FROM users u, users_roles ur, role r
								 WHERE u.uid = ur.uid AND r.rid = ur.rid AND r.name = "author"
								       AND u.uid not in
									   (SELECT author
									    FROM topicmgr_states
										WHERE entity_id = :topic_id
									   )');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetchAll();
  	return $arr;
  }
  
  public function get_authors()
  {
  	$stmt = $this->dbh->prepare('SELECT u.uid, u.name
								 FROM users u, users_roles ur, role r
								 WHERE u.uid = ur.uid AND r.rid = ur.rid AND r.name = "author"');
  	$stmt->execute();
  	$authors = $stmt->fetchAll();
  	$str = "";
  	foreach($authors as $author)
  		$str .= '<option value = "' . $author["uid"] . '">' . " " . $author["name"] . '</option>';
  	return $str;
  }
  
  public function get_counties($search_county)
  {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "county"');
  	$stmt->execute();
  	$counties = $stmt->fetchAll();
  	$str = "";
  	foreach($counties as $county)
  		if($county["tid"]==$search_county)
  			$str .= '<option value = "' . $county["tid"] . '" selected>' . " " . $county["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $county["tid"] . '">' . " " . $county["name"] . '</option>';
  	return $str;
  }
  
  public function get_eras($search_era)
  {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "era"');
  	$stmt->execute();
  	$eras = $stmt->fetchAll();
  	$str = "";
  	foreach($eras as $era)
  		if($era["tid"]==$search_era)
  			$str .= '<option value = "' . $era["tid"] . '" selected>' . " " . $era["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $era["tid"] . '">' . " " . $era["name"] . '</option>';
  	return $str;
  }
  
  public function get_topic_authors($topic_id)
  {
  	$stmt = $this->dbh->prepare('SELECT author, state, reminder1, reminder2, reminder3, w9onfile, name author_name, invited
								 FROM topicmgr_states, users
								 WHERE entity_id = :topic_id AND author = uid
								 ORDER BY sid');
  	
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetchAll();
  	return $arr;
  }
  
  public function update_state($topic_id,$author_id,$status){
  	$stmt = $this->dbh->prepare('UPDATE topicmgr_states
  								 SET state = :status
  								 WHERE entity_id = :topic_id AND author = :author_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->bindParam(':status', $status);
  	$stmt->execute();
  	$this->update_final_state($topic_id);
  }
  
  public function update_final_state($topic_id){
  	//the topic is accepted if there at least 1 author accepts it
  	$result = "Disagreed";
  	$stmt = $this->dbh->prepare('SELECT *
  								 FROM topicmgr_states
  								 WHERE entity_id = :topic_id AND state = "Accepted"');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	if($stmt->rowCount()>0)
  		$result = "Accepted";
  	else {
  		//check if the topic is still assigned
		$stmt = $this->dbh->prepare('SELECT *
								     FROM topicmgr_states
		  							 WHERE entity_id = :topic_id AND state = "Assigned"');
		$stmt->bindParam(':topic_id', $topic_id);
		$stmt->execute();
		if($stmt->rowCount()>0)
			$result = "Assigned";
	}
	//update primary author in topicmgr_final_state
	$stmt = $this->dbh->prepare('SELECT *
								 FROM topicmgr_states
								 WHERE entity_id = :topic_id
								 ORDER BY sid');
	$stmt->bindParam(':topic_id', $topic_id);
	$stmt->execute();
	$au_state = $stmt->fetch();
	$primary_au_id = $au_state['author'];
	$stmt = $this->dbh->prepare('UPDATE topicmgr_final_state
								 SET first_author = :primary_au_id
								 WHERE entity_id = :topic_id');
	$stmt->bindParam(':topic_id', $topic_id);
	$stmt->bindParam(':primary_au_id', $primary_au_id);
	$stmt->execute();
	//update state in topicmgr_final_state
  	$stmt = $this->dbh->prepare('UPDATE topicmgr_final_state
  								 SET state = :result
  								 WHERE entity_id = :topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':result', $result);
  	$stmt->execute();
  }
  
  public function get_categories($search_cat) {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "category"');
  	$stmt->execute();
  	$categories = $stmt->fetchAll();
  	$str = "";
  	foreach($categories as $category){
  		if($category["tid"]==$search_cat)
  			$str .= '<option value = "' . $category["tid"] . '" selected>' . " " . $category["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $category["tid"] . '">' . " " . $category["name"] . '</option>';
  	}
  	return $str;
  }
  
  public function get_themes($search_theme) {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "theme"');
  	$stmt->execute();
  	$themes = $stmt->fetchAll();
  	$str = "";
  	foreach($themes as $theme){
  		if($theme["tid"]==$search_theme)
  			$str .= '<option value = "' . $theme["tid"] . '" selected>' . " " . $theme["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $theme["tid"] . '">' . " " . $theme["name"] . '</option>';
  	}
  	return $str;
  }
  
  public function get_subthemes($search_subtheme) {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "sub_theme"');
  	$stmt->execute();
  	$subthemes = $stmt->fetchAll();
  	$str = "";
  	foreach($subthemes as $subtheme){
  		if($subtheme["tid"]==$search_subtheme)
  			$str .= '<option value = "' . $subtheme["tid"] . '" selected>' . " " . $subtheme["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $subtheme["tid"] . '">' . " " . $subtheme["name"] . '</option>';
  	}
  	return $str;
  }
  
  public function get_regions($search_region) {
  	$stmt = $this->dbh->prepare('SELECT DISTINCT tid, t.name
								 FROM taxonomy_vocabulary v, taxonomy_term_data t
								 WHERE v.vid = t.vid AND v.machine_name = "region"');
  	$stmt->execute();
  	$regions = $stmt->fetchAll();
  	$str = "";
  	foreach($regions as $region)
  		if($region["tid"]==$search_region)
  			$str .= '<option value = "' . $region["tid"] . '" selected>' . " " . $region["name"] . '</option>';
  		else
  			$str .= '<option value = "' . $region["tid"] . '">' . " " . $region["name"] . '</option>';
  	return $str;
  }
  
  public function filter_articles($author, $status) {
  	$sql = 'SELECT DISTINCT n.nid
			FROM node n, workbench_moderation_node_history wmnh, field_data_field_authors fdfa
			WHERE type = "article" AND n.nid = wmnh.nid AND wmnh.current = 1 AND n.nid = fdfa.entity_id';
  	
	
  	if($author!='all')
  	  	$sql .= " AND fdfa.field_authors_target_id = :author";
  	
  	if($status!='all')
  	  	$sql .= " AND wmnh.state = :status";	
  	
  	$stmt = $this->dbh->prepare($sql);
  	if($author!='all')
  		$stmt->bindParam(':author', $author);
  	if($status!='all')
  		$stmt->bindParam(':status', $status);
  	$stmt->execute();
  	
  	$arr = $stmt->fetchAll();
  	
  	$results = array();
  	foreach($arr as $article_id) {
  		$article  = node_load($article_id);
  		
  		$authors = array();
  		foreach($article->field_authors['und'] as $author_id){
  			$author = user_load($author_id['target_id']);
  			$authors[] = $author->name;
  		}
  		
  		$reviewers = array();
  		foreach($article->field_editorial_review_group['und'] as $reviewer_id){
  			$reviewer = user_load($reviewer_id['target_id']);
  			$reviewers[] = $reviewer->name;
  		}
  		
  		$factcheckers = array();
  		foreach($article->field_fact_checker['und'] as $factchecker_id){
  			$factchecker = user_load($factchecker_id['target_id']);
  			$factcheckers[] = $factchecker->name;
  		}
  		
  		$results[] = array("article_id" => $article->nid, "title" => $article->title, "authors" => $authors, "reviewers" => $reviewers, "factcheckers" => $factcheckers,
  						   "state" => $article->workbench_moderation['current']->state, "created" => date('Y-m-d H:i:s',$article->created));
  	}
  	return $results;
  }
  
  public function filter_topics($category,$status,$author,$county) {

  	$query_str = 'SELECT topic_id, category, topic, L.created, state, u.name author, R.name county, R.field_county_tid, fdfaa.field_associated_article_target_id article_id
				  FROM (SELECT nid topic_id, ttd.name category, title topic, created, fdffs.state state, first_author
     					FROM node, field_data_field_category fdfc, taxonomy_term_data ttd, topicmgr_final_state fdffs
     					WHERE nid = fdfc.entity_id AND fdfc.field_category_tid = ttd.tid AND nid = fdffs.entity_id';
  	if($category!='all')
  		$query_str .= ' AND ttd.tid = :category';
  		
	if($status!='all')
  		$query_str .= ' AND fdffs.state = :status';
  	
  	if($author!='all')
  		$query_str .= ' AND first_author = :author';
  		
  	$query_str .= ') L LEFT JOIN (SELECT  *
								  FROM field_data_field_county fdfc1, taxonomy_term_data ttd1
		 						  WHERE fdfc1.field_county_tid = ttd1.tid
  								 ) R
								   ON L.topic_id = R.entity_id
								   		LEFT JOIN users u ON L.first_author = uid
								   		LEFT JOIN field_data_field_associated_article fdfaa ON L.topic_id = fdfaa.entity_id';
  	if($county!='all')
  		$query_str .= ' WHERE field_county_tid = :county';
  
	$query_str .= ' ORDER BY topic';
  	
  	$stmt = $this->dbh->prepare($query_str);
  	if($category!='all')
  		$stmt->bindParam(':category', $category);
  	if($status!='all')
  		$stmt->bindParam(':status', $status);
  	if($author!='all')
  		$stmt->bindParam(':author', $author);
  	if($county!='all')
  		$stmt->bindParam(':county', $county);
  		
    $stmt->execute();
    $results = $stmt->fetchAll();

    return $results;
  }
  
  public function get_topic_titles(){
  	$stmt = $this->dbh->prepare('SELECT title
								 FROM node
								 WHERE type="topic"');
  	$stmt->execute();
  	$topic_titles = $stmt->fetchAll();
  	$arr = array();
    foreach($topic_titles as $topic_title)
  		array_push($arr,$topic_title["title"]);
  	return $arr;
  }

  public function get_authors_autocomplete(){
  	$stmt = $this->dbh->prepare('SELECT u.uid
  	                             FROM users u, users_roles ur, role rl
                                 WHERE u.uid = ur.uid AND rl.rid = ur.rid AND rl.name = "author"');
    $stmt->execute();
    $authors = $stmt->fetchAll();
    $arr = array();
    foreach($authors as $author){
  		$author_user = user_load($author["uid"]);
  		$author_full_name = '';
  		if(count($author_user->field_first_name)!=0 && count($author_user->field_last_name)!=0)
  			$author_full_name = $author_user->field_first_name['und'][0]['value'] . ' ' . $author_user->field_last_name['und'][0]['value'];
  		if($author_full_name != '')
    		array_push($arr,$author_full_name);
    }
  	return $arr;
  }  
  
  public function insert_topic_author_note($topic_id,$author_id,$note_content,$date_taken,$commenter) {
  	$stmt = $this->dbh->prepare('INSERT INTO topicmgr_note_topic_author(topic_id,author_id,date_taken,commenter,note_content)
  								 VALUES(:topic_id,:author_id,:date_taken,:commenter,:note_content)');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->bindParam(':date_taken', $date_taken);
  	$stmt->bindParam(':commenter', $commenter);
  	$stmt->bindParam(':note_content', $note_content);
  	$stmt->execute();
  	return $this->dbh->lastInsertId();
  }
  
  public function update_topic_author_note($note_id,$note_content) {
  	$stmt = $this->dbh->prepare('UPDATE topicmgr_note_topic_author
  								 SET note_content = :note_content
  								 WHERE note_id = :note_id');
  	$stmt->bindParam(':note_id', $note_id);
  	$stmt->bindParam(':note_content', $note_content);
  	$stmt->execute();
  	
  	$stmt = $this->dbh->prepare('SELECT date_taken, name commenter
  								 FROM topicmgr_note_topic_author, users
  								 WHERE note_id = :note_id AND commenter = uid');
  	$stmt->bindParam(':note_id', $note_id);
  	$stmt->execute();
  	return $stmt->fetch();
  }
  
  public function delete_author_note($note_id) {
  	$stmt = $this->dbh->prepare('DELETE FROM topicmgr_note_topic_author
  								 WHERE note_id = :note_id');
  	$stmt->bindParam(':note_id', $note_id);
  	$stmt->execute();
  }
  
  public function get_author_notes($topic_id,$author_id) {
  	$stmt = $this->dbh->prepare('SELECT note_id, date_taken, name commenter, note_content
								 FROM topicmgr_note_topic_author, users
								 WHERE topic_id = :topic_id AND author_id = :author_id AND commenter = uid');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->execute();
  	return $stmt->fetchAll();
  }
  
  public function get_author_note($note_id) {
  	$stmt = $this->dbh->prepare('SELECT note_content
								 FROM topicmgr_note_topic_author
								 WHERE note_id = :note_id');
  	$stmt->bindParam(':note_id', $note_id);
  	$stmt->execute();
  	return $stmt->fetch();
  }
  
  public function set_invited_author($topic_id,$author_id,$invited){
  	$stmt = $this->dbh->prepare('UPDATE topicmgr_states
								 SET invited = :invited
								 WHERE entity_id = :topic_id AND author = :author_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->bindParam(':invited', $invited);
  	$stmt->execute();
  }
  
  public function get_article_id($topic_id)
  {
//  	$stmt = $this->dbh->prepare('SELECT article_id
//  								 FROM topicmgr_final_state
//								 WHERE entity_id = :topic_id');
//  	$stmt->bindParam(':topic_id', $topic_id);
//  	$stmt->execute();
//  	$arr = $stmt->fetch();
//  	return $arr["article_id"];
	$node = node_load($topic_id);
	return $node->field_associated_article['und'][0]['target_id'];
  }
  
  private function get_topic_id($article_id)
  {
//  	$stmt = $this->dbh->prepare('SELECT field_associated_topic_target_id
//  								 FROM field_data_field_associated_topic
//								 WHERE entity_id = :article_id');
//  	$stmt->bindParam(':article_id', $article_id);
//  	$stmt->execute();
//  	$arr = $stmt->fetch();
  	
//  	return $arr["field_associated_topic_target_id"];
	$node = node_load($article_id);
	return $node->field_associated_topic['und'][0]['target_id'];
  }
  
  private function category_match($article_id,$category)
  {
  	if($category=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_category_tid
  								 FROM field_data_field_category
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	if($arr["field_category_tid"] != $category)
  		return false;
  	return true;
  }

  private function theme_match($article_id,$theme)
  {
  	if($theme=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_theme_tid
  								 FROM field_data_field_theme
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	if($arr["field_theme_tid"] != $theme)
  		return false;
  	return true;
  }
  
  private function subtheme_match($article_id,$subtheme)
  {
  	if($subtheme=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_sub_theme_tid
  								 FROM field_data_field_sub_theme
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	if($arr["field_sub_theme_tid"] != $subtheme)
  		return false;
  	return true;
  }
  
  private function county_match($article_id,$county)
  {
  	
  	if($county=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_county_tid
  								 FROM field_data_field_county
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	
  	if($arr["field_county_tid"] != $county)
  		return false;
  	return true;
  }
  
  private function era_match($article_id,$era)
  {
  	if($era=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_era_tid
  								 FROM field_data_field_era
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	if($arr["field_era_tid"] != $era)
  		return false;
  	return true;
  }
  
  private function region_match($article_id,$region)
  {
  	if($region=='all')
  		return true;
  	$topic_id = $this->get_topic_id($article_id);
  	$stmt = $this->dbh->prepare('SELECT field_region_tid
  								 FROM field_data_field_region
								 WHERE entity_id=:topic_id');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	if($arr["field_region_tid"] != $region)
  		return false;
  	return true;
  }
  
  public function content_match($article_id,$keywords)
  {
  	$stmt = $this->dbh->prepare('SELECT body_value
  								 FROM field_data_body
								 WHERE entity_id =:article_id');
  	$stmt->bindParam(':article_id', $article_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	
  	$trimmed_text = trim(utf8_encode(strip_tags(str_replace("</p>","</p> ",$arr["body_value"]))));
  	$trimmed_keywords = trim($keywords);
  	$exact_match = false;
  	//check if this is axact matching
  	if(strlen($trimmed_keywords)>=2 && substr($trimmed_keywords,0,1)=='"'&&substr($trimmed_keywords,strlen($trimmed_keywords)-1,1)=='"'){
  		$trimmed_keywords = trim(substr($trimmed_keywords,1,strlen($trimmed_keywords)-2));
  		$exact_match = true;
  	}
  	
  	$content_words = preg_split("/[\s]+/", $trimmed_text);
  	$keywords_arr = preg_split("/[\s]+/", $trimmed_keywords);

  	if($trimmed_keywords=='')
  		return true;

  	if($exact_match){
	  	$haystack = '';
	  	$begin = true;
	  	foreach($content_words as $cw) {
	  		if($cw!=''){
	  			if(!$begin)
	  				$haystack .= " ";
	  			$haystack .= strtolower($cw);
	  			$begin = false;
	  		}
	  	}
	
	  	$needle = '';
	  	$begin = true;
	  	foreach($keywords_arr as $k) {
	  		if($k!=''){
	  			if(!$begin)
	  				$needle .= " ";
	  			$needle .= strtolower($k);
	  			$begin = false;
	  		}
	  	}
	  	if(strpos($haystack,$needle)===false)
	  		return false;
	  	return true;
  	}
  	
  	//not exact match
  	foreach ($keywords_arr as $k){
  		$found = false;
  		foreach($content_words as $cw) {
  			if(strpos(strtolower($cw),strtolower($k))!==false){
  				$found = true;
  				break;
  			}
  		}
  		if(!$found)
  			return false;
  	}
  	return true;
  }
  
  public function title_match($article_id,$title_keywords)
  {
  	$stmt = $this->dbh->prepare('SELECT title
  								 FROM node
								 WHERE nid =:article_id');
  	$stmt->bindParam(':article_id', $article_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	
  	$trimmed_text = trim(utf8_encode($arr["title"]));
  	$trimmed_keywords = trim($title_keywords);
  	$exact_match = false;
  	//check if this is axact matching
  	if(strlen($trimmed_keywords)>=2 && substr($trimmed_keywords,0,1)=='"'&&substr($trimmed_keywords,strlen($trimmed_keywords)-1,1)=='"')
  	{
  		$trimmed_keywords = trim(substr($trimmed_keywords,1,strlen($trimmed_keywords)-2));
  		$exact_match = true;
  	}

  	$title_words = preg_split("/[\s]+/", $trimmed_text);
  	$keywords_arr = preg_split("/[\s]+/", $trimmed_keywords);
  	
  	if($trimmed_keywords=='')
  		return true;
  	if($exact_match){
	  	$haystack = '';
	  	$begin = true;
	  	foreach($title_words as $tw) {
	  		if($tw!=''){
	  			if(!$begin)
	  				$haystack .= " ";
	  			$haystack .= strtolower($tw);
	  			$begin = false;
	  		}
	  	}
	
	  	$needle = '';
	  	$begin = true;
	  	foreach($keywords_arr as $k) {
	  		if($k!=''){
	  			if(!$begin)
	  				$needle .= " ";
	  			$needle .= strtolower($k);
	  			$begin = false;
	  		}
	  	}
	  	if(strpos($haystack,$needle)===false)
	  		return false;
	  	return true;
  	}
  	//not exact match
  	foreach ($keywords_arr as $k){
  		$found = false;
  		foreach($title_words as $cw) {
  			if(strpos(strtolower($cw),strtolower($k))!==false){
  				$found = true;
  				break;
  			}
  		}
  		if(!$found)
  			return false;
  	}
  	return true;
  }
  
  private function author_match($article_id,$author_keywords)
  {
  	if(trim($author_keywords)=='')
  		return true;
  	$article_node = node_load($article_id);
  	
  	foreach($article_node->field_authors['und'] as $r){
  		$author = user_load($r['target_id']);
  		$full_name = $author->field_first_name['und'][0]['value'] . ' ' . $author->field_last_name['und'][0]['value'];
  		if(strtolower(trim($author_keywords)) == strtolower(trim($full_name)))
 			return true;
  	}

 	return false; 
  }
  
  private function is_published($article_id)
  {
  	$stmt = $this->dbh->prepare('SELECT *
  								 FROM workbench_moderation_node_history 
								 WHERE nid =:article_id AND current = 1 AND state = "published"');
  	$stmt->bindParam(':article_id', $article_id);
  	$stmt->execute();
  	if($stmt->rowCount()==0)
  		return false;
  	return true;
  }
  
  private function trim_body($snippet) {
  	$trim_text = str_replace("</p>","</p> ",$snippet);
  	$trim_text = strip_tags($trim_text);
  	$words = preg_split("/[\s]+/", $trim_text);
  	//get first 130 characters
  	$ret_words = "";
  	$begin = true;
  	
  	foreach($words as $w) {
  		if($w!=''){
	  		if(!$begin)
	  			$ret_words .= " ";
	  		if(strlen($ret_words.$w)>130)
	  			break;
	    	$ret_words .= $w;
	    	$begin = false;
  		}
  	}
  	$ret_words .= '...';
    return utf8_encode($ret_words);	
  }
  
  private function get_snippet($article_id)
  {
  	$stmt = $this->dbh->prepare('SELECT body_value
  								 FROM field_data_body
								 WHERE entity_id =:article_id');
  	$stmt->bindParam(':article_id', $article_id);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	$snippet = $arr["body_value"];
  	
  	return $this->trim_body($snippet);
  }
  
  public function get_articles($only_published,$content_keyword,$title_keyword,$author_keyword,$category,$theme,$subtheme,$county,$era,$region)
  {
  	$stmt = $this->dbh->prepare('SELECT nid, title
  								 FROM node
								 WHERE type="article"');
  	$stmt->execute();
  	$article_ids = $stmt->fetchAll();
  	$count = 0;
  	$result_arr = array();
  	foreach ($article_ids as $article_id){
  		if($only_published && !$this->is_published($article_id["nid"]))
  			continue;
  		if(!$this->category_match($article_id["nid"],$category))
  			continue;
  		if(!$this->theme_match($article_id["nid"],$theme))
  			continue;
  		if(!$this->subtheme_match($article_id["nid"],$subtheme))
  			continue;
  		if(!$this->county_match($article_id["nid"],$county))
  			continue;
  		if(!$this->era_match($article_id["nid"],$era))
  			continue;
  		if(!$this->region_match($article_id["nid"],$region))
  			continue;
  		if(!$this->content_match($article_id["nid"],$content_keyword))
  			continue;
  		if(!$this->title_match($article_id["nid"],$title_keyword))
  			continue;
  		if(!$this->author_match($article_id["nid"],$author_keyword))
  			continue;
  		//add this entry to SESSION variable
  		$entry = array("article_id" => $article_id["nid"] ,"article_title" => $article_id["title"], "snippet" => $this->get_snippet($article_id["nid"]));
  		array_push($result_arr,$entry);
  		$count = $count + 1;
  	}
  	$_SESSION["search_results"] = $result_arr;
  	return $count;
  }
  
  public function get_article_list(){
  	$stmt = $this->dbh->prepare('SELECT nid article_id, title
  								 FROM node
								 WHERE type="article"');
  	$stmt->execute();
  	return $stmt->fetchAll();
  }
  
  public function add_new_thread($nid,$vid,$start,$stop){
  	$stmt = $this->dbh->prepare('INSERT INTO oep_comments_threads(start,stop,nid,vid)
  								 VALUES(:start,:stop,:nid,:vid)
  								');
  	$stmt->bindParam(':nid', $nid);
  	$stmt->bindParam(':vid', $vid);
  	$stmt->bindParam(':start', $start);
  	$stmt->bindParam(':stop', $stop);
  	$stmt->execute();
  	return $this->dbh->lastInsertId();
  }
  
  private function get_user_fullname($uid){
  	$fullname = '';
  	$stmt = $this->dbh->prepare('SELECT field_first_name_value firstname
  								 FROM field_data_field_first_name
  								 WHERE entity_id = :uid
  								');
  	
  	$stmt->bindParam(':uid', $uid);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	$fullname .= $arr["firstname"];
  	
  	$stmt = $this->dbh->prepare('SELECT field_last_name_value lastname
  								 FROM field_data_field_last_name
  								 WHERE entity_id = :uid
  								');
  	
  	$stmt->bindParam(':uid', $uid);
  	$stmt->execute();
  	$arr = $stmt->fetch();
  	$fullname .= ' '.$arr["lastname"];
  	return $fullname;
  }
  
  public function get_all_comments($nid,$vid){
  	$stmt = $this->dbh->prepare('SELECT thid, start, stop
  								 FROM oep_comments_threads
  								 WHERE nid = :nid AND vid = :vid
  								');

  	$stmt->bindParam(':nid', $nid);
  	$stmt->bindParam(':vid', $vid);
  	$stmt->execute();
  	
  	return $stmt->fetchAll();
  }
  
  public function get_comments($thid){
  	$stmt = $this->dbh->prepare('SELECT uid, created, comment
  								 FROM oep_comments
  								 WHERE thid = :thid
  								');

  	$stmt->bindParam(':thid', $thid);
  	$stmt->execute();
  	$comments = $stmt->fetchAll();
  	$comments_ret = array();
  	foreach($comments as $comment){
  		$comments_ret[] = array("fullname" => $this->get_user_fullname($comment["uid"]),
  							    "created_date" => date("Y-m-d H:i:s", $comment["created"]),
  							    "comment" => $comment["comment"]);
  	}
  	return $comments_ret;
  }
  
  public function insert_comment($thid,$comment,$uid,$created,$vid) {
  	$stmt = $this->dbh->prepare('INSERT INTO oep_comments(thid,comment,uid,created)
								 VALUES(:thid,:comment,:uid,:created)
  								');
	
  	$stmt->bindParam(':thid', $thid);
  	$stmt->bindParam(':comment', $comment);
  	$stmt->bindParam(':uid', $uid);
  	$stmt->bindParam(':created', $created);
  	$stmt->execute();
  	//also update oep_review table
  	$this->update_oep_review($vid,$uid,"incomplete");
  }
  
  public function update_oep_review($vid,$uid,$state) {
  	
  	$stmt = $this->dbh->prepare('SELECT * FROM oep_review
								 WHERE vid = :vid AND uid = :uid
  								');
	
  	$stmt->bindParam(':vid', $vid);
  	$stmt->bindParam(':uid', $uid);
  	$stmt->execute();
  	if($stmt->rowCount()==0){
  		//make an insert
  		$stmt = $this->dbh->prepare('INSERT INTO oep_review(uid,vid,state)
									 VALUES(:uid,:vid,:state)
  								   ');
  	}
  	else{
  		//update state value
  		$stmt = $this->dbh->prepare('UPDATE oep_review
									 SET state = :state
									 WHERE uid = :uid AND vid = :vid
  								   ');
  	}
  	$stmt->bindParam(':vid', $vid);
	$stmt->bindParam(':uid', $uid);
	$stmt->bindParam(':state', $state);
	$stmt->execute();
  }
  
  public function remove_author($topic_id,$author_id){
  	$stmt = $this->dbh->prepare('SELECT *
					  			 FROM topicmgr_states
					  			 WHERE entity_id = :topic_id
					  			');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->execute();
  	if($stmt->rowCount()<=1)
  		return 0;
  	$stmt = $this->dbh->prepare('DELETE
					  			 FROM topicmgr_states
					  			 WHERE entity_id = :topic_id AND author = :author_id
					  			');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->execute();
  	$stmt = $this->dbh->prepare('DELETE
					  			 FROM topicmgr_note_topic_author
					  			 WHERE topic_id = :topic_id AND author_id = :author_id
					  			');
  	$stmt->bindParam(':topic_id', $topic_id);
  	$stmt->bindParam(':author_id', $author_id);
  	$stmt->execute();
  	$this->update_final_state($topic_id);
  	return 1;
  }
  
  public function topic_title_existed($topic){
  	$stmt = $this->dbh->prepare("SELECT *
					  			 FROM node 
					  			 WHERE type = 'topic' AND title = :topic
					  		   	");
  	$stmt->bindParam(':topic', $topic);
  	$stmt->execute();
  	if($stmt->rowCount()>=1)
  		return true;
  	return false;
  }
  
  public function oep_review_completed_state($nid,$uid){
  	
  	$stmt = $this->dbh->prepare("SELECT * FROM oehc_dev.oep_review
								 WHERE state = 'complete' AND vid = (select max(revision_id)
								 									 from field_revision_body
								 									 where entity_id = :nid)
								 						  AND uid = :uid
  							    ");
  	$stmt->bindParam(':nid', $nid);
  	$stmt->bindParam(':uid', $uid);
  	$stmt->execute();
  	if($stmt->rowCount()>=1)
  		return true;
  	return false;
  }
  
  public function get_number_of_complete_reviews($vid){
  	$stmt = $this->dbh->prepare("SELECT * FROM oehc_dev.oep_review
						  		 WHERE state = 'complete' AND vid = :vid
						  		");
  	$stmt->bindParam(':vid', $vid);
  	$stmt->execute();
  	return $stmt->rowCount();
  }
  
  public function validate_factchecker($nid, $uid){
  	$node = node_load($nid);
  	$uids = array();
  	foreach($node->field_fact_checker['und'] as $r)
  		$uids[] = $r['target_id'];
  	
  	if($node->workbench_moderation['current']->state=='reviewer_assigned' && in_array($uid, $uids))
  		return true;
  	return false;
  }
  
  public function validate_eic($nid,$uid){
  	$node = node_load($nid);
  	$uids = array();
  	foreach($node->field_chief_in_editor['und'] as $r)
  		$uids[] = $r['target_id'];
  	
  	if($node->workbench_moderation['current']->state=='eic_in_review' && in_array($uid, $uids))
  		return true;
  	return false;
  }
  
  public function validate_me($nid,$uid){
  	$node = node_load($nid);
  	$uids = array();
  	foreach($node->field_managing_editor['und'] as $r)
  		$uids[] = $r['target_id'];
  	
  	if($node->workbench_moderation['current']->state=='me_in_review' && in_array($uid, $uids))
  		return true;
  	return false;
  }
  
  public function get_eic_users(){
  	$stmt = $this->dbh->prepare("SELECT u.uid, u.name, field_first_name_value first_name, field_last_name_value last_name
								 FROM users_roles ur, role r, users u, field_data_field_first_name fdffn, field_data_field_last_name fdfln
								 WHERE ur.rid = r.rid AND ur.uid = u.uid AND r.name = 'editor in chief'
								       AND u.uid = fdffn.entity_id AND  u.uid = fdfln.entity_id
						  		");
  	$stmt->execute();
  	return $stmt->fetchAll();
  }
  
  public function get_me_users(){
  	$stmt = $this->dbh->prepare("SELECT u.uid, u.name, field_first_name_value first_name, field_last_name_value last_name
								 FROM users_roles ur, role r, users u, field_data_field_first_name fdffn, field_data_field_last_name fdfln
								 WHERE ur.rid = r.rid AND ur.uid = u.uid AND r.name = 'managing editor'
								       AND u.uid = fdffn.entity_id AND  u.uid = fdfln.entity_id
						  		");
  	$stmt->execute();
  	return $stmt->fetchAll();
  }
  
  public function get_ce_users(){
  	$stmt = $this->dbh->prepare("SELECT u.uid, u.name, field_first_name_value first_name, field_last_name_value last_name
								 FROM users_roles ur, role r, users u, field_data_field_first_name fdffn, field_data_field_last_name fdfln
								 WHERE ur.rid = r.rid AND ur.uid = u.uid AND r.name = 'copy editor'
								       AND u.uid = fdffn.entity_id AND  u.uid = fdfln.entity_id
						  		");
  	$stmt->execute();
  	return $stmt->fetchAll();
  }
  
  public function markAllIncompleteReviewers($nid)
  {
  	$vid = OepUtil::get_latest_revision($nid);
  	$stmt = $this->dbh->prepare("UPDATE oep_review
  								 SET state = 'incomplete'
  								 WHERE vid = :vid
						  		");
  	$stmt->bindParam(':vid', $vid);
  	$stmt->execute();  	
  }
  
  public function add_article_comment($nid,$vid,$uid,$content,$created)
  {
  	$stmt = $this->dbh->prepare('INSERT INTO article_comments (nid,vid,uid,content,created)
		   						 VALUES(:nid,:vid,:uid,:content,:created)');
  	$stmt->bindParam(':nid', $nid);
  	$stmt->bindParam(':vid', $vid);
  	$stmt->bindParam(':uid', $uid);
  	$stmt->bindParam(':content', $content);
  	$stmt->bindParam(':created', $created);
  	$stmt->execute(); 
  }
  
  public function insert_libs() {

	$jqwidgets_scripts_root =  drupal_get_path('module', 'topicmgr')  . '/libs/jqwidgets/scripts/';
  	return 
   '
    <link rel="stylesheet" type="text/css" href="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/css/topicmgr.css"/>
	<link rel="stylesheet" type="text/css" href="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/css/base/jquery.ui.all.css"/>
	<link rel="stylesheet" type="text/css" href="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/libs/jqwidgets/styles/jqx.base.css" />
	<script src="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/scripts/ajax.googleapis.com/jquery.min.js"></script>
	<script src="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/scripts/ajax.googleapis.com/jquery-ui.min.js"></script>
	<script src="' . base_path() . drupal_get_path('module', 'topicmgr')  . '/scripts/date.format.js"></script>
	<script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxcore.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxbuttons.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxscrollbar.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxmenu.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxgrid.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxgrid.pager.js"></script>
	<script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxgrid.selection.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxdata.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxlistbox.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxdropdownlist.js"></script>
    <script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxgrid.columnsresize.js"></script>
  	<script type="text/javascript" src="' . base_path() . $jqwidgets_scripts_root .'jqxgrid.sort.js"></script>';
  }
  
  public static function exist($nid,$vid){
  	$sql = "SELECT *
  			FROM field_revision_body
			WHERE entity_id = $nid AND revision_id = $vid";
  	$result = db_query($sql);
  	
  	if($result->rowCount()==0)
  		return false;
  		
  	return true;
  }
  
  public static function get_deadline($nid) {
  	$sql = "SELECT field_deadline_value
		    FROM field_revision_field_deadline
			WHERE revision_id = (SELECT max(revision_id)
			 				     FROM field_revision_field_deadline
								 WHERE entity_id = $nid)";
    $result = db_query($sql)->fetch();
  	return $result->field_deadline_value;
  }
  
  public static function get_revision_index($nid,$vid) {
  	$sql = "SELECT *
  			FROM field_revision_body
			WHERE entity_id = $nid
			ORDER BY revision_id";
  	$result = db_query($sql);
  	$rev_count = $result->rowCount();

  	$arr = $result->fetchAll();
  	
  	foreach($arr as $key => $r) {
  		if($vid==$r->revision_id) {
  			$index = $key;
  			break;
  		}
  	}
  	
  	if($index > 0)
  		$prev_rev = $arr[$index-1]->revision_id;

  	if($index < $rev_count - 1)
  		$next_rev = $arr[$index+1]->revision_id;
  	
  	return array("rev_count" => $rev_count, "current" => $index + 1,
  	             "prev_rev" => $prev_rev, "next_rev" => $next_rev);
  }
  
  public static function get_latest_revision($nid)
  {
  	$sql =  "SELECT max(vid) vid
  			 FROM node_revision
  			 WHERE nid = $nid
  			";
  	$result = db_query($sql)->fetch();
	
  	return $result->vid;
  }

  public static function get_article_comments($nid,$vid)
  {
  	$sql =  "SELECT *
  			 FROM article_comments
  			 WHERE nid = $nid AND vid = $vid
  			 ORDER BY created
  			";
  	$result = db_query($sql)->fetchAll();

  	$ret = array();
	foreach($result as $r)
	{
		$user = user_load($r->uid);
		$ret[] = array('comment_id' => $r->comment_id, 'username' => $user->name, 'content' => $r->content, 'created_date' => date('Y-m-d H:i:s',$r->created));
	}
  	return $ret;
  }
  
  public static function remove_comment($comment_id)
  {
  	$sql =  "DELETE
  			 FROM {article_comments}
  			 WHERE comment_id = $comment_id
  			";
  	db_query($sql);
  }
  
  public function test() {
  	dpm('Is OepUtil working?');
  }
  
//   public static function update_deadline($vid, $new_deadline) {
//   	$sql = "update field_revision_field_deadline
// 			set field_deadline_value = $new_deadline
// 			where revision_id = $vid";
//   	db_query($sql);
//   }
}