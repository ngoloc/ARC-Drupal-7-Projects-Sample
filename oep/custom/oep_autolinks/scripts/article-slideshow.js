$(window).load(function() {
	
	$('#featured').orbit({bullets: true, directionalNav: false, startClockOnMouseOut: true, pauseOnHover: true});
	$("div.timer").css({display:'none'});
	$(".field-items").css("padding-top","10px");
});