jQuery(document).ready(function() { 
    // call the tablesorter plugin 
    jQuery("#deadline_table").tablesorter(
        // sort on the deadline column, order asc 
        {sortList: [[1,0]],
	    	headers: { 3: {sorter: false} }
	    	
    }); 
}); 
