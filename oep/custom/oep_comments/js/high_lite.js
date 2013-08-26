function Range ( startWord, endWord ) {
	if ( startWord > endWord ) {
		this.startWord = endWord;
		this.endWord = startWord;
	} else {
		this.startWord = startWord;
		this.endWord = endWord;
	}
	return;
}

function isInRanges( wordNum, rangeList ){
	if ( rangeList === undefined || rangeList == null ) rangeList = ranges;
	for ( var i = 0; i < rangeList.length; i++ ) {
		if (wordNum >= rangeList[i].startWord && wordNum <= rangeList[i].endWord)
			return true;
	}
	
	return false;
}

function intersectRanges( range, rangeList ){
	if ( rangeList === undefined )rangeList = null;
	for ( var i = range.startWord; i <= range.endWord; i++ ) {
		if ( isInRanges(i, rangeList) )
			return true;
	}
	
	return false;
}


function HighLite ( ) {
	this.ranges = new Array ( );
	this.colors = new Array ( "#949170", "#8c8c86", "#6e6194", "#c8c1e0", "#9dad83", "#617a38", "#8b8de4", "#8390ad", "#615d36", "#616053", "#e6dc80" );
	this.startWord = null;
	this.endWord = null;
	this.pendingStart = null;
	this.pendingEnd = null;
	return;
}

HighLite.prototype.init = function ( ranges ) {
	for ( var i = 0; i < ranges.length; i++ ) {
		this.addRange ( ranges[i] );
	}
}

HighLite.prototype.addRange = function ( range ) {
	this.ranges.unshift ( range );
	var color = this.colors.shift ( );
	this.colors.push ( color );
	for ( var i = range.startWord; i <= range.endWord; i++ ) {
		var word = document.getElementById('word_' + i);
		
		if (word == null)
			return;
		
		word.style.backgroundColor = color;
		word.style.cursor = 'pointer';
		
		word.onchange = word.onclick;
		word.onclick = null;
		
		if (word.addEventListener){
			word.addEventListener('click', this.commentBox, false); 
			word.startWord = range.startWord;
			word.endWord = range.endWord;
		} else if (word.attachEvent){
			word.attachEvent('onclick', this.commentBox);
			word.startWord = range.startWord;
			word.endWord = range.endWord;
		}
	}
	this.startWord = this.pendingStart;
	this.endWord = this.pendingEnd;

	this.pendingStart = null;
	this.pendingEnd = null;
}

HighLite.prototype.updateRange = function ( range ) {
	this.ranges.unshift ( range );
	var color = this.colors.shift ( );
	this.colors.push ( color );
	for ( var i = range.startWord; i <= range.endWord; i++ ) {
		var word = document.getElementById('word_' + i);
		
		if (word == null)
			return;
		
		word.style.backgroundColor = color;
		word.style.cursor = 'pointer';
		
		word.onchange = word.onclick;
		word.onclick = null;
		
		if (word.addEventListener){
			word.addEventListener('click', this.commentBox, false); 
			word.startWord = range.startWord;
			word.endWord = range.endWord;
		} else if (word.attachEvent){
			word.attachEvent('onclick', this.commentBox);
			word.startWord = range.startWord;
			word.endWord = range.endWord;
		}
	}
}

HighLite.prototype.removeRange = function ( range ){
	for (var i = range.startWord; i <= range.endWord; i++) {
		var word = document.getElementById('word_' + i);
		word.style.backgroundColor = 'transparent';
		word.style.cursor = 'text';
		
		if (word.addEventListener) {
			word.removeEventListener('click', this.commentBox, false);
		}
		else if (word.attachEvent) {
			word.detachEvent('onclick', this.commentBox);
		}
		
		word.onclick = word.onchange;
	}
	
	highlite.startWord = null; 
	highlite.endWord = null; 
	
	highlite.pendingStart = null;
	highlite.pendingEnd = null; 
	
	highlite.ranges.shift();
}

HighLite.prototype.commentBox = function ( evt ) {
	// Clear highlite status
	highlite.startWord = null;
	highlite.endWord = null;
	highlite.pendingStart = null;
	highlite.pendingEnd = null;
	
	box = document.getElementById('comments');
	var comments = document.getElementById('comment_list');

	if ( document.getElementById("new_comment") != null ) {
		document.getElementById("new_comment").innerText="";
		document.getElementById("new_comment").value="";
	}

	comments.innerHTML = '<h3>Loading</h3>';

	if ( !evt ) {
		var evt = window.event;
	}

	if ( evt['target'] ) {
		highlite.startWord = evt['target']['startWord'];
		highlite.endWord = evt['target']['endWord'];
	} else {
		highlite.startWord = evt['srcElement']['startWord'];
		highlite.endWord = evt['srcElement']['endWord'];
	}

	requestContent ( getCommentUrl ( highlite.startWord, highlite.endWord ), 'highlite.fillComments', 'callback', true );

	var posx = 0;
	var posy = 0;

	if ( evt.pageX || evt.pageY ) {
		posx = evt.pageX;
		posy = evt.pageY;
	} else if ( evt.clientX || evt.clientY ) {
		posx = evt.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
		posy = evt.clientY + document.body.scrollTop + document.documentElement.scrollTop;
	}

	if ( ( evt.clientY + box.clientHeight ) > window.innerHeight ) {
		box.style.top = posy - box.clientHeight + 'px';
	} else {
		box.style.top = posy + 'px';
	}

	posx = posx - document.getElementById('container').offsetLeft;

	// Fix clippling of the comment box
	if (posx > 510)	posx = 510;
	
	box.style.left = posx + 'px';
	
	box.style.display = 'block';
}

