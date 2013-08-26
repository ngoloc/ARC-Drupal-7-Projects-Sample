$(document).ready(function() {
	load_authors();
});

function get_author_content(author_id, check0, check1, reminder1style, reminder1date, reminder2style, reminder2date, reminder3style, reminder3date,
		w9onfilestyle, w9onfiledate, invited_check){
	var str = 	
	   '<div id="invite_checkbox" class="form-item form-type-checkbox" style="margin-top:10px; margin-bottom:20px">'
	  +'	<input type="checkbox" class="form-checkbox" name="author_invited_checkbox_' + author_id +  '" id="author_invited_checkbox_id_' + author_id + '" onchange="author_invited(this)" ' + invited_check + '> <label class="option" for="author_invited_checkbox_id_' + author_id + '">Author Invited </label>'
	  +'	<img style="float:right; margin-right:20px;cursor:pointer;" title="remove author" src="'+ images_root + 'delete_icon.png" onclick="remove_author(' + author_id + ',' + topic_id + ')">'
	  +'</div>'
	  + '<div id="accept_state" class="form-item form-type-radio" style="margin-top:10px; margin-bottom:10px">'
      +'	<input type="radio" class="form-radio" value="0" name="' + author_id + '" id="edit-topic-radios-0-' + author_id + '" ' + check0 + ' onclick="status_onclick(this)">  <label for="edit-topic-radios-0-' + author_id + '" class="option">Agreed to write </label>'
	  +'</div>'
      +'<div id="reject_state" class="form-item form-type-radio">'
	  +'	<input type="radio" class="form-radio" value="1" name="' + author_id + '" id="edit-topic-radios-1-' + author_id + '" ' + check1 + ' onclick="status_onclick(this)">  <label for="edit-topic-radios-1-' + author_id + '" class="option">Declined/Rejected </label>'
	  +'</div>'
	  +'<div style="height:20px; margin-top:-20px; margin-left:125px; margin-bottom:15px"><img id=' + author_id + ' src="' + images_root + 'loading.gif" width=20 height=20></img></div>'
      +'<div id="flags" style="width:420px;float:left;">'
	  +'<div id="reminder1" ' + reminder1style + ' class="reminder_unclicked" onclick="reminder1_onclick(this)">1st Reminder</div>'
	  +'<div id="reminder1_date" class="reminder_date">'+ reminder1date +'</div>'
      +'<div id="reminder2" ' + reminder2style + ' class="reminder_unclicked" onclick="reminder2_onclick(this)">2nd Reminder</div>'
      +'<div id="reminder2_date" style="margin-left:150px;" class="reminder_date">' + reminder2date + '</div>'
      +'<div id="reminder3" ' + reminder3style + ' class="reminder_unclicked" onclick="reminder3_onclick(this)">3rd Reminder</div>'
      +'<div id="reminder3_date" style="margin-left:300px;" class="reminder_date">' + reminder3date + '</div>'
      +'<div id="w9onfile" ' + w9onfilestyle + ' class="reminder_unclicked" onclick="w9onfile_onclick(this)">W-9 on file</div>'
      +'<div id="w9onfiledate" class="reminder_date">' + w9onfiledate + '</div>'
      +'<input type="hidden" id="tab_index" value="' + author_id + '"></input>'
      +'</div>'
	  +'<div id="authorNotes" style="float:left; width:210px; margin-bottom:20px;" class="note_skin">'
      +'	<p style="font-size: x-small; margin-left:5px; margin-top:5px; margin-bottom:5px; color:white;"><b>TIMELINE NOTES</b></p>'
      +'    <div id="notes_author_' + author_id + '">'
      +'  	</div>'
      +'  	<div style="text-align:center;margin-top:10px;"><img id= "img_notes_' + author_id + '" src="' + images_root + 'loading.gif" width=20 height=20></img></div>'
      +'	<div id="timeline_add_note_' + author_id + '" class="add_note" title="add new note" onclick="time_add_note_func(this)"></div>'
      +'</div>';
	return str;
}

