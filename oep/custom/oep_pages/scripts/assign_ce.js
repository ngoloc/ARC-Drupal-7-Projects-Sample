function assign_ce(nid)
{
	$.ajax({
			type: "POST",
			url: ajax_url,
			data: {
			  "func_no" : 32
			},
			dataType: "json",
			success: function (data) {
						var select_ces = "";
					    for (key in data)
					    	select_ces += '<option value="' + data[key]['uid'] + '">' + data[key]["name"] + ' - ' + data[key]["first_name"] + ' ' + data[key]["last_name"] + '</option>';

						$("#dialog").html('Choose a copy editor:&nbsp<select id="user_select">' + select_ces + '</select>');
						
						$("#dialog").dialog({buttons: { "Assign": function() {
																	var uid = $("#user_select option:selected").val();
																	$.ajax({
																		type: "POST",
																		url: ajax_url,
																		data: {
																		  "func_no" : 33,
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

function submit_to_editorial_coordinator(nid) {
	$("#notice_dialog").html('Do you want to submit to editorial coodinator?');
	$("#notice_dialog").dialog({buttons: { "OK": function() {
													$.ajax({
														type: "POST",
														url: ajax_url,
														data: {
														  "func_no" : 34,
														  "nid" : nid,
														},
														success: function () {
																	$("#notice_dialog").dialog("close");
																	window.location.reload();
														         },
														error: function (xmlhttp) {
	//																		           alert("An error occured: " + xmlhttp.status);
														         }
													});
										  		 },
										   "Cancel": function() {
												 		$(this).dialog("close");
												      },
								   		},
							  modal:true,
					   		});
}

