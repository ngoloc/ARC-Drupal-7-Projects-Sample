var color;
var start = -1;
var stop = -1;
var color_index = 0;
var colorArr = new Array("#FFA500","#EEEE00","#61B329","#0AC92B","#43D58C","#05E9FF","#7EC0EE","#1464F4");

function next_color(){
	var c_index = color_index;
	color_index = (color_index+1)%8;
	return colorArr[c_index];
}

function test(){
    alert("hello");
}

function word_click(element){
	
	var word_id = parseInt(element.id.split("_")[1]);

	if($("#word_"+word_id).css("background-color") != 'rgba(0, 0, 0, 0)'
	   && $("#word_"+word_id).css("background-color") != 'transparent'){
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
						
						reply_str += '<p>Add your comment here:</p><textarea id="comment_text" class="comment_textarea"></textarea>';
						
						$("#dialog").html(reply_str);
						$("#dialog").dialog({buttons: { "OK": function() {
																	$.ajax({
																		type: "POST",
																		url: ajax_url,
																		data: {
																		  "func_no" : 23,
																		  "thid" : thid,
																		  "vid" : vid,
																   		  "comment" : $("#comment_text").val(),
																		},
																		success: function () {
																					$("#dialog").dialog("close");
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
											 modal:true,
											 close: function(){
														  start = -1;
														  stop = -1;
													}
										   });
						
			         },
			error: function (xmlhttp) {
			           //alert("An error occured: " + xmlhttp.status);
			         }
		});
	}
	else {
		if(start==-1)
			start = word_id;
		else {
			if(word_id>start)
				stop = word_id;
			else {
				stop = start;
				start = word_id;
			}
			//check if the region is available for commenting
			var avail = true;
			for(var i = start;i<=stop;i++){			
				if($("#word_"+i).css("background-color") != 'rgba(0, 0, 0, 0)'
				   && $("#word_"+i).css("background-color") != 'transparent'){
					avail = false;
					break;
				}
			}
			if(!avail){
				start = -1;
				stop = -1;
				alert("Cannot overlap previous comments");
			}
			else{
				
				color = next_color();
				for(var i=start;i<=stop;i++){
					$("#word_"+i).css({"background-color":color,"cursor":"pointer"});
				}
				
				$("#dialog").html('<p>Add your comment here:</p><textarea id="comment_text" class="comment_textarea"></textarea>');
				var ok = false;
				$("#dialog").dialog({buttons: { "OK": function() {
															$.ajax({
																type: "POST",
																url: ajax_url,
																data: {
																  "func_no" : 17,
																  "nid" : nid,
																  "vid" : vid,
														   		  "start" : start,
														   		  "stop" : stop,
																},
																dataType: "text",
																success: function (data) {
																			//data is new thread
																			//send another ajax to add new comment
																			$.ajax({
																				type: "POST",
																				url: ajax_url,
																				data: {
																				  "func_no" : 23,
																				  "thid" : data,
																				  "vid" : vid,
																		   		  "comment" : $("#comment_text").val(),
																				},
																				success: function () {
																							ok = true;
																							for(var i=start;i<=stop;i++)
																								$("#wordhid_"+i).val(data);
																							$("#dialog").dialog("close");
																				         },
																				error: function (xmlhttp) {
																				           //alert("An error occured: " + xmlhttp.status);
																				         }
																			});
																         },
																error: function (xmlhttp) {
																           //alert("An error occured: " + xmlhttp.status);
																         }
															});
												  		  },
												"Cancel": function() {
												  			  $(this).dialog("close");
												  		  }
											  },
									 modal:true,
									 close: function(){
												  if(!ok)
													  for(var i=start;i<=stop;i++)
											  			$("#word_"+i).css({"background-color":"rgba(0, 0, 0, 0)","cursor":"auto"});
												  start = -1;
												  stop = -1;
											}
								});
			}
		}
	}	
}

function remove_comment(comment_id)
{
	$.ajax({
		type: "POST",
		url: ajax_url,
		data: {
		  "func_no" : 39,
		  "nid" : nid,
   		  "vid" : vid,
		  "comment_id" : comment_id,
		},
		dataType: "json",
		success: function (data) {
			        reload_comments(data);
		         },
		error: function (xmlhttp) {
		           alert("An error occured: " + xmlhttp.status);
		         }
	});
}

function reload_comments(data)
{
	var html_str = "";
	for(key in data){
		html_str += '<div id="comment_' + data[key]['comment_id'] + '" class="each_comment_div">'
				   +'<div class="each_comment_header">'
				   +'<span style="float:left">' + data[key]['username'] + ' commented on ' +  data[key]['created_date'] + '</span>'
				   +'<img style="float:right; cursor:pointer;" src="'+ base_url + misc_url + '/images/dialog-close.png" height=13 width=13 onclick="remove_comment(' + data[key]['comment_id'] + ')"></img>'
                   +'</div>'
				   +'<div class="each_comment_body">'
				   + data[key]['content']
				   +'</div>'
				   +'</div>';
			}
	$("#comments_div").html(html_str);
}

function add_comment()
{
	var content = tinyMCE.get('leave_comments').getContent();
	tinyMCE.get('leave_comments').setContent('');
	$("#loading_seach_img").css('visibility','visible');

	$.ajax({
		type: "POST",
		url: ajax_url,
		data: {
		  "func_no" : 37,
		  "nid" : nid,
   		  "vid" : vid,
   		  'content' : content,
		},
		dataType: "json",
		success: function (data) {
			        reload_comments(data);
					$("#loading_seach_img").css('visibility','hidden');
		         },
		error: function (xmlhttp) {
		           alert("An error occured: " + xmlhttp.status);
		         }
	});
}

$(document).ready(function(e) {
	
		//load all the comments
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
						for(var i=0;i<wordcount;i++)
							$("#word_"+i).css("visibility","visible");
						for(key in data){
							
							color = next_color();
							for(var i=parseInt(data[key]["start"]);i<=parseInt(data[key]["stop"]);i++){
								
								$("#word_"+i).css({"background-color":color,"cursor":"pointer"});
								$("#wordhid_"+i).val(data[key]["thid"]);
							}
						}
						$("#dialog").dialog("close");
			         },
			error: function (xmlhttp) {
			           //alert("An error occured: " + xmlhttp.status);
			         }
		});
		
		//load all the comments for whole article
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
						reload_comments(data);
						tinyMCE.init({
					        // General options
					        mode : "textareas",
					        theme : "advanced",
					        plugins : "autolink,lists,spellchecker,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template",

					        // Theme options
					        theme_advanced_buttons1 : "save,newdocument,|,bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,styleselect,formatselect,fontselect,fontsizeselect",
					        theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
					        theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
					        theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,spellchecker,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,blockquote,pagebreak,|,insertfile,insertimage",
					        theme_advanced_toolbar_location : "top",
					        theme_advanced_toolbar_align : "left",
					        theme_advanced_statusbar_location : "bottom",
					        theme_advanced_resizing : true,

					        // Skin options
					        skin : "o2k7",
					        skin_variant : "silver",

					        // Example content CSS (should be your site CSS)
					        content_css : "css/example.css",

					        // Drop lists for link/image/media/template dialogs
					        template_external_list_url : "js/template_list.js",
					        external_link_list_url : "js/link_list.js",
					        external_image_list_url : "js/image_list.js",
					        media_external_list_url : "js/media_list.js",

					        // Replace values for the template plugin
					        template_replace_values : {
					                username : "Some User",
					                staffid : "991234"
					        }
						});
			         },
			error: function (xmlhttp) {
			           //alert("An error occured: " + xmlhttp.status);
			         }
		});
		


});