$("#add_tab1").click(function() {
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
					"func_no" : 7,
					"topic_id" : topic_id,
			  },
	    dataType: "json",
		success: function (data) {
				  	var select_authors = "";
				    var count = 0;
				    for (key in data){
				    	select_authors += '<option value="' + data[key]["author_id"] + '">' + data[key]["author_name"] + '</option>';
				    	count++;
				    }
				    		            
				    if(count == 0) {
				    	$("#dialog").html("No available authors");
				    	$("#dialog" ).dialog({buttons: { "Ok": function() {
				    												$(this).dialog("close");
				    					  			   		   }},
				    					  	  modal:true});
				    }
				    else {
				    	$("#dialog").html('Choose an author:&nbsp<select id="user_select">' + select_authors + '</select>');
					    $("#dialog" ).dialog({buttons: { "Ok": function() {
					    											var author_id = $("#user_select option:selected").val();
					    											var labelText = $("#user_select option:selected").text();
					    											var reminder1style = 'style="color:rgb(59, 59, 59); background-color:white;"';
					    											var reminder2style = 'style="margin-left:150px; margin-top:-87px; color:rgb(59, 59, 59); background-color:white;"';
					    											var reminder3style = 'style="margin-left:300px; margin-top:-87px; color:rgb(59, 59, 59); background-color:white;"';
					    											var w9onfilestyle = 'style="color:rgb(59, 59, 59); background-color:white;"';
					    											var content = get_author_content(author_id,'','',reminder1style,'',reminder2style,'',reminder3style,'',w9onfilestyle,'',''); 
					    										    if (labelText && content) {
																        //send another ajax to add a new topic-author
																        $.ajax({
																        	type: "POST",
																        	url: ajax_callback_str,
																        	data: {
																        		"func_no" : 8,
																        		"topic_id": topic_id,
																        		"author_id" : author_id,
																	        },
																	        success: function () {
																			        	tabView.addTab( new YAHOO.widget.Tab({ label: labelText, content: content }) );
																			        	$("img#" + author_id).hide();
																			        	$("#img_notes_" + author_id).hide();
																	        		 },
																	        error: function (xmlhttp) {
																	        			//alert("An error occured: " + xmlhttp.status);
																	        	   }
																        });
					    										    }
					    										    $(this).dialog("close");
				                            				   } },
				                              modal:true});
				    }
				 },
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
			   }
	});
});

function status_onclick(element){
	$("img#" + element.name).show();
	element.checked = false;
	var status = "";
	if(element.value == 0)
		status = "Accepted";
	else
	   	status = "Disagreed";

	//send an Ajax request for changing topic-author status
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 5,
			"topic_id" : topic_id,
			"author_id" : element.name,
			"status" : status,
		},
		success: function () {
					element.checked = true;
					$("img#" + element.name).hide();
				 },
		error: function (xmlhttp) {
		          //alert("An error occured: " + xmlhttp.status);
			   }
	});
}
						  
function toggle_color(reminder,reminder_date,author_id) {
	var now = new Date();
	var date = "";
	                      	
	if(reminder.style.color === "rgb(59, 59, 59)")
		date = now.format("yyyy-mm-dd H:MM:ss");
	
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 9,
			"topic_id" : topic_id,
			"reminder" : reminder.id,
			"date" : date,
			"author_id" : author_id,
		},
		success: function () {
					//toggle the reminder color
					if(reminder.style.color === "rgb(59, 59, 59)") {
						reminder.style.color = "white";
						reminder.style.backgroundColor = "navy";
						reminder_date.innerHTML = now.format("yyyy-mm-dd H:MM:ss");
					}
					else {
					    reminder.style.color = "rgb(59, 59, 59)";
						reminder.style.backgroundColor = "white";
						reminder_date.innerHTML = "";		
					}
				 },
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
		       }
	});
}

