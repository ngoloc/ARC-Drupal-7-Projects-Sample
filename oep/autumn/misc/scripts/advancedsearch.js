function index_span_onclick(element){
	$("#submit_bool_hid").val(0);
	$("#page_hid").val(element.innerHTML);
	$("#advanced_search_form").submit();
}


function prev_sheet(){
	var prev_index = parseInt($("#index_span_1").html())-max_pages_per_sheet;
	$("#submit_bool_hid").val(0);
	$("#page_hid").val(prev_index);
	$("#advanced_search_form").submit();
}

function next_sheet(){
	var next_index = parseInt($("#index_span_"+max_pages_per_sheet).html())+1;
	$("#submit_bool_hid").val(0);
	$("#page_hid").val(next_index);
	$("#advanced_search_form").submit();
}

function advanced_search_submit(){
	$("#submit_bool_hid").val(1);
	$("#page_hid").val(1);
	$("#advanced_search_form").submit();
}

$(document).ready(function() {
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
		  "func_no" : 19,
		},
		dataType: "json",
		success: function (data) {
					
					$("input#search_author").autocomplete({
				    	source: data,
					});
		         },
		         error: function (xmlhttp) {
		           //alert("An error occured: " + xmlhttp.status);
		         }
	});
});