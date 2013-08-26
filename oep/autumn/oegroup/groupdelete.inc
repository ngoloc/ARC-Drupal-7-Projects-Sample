<?php
//@file: most of this is based on loc's other group module files---conwayb 
function groupdelete(){
header('Content-type: text/plain; charset=UTF-8');
header('Content-Disposition: inline');

if(isset($_GET['gr'])){
$group_id = $_GET['gr'];
$mGroup = new Group();
if($mGroup->get_groupname($group_id)=='')
   return "group not found";
$mUser = new User();

$html_string = "<div><h1>Delete Group:&nbsp";
$html_string .= "<span style='font-size: 1.2em;'>" . $mGroup->get_groupname($group_id) . "?</span></h1></div>";
$html_string .= "<form name = 'gr_delete_form' action = 'group_list' method= 'POST'>"
                . "<table id='gr_delete_table'>"
	        ."<thead>"
		
                ."<th>Users</th></thead>";	 

               $g_users = $mUser->getAllUsersName($group_id);
               foreach($g_users as $gu){
               $html_string .= "<tr><td>" . $gu  . "<br></td></tr>";
                            }
               $html_string .= "</table>"
                             . "<input class= 'form-submit' type='submit' value= 'Delete'>"
                              ."<input type='hidden' name='group_id_delete' value='". $group_id . "'></form>";
        

return $html_string;
}
};
