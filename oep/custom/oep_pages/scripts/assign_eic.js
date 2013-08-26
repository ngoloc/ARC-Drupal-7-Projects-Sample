function assign_eic(nid)
{
	$.ajax({
			type: "POST",
			url: ajax_url,
			data: {
			  "func_no" : 16
			},
			dataType: "json",
			success: function (data) {
						var select_eics = "";
					    for (key in data)
					    	select_eics += '<option value="' + data[key]['uid'] + '">' + data[key]["name"] + ' - ' + data[key]["first_name"] + ' ' + data[key]["last_name"] + '</option>';

						$("#dialog").html('Choose an editor in chief:&nbsp<select id="user_select">' + select_eics + '</select>');
						
						$("#dialog").dialog({buttons: { "Assign": function() {
																	var uid = $("#user_select option:selected").val();
																	$.ajax({
																		type: "POST",
																		url: ajax_url,
																		data: {
																		  "func_no" : 27,
																		  "nid" : nid,
																		  "uid" : uid,
																		},
																		success: function () {
																					$("#dialog").dialog("close");
																					window.location.reload();
																		         },
																		error: function (xmlhttp) {
										//								           alert("An error occured: " + xmlhttp.status);
																		         }
																	});
																	
															 	  },
													    "Cancel": function() {
															 		$(this).dialog("close");
															 },
													  },
											 modal:true,
											});
			         },
			error: function (xmlhttp) {
			           alert("An error occured: " + xmlhttp.status);
			         }
		});
}