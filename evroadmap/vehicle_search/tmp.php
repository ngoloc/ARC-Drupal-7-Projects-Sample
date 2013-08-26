<?php
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
$vrs = variable_get('vr_config_options', array());
// $connector_type_options = get_cck_field_options('node','field_connector_type','vehicle');
// unset($connector_type_options['_none']);
// unset($connector_type_options['other']);
// var_dump($connector_type_options);
$src_str = '/sites/all/modules/custom/vehicle_search';
?>

<link rel="StyleSheet" href="<?php echo $src_str;?>/css/vehicle_simple_search.css" type="text/css">
<script src="<?php echo $src_str;?>/scripts/json2.js"></script>
<script>
var ajax_callback_str = "<?php echo base_path();?>simple-ev-ajax-callback";
</script>
<script src="<?php echo $src_str;?>/scripts/vehicle_simple_search.js"></script>

<form id="ev_simple_search" autocomplete="off">
	<div style="float: left;">
		<div id="toggle_ev" class="toggle_off" onclick="ev_onclick()">
		100% electric
		</div>
		<select id="battery_range_sel" style="margin-top:10px">
			<option value="title">Vehicle Range</option>
			<?php 
			foreach($vrs as $vr)
				echo '<option value="' . $vr['begin'] . '_' . $vr['end'] . '">' . $vr['begin'] . 'mi - ' . $vr['end'] . 'mi</option>';
			?>
		</select>
	</div>
	<div style="float: left; margin-left:10px;">
		<div id="toggle_phev" class="toggle_off" onclick="phev_onclick()">
		Plugin Hybrid
		</div>
		<select id="vehicle_type_sel" style="margin-top:10px">
			<option value="title">Vehicle Type</option>
		</select>
	</div>
	<div style="float: left; margin-left:10px;">
		<div id="toggle_hev" class="toggle_off" onclick="hev_onclick()">
		Hybrid
		</div>
		<select id="connector_type_sel" style="margin-top:10px">
			<option value="title">Connector Type</option>
		</select>
	</div>
	<div style="float: left; width:350px; margin-top:10px;">
		<input type="submit" value="Search" />&nbsp<a href="/vehicle-advanced-search" style="font-size:0.8em;">Advanced Search >>></a>
		<input type="hidden" id="vehicle_type" value="[]" autocomplete="off">
	</div>
</form>