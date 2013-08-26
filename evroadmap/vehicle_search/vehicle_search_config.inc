<?php 

function parse_options_array($options,$unit) {
	$display_arr = array();
	foreach ($options as $option) 
		$display_arr[$option['begin'].'_'.$option['end']] = number_format($option['begin']) . $unit . ' - ' . number_format($option['end']) . $unit; 
	return $display_arr;
}

function advanced_search_config() {
	$form = array();
	//vehicle range
	$form['vehicle_range'] = array(
		'#type' => 'fieldset',
		'#title' => t('Vehicle Range'),
		'#collapsible' => True,
		'#collapsed' => False,
	);
	
	$form['vehicle_range']['vr_checkboxes'] = array(
		'#type' => 'checkboxes',
		'#options' => parse_options_array(variable_get('vr_config_options', array()),'mi'),
		'#title' => t('Check the vehicle range options you want to remove'),
	);
	
	$form['vehicle_range']['vr_new_range_begin'] = array(
		'#type' => 'textfield',
		'#maxlength' => 3,
		'#size' => 3,
		'#title' => t('Begin Value'),
		'#prefix' => '<div style="padding-left:20px"> <label>Add new Vehicle Range value (in mile)</label>'
	);
	
	$form['vehicle_range']['vr_new_range_end'] = array(
		'#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
		'#title' => t('End Value'),
		'#suffix' => '</div>',
	);
	
	//price
	$form['price'] = array(
		'#type' => 'fieldset',
		'#title' => t('Price'),
		'#collapsible' => True,
		'#collapsed' => False,
	);
	
	$form['price']['p_less_than'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#maxlength' => 10,
		'#title' => t('Less than (in US dollar)'),
		'#default_value' => variable_get('p_config_less_than', ""),
	);
	
	$form['price']['p_more_than'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#maxlength' => 10,
		'#title' => t('More than (in US dollar)'),
		'#default_value' => variable_get('p_config_more_than', ""),
	);
	
	$form['price']['p_checkboxes'] = array(
		'#type' => 'checkboxes',
		'#options' => parse_options_array(variable_get('p_config_options', array()),'$'),
		'#title' => t('Check the interval vehicle price options you want to remove'),
	);
	
	$form['price']['p_new_range_begin'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#maxlength' => 10,
		'#title' => t('Begin Value'),
		'#prefix' => '<div style="padding-left:20px"> <label>Add new Price range (in US dollar)</label>'
	);
	
	$form['price']['p_new_range_end'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#maxlength' => 10,
		'#title' => t('End Value'),
		'#suffix' => '</div>',
	);	
	
	//top speed
	$form['top_speed'] = array(
		'#type' => 'fieldset',
		'#title' => t('Top Speed'),
		'#collapsible' => True,
		'#collapsed' => False,
	);
	
	$form['top_speed']['ts_less_than'] = array(
		'#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
		'#title' => t('Less than (in mph)'),
		'#default_value' => variable_get('ts_config_less_than', ""),
	);
	
	$form['top_speed']['ts_more_than'] = array(
		'#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
		'#title' => t('More than (in mph)'),
		'#default_value' => variable_get('ts_config_more_than', ""),
	);
	
	$form['top_speed']['ts_checkboxes'] = array(
		'#type' => 'checkboxes',
		'#options' => parse_options_array(variable_get('ts_config_options', array()),'mph'),
		'#title' => t('Check the interval vehicle top speed options you want to remove'),
	);
	
	$form['top_speed']['ts_new_range_begin'] = array(
		'#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
		'#title' => t('Begin Value'),
		'#prefix' => '<div style="padding-left:20px"> <label>Add new Top Speed range (in mph)</label>'
	);
	
	$form['top_speed']['ts_new_range_end'] = array(
		'#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
		'#title' => t('End Value'),
		'#suffix' => '</div>',
	);
	
	$form['#submit'][] = 'advanced_search_config_submit';
	$form['#validate'][] = 'advanced_search_config_validate';
	return system_settings_form($form);
}

function advanced_search_config_validate($form, &$form_state) {
	//check vehicle range
	if($form_state['values']['vr_new_range_begin']!='' || $form_state['values']['vr_new_range_end']!='')
		if(!vr_new_range_ok($form_state)) {
		form_set_error('vr_new_range_begin','Invalid vehicle range. Make sure the range contains only positive integers, begin value < end value and doesn\'t overlap other ranges');
		form_set_error('vr_new_range_end','Invalid vehicle range. Make sure the range contains only positive integers, begin value < end value and doesn\'t overlap other ranges');
		return;
	}
	//check price
	if(!check_input_values($form_state,'p'))
		return;
	//check top speed
	if(!check_input_values($form_state,'ts'))
		return;
}

