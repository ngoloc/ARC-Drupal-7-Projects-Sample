function change_caption(fid, ajax_url){
	$("#dialog").html('Type new caption: <input type="text" id="caption_input" style="border: solid 1px" width="100px"/>');
	$("#dialog").dialog({buttons: { "OK": function() {
													var caption_text = $("#caption_input").val();
													$.ajax({
														type: "POST",
														url: ajax_url,
														data: {
														  "func_no" : 1,
														  "fid" : fid,
														  "caption_val": caption_text,
														},
														success: function () {
															    $("#caption_text").text(caption_text);
																$("#dialog").dialog("close");
													         },
													    error: function (xmlhttp) {
//													    		alert("An error occured: " + xmlhttp.status);
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

function show_caption(fid, caption) {
	$('.img_' + fid).each(function(){
								 var k = $(this).clone();
							     var k_width = k.css("width");
								 $(this).replaceWith($('<div style="text-align:center; padding-bottom:15px; width: ' + k_width + ';"></div>').append(k).append('<br style="1.5em"/>' + caption));
							   });
}