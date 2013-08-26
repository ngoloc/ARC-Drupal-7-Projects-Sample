<?php

function groupaf() {
        global $user;
        if (in_array('administrator', array_values($user->roles)) || in_array('editorial coordinator', array_values($user->roles))) {
          $form['group_name'] = array(
          '#type' => 'textfield',
          '#title' => t('Group Name'),
          '#size' => 25,
          );
        
          $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('add editorial review group'),
          );
        } else {
            $form['access_error'] = array(
	          '#prefix' => '<p>Unautherized User</p>',
            );            
          }
        return $form;
}

function groupaf_submit($form, $form_state) {
        $group_name = $form_state['values']['group_name'];
        $mGroup = new Group();
        if($mGroup->insert_group($group_name)){
          drupal_goto('mypages/group_list');
        }
        else
          form_set_error('group_name', t('Group Name already exists'));
}

/**
* Validate Group Name submission.
*/
function groupaf_validate($form, &$form_state) {
        $group_name = $form_state['values']['group_name'];
        if (trim($group_name)=='') {
                form_set_error('group_name', t('Group Name cannot be blank.'));
        }
}
