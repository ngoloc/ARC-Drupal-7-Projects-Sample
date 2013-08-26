function submit_assigned_reviewers(nid) {
;
	$.ajax({
		type: "POST",
		url: ajax_url, 
		dataType: 'json',
		data: {
		  "func_no" : 35,
		  "nid" : nid,
		},
		success: function (data) {
					var reviewers_str = '<ul>';
					for (key in data['reviewers'])
						reviewers_str += '<li>' + data['reviewers'][key] + '</li>';
					reviewers_str += '</ul>';
					
					var fact_checkers_str = '<ul>';
					for (key in data['fact_checkers'])
						fact_checkers_str += '<li>' + data['fact_checkers'][key] + '</li>';
					fact_checkers_str += '</ul>';
					
					var reviewers_number = data['reviews_number'];
					$("#submit_dialog").html( '<h3 style="text-align:center">Review</h3>'
					 		  				 +'<p><b>Reviewers:</b></p>'
					 		  				 +reviewers_str
					 		  				 +'<p><b>Factcheckers:</b></p>'
					 		  				 +fact_checkers_str
											 +'<p><b>Number of Reviews: ' + reviewers_number +'</b></p>'
											 +'<p><a href="../article/' + nid +'/approve">Click here to reassign reviewers</p>');
					
					$("#submit_dialog").dialog({buttons: { "Submit": function() {
						
																			$.ajax({
																				type: "POST",
																				url: ajax_url,
																				data: {
																				  "func_no" : 36,
																				  "nid" : nid,
																				},
																				dataType: 'text',
																				success: function (data) {
																							if(data == '') {
																								$("#submit_dialog").dialog("close");
																								window.location.reload();
																							}
																							else
																								$("#submit_dialog").append('<p style="color: red">' + data +'</p>');
																				         },
																				error: function (xmlhttp) {
																							alert("An error occured: " + xmlhttp.status);
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