function reminder1_onclick(element){
	
	var siblings = element.parentNode.children,
	    sibWithId = null, reminder_date;
	for(var i = siblings.length; i--;) {
	    if(siblings[i].id=="tab_index") {
	    	sibWithId = siblings[i];
			break;
		}
	}
	for(var i = siblings.length; i--;) {
	    if(siblings[i].id=="reminder1_date") {
	        reminder_date = siblings[i];
	        break;
	    }
	}

	toggle_color(element,reminder_date,sibWithId.value);
}	
						  
function reminder2_onclick(element){
    var siblings = element.parentNode.children,
        sibWithId = null, reminder_date;
	for(var i = siblings.length; i--;) {
		if(siblings[i].id=="tab_index") {
			sibWithId = siblings[i];
			break;
		}
	}
	for(var i = siblings.length; i--;) {
	    if(siblings[i].id=="reminder2_date") {
	    	reminder_date = siblings[i];
			break;
		}
	}	    		
	toggle_color(element,reminder_date,sibWithId.value);
}
						  
function reminder3_onclick(element){
	var siblings = element.parentNode.children,
	    sibWithId = null, reminder_date;
	for(var i = siblings.length; i--;) {
		if(siblings[i].id=="tab_index") {
			sibWithId = siblings[i];
			break;
		}
	}
	for(var i = siblings.length; i--;) {
		if(siblings[i].id=="reminder3_date") {
			reminder_date = siblings[i];
			break;
		}
	}
	    		
	toggle_color(element,reminder_date,sibWithId.value);
}

function w9onfile_onclick(element){
	var siblings = element.parentNode.children,
	    sibWithId = null, w9onfiledate;
	for(var i = siblings.length; i--;) {
		if(siblings[i].id=="tab_index") {
			sibWithId = siblings[i];
			break;
		}
	}
	for(var i = siblings.length; i--;) {
		if(siblings[i].id=="w9onfiledate") {
			w9onfiledate = siblings[i];
			break;
		}
	}
	    		
	toggle_color(element,w9onfiledate,sibWithId.value);
}

function load_authors(){
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 6,
			"topic_id" : topic_id,
		},
		dataType: "json",
		success: function (data) {
					var first_author = true;
					
					for (key in data){
						var check0="",check1="",invited_check="",
	    					reminder1style = 'style="color:rgb(59, 59, 59); background-color:white;"',
	    					reminder2style = 'style="margin-left:150px; margin-top:-87px; color:rgb(59, 59, 59); background-color:white;"',
	    					reminder3style = 'style="margin-left:300px; margin-top:-87px; color:rgb(59, 59, 59); background-color:white;"',
	    					w9onfilestyle = 'style="color:rgb(59, 59, 59); background-color:white;"',
	    					reminder1date="", reminder2date="", reminder3date="", w9onfiledate="";
	    				if(data[key]["state"]=="Accepted")
	    					check0 = 'checked="checked"';
	    				else if(data[key]["state"]=="Disagreed")
	    					 check1 = 'checked="checked"';
	    				
	    				if(data[key]["invited"]==1)
	    					invited_check = 'checked="checked"';
	    				
	    				if(data[key]["reminder1"] != null){
	    					reminder1style = 'style="color:white; background-color:navy;"';
	    					reminder1date = new Date(data[key]["reminder1"]*1000).format("yyyy-mm-dd H:MM:ss");
	    					
	    				}
	    				
	    				if(data[key]["reminder2"] != null){
	    					reminder2style = 'style="margin-left:150px; margin-top:-87px; color:white; background-color:navy;"';
	    					reminder2date = new Date(data[key]["reminder2"]*1000).format("yyyy-mm-dd H:MM:ss");
	    				}
	    				
	    				if(data[key]["reminder3"] != null){
	    					reminder3style = 'style="margin-left:300px; margin-top:-87px; color:white; background-color:navy;"';
	    					reminder3date = new Date(data[key]["reminder3"]*1000).format("yyyy-mm-dd H:MM:ss");
	    				}
	    				
	    				if(data[key]["w9onfile"] != null){
	    					w9onfilestyle = 'style="color:white; background-color:navy;"';
	    					w9onfiledate = new Date(data[key]["w9onfile"]*1000).format("yyyy-mm-dd H:MM:ss");
	    				}
	    				
		    		    var labelText = data[key]["author_name"];
		    		    
		    		    var content =  get_author_content(data[key]["author"],check0,check1,reminder1style,reminder1date,reminder2style,reminder2date,reminder3style,reminder3date,
		    		    		w9onfilestyle,w9onfiledate,invited_check);
		    		    if (first_author)
		    		    	tabView.addTab( new YAHOO.widget.Tab({ label: labelText, content: content, active: true }) );
		    		    else
		    		    	tabView.addTab( new YAHOO.widget.Tab({ label: labelText, content: content }) );
		    		    first_author = false;
					    $("img#" + data[key]["author"]).hide();
					    $("#img_notes_" + data[key]["author"]).hide();
					    $("#add_tab1").css("visibility","visible");
					    load_author_notes(data[key]["author"],topic_id);
		    	    }   
			     },
		error: function (xmlhttp) {
					    	//alert("An error occured: " + xmlhttp.status);
			   }
	});
}

