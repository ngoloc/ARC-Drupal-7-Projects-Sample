<?php
include_once "group.inc";

function groupedit() {
           header('Content-type: text/plain; charset=UTF-8');
           header('Content-Disposition: inline');
           global $user;
           if (in_array('administrator', array_values($user->roles)) || in_array('editorial coordinator', array_values($user->roles))) {
             if(isset($_GET['gr'])){
               $group_id = $_GET['gr'];
               $mGroup = new Group();
               if($mGroup->get_groupname($group_id)=='')
                 return "group not found";
               $html_string  = '<form name="gr_edit" action="group_list" method="post">' 
                              .'<h2>Group ' . $mGroup->get_groupname($group_id) . '</h2>'
                              .'<table border="1">'
                              .'<tr>'
                              .'<th>Users</th>'
                              .'<th>Group Member</th>'
                              .'</tr>';
               $mUser = new User();
               $users = $mUser->get_editorial_users();
               foreach ($users as $u) {
               	 if($mGroup->contains($u['uid'],$group_id)){
               	   $html_string .=  '<tr>'
                                   .'<td>' . $u['name'] . '</td>'
                                   .'<td><input name="user[]" type="checkbox" value="' . $u['uid'] . '" checked="checked"/></td>'
                                   .'</tr>';
               	 }
               	 else {
               	   $html_string .=  '<tr>'
                                   .'<td>' . $u['name'] . '</td>'
                                   .'<td><input name="user[]" type="checkbox" value="' . $u['uid'] . '"/></td>'
                                   .'</tr>';
               	 }
               }
               $html_string .= '</table>'
                              .'<input class="form-submit" type="submit" value="Apply" />'
                              .'<input type="hidden" name="gr_edit_h" value="' . $group_id . '">'
                              .'</form>';
               return $html_string;
             }
           } else {
               return "Unauthorized User";
             }
  
         }

