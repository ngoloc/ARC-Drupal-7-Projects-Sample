(function ($) {
  $(document).ready(function(){ 
   $("table tr.toggle").attr("style","font-weight:bold; cursor:pointer; color:white; background-color:rgba(0,0,0,0.51)");
   $("table tr.collapsible").attr("style","display:none");
 
  
   $("table tr.toggle").click(function(){
      $("table tr.collapsible").toggle(400);
      })


  });


   $("input:checkbox").live('click',  function(){
    // alert('value: ' +  this.value +  'checked: ' + this.checked);
      var nid = this.value;
      var checked = this.checked;
    $.post("/mypages/featured_article", { nid: nid, checked: checked });
             //function(data) {
             //var content = $( data ).find( '#response' )
             //alert("Data Loaded: " + content.text());}) 
          //  $(this).closest('tr ').hide('slow');
     })


})(jQuery);


//function ajaxBox(obj){
//alert('value: ' + obj.value + 'checked: ' + obj.checked);

//}