function edit_author_note(element) {
	var note_args = element.id.split("_");
	var note_id = note_args[2];
	var author_id = note_args[3];
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 12,
			"note_id" : note_id,
		},
		dataType: "text",
		success: function (data) {
					$("#edit_note_dialog").html('<textarea id="edit_note_textarea" cols=35 rows=5 >'+ data +'</textarea>');
					$("#edit_note_dialog" ).dialog({buttons: { 	"Delete" :function() {
																			$("#caution_dialog").html("Do you want to delete the note ?");
																	    	$("#caution_dialog" ).dialog({buttons: { "Ok": function() {
																			    											 $("#img_notes_" + author_id).show();
																															 $.ajax({
																																type: "POST",
																																url: ajax_callback_str,
																																data: {
																																	"func_no" : 14,
																																	"note_id" : note_id,
																																},
																																success: function () {
																																			$("div#note_" + note_id).remove();
																																			$("#img_notes_" + author_id).hide();
																																		 },
																																error: function (xmlhttp) {
																																			 //alert("An error occured: " + xmlhttp.status);
																																       }
																															 });
																			    										   	 $(this).dialog("close");
																			    										   	 $("#edit_note_dialog").dialog("close");
																			    					  			   		   },
																			    					  			   	 "Cancel": function(){
																			    					  			   			     $(this).dialog("close");
																			    					  			   		   }},
																	    					  	  modal:true});
																			
																          },
																"Apply Change": function() {
																		$("#img_notes_" + author_id).show();
																		var text = $("#edit_note_textarea").val();
																		  
																		$.ajax({
																			type: "POST",
																			url: ajax_callback_str,
																			data: {
																				"func_no" : 13,
																				"note_id" : note_id,
																				"note_content" : text,
																			},
																			dataType: "json",
																			success: function (ret) {
																						var date = new Date(ret["data"]["date_taken"]*1000);
																						date = date.format("yyyy-mm-dd H:MM:ss");
																						var new_note =  ret["note_content"]
																								   	   +'<br>'
																									   +'<span style="font-size: 10px; color: gray;">[' + date + '] by <b>' + ret["data"]["commenter"] + '</b></span>'
																									   +'<div class="edit_note" id="note_edit_' + note_id  + '_' + author_id + '" title="edit note" onclick="edit_author_note(this)"></div>';
																						$("div#note_" + note_id).html(new_note);
																						$("#img_notes_" + author_id).hide();
																					 },
																			error: function (xmlhttp) {
																						 //alert("An error occured: " + xmlhttp.status);
																			       }
																		});
																		$("#edit_note_dialog").dialog("close");
					                                 				}
															} ,
												   modal:true});
				 },
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
		       }
	});
	

}

