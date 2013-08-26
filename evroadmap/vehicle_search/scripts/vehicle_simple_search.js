function ev_onclick(){
(function ($) {
	var arr = JSON.parse($("#vehicle_type").val());
	
	if($('#toggle_ev').attr('class') == 'toggle_on') {
		for (key in arr){
			if(arr[key] == '100% electric')
			{
				arr.splice(key,1);
				break;
			}
		}
		
		$('#toggle_ev').attr('class','toggle_off');	
	}
	else {
		arr.push('100% electric');
		$('#toggle_ev').attr('class','toggle_on');
	}
	$("#vehicle_type").val(JSON.stringify(arr));
}(jQuery))
}

function phev_onclick(){
(function ($) {
	var arr = JSON.parse($("#vehicle_type").val());
	if($('#toggle_phev').attr('class') == 'toggle_on') {
		for (key in arr){
			if(arr[key] == 'plugin hybrid')
			{
				arr.splice(key,1);
				break;
			}
		}
		
		$('#toggle_phev').attr('class','toggle_off');	
	}
	else {
		arr.push('plugin hybrid');
		$('#toggle_phev').attr('class','toggle_on');
	}
	$("#vehicle_type").val(JSON.stringify(arr));
}(jQuery))
}

function hev_onclick(){
(function ($) {
	var arr = JSON.parse($("#vehicle_type").val());
	if($('#toggle_hev').attr('class') == 'toggle_on') {
		for (key in arr){
			if(arr[key] == 'hybrid')
			{
				arr.splice(key,1);
				break;
			}
		}
		
		$('#toggle_hev').attr('class','toggle_off');	
	}
	else {
		arr.push('hybrid');
		$('#toggle_hev').attr('class','toggle_on');
	}
	$("#vehicle_type").val(JSON.stringify(arr));
}(jQuery))
}

function simple_search() {
(function ($) {
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 1,
			"vehicle_type" : $('#vehicle_type').val(),
			"battery_range" : $('#battery_range_sel').val(),
			"vehicle_class" : $('#vehicle_class_sel').val(),
			"connector_type" : $('#connector_type_sel').val(),
		},
		dataType: "text",
		success: function (redirect) {
					window.location.href = redirect;
				},
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
		       }
	});
	
}(jQuery))
}
