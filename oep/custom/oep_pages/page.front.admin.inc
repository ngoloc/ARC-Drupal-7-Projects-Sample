<?php
function pages_front_admin(){
drupal_add_js("sites/all/modules/custom/oep_pages/rowcollapse.js");
$output =  "<h1 style= 'text-align:center'>Front Page Content Management</h1>";

//First Table//////
$output .= "<h2>Featured Article Slideshow</h2>";

//slideshow title
$headers = array(
	t('Slideshow Title'), "",);

$sql = "select tid, name from taxonomy_term_data where vid = 9";

$result = db_query($sql);
$slidetitle = array();
foreach ($result as $r){;
$slidetitle[] = $r;}

//render table
$rows = array();

foreach($slidetitle as $s){
        $title = $s->name;
        $tid = $s->tid;

       $rows[] = array(
                 t($title),
                 l(t('Edit'), "taxonomy/term/110/edit"),
        );
}

$output .= theme('table', array('header' => $headers, 'rows' => $rows) );

$output .= "<br>";

//featured articles
//headers
$headers = array(
      t('Featured Article Title'),
      t('Images'),
      t('View'),
      t('Featured'),
    );


$sql = "select node.nid, uri, node.title from node 
        join field_data_field_featured fdff 
        on node.nid = fdff.entity_id 
        left join field_data_field_media fdfm
        on fdfm.entity_id = node.nid
        left join file_managed 
        on field_media_fid = file_managed.fid 
        where field_featured_value = 1";

$result = db_query($sql);
$featured = array();
foreach ($result as $r){
$featured[] = $r;}

//render table
$rows = array();
foreach($featured as $f){
        $nid = $f->nid;
        $title = $f->title;
        $uri = $f->uri;      
        if($uri) $uri = "<img height= '60' width= '60' src='" . file_create_url($uri) . "'></img> ";
        
        $rows[] = array(
        	t($title),
        	t($uri),
        	l(t('View'), "node/$nid/view"),
        	"<input value= $nid checked='yes' type='checkbox''></input>",
        );
}



$output .= theme('table', array('header' => $headers, 'rows' => $rows) );

$output .= l(t('Click to add more Featured Articles'), "mypages/by-state/published", array('attributes' => array('style' => "float:right; padding: 5px"))) . "<br>";

/**********************
//*Second Table */
$output .= "<h2>Front Page Images and Captions</h2>"; 
//footer sql
$sql = "select uri, nid, title 
        from node join field_data_field_sponsor_image fdfsi 
        on node.nid = fdfsi.entity_id 
        join file_managed 
        on fdfsi.field_sponsor_image_fid = file_managed.fid 
        where node.type = 'footer_sponsors_display'"; 
$result = db_query($sql);
$footer = array();
foreach ($result as $r){;
$footer[] = $r;}

//posters sql
$sql = "select uri, nid, title 
        from node join field_data_field_poster_image fdfpi 
        on node.nid = fdfpi.entity_id 
        join file_managed 
        on fdfpi.field_poster_image_fid = file_managed.fid 
        where node.type = 'event_posters_display'";

$result = db_query($sql);
$poster = array();
foreach ($result as $r){;
$poster[] = $r;}

//awards image sql
$sql = "select uri, nid, title 
        from node join field_data_field_sponsor_image fdfsi 
        on node.nid = fdfsi.entity_id 
        join file_managed 
        on fdfsi.field_sponsor_image_fid = file_managed.fid 
        where node.type = 'footer_sponsors_display'";
$result = db_query($sql);
$awards = array();
foreach ($result as $r){;
$awards[] = $r;}

//calendar donation ans sidebar image  sql
$sql = "select uri, nid, title 
        from node join field_data_field_calendar_image fdfci 
        on node.nid = fdfci.entity_id  
        join file_managed 
        on fdfci.field_calendar_image_fid = file_managed.fid 
        where node.type = 'calendar_image'";
$result = db_query($sql);
$content = array();
foreach ($result as $r){
$content[] = $r;}

$sql = "select uri, nid, title 
        from node join field_data_field_donate_image fdfdi 
        on node.nid = fdfdi.entity_id
        join file_managed
        on fdfdi.field_donate_image_fid = file_managed.fid
        where node.type = 'donate_image'";
$result = db_query($sql);
foreach ($result as $r){
$content[] = $r;}

$sql =	"select uri, nid, title          
	from node join field_data_field_sidebar_image fdfsi          
	on node.nid = fdfsi.entity_id           
	join file_managed          
	on fdfsi.field_sidebar_image_fid = file_managed.fid          
	where node.type = 'sidebar_image'";
$result = db_query($sql);
foreach ($result as $r){
$content[] = $r;}



//awards sql
$sql = "select uri, nid, title 
        from node join field_data_field_award_image fdfai 
        on node.nid = fdfai.entity_id 
        join file_managed 
        on fdfai.field_award_image_fid = file_managed.fid 
        where node.type = 'award'";
$result = db_query($sql);
$awards = array();
foreach ($result as $r){;
$awards[] = $r;}


//headers
$headers = array(
      t('Section/Title'), 
      t('Images'),  
      t('Edit'),        
    );
$rows = array();


//render posters row
$imgpath = array();
$newpath ="";

foreach($poster as $p){
       $imgpath[] = $p->uri;
       }
foreach ($imgpath as $img){
       $newpath .= "<img src='" . image_style_url('thumbnail', $img) . "'></img> ";
       }

 $title = $poster[0]->title;
 $nid = $poster[0]->nid;
 $rows[] = array(
        t($title),
        $newpath,
        l(t("Edit"), "node/$nid/edit"),
        );

//render footer row
$newpath = "";
$imgpath = array();

foreach ($footer as $c){
         $imgpath[] = $c->uri;
        }
foreach ($imgpath as $img){
        $newpath .= "<img src='" . image_style_url('thumbnail', $img) . "'></img> ";
        }
 $nid = $footer[0]->nid;
 $title = $footer[0]->title;
 $rows[] = array(
          t($title),
          $newpath,
          l(t("Edit"), "node/$nid/edit"), 
          );

//render calendar adn donate content
 $newpath = "";
//$imgpath = array();

 foreach ($content as $c){
         $newpath = $c->uri;
         //  $newpath = "<img height= '60' width= '80' src='" . image_style_url('thumbnail', $newpath) . "'></img>";
         $newpath = "<img height= '60' width= '80' src='" . file_create_url($newpath) . "'></img> ";
         $title = $c->title;
         $nid = $c ->nid;
         $rows[] = array(
                   t("$title"),
                   $newpath,
                   l(t("Edit"), "node/$nid/edit"),
                  );
 }

//render awards
 $newpath = "";//$imgpath = array();
 $rows[] =array(
         'data' => array(
         t("Awards Images and Content. Click to Expand"),
         t("Click to Expand"),
         "",),'class'=>array('toggle'),);

foreach ($awards as $a){
         $newpath = $a->uri;
         $newpath = file_create_url($newpath);
         //$newpath = "<img height= '60' width= '80' src='" . image_style_url('thumbnail', $newpath) . "'></img>"; 
         $newpath = "<img height= '60' width= '80' src='" . $newpath . "'></img> ";
         $title = $a->title;
         $nid = $a ->nid;
         $rows[] = array(
                  'data' => array(
                  t("$title"),
                  $newpath,
                  l(t("Edit"), "node/$nid/edit"),          ), 'class' => array("collapsible"));
}


$output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'front_content_table') ));

return $output;
}

//function oep_pages_menu() {
 // $items['mypages/featured_article'] = array(
   // 'page callback' => 'featured_article_update_ajax',
   // 'type' => MENU_CALLBACK,
//  );
 // return $items;
//}
  
function featured_article_update_ajax() {
//  $photo = mymodule_get_photo($nid); // returns the filepath of my photo
//  $image = theme('image', $photo);
 // drupal_json(array('status' => 0, 'data' => $image));
 $nid = $_POST['nid'];
 $checked = $_POST['checked'];
if($checked == 'true'){
$result = db_update('field_data_field_featured') 
                   ->fields(array('field_featured_value' => 1,))
                   ->condition('entity_id', $nid)
                   ->execute();

}
 //$response = "<div id='response'>Checked</div>";
else {
$result = db_update('field_data_field_featured') 
                   ->fields(array('field_featured_value' => 0,))
                   ->condition('entity_id', $nid)
                   ->execute();
}
//$response = "<div id='response'>Unchecked</div>";
 //return $response;
 exit;

} 

?>