function load_author_notes(author_id,topic_id) {
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 11,
			"topic_id" : topic_id,
			"author_id" : author_id,
		},
		dataType: "json",
		success: function (data) {
					for (key in data){
						var note_id = data[key]["note_id"];
						var date_taken = data[key]["date_taken"];
						var date = new Date(date_taken*1000);
						date = date.format("yyyy-mm-dd H:MM:ss");
						var commenter = data[key]["commenter"];
						var note_content = data[key]["note_content"];
						var new_note =  
							'<div class="note" id="note_' + note_id + '" >' + note_content
						   +'<br>'
	        			   +'<span style="font-size: 10px; color: gray;">[' + date + '] by <b>' + commenter + '</b></span>'
	        			   +'<div class="edit_note" id="note_edit_' + note_id + '_' + author_id + '" title="edit note" onclick="edit_author_note(this)"></div>'
	        			   +'</div>';
						var div_string = "notes_author_" + author_id;
						$("div#" + div_string).append(new_note);
					}
				 },
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
		       }
	});
}

function time_add_note_func(element){
	var author_id = element.id.substring(18);
	$("#add_note_dialog").html('<textarea id="add_note_textarea" cols=35 rows=5 ></textarea>');
	$("#add_note_dialog" ).dialog({buttons: { "Ok": function() {
														$("#img_notes_" + author_id).show();
														var text = $("#add_note_textarea").val();
														var now = new Date();
														var date = now.format("yyyy-mm-dd H:MM:ss");  
														$.ajax({
															type: "POST",
															url: ajax_callback_str,
															data: {
																"func_no" : 10,
																"topic_id" : topic_id,
																"author_id" : author_id,
																"note_content" : text,
																"date_taken" : date,
																"commenter" : userid,
															},
															dataType: "json",
															success: function (ret) { 
																		var new_note =  
																			'<div class="note" id="note_' + ret['data'] + '" >' + ret['note_content']
																		   +'<br>'
										                    			   +'<span style="font-size: 10px; color: gray;">[' + date + '] by <b>' + username + '</b></span>'
										                    			   +'<div class="edit_note" id="note_edit_' + ret['data'] + '_' + author_id + '" title="edit note" onclick="edit_author_note(this)"></div>'
										                    			   +'</div>';
																		var div_string = "notes_author_" + author_id;
																		$("div#" + div_string).append(new_note);
																		$("#img_notes_" + author_id).hide();
																	 },
															error: function (xmlhttp) {
																		 //alert("An error occured: " + xmlhttp.status);
															       }
														});
														$("#add_note_dialog").dialog("close");
														
	                                 				}
											} ,
								   modal:true});
}

function author_invited(element) {
	var invited_authors_args = element.name.split("_");
	var author_id = invited_authors_args[3];
	$("img#" + author_id).show();
	
	$.ajax({
		type: "POST",
		url: ajax_callback_str,
		data: {
			"func_no" : 15,
			"topic_id" : topic_id,
			"author_id" : author_id,
			"invited" : element.checked == true? 1 : 0,
		},
		success: function () {
					$("img#" + author_id).hide();
				 },
		error: function (xmlhttp) {
					 //alert("An error occured: " + xmlhttp.status);
		       }
	});	
}

function remove_author(author_id,topic_id) {
	$("#caution_dialog").html("Do you want to remove the author ?");
	$("#caution_dialog" ).dialog({buttons: { "Ok": function() {
													 $.ajax({
															type: "POST",
															url: ajax_callback_str,
															data: {
																"func_no" : 25,
																"topic_id" : topic_id,
																"author_id" : author_id,
															},
															dataType: "text",
															success: function (data) {
																		if(parseInt(data)==0)
																			alert("At least one author must be assigned for this topic");
																		else{
																			//successfully remove the author
																			tabView.removeTab(tabView.get('activeTab'));
																			// make tab at index 0 active
																			tabView.set('activeIndex', 0);
																			$("#caution_dialog").dialog("close");
																		}
																	 },
															error: function (xmlhttp) {
																		 //alert("An error occured: " + xmlhttp.status);
															       }
													 });
						  			   		   	   },
							  			   	 "Cancel": function(){
							  			   			     $(this).dialog("close");
							  			   	 		   }
						  			   	   },
						  		  modal:true
						  		});
}