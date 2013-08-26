
$(document).ready(function () {
		$(".ev_hoverable").hover(function(){
			$(this).css('background-color','rgba(10, 10, 10, 0.5)');
		}, function(){
			$(this).css('background-color','rgba(255, 255, 255, 1)');
		});
		$('#page').css('background-image','none');	
		$('#sidebar-first').hide();
		$('#main-wrapper').css('margin-left', '20px');
	    // Create jqxTree 
	    var theme = 'black';
	    // create jqxTree
	    $('#jqxTree').jqxTree({ height: '400px', hasThreeStates: true, checkboxes: true, width: '205px', theme: theme });
	    $('#jqxCheckBox').jqxCheckBox({ width: '200px', height: '25px', checked: true, theme: theme });
	    $('#jqxCheckBox').on('change', function (event) {
	        var checked = event.args.checked;
	        $('#jqxTree').jqxTree({ hasThreeStates: checked });
	    });
	    $("#jqxTree").jqxTree('selectItem', $("#home")[0]);
	    $('.jqx-tree-item-li').css('list-style','none');
	    $("#jqxTree").css({
    	    'margin-left' : "0px",
    	    'margin-top' : "10px",
    	    'float' : 'none',
    	    'width' : 'auto'
    	});
	    
	    $('#jqxTree').on('checkChange', function (event) 
	    {
	        var args = event.args;
	        var element = args.element;
	        var checked = args.checked;
	        var arr = JSON.parse($("input[name=make_model_hid]").val());
	        var id = parseInt(element.id);
	        if(checked == false) {
	        	for (key in arr){
	    			if(arr[key] == id)
	    			{
	    				arr.splice(key,1);
	    				break;
	    			}
	    		}
	        }
	        else {
	        	var found = false;
	        	//check if this id already exists
	        	for (key in arr){
	    			if(arr[key] == id)
	    			{
	    				found = true;
	    				break;
	    			}
	    		}
	        	if(!found)
	        		arr.push(id);
	        }
	        $("input[name=make_model_hid]").val(JSON.stringify(arr));
	    });
});

function toggle_div(div_id){
	$('#' + div_id).toggle('fast');
}

function vehicle_link_clicked(link) {
	window.location.href = link;
}