function check_input_values($form_state, $prefix){
	if(!$form_state['values'][$prefix.'_less_than']) {
		form_set_error($prefix .'_less_than','This field must be non-empty');
		return false;
	}
	
	if(!$form_state['values'][$prefix.'_more_than']) {
		form_set_error($prefix.'_more_than','This field must be non-empty');
		return false;
	}
	
	$less_than = $form_state['values'][$prefix.'_less_than'];
	$more_than = $form_state['values'][$prefix.'_more_than'];
	if(!ctype_digit($less_than) || $less_than <0) {
		form_set_error($prefix.'_less_than','Invalid Less than value. This value must be a non-negative integer.');
		return false;
	}
	
	if(!ctype_digit($more_than) || $more_than <0) {
		form_set_error($prefix.'_more_than','Invalid More than value. This value must be a non-negative integer.');
		return false;
	}
		
	if($less_than >= $more_than) {
		form_set_error($prefix.'_more_than','More than value must be bigger than Less than value');
		return false;
	}
	
	if($less_than > min_range($prefix.'_config_options')) {
		form_set_error($prefix.'_less_than','Invalid value');
		return false;
	}
	
	if($more_than < max_range($prefix.'_config_options')) {
		form_set_error($prefix.'_more_than','Invalid value');
		return false;
	}
	
	if($form_state['values'][$prefix.'_new_range_begin']!='' || $form_state['values'][$prefix.'_new_range_end']!='')
		if(!new_range_ok($form_state,$prefix)) {
		form_set_error($prefix.'_new_range_begin','Invalid range. Make sure the range contains only positive integers, begin value < end value and doesn\'t overlap other ranges');
		form_set_error($prefix.'_new_range_end','Invalid range. Make sure the range contains only positive integers, begin value < end value and doesn\'t overlap other ranges');
		return false;
	}
	return true;
}



function vr_new_range_ok($form_state) {
	$begin_val = $form_state['values']['vr_new_range_begin'];
	$end_val = $form_state['values']['vr_new_range_end'];

	if(ctype_digit($begin_val) && $begin_val >= 0) {
		if(ctype_digit($end_val) && $end_val > $begin_val) {
			//check for overlapping with other segments
			$vr_config_options = variable_get('vr_config_options', array());
			foreach($vr_config_options as $vr_config_option) {
				$begin = $vr_config_option['begin'];
				$end = $vr_config_option['end'];
				if(!($end_val<=$begin) && !($begin_val>=$end))
					return False;
			}
			return True;
		}
	}
	return False;
}

function min_range($config_name) {
	$min_val = PHP_INT_MAX;
	$config_options = variable_get($config_name, array());
	foreach($config_options as $config_option) {
		$begin = $config_option['begin'];
		if($min_val > $begin)
			$min_val = $begin;
	}
	return $min_val;
}

function max_range($config_name) {
	$max_val = 0;
	$config_options = variable_get($config_name, array());
	foreach($config_options as $config_option) {
		$end = $config_option['end'];
		if($max_val < $end)
			$max_val = $end;
	}
	return $max_val;
}

function new_range_ok($form_state, $prefix) {
	$begin_val = $form_state['values'][$prefix . '_new_range_begin'];
	$end_val = $form_state['values'][$prefix . '_new_range_end'];

	if(ctype_digit($begin_val) && $begin_val >= 0) {
		if(ctype_digit($end_val) && $end_val > $begin_val) {
			//check for overlapping with other segments
			$config_options = variable_get($prefix.'_config_options', array());
			foreach($config_options as $config_option) {
				$begin = $config_option['begin'];
				$end = $config_option['end'];
				if(!($end_val<=$begin) && !($begin_val>=$end))
					return False;
			}
			
			$less_than = $form_state['values'][$prefix.'_less_than'];
			$more_than = $form_state['values'][$prefix.'_more_than'];
			if(!($begin_val>=$less_than) || !($end_val<=$more_than))
				return False;
			return True;
		}
	}
	return False;
}

function range_cmp($a,$b) {
	 if ($a['begin'] == $b['begin']) {
        return 0;
    }
    return ($a['begin'] < $b['begin']) ? -1 : 1;
}

function arrange($options) {
	uasort($options, 'range_cmp');
	return $options;
}

function remove_ranges($form_state,$prefix) {
	foreach($form_state['values'][$prefix.'_checkboxes'] as $range_val) 
		if($range_val !=0 )
		{
			$range_val_arr = explode('_',$range_val);
			$begin = $range_val_arr[0];
			$end = $range_val_arr[1];
			$config_options = variable_get($prefix.'_config_options', array());
			//remove the range(begin,end) from $config_options
			foreach($config_options as $key => $val)
				if($val['begin'] == $begin && $val['end'] == $end) {
					unset($config_options[$key]);
					variable_set($prefix.'_config_options',$config_options);
					break;
				}
		}
}

function advanced_search_config_submit($form, &$form_state) {
	variable_set("p_config_less_than", $form_state['values']['p_less_than']);
	variable_set("p_config_more_than", $form_state['values']['p_more_than']);
	variable_set("ts_config_less_than", $form_state['values']['ts_less_than']);
	variable_set("ts_config_more_than", $form_state['values']['ts_more_than']);
	if($form_state['values']['vr_new_range_begin']!='') {
		$vr_config_options = variable_get('vr_config_options', array());
		$vr_config_options[] = array('begin' => $form_state['values']['vr_new_range_begin'],
								 	 'end' => $form_state['values']['vr_new_range_end']);
		variable_set('vr_config_options', arrange($vr_config_options));
	}
	if($form_state['values']['p_new_range_begin']!='') {
		$p_config_options = variable_get('p_config_options', array());
		$p_config_options[] = array('begin' => $form_state['values']['p_new_range_begin'],
				'end' => $form_state['values']['p_new_range_end']);
		variable_set('p_config_options', arrange($p_config_options));
	}
	if($form_state['values']['ts_new_range_begin']!='') {
		$ts_config_options = variable_get('ts_config_options', array());
		$ts_config_options[] = array('begin' => $form_state['values']['ts_new_range_begin'],
									 'end' => $form_state['values']['ts_new_range_end']);
		variable_set('ts_config_options', arrange($ts_config_options));
	}
	
	//remove vr ranges
	remove_ranges($form_state,'vr');
	remove_ranges($form_state,'p');
	remove_ranges($form_state,'ts');
}