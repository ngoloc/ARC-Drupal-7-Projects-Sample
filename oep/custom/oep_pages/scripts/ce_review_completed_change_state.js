function change_state(nid)
{
	$("#change_state_dialog").html( '<div class="form-item form-type-radio" style="font-size : 110%;">'
								   +'	<input type="radio" class="form-radio" value="assigned_entry" name="moderation_state" id="assigned_entry" checked><label for="assigned_entry" class="option" style="color:red">Assigned Entry</label><br>'
								   +'	<input type="radio" class="form-radio" value="needs_review" name="moderation_state" id="needs_review"><label for="needs_review" class="option" style="color:red">Needs Review</label><br>'
								   +'	<input type="radio" class="form-radio" value="published" name="moderation_state" id="published"><label for="published" class="option" style="color:green">Published</label><br>'
								   +'</div>');
	$("#change_state_dialog").dialog({buttons: { "OK": function() {
														 var state = $("input[type='radio'][name='moderation_state']:checked").val();
														 $.ajax({
																type: "POST",
																url: ajax_url,
																data: {
																  "func_no" : 26,
														   		  "state" : state,
														   		  "nid" : nid,
																},
																success: function () {
																	 		$('#change_state_dialog').dialog("close");
																			window.location.reload();
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
									  modal: true,
									});
		
}