HighLite.prototype.fillComments = function ( commentXML ) {	
	var box = document.getElementById('comment_list');

	var comments = commentXML.getElementsByTagName('comment');
	var parsedComments = new Array ( );
	for ( j = 0; j < comments.length; j++ ) {
		var comment = comments.item(j);
		parsedComments[j] = new Array ( );
		for ( k = 0; k < comment.childNodes.length; k++ ) {
			var content = comment.childNodes[k];  // the cracked version of childNodes... ;)
			if ( content.firstChild ) {
				parsedComments[j][content.nodeName] = content.firstChild.nodeValue;
			} else {
				parsedComments[j][content.nodeName] = '';
			}
		}
	}
	var newHTML = "";
	for ( i = 0; i < parsedComments.length; i++ ) {
		newHTML += "<h3>" + parsedComments[i]['author'] + "</h3>";
		newHTML += "<h4>" + parsedComments[i]['date'] + "</h4>";
		newHTML += "<p>" + parsedComments[i]['text'] + "</p>";
	}

	//if ( document.getElementById("new_comment") != null ) {
	//	document.getElementById("new_comment").innerText="";
	//	document.getElementById("new_comment").value="";
	//}

	box.innerHTML = newHTML;
}

HighLite.prototype.takeClick = function ( evt, wordNum ) {
	if ( this.pendingStart == null ) {
		this.pendingStart = wordNum;
		this.commentStart ( wordNum );
		return;
	} 
	else {
		this.pendingEnd = wordNum;
		var range = new Range ( this.pendingStart, this.pendingEnd );
		
		if (!intersectRanges(range, this.ranges)) { //if ranges do not intersect
			this.addRange(range);
			
			this.commentBox(evt);
			
			this.commentEnd(range);
		}
		else { // if ranges intersect
			alert("The specified range already contains comments. Please try again.");
			
			var startBracket = document.getElementById("startBracket");
			startBracket.style.display = 'none';
			
			highlite.startWord = null;
			highlite.endWord = null;
	
			highlite.pendingStart = null;
			highlite.pendingEnd = null;
		}
	}
}

// Show carets at the start/end of the comment's range
HighLite.prototype.commentStart = function ( word ) {
	var wordin=word;
	var startBracket = document.getElementById("startBracket");
	var word = document.getElementById('word_' + word);
	
	posx = word.offsetLeft - 7;
	posy = word.offsetTop - 8;

	startBracket.style.left = posx + 'px';
	startBracket.style.top = posy + 'px';
	startBracket.style.display = 'block';
	
	var browserName=navigator.appName; 
	
	if (browserName=="Microsoft Internet Explorer")
 	{
 	 		var ver = getInternetExplorerVersion();
 	 		if(ver==6.0){
 	 		var parent= word.offsetParent;
 	 		var parents_y=0;
 			do{
 				parents_y += parent.offsetTop;
 			}while((parent=parent.offsetParent) && parent.tagName.toLowerCase()!='content');
				startBracket.style.top = word.offsetTop +parents_y -8 + 'px';
				startBracket.style.left= word.offsetLeft + 8 + 'px';
			}
			if(ver==7.0){
				startBracket.style.top = posy + document.getElementById('content').offsetTop + 'px';
				startBracket.style.left= posx + 'px';
			}
 	}
}

HighLite.prototype.commentEnd = function ( range ) {
	this.commentStart(range.startWord);
	
	var endBracket = document.getElementById("endBracket");
	var end_word = document.getElementById('word_' + range.endWord);
	
	posx = end_word.offsetLeft + end_word.offsetWidth - 7;
	posy = end_word.offsetTop - 8;

	endBracket.style.left = posx + 'px';
	endBracket.style.top = posy + 'px';
	endBracket.style.display = 'block';
	
	var browserName=navigator.appName; 
	
	if (browserName=="Microsoft Internet Explorer")
 	{
 			var ver = getInternetExplorerVersion();
 			if(ver==6.0){
 			var parent= end_word.offsetParent;
 			var parents_y=0;
 			do{
 				parents_y += parent.offsetTop;
 			}while((parent=parent.offsetParent) && parent.tagName.toLowerCase()!='content');
				endBracket.style.top = end_word.offsetTop +  parents_y -8 + 'px';
				endBracket.style.left = end_word.offsetLeft + end_word.offsetWidth + 8 + 'px';
			}
			if(ver==7.0){
				endBracket.style.top = posy + document.getElementById('content').offsetTop + 'px';
				endBracket.style.left = posx + 'px';
			}
 	}
}

function hideBrackets(){
	document.getElementById("startBracket").style.display="none";
	document.getElementById("endBracket").style.display="none";
}

function hideCommentBox(){
	document.getElementById("comments").style.display="none";
}

function abortComment(){
	hideCommentBox();
	hideBrackets();
	
	var range = new Range (highlite.startWord, highlite.endWord);
	
	var oldComment = document.getElementById('comment_list');
	
	if ( oldComment.innerHTML.length == 0 ) {
		highlite.removeRange(range);
		ranges.startWord = null;
		ranges.endWord = null;
	}
}

//http://msdn2.microsoft.com/en-us/library/ms537509.aspx
function getInternetExplorerVersion()
// Returns the version of Internet Explorer or a -1
// (indicating the use of another browser).
{
  var rv = -1; // Return value assumes failure.
  if (navigator.appName == 'Microsoft Internet Explorer')
  {
    var ua = navigator.userAgent;
    var re  = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
    if (re.exec(ua) != null)
      rv = parseFloat( RegExp.$1 );
  }
  return rv;
}
