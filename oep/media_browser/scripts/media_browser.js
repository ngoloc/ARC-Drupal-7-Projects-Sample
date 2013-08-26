jQuery(document).ready(function() {
	var med_data_arr = JSON.parse( jQuery("input[name=media_data_hid]").val() );
	var i = 0;
	for (key in med_data_arr) {
		var uri = med_data_arr[key]['uri'];
		var caption = med_data_arr[key]['caption'];
		media_markup_append(i, uri);
		if(caption != '')
			jQuery("#caption_div_" + i).text(caption);
		i++;
	}
});

function change_caption(med_id) {
	(function ($) {
		var caption_id = "#caption_div_" + med_id;
		$("#caption_div").html('<input style="border: solid 1px #CCCCCC; width: 250px;" id="caption_text" type="text" maxlength="100"/>');
		$("#caption_div").dialog({buttons: { "OK": function() {
														var med_data_arr = JSON.parse($("input[name=media_data_hid]").val());
														for (key in med_data_arr){
															if(med_data_arr[key]['id'] == med_id)
															{
																med_data_arr[key]['caption'] = $("#caption_text").val();
																break;
															}
														}
														$("input[name=media_data_hid]").val(JSON.stringify(med_data_arr));
														
														$(caption_id).text($("#caption_text").val());
														$(this).dialog("close");
				  				  					},
											  "Cancel": function() {
															$(this).dialog("close");
													  },
											},
								   modal:true,
								 });
	}(jQuery));
}

function remove_file(med_id) {
	(function ($) {
		var med_data_arr = JSON.parse($("input[name=media_data_hid]").val());
		for (key in med_data_arr){
			if(med_data_arr[key]['id'] == med_id)
			{
				med_data_arr.splice(key,1);
				break;
			}
		}

		$("input[name=media_data_hid]").val(JSON.stringify(med_data_arr));
		$("#med_div_"+med_id).remove();
	}(jQuery));
}

function valid_media(file_format) {
	var allowed_formats = ['bmp', 'gif', 'jpg', 'jpeg', 'png', 'tif', 'avi', 'wmv', 'ogv', 'mp4', 'webm', 'mov'];
	if(jQuery.inArray(file_format, allowed_formats) > -1)
		return true;
	return false;
}

//jQuery(".med_img_class")
//.load(function(){
//   img_real_width = this.width;
//   img_real_height = this.height;
//	
//	   if(this.width > this.height) {
//		   jQuery(this).height('auto');
//		   jQuery(this).width('100%');	
//	   }
//	   else {
//		   jQuery(this).height('100%');
//		   jQuery(this).width('auto');
//	   }
////   alert('id='+this.id + 'W='+img_real_width+' H='+img_real_height);
//});

function med_img_onload(med_id){
	var img = document.getElementById("med_img_" + med_id);
	h = img.height;
	w = img.width;
	 if(w > h) {
		   img.style.height = 'auto';
		   img.style.width = '100%';
	   }
	   else {
		   img.style.height = '100%';
		   img.style.width = 'auto';
	   };
	img.style.visibility = 'visible';
}

function media_markup_append(med_id, uri) {
	(function ($) {
		var split_uri = uri.split("/");
		var file_name = split_uri[split_uri.length-1];
		var split_extension = uri.split('.');
		var file_extension = split_extension[split_extension.length-1].toLowerCase();
		var media_icon = '/sites/entry_images/icon/media_icon.png';
		if(jQuery.inArray(file_extension, ['bmp', 'gif', 'jpg', 'jpeg', 'png', 'tif']) > -1)
			media_icon = uri;
		
		jQuery('#media_markup').append(
				'<div id="med_div_' + med_id + '" style="height:140px; width:800px; display:table; margin-bottom:20px;">' +
					'<div style="height:140px; width:50px; padding-left:10px; vertical-align: middle; display:table-cell; ">' +
					'<img title="Remove media" id="remove_file_img" height="25" width="25" onclick ="remove_file(' + med_id + ')" style="cursor: pointer;" src="' + delete_icon_url + '"></img>' +			
					'</div>' +
					'<div style=" table-layout: fixed; height:140px; width:250px; border-left:1px solid #CCCCCC; border-right:1px solid #CCCCCC; text-align:center; display:table-cell;" >' +
						'<div style="width: 120px; display: inline-block; height: 120px;">' +
							'<div style="width: 120px; display: table-cell; vertical-align: middle; height: 120px;">' +
								'<img style="visibility:hidden" onload="med_img_onload('+ med_id +')" id="med_img_' + med_id + '" src="' + media_icon +'"></img>' +
							'</div>' +
						'</div>' +
						'<br/>' +
						'<a target="_blank" href="' + uri + '">' + file_name + '</a>' +
					'</div>' +
					'<div id="caption_div_' + med_id + '" style="height:140px; width:200px; padding-left:10px; padding-right:10px; vertical-align: middle; display:table-cell;">' +
						'Caption is not set.' +
					'</div>' +
					'<div style="height:140px; width:200px; padding-left:10px; vertical-align: middle; display:table-cell; border-left:1px solid #CCCCCC;">' +
						'<a href="javascript:change_caption(' + med_id + ')">Change caption</a>' +
					'</div>' +
				'</div>'
			  );
	}(jQuery));
}

function add_new_file() {
	(function ($) {
		window.AJAXPLO = {
				ajaxplorerPopupCallback: function(uri) {
					window.AJAXPLO = null;
					var split_extension = uri.split('.');
					if(split_extension.length < 2) {
						$('#error_div').show();
						return;
					}
					var file_extension = split_extension[split_extension.length-1].toLowerCase();
					if(!valid_media(file_extension)) {
						$('#error_div').show();
						return;
					}
					var split_uri = uri.split("/");
					var media_icon = '/sites/entry_images/icon/media_icon.png';
					if(jQuery.inArray(file_extension, ['bmp', 'gif', 'jpg', 'png', 'tif']) > -1)
						media_icon = uri;
					
					media_markup_append(med_id, uri);
					
					//insert value to hidden field 
					var obj = new Object();
					obj.id = med_id;
					obj.uri = uri;
					obj.caption = '';
						
					var med_data_arr = JSON.parse( $("input[name=media_data_hid]").val() );
					med_data_arr.push(obj);
					$("input[name=media_data_hid]").val(JSON.stringify(med_data_arr));
					$('#error_div').hide();
					med_id++;
				}
		};   
		window.open("/sites/all/libraries/ajaxplorer/?external_selector_type=popup&media_path=/sites/entry_images", "ajaxplorer_textbox",
			        "status=0, toolbar=0, location=0, menubar=0, directories=0, " +
					"resizable=1, scrollbars=0, width=800, height=600"
				   );
	}(jQuery));
}
