<?php
/*
 * Display output
**@ table output change: commented out view columnthis is the same thing as clicking on Topic title link--conwayb; 12/10/12
 */
function topiclist() {
 	header('Content-type: text/plain; charset=UTF-8');
 	header('Content-Disposition: inline');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/__jquery.tablesorter/jquery.tablesorter.js', 'file');
	$oep_util = new OepUtil();
	$catergories_str = $oep_util->get_categories();
	$images_root = base_path() . drupal_get_path('module', 'topicmgr') . '/images/';
	$html_string = $oep_util->insert_libs() .
	'
	 <div class="form-item form-type-select form-item-field-theme-und">
	 	 Category:&nbsp;
		 <select class="form-select" id="category_select">
		    <option value="all">All</option>'.
			$catergories_str
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
         <img id="loading_search_img" src="' . $images_root . 'loading.gif" width=20 height=20 style="margin-bottom:-10px"></img>
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
					          	row["category"] = topic["category"];
					          	row["topic"] = topic["topic"];
					          	row["author"] = topic["author"] == null? "" : topic["author"];
					          	row["state"] = topic["state"];
								var created = new Date(topic["created"]*1000);
								row["created"] = created;

								
								if(topic["state"]=="New")
								{
								  row["article"] = "";
								}
								else
								{
								  row["article"] = "<a href=\'../node/" + topic["article_id"] + "\'>link to article</a>";
								}
								row["view"] = "<a href=\'' . base_path() . 'node/" + topic["topic_id"] + "\'>view</a>";
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
									return "<span style=\"margin: 4px; float: left;\"><a href=\'../node/" + topic_id + "\'>" + topic + "</a></span>";
				                };
				              
				              var author_render = function (id, row, column, value) {
				                  var status = $("#jqxgrid").jqxGrid("getcellvalue", id, "state");
				              	  var topic_id = $("#jqxgrid").jqxGrid("getcellvalue", id, "topic_id");
					              var author = $("#jqxgrid").jqxGrid("getcellvalue", id, "author");
					              
				              	  if(status=="New")
									return "<span style=\"margin: 4px; float: left;\"><a href=\'author_topic_assignment?topic_id=" + topic_id + "\'>assign</a></span>";
								  else
									return "<span style=\"margin: 4px; float: left;\">" + author + "</span>";
				                };
				                
				              $("#jqxgrid").jqxGrid(
				              {
				              	sortable: true,
				                width: 840,
				                source: dataAdapter,
                				pageable: true,
                				autoheight: true,
                				columnsresize: true,
				                columns: [
				                  { text: "Category", datafield: "category", width: 100},
				                  { text: "Topic", datafield: "topic", cellsrenderer: topic_render, width: 350 },
				                  { text: "Status", datafield: "state", width: 80 },
				                  { text: "Date Created", datafield: "created", width: 100, cellsformat: "d" },
				                  { text: "Author", datafield: "author", cellsrenderer: author_render, width: 100 },
				                  { text: "", datafield: "article", width: 110, sortable: false }
				                ]
				              });
				              $("#jqxgrid").css("margin-top","20px");
				              $("#columntablejqxgrid").css("z-index","0");
				              $(".jqx-grid-cell").css("z-index","0");
				              $(".jqx-grid-cell").css("border-style","ridge");
				              $(".jqx-grid-cell").css("border-width","2px");
				              $("#loading_img").hide();
						}
    					
    					function basic_search(category,status){
    					  $.ajax({
					        type: "POST",
					        url: ajax_callback_str,
					        data: {
					          "func_no" : 1,
					          "category" : category,
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
    					
    					$("#category_select").change(function () {
    					  var category = $(this).find("option:selected").val();
    					  var status = $("#status_select").find("option:selected").val();
    					  $("#loading_img").show();
    					  basic_search(category,status);
				        }).change();
				        
    					$("#status_select").change(function () {
    					  var category = $("#category_select").find("option:selected").val();
    					  var status = $(this).find("option:selected").val();
    					  $("#loading_img").show();
    					  basic_search(category,status);
				        });
				        
				        $("#jqxgrid").bind("pagechanged", function (event) {
						     var args = event.args;
						     var pagenum = args.pagenum;
						     var pagesize = args.pagesize;
						     $("#jqxgrid").css("margin-top","20");
						     $("#columntablejqxgrid").css("z-index","0px");
						     $(".jqx-grid-cell").css("z-index","0");
						     $(".jqx-grid-cell").css("border-style","ridge");
				             $(".jqx-grid-cell").css("border-width","2px");
						});                       
				        
				        $(document).ready(function() {
				        	$("#loading_search_img").hide();
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

