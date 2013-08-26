<?php
/*
 * Display output
 */
function advancedsearch() {
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/ajax.googleapis.com/jquery.min.js', 'file');
	drupal_add_js(drupal_get_path('module', 'misc') . '/scripts/json2.js', 'file');
	header('Content-type: text/plain; charset=UTF-8');
	header('Content-Disposition: inline');
	$oep_util = new OepUtil();
	$search_content = $_POST["search_content"] == null ? "" : $_POST["search_content"];
	$search_title = $_POST["search_title"] == null ? "" : $_POST["search_title"];
	$search_author = $_POST["search_author"] == null ? "" : $_POST["search_author"];
	$search_cat = $_POST["search_cat"] == null ? "all" : $_POST["search_cat"];
	$search_theme = $_POST["search_theme"] == null ? "all" : $_POST["search_theme"];
	$search_subtheme = $_POST["search_subtheme"] == null ? "all" : $_POST["search_subtheme"];
	$search_county = $_POST["search_county"] == null ? "all" : $_POST["search_county"];
	$search_era = $_POST["search_era"] == null ? "all" : $_POST["search_era"];
	$search_region = $_POST["search_region"] == null ? "all" : $_POST["search_region"];
	
	$html_string = $oep_util->insert_libs().
	' 
	<form method="post" action="advanced_search" id="advanced_search_form" style="width:700px; height:200px;">
		<div class="form-item form-type-select form-item-field-theme-und" style="width:250px;float:left">
			<div><label for="search_content">Content</label><input type="text" value="' . htmlspecialchars($search_content) . '" id="search_content" name="search_content"></div>
			<div><label for="search_title">Title</label><input type="text" value="'. htmlspecialchars($search_title) .'" id="search_title" name="search_title"></div>
			<div><label for="search_author">Author</label><input type="text" value="'. htmlspecialchars($search_author) .'" id="search_author" name="search_author"></div>
			<input type="submit" value="search" id="search_button" class="form-submit" style="margin-top:30px" onclick="advanced_search_submit()">		
		</div>
		
		<div class="form-item form-type-select form-item-field-theme-und" style="width:220px; float:left">
			<div>
				<label for="search_cat">Category</label>
				<select style="width: 170px;" id="search_cat" name="search_cat" class="form-select">
					<option value="all"></option>'.
					$oep_util->get_categories($search_cat)
			  .'</select>
			</div>
			<div>
				<label for="search_theme">Theme</label>
				<select style="width: 170px;" id="search_theme" name="search_theme" type="select" class="form-select">
					<option value="all"></option>'.
			  		$oep_util->get_themes($search_theme)
			  .'</select>
			</div>
			<div>
				<label for="search_subtheme">Sub Theme</label>
				<select style="width: 170px;" id="search_subtheme" name="search_subtheme" type="select" class="form-select">
					<option value="all"></option>'.
			  		$oep_util->get_subthemes($search_subtheme)
			  .'</select>	
			</div>
		</div>
		
		<div class="form-item form-type-select form-item-field-theme-und" style="width:220px; float:left">
			<div>
				<label for="search_county">County</label>
				<select style="width: 170px;" id="search_county" name="search_county" type="select" class="form-select">
					<option value="all"></option>'.
			  		$oep_util->get_counties($search_county)
			  .'</select>
			</div>
			<div>
				<label for="search_era">Era</label>
				<select style="width: 170px;" id="search_era" name="search_era" type="select" class="form-select">
					<option value="all"></option>'.
			  		$oep_util->get_eras($search_era)
			  .'</select>
			</div>
			<div>
				<label for="search_region">Region</label>
				<select style="width: 170px;" id="search_region" name="search_region" type="select" class="form-select">
					<option value="all"></option>'.
			  		$oep_util->get_regions($search_region)
			  .'</select>	
			</div>
		</div>
		
		<input type="hidden" name="submit_bool_hid" id="submit_bool_hid"></input>
		<input type="hidden" name="page_hid" id="page_hid" value=1></input>
	</form>
	<script>
		var ajax_callback_str = "' . base_path() . 'ajax_callback";
	</script>
	<script src="' . base_path() . drupal_get_path('module', 'misc')  .'/scripts/advancedsearch.js"></script>
	';
	if(isset($_POST["submit_bool_hid"])){

		$submit = $_POST["submit_bool_hid"];
		
		if($submit) {
			//renew session variables
			$_SESSION["search_content"] = $_POST["search_content"];
			$_SESSION["search_title"] = $_POST["search_title"];
			$_SESSION["search_author"] = $_POST["search_author"];
			$_SESSION["search_cat"] = $_POST["search_cat"];
			$_SESSION["search_theme"] = $_POST["search_theme"];
			$_SESSION["search_subtheme"] = $_POST["search_subtheme"];
			$_SESSION["search_county"] = $_POST["search_county"];
			$_SESSION["search_era"] = $_POST["search_era"];
			$_SESSION["search_region"] = $_POST["search_region"];
			$_SESSION['search_results'] = array();
			$_SESSION['search_results_number'] = $oep_util->get_articles(True,$_SESSION["search_content"],$_SESSION["search_title"],$_SESSION["search_author"],
																	      	  $_SESSION["search_cat"],$_SESSION["search_theme"],$_SESSION["search_subtheme"],
																	     	  $_SESSION["search_county"],$_SESSION["search_era"],$_SESSION["search_region"]);
		}
		//show out the results
		$result_number = $_SESSION['search_results_number'];
		$html_string .='<p style="margin-top:80px"><b>' . $result_number . ' articles found</b></p>';
		
		if($result_number > 0){
			$page = $_POST["page_hid"];
			//show out the links and the results
			$max_results_per_page = 10;//this cannot be changed
			$max_pages_per_sheet = 10; //make it properly with "index span"
			$max_results_per_sheet = $max_results_per_page*$max_pages_per_sheet;

			$html_string .='<div style="text-align:center; width: 500px; margin-bottom:10px;" >';
			$first_page_index = (int)(($page-1)/$max_pages_per_sheet)*$max_pages_per_sheet + 1;
			$max_page_index = (int)(($result_number-1)/$max_results_per_page) + 1;
			$last_page_index = ($first_page_index + $max_pages_per_sheet - 1) > $max_page_index ? $max_page_index : ($first_page_index + $max_pages_per_sheet - 1);
			
			if($first_page_index>1)
				$html_string .='<img id="prev_img" style="cursor: pointer;" src="' . base_path() . drupal_get_path('module', 'topicmgr') . '/images/previous.png" height="15" width="15" onclick="prev_sheet()"/>';
			$j = 1;
			for($i=$first_page_index;$i<=$last_page_index;$i++)	{
				if($i==$page)
					$html_string .='<span class="index_span" style="color:black; font-size:x-large;" id="index_span_' . $j . '" onclick="index_span_onclick(this)">' . $i . '</span>';
				else
					$html_string .='<span class="index_span" id="index_span_' . $j . '" onclick="index_span_onclick(this)">' . $i . '</span>';
				$j++;
			}
			if($max_page_index>$last_page_index)
				$html_string .='<img id="next_img" style="cursor: pointer;" src="' . base_path() . drupal_get_path('module', 'topicmgr') . '/images/next.png" height="15" width="15" onclick="next_sheet()"/>';
			
			//load n_articles to the webpage
			$current_pos = ($page-1)*$max_results_per_page + 1;
			$n_articles = ($result_number - $current_pos + 1) > $max_results_per_page ? $max_results_per_page : ($result_number - $current_pos + 1);
			$results = array_slice($_SESSION['search_results'],$current_pos-1,$n_articles);
			$html_string .='</div>			
							<div id="result_div">';
								$i = 1;
								foreach($results as $result){
									$html_string .=	'<div id="art_div_' . $i .'" style="width: 300px; height:200px; display:inline-block; padding-right:30px; float:left;">'
												   .'<p style="margin-bottom:5px"><b><a href="../mypages/article/' . $result["article_id"] . '/view">' . $result["article_title"] . '</a></b></p>'
									  			   .'<div>' . $result["snippet"] . '</div>'
									  			   .'<p><a href="../mypages/article/' . $result["article_id"] . '/view">[ click for more ]</a></p>'						
												   .'</div>';
									$i++;
								}
			$html_string .='</div>';
			
// 			if($max_page_index > $max_pages_per_sheet) {
// 				$html_string .= '<span id="index_sel_span" style="margin-left:10px;">
// 				<select style="display: inline-block" id="index_sel" name="index_sel" type="select" class="form-select">';
// 				$sheets = (int)($result_number/$max_results_per_sheet);
// 				$remainder = $result_number%$max_results_per_sheet;
			
// 				for($i=1;$i<=$sheets;$i++) {
						
// 					$begin_page = ($i-1)*$max_pages_per_sheet + 1;
// 					$end_page = $i*$max_pages_per_sheet;
						
// 					if($page >= $begin_page && $page <= $end_page){
// 						$html_string .= '<option value="' . $begin_page . '_' . $end_page . '" selected>' . $begin_page . ' - ' . $end_page . '</option>';
// 						$html_string .= '<script>var prev_sel = "' . $begin_page . '_' . $end_page . '";</script>';
							
// 					}
// 					else
// 						$html_string .= '<option value="' . $begin_page . '_' . $end_page . '">' . $begin_page . ' - ' . $end_page . '</option>';
// 				}
			
// 				if($remainder>0) {
// 					$remainder_pages = (int)(($remainder-1)/$max_results_per_page)+1;
// 					$begin_page = $sheets*$max_pages_per_sheet + 1;
// 					$end_page = $sheets*$max_pages_per_sheet + $remainder_pages;
// 					if($page >= $begin_page && $page <= $end_page){
// 						$html_string .= '<option value="' . $begin_page . '_' . $end_page . '" selected>' . $begin_page . ' - ' . $end_page . '</option>';
// 						$html_string .= '<script>var prev_sel = "' . $begin_page . '_' . $end_page . '";</script>';
// 					}
// 					else
// 						$html_string .= '<option value="' . $begin_page . '_' . $end_page . '">' . $begin_page . ' - ' . $end_page . '</option>';
// 				}
			
// 				$html_string .= '	</select>
// 				</span>';
// 			}
			
			$html_string .='<script>
							    	var max_pages_per_sheet = "' . $max_pages_per_sheet . '";
							    	$("#index_sel").change(function(){
							    		var arr = $("#index_sel option:selected").val().split("_");
										var begin_page = arr[0];
										$("#submit_bool_hid").val(0);
										$("#page_hid").val(begin_page);
										$("#index_sel").val(prev_sel);
										$("#advanced_search_form").submit();
									});
					    	</script>';
		}
	}
	return $html_string;
}
