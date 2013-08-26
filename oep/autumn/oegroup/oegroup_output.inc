<?php
include_once "group.inc";
drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
drupal_add_css(drupal_get_path('module', 'misc') . '/css/table/gray.css', 'file');
/*
 * Display output
*/
function oegroup_output() {
		   
           header('Content-type: text/plain; charset=UTF-8');
           header('Content-Disposition: inline');
           global $user;
           $mUser = new User();
           $mGroup = new Group();
		   if(isset($_POST['group_id_delete']))
		   {
		   		$groud_id = $_POST["group_id_delete"];
		   		$group_name = $mGroup->get_groupname($groud_id);
		   		$mGroup->delete_group($groud_id);
		   		drupal_set_message('Group "' . $group_name . '" was deleted');
		   }
		   
           if (in_array('administrator', array_values($user->roles)) || in_array('editorial coordinator', array_values($user->roles))) {
             //check if post method has been sent
             if(isset($_POST['gr_edit_h'])&&isset($_POST['user']))
             {
               $group_id = $_POST['gr_edit_h'];
               $users = $mUser->get_editorial_users();
               
	           foreach ($users as $u) {
	             //First remove the user
	             $mGroup->removeUserFromGroup($u['uid'], $group_id);
	           }
	           
	           foreach($_POST['user'] as $uid){
	             //Add the user if the user is currently in the group
                 $mGroup->insertUserToGroup($uid, $group_id);
	           }
             }

             $groups = $mGroup->get_groups();
             $html_string =  '<table class="sticky-enabled" id="group_table"  border="1">'
                            .'<thead>'
			    .'<tr>'
                            .'<th>Group</th>'
                            .'<th>Users</th>'
                            .'<th>Edit</th>'
			    .'<th>Delete</th>'
                            .'</tr>'
                            .'</thead>';
             foreach ($groups as $group) {
               $g_users = $mUser->getAllUsersName($group['group_id']);
               $users_string = '';
               foreach($g_users as $gu)
                 $users_string .= $gu . '<br/>';
               $html_string .=  '<tr>'
                               .'<td>' . $group['group_name'] . '</td>'
                               .'<td>' . $users_string . '</td>'
                               .'<td><a href="http://www.research.pdx.edu/dev/oep/mypages/group_edit?gr=' . $group['group_id'] . '">edit</a></td>'
                               .'<td><a href="http://www.research.pdx.edu/dev/oep/mypages/group_delete?gr=' . $group['group_id'] . '">delete</a></td>'
                               .'</tr>';
             }
             $html_string .= '</table>';
             $html_string .= '<a href="http://www.research.pdx.edu/dev/oep/mypages/group_add">add a new group</a>';
             $html_string .= '<script>
  				jQuery(document).ready(function() {
				        jQuery("#group_table").tablesorter(
                                        {sortList: [[0,0]],
                                        headers: {1: {sorter: false}, 2: {sorter: false}, 3: {sorter: false} }
					});
				}); 
				   
  	 		     </script>';
             return $html_string;
           } else {
               return "Unauthorized User";
             }
         }

