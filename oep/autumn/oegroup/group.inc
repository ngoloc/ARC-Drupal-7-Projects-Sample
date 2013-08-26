<?php

class Group {
  private $dbh;

  public function __construct() {
           $this->dbh =  new PDO("mysql:host=mysql.rc.pdx.edu;dbname=oehc_dev", "oehc_l", "CofjedsOk0");
         }

  public function insert_group($groupname){
           // check if this entry already existed in the table
           $stmt = $this->dbh->prepare('SELECT *
                                        FROM groups
                                        WHERE group_name = :groupname');
           $stmt->bindParam(':groupname', $groupname);
           $stmt->execute();
           if($stmt->rowCount()>=1)
             return false;
           $stmt = $this->dbh->prepare('INSERT INTO groups
                                        VALUES(\'\',:groupname)');
           $stmt->bindParam(':groupname', $groupname);
           $stmt->execute();
           return true;
         }
  
  public function delete_group($group_id){
  		   //check if this entry already existed in the table
           $stmt = $this->dbh->prepare('DELETE
                                        FROM user_group
                                        WHERE group_id = :group_id');
           
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
           
           $stmt = $this->dbh->prepare('DELETE
                                        FROM groups
                                        WHERE group_id = :group_id');
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
  }
         
  public function get_groupname($group_id){
           $stmt = $this->dbh->prepare('SELECT *
                                        FROM groups
                                        WHERE group_id = :group_id');
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
           
           if($stmt->rowCount()==0)
             return '';
           else {
             $gr_arr = $stmt->fetchAll();
             return $gr_arr[0]['group_name'];
           }
         }

  public function get_groups(){
           $stmt = $this->dbh->prepare('SELECT *
                                        FROM groups
           								ORDER BY group_name');
           $stmt->execute();
           return $stmt->fetchAll();
         }
  public function insertUserToGroup($uid, $group_id){
           $stmt = $this->dbh->prepare('INSERT INTO user_group
                                        VALUES(:uid,:group_id)');
           $stmt->bindParam(':uid', $uid);
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
         }

  public function removeUserFromGroup($uid, $group_id){
           $stmt = $this->dbh->prepare('Delete from user_group
                                        WHERE uid = :uid AND group_id = :group_id');
           $stmt->bindParam(':uid', $uid);
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
         }
  public function contains($uid, $group_id){
         $stmt = $this->dbh->prepare('SELECT *
                                      FROM user_group
                                      WHERE uid = :uid AND group_id = :group_id');
         $stmt->bindParam(':uid', $uid);
         $stmt->bindParam(':group_id', $group_id);
         $stmt->execute();
         if($stmt->rowCount()>=1)
           return true;
         return false;
  }
}

class User {
  private $dbh;
  public function __construct() {
           $this->dbh =  new PDO("mysql:host=mysql.rc.pdx.edu;dbname=oehc_dev", "oehc_l", "CofjedsOk0");
         }

  public function get_editorial_users(){
           $stmt = $this->dbh->prepare('SELECT DISTINCT u.uid, u.name
                                        FROM users u, users_roles ur, role r
                                        WHERE u.uid = ur.uid AND ur.rid = r.rid AND (r.name = "administrator" OR r.name = "editorial coordinator" OR r.name= "editor")
           								ORDER BY u.name');
           $stmt->execute();
           return $stmt->fetchAll();
         }
  public function getAllUsersName($group_id){
           $stmt = $this->dbh->prepare('SELECT *
                                        FROM users u,user_group ug
                                        WHERE group_id = :group_id AND u.uid = ug.uid
           								ORDER BY u.name');
           $stmt->bindParam(':group_id', $group_id);
           $stmt->execute();
           $ugs = $stmt->fetchAll();
           $users = array();
           foreach ($ugs as $ug){
             array_push($users, $ug['name']);
           }
           return $users;
         }
}
