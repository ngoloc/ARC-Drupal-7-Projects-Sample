(function($){
$(document).ready(function () { 
    var today = new Date();
    today = today.valueOf();
    $("#deadline_table tr").each(function(i) {
    if(i > 0){
    if($(this).children(':nth-child(2)').html() != "Deadline Not Set"){
    var date_string = $(this).children(':nth-child(2)').html();
    var date = date_string.split("-");   
     y = date[0];
     m = parseInt(date[1]) - 1;
     d = date[2].split(" ");
     var deadline = new Date();
     deadline = deadline.setFullYear(y,m,d[0]);
     deadline = deadline.valueOf();
     var diff = deadline - today;
     
     diff = diff/1000/60/60/24;
     if(diff > 15 && $(this).attr("class") == "odd") $(this).css("background-color", "#71E265" ); //green
     else if(diff > 15 && $(this).attr("class") == "even") $(this).css("background-color", '#99EA90'); //lighter green
     else if(diff < 15 && diff > 5 && $(this).attr("class") == "odd") $(this).css("background-color", "#FFF147"); //yellow
     else if(diff < 15 && diff > 5 && $(this).attr("class") == "even")  $(this).css("background-color",'#FFF57F'); //lighter yellow
     else if(diff <= 5 && $(this).attr("class") == "odd") $(this).css("background-color", "#D96D6D"); //red
     else if(diff <= 5 && $(this).attr("class") == "even") $(this).css("background-color", "#FF7F7F"); //lighter red
   
      }
     }
    });
    
});
})(jQuery);
