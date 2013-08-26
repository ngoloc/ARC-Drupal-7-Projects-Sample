var color;
var color_index = 0;
var colorArr = new Array("#FFA500","#EEEE00","#61B329","#0AC92B","#43D58C","#05E9FF","#7EC0EE","#1464F4");

function next_color(){
	var c_index = color_index;
	color_index = (color_index+1)%8;
	return colorArr[c_index];
}

function word_click(element){
	var word_id = parseInt(element.id.split("_")[1]);
	if($("#word_"+word_id).css("backgroundColor") != 'transparent'
	   && $("#word_"+word_id).css("backgroundColor") != 'rgba(0, 0, 0, 0)'){
		//show reply dialog
		var thid = $("#wordhid_"+word_id).val();
		$.ajax({
			type: "POST",
			url: ajax_url,
			data: {
			  "func_no" : 22,
	   		  "thid" : thid
			},
			dataType: "json",
			success: function (data) {
						var reply_str = '';
						for(key in data){
							reply_str +=  '<div class="comment_message"><b>'+data[key]["fullname"]+'</b> wrote on <b>'+ data[key]["created_date"] + '</b><br/>'
							 			 +data[key]["comment"]+'</div>';
						}
						
						$("#dialog").css('font-size','small');
						$("#dialog").html(reply_str);
						$("#dialog").dialog({buttons: { "OK": function() {
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
}



function reload_comments(nid,vid)
{
	color_index = 0;
	$("#ajax_layout").css("visibility","visible");
	$.ajax({
		type: "POST",
		url: ajax_url,
		data: {
		  "func_no" : 24,
		  "nid" : nid,
   		  "vid" : vid,
		},
		dataType: "json",
		success: function (data) {
						var rev_count = data['revision_index']['rev_count'];
						var current = data['revision_index']['current'];
						var prev_rev = data['revision_index']['prev_rev'];
						var next_rev = data['revision_index']['next_rev'];
						var revision_body = data['revision_body'];
				 		var rev_output = '<br/><p>';
				 		if(prev_rev != null)
					 		rev_output += '<img id="prev_img" style="cursor: pointer;" src="' + base_url + misc_url + '/images/previous.png"' + ' height="15" width="15" onclick = "reload_comments(' + nid + ',' + prev_rev + ')"/>';
					  		rev_output += '&nbspRevision:&nbsp' + current + '/' + rev_count + '&nbsp';
				  		if(next_rev != null)
					  		rev_output += '<img id="next_img" style="cursor: pointer;" src="' + base_url + misc_url + '/images/next.png"' + ' height="15" width="15" onclick = "reload_comments(' + nid + ',' + next_rev + ')"/>';
				  		rev_output += '</p>';
						//load whole comments for this revision
						$.ajax({
							type: "POST",
							url: ajax_url,
							data: {
							  "func_no" : 38,
					   		  "nid" : nid,
					   		  "vid" : vid,
							},
							dataType: "json",
							success: function (data) {
											var bottom_comments_str = '<div id="comments_div" style="padding-top: 20px; font-size:90%;">';
					          				
											for(key in data){
												bottom_comments_str += '<div id="comment_' + data[key]['comment_id'] + '" class="each_comment_div">'
																	   +'<div class="each_comment_header">'
																	   +'<span style="float:left">' + data[key]['username'] + ' commented on ' +  data[key]['created_date'] + '</span>'
													                   +'</div>'
																	   +'<div class="each_comment_body">'
																	   + data[key]['content']
																	   +'</div>'
																	   +'</div>';
													}
											bottom_comments_str += '</div>';
											
											$.ajax({
									  			type: "POST",
									  			url: ajax_url,
									  			data: {
									  			  "func_no" : 18,
									  			  "nid" : nid,
									  	 		  "vid" : vid,
									  			},
									  			dataType: "json",
									  			success: function (data) {
									  				        
									  						$("#ajax_layout").css("visibility","hidden");
									  						$("#view_dialog").html(rev_output + revision_body + bottom_comments_str);
									  						for(var i=0;i<wordcount;i++)
									  							$("#word_"+i).css("visibility","visible");
									  						for(key in data) {
									  							color = next_color();
									  							for(var i=parseInt(data[key]["start"]);i<=parseInt(data[key]["stop"]);i++){
									  								$("#word_"+i).css({"background-color":color,"cursor":"pointer"});
									  								$("#wordhid_"+i).val(data[key]["thid"]);
									  							}
									  						}
									  						
									  						$("#view_dialog").dialog({ position: 'center',
																   buttons: { "OK": function() {
																			          $(this).dialog("close");
																			        }
																		    },
																   modal: true,
																   width: '850px',
								  								});
									  			         },
									  			error: function (xmlhttp) {
									  			           alert("An error occured: " + xmlhttp.status);
									  			         }
									  		});
							         },
							error: function (xmlhttp) {
							           alert("An error occured: " + xmlhttp.status);
							         }
						});

		         },
		error: function (xmlhttp) {
		           alert("An error occured: " + xmlhttp.status);
		         }
	});
}

function view_comments(nid,vid)
{
	//load all the comments
	reload_comments(nid,vid);
}