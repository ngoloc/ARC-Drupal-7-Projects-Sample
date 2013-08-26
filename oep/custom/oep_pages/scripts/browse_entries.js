  
function basic_search(author,status){
	  $.ajax({
	      type: "POST",
	      url: ajax_callback_str,
	      data: {
	        "func_no" : 30,
	        "author" : author,
	        "status" : status,
	      },
	      dataType: "text",
	      success: function (data) {
	    	 $('#wrapper').html(data);
	    	 $("#loading_img").hide();
	    	 $("table").tablesorter({sortList: [[0,0]],
									 headers: { 1: { sorter: false}, 2: {sorter: false}, 3: {sorter: false}, 4: {sorter: false} }
               					    }); 
	      },
	      error: function (xmlhttp) {
	        alert("An error occured: " + xmlhttp.status);
	      }
      });
}


	$("#author_select").change(function () {									
									var author = $(this).find("option:selected").val();
									var status = $("#status_select").find("option:selected").val();
									$("#loading_img").show();
									basic_search(author,status);
		    				   });
						        
	$("#status_select").change(function () {;
		    					   var author = $("#author_select").find("option:selected").val();
		    					   var status = $(this).find("option:selected").val();
		    					   $("#loading_img").show();
		    					   basic_search(author,status);
						      });
						        
$(document).ready(function() {
	$("#author_select").change();
});