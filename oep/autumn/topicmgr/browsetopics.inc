<?php
/*
 * Display output
 */
function browsetopics() {
	header('Content-type: text/plain; charset=UTF-8');
	header('Content-Disposition: inline');
	$oep_util = new OepUtil();
	$authors_str = $oep_util->get_authors();
	$counties_str = $oep_util->get_counties();
	$images_root = base_path() . drupal_get_path('module', 'topicmgr') . '/images/'; 
	$html_string = $oep_util->insert_libs() .
	'
	 <div class="form-item form-type-select form-item-field-theme-und">
	 	 Author:&nbsp;
		 <select class="form-select" id="author_select">
		    <option value="all">All</option>'.
			$authors_str
		.'</select>
		  &nbsp;&nbsp;&nbsp;
		  County:&nbsp;
		  <select class="form-select" id="county_select">
		    <option value="all">All</option>'.
		    $counties_str
		.'</select>
		  &nbsp;&nbsp;&nbsp;
		  Status:&nbsp;
		  <select class="form-select" id="status_select">
		    <option value="all">All</option>
		    <option value="New">New</option>
		    <option value="Assigned">Assigned</option>
		    <option value="Accepted">Accepted</option> 
		    <option value="Disagreed">Disagreed</option>
		    <option value="Returned">Returned</option>
		  </select>
		  <img id="loading_img" src="' . $images_root . 'loading.gif" width=20 height=20 style="margin-bottom:-10px"></img>
		  <br/>
		  <p>OR</p>
	  </div>
      <p>Enter your keyword:	&nbsp; <input id="tags" class="form-text" type="text"></input>
         <input type="button" class="form-submit" value="Search" id="search_submit">
         <img id="loading_seach_img" src="' . $images_root . 'loading.gif" width=20 height=20 style="margin-bottom:-10px"></img>
      </p>
      <div style="margin-top:120px" class="form-item form-type-select form-item-field-theme-und" id="jqxgrid">
	  </div>
      <script>
      					var ajax_callback_str = "'. base_path() .'ajax_callback";
    					function show_grid(data){
    						  var grid_data = new Array();
					          for (topic_key in data){
					            var topic = data[topic_key];
					            var row = {};
					            row["topic_id"] = topic["topic_id"];
					            row["topic"] =  topic["topic"];
								if(topic["state"]=="New")
								{
					          	  row["author"] = "";
					          	  row["manage"] = "";
								}
								else
								{
								  row["author"] = topic["author"];
								  row["manage"] = "<a href=\'' . base_path() . 'mypages/topic_manager?topic_id=" + topic["topic_id"] + "\'>manage</a>"; 
								}

								row["county"] = topic["county"] == null ? "" : topic["county"];
								var created = new Date(topic["created"]*1000);
								row["created"] = created;
								row["state"] = topic["state"];
								
								grid_data.push(row);
					          }
					          
					          var source =
				              {
				                localdata: grid_data,
				                datatype: "array"
				              };
				              
				              var dataAdapter = new $.jqx.dataAdapter(source, {
				                loadComplete: function (grid_data) { }
				              });
				              
				               var topic_render = function (id, row, column, value) {
									var topic_id = $("#jqxgrid").jqxGrid("getcellvalue", id, "topic_id");
									var topic =  $("#jqxgrid").jqxGrid("getcellvalue", id, "topic");
									return "<span style=\"margin: 4px; float: left;\"><a href=\'' . base_path() . 'node/" + topic_id + "\'>" + topic + "</a></span>";
				                };
				              
				              $("#jqxgrid").jqxGrid(
				              {
				              	sortable: true,
				                width: 810,
				                source: dataAdapter,
                				pageable: true,
                				autoheight: true,
                				columnsresize: true,
				                columns: [
				                  { text: "Topic", datafield: "topic", cellsrenderer: topic_render, width: 350},
				                  { text: "Author", datafield: "author", width: 100 },
				                  { text: "County", datafield: "county", width: 100 },
				                  { text: "Date Created", datafield: "created", width: 100, cellsformat: "d" },
				                  { text: "Status", datafield: "state", width: 80 },
				                  { text: "", datafield: "manage", width: 80, sortable: false}
				                ]
				              });
				              $("#jqxgrid").css("margin-top","20px");
				              $("#columntablejqxgrid").css("z-index","0");
				              $(".jqx-grid-cell").css("z-index","0");
				              $(".jqx-grid-cell").css("border-style","ridge");
				              $(".jqx-grid-cell").css("border-width","2px");
				              $("#loading_img").hide();
						}
    					
    					function basic_search(author,county,status){
    					  $.ajax({
					        type: "POST",
					        url: ajax_callback_str,
					        data: {
					          "func_no" : 4,
					          "author" : author,
					          "county" : county,
					          "status" : status,
					        },
					        dataType: "json",
					        success: function (data) {
					          show_grid(data);
					        },
					        error: function (xmlhttp) {
					          //alert("An error occured: " + xmlhttp.status);
					        }
					      });
    					}
    					
    					$("#author_select").change(function () {
    					  var author = $(this).find("option:selected").val();
    					  var county = $("#county_select").find("option:selected").val();
    					  var status = $("#status_select").find("option:selected").val();
    					  $("#loading_img").show();
    					  basic_search(author,county,status);
    					}).change();
    					
    					$("#county_select").change(function () {
    					  var author = $("#author_select").find("option:selected").val();
    					  var county = $(this).find("option:selected").val();
    					  var status = $("#status_select").find("option:selected").val();
    					  $("#loading_img").show();
    					  basic_search(author,county,status);
				        });
				        
    					$("#status_select").change(function () {
    					  var author = $("#author_select").find("option:selected").val();
    					  var county = $("#county_select").find("option:selected").val();
    					  var status = $(this).find("option:selected").val();
    					  $("#loading_img").show();
    					  basic_search(author,county,status);
				        });
				        
				        $("#jqxgrid").bind("pagechanged", function (event) {
						     var args = event.args;
						     var pagenum = args.pagenum;
						     var pagesize = args.pagesize;
						     $("#jqxgrid").css("margin-top","20px");
				             $("#columntablejqxgrid").css("z-index","0");
						     $(".jqx-grid-cell").css("z-index","0");
						     $(".jqx-grid-cell").css("border-style","ridge");
				             $(".jqx-grid-cell").css("border-width","2px");
						});                       
				        
				        $(document).ready(function() {
				        	$("#loading_seach_img").hide();
				        	$("input#tags").attr("value", "");
				        	
				        	$.ajax({
								type: "POST",
								url: ajax_callback_str,
								data: {
								  "func_no" : 2,
								},
								dataType: "json",
								success: function (data) {
											$("input#tags").autocomplete({
										    	source: data,
											});
								         },
								         error: function (xmlhttp) {
								           //alert("An error occured: " + xmlhttp.status);
								         }
							});
						});
						
						$.ui.autocomplete.prototype._renderMenu = function( ul, items ) {
						   var self = this;
						   $.each( items, function( index, item ) {
						      if (index < 4) // here we define how many results to show
						         {self._renderItem( ul, item );}
						      });
						}
						
						function search_keywords(){
							$("#loading_seach_img").show();
							$.ajax({
								type: "POST",
								url: ajax_callback_str,
								data: {
								  "func_no" : 3,
								  "keyword" : $("#tags").val(),
								},
								dataType: "json",
								success: function (data) {
											show_grid(data);
											$("#loading_seach_img").hide();
								         },
								error: function (xmlhttp) {
								           //alert("An error occured: " + xmlhttp.status);
									   }
							});
						}
						
						$("#search_submit").click(function(){
							search_keywords();
						});
					  
						$("#tags").bind("keypress", function(e) {
					    	if(e.keyCode==13){
					         	search_keywords();
					      	}
						});
						
	  </script>		
	  			';
	
	return $html_string;
}

