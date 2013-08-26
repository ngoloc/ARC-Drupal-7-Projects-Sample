(function($){
  $(document).ready(function() {
	  $("#header").css('background-image','url('+header_img+')');

	display_tab_content(nid, "Home");
  });
  })(jQuery);

function display_tab_content(nid, title) {	
	$.ajax({
		type: "POST",
		url: theme_event_callback,
		data: {
		  "func_no" : 1,
		  "nid":  nid,
		  "title": title,
		},
		dataType: "text",
		success: function (data) {
					$("#right_tab_content").html(data);
          $(".register-event").click(function(){
            window.location=$(this).find("a").attr("href");
          });

		      },
		         error: function (xmlhttp) {
		           //alert("An error occured: " + xmlhttp.status);
		         }
	});
}

function event_tab_onclick(obj){
	display_tab_content(nid,obj.innerHTML);
}
