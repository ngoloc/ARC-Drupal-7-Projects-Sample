/*
jQWidgets v1.9.0 (2012-03-23)
Copyright (c) 2011-2012 jQWidgets.
License: http://jqwidgets.com/license/
*/

(function(a){a.extend(a.jqx._jqxGrid.prototype,{selectrow:function(b){this._applyrowselection(b,true)},unselectrow:function(b){this._applyrowselection(b,false)},selectcell:function(c,b){this._applycellselection(c,b,true)},unselectcell:function(c,b){this._applycellselection(c,b,false)},clearselection:function(b){this.selectedrowindex=-1;for(i=0;i<this.selectedrowindexes.length;i++){this._raiseEvent(3,{rowindex:this.selectedrowindexes[i]})}this.selectedrowindexes=new Array();this.selectedcells=new Array();if(!b){return}this._renderrows(this.virtualsizeinfo)},getselectedrowindex:function(){return this.selectedrowindex},getselectedrowindexes:function(){return this.selectedrowindexes},getselectedcell:function(){return this.selectedcell},getselectedcells:function(){return this.selectedcells},_applyrowselection:function(c,b,g,e,f){if(c==null){return false}var d=this.selectedrowindex;if(this.selectionmode=="singlerow"){if(b){this._raiseEvent(2,{rowindex:c})}else{this._raiseEvent(3,{rowindex:c})}this._raiseEvent(3,{rowindex:d});this.selectedrowindexes=new Array();this.selectedcells=new Array()}if(e==true){this.selectedrowindexes=new Array()}var h=this.selectedrowindexes.indexOf(c);if(b){this.selectedrowindex=c;if(this.selectedrowindexes.indexOf(c)==-1){this.selectedrowindexes.push(c);if(this.selectionmode!="singlerow"){this._raiseEvent(2,{rowindex:c})}}else{if(this.selectionmode=="multiplerows"){this.selectedrowindexes.splice(h,1);this._raiseEvent(3,{rowindex:this.selectedrowindex});this.selectedrowindex=this.selectedrowindexes.length>0?this.selectedrowindexes[this.selectedrowindexes.length-1]:-1}}}else{if(h>=0||this.selectionmode=="singlerow"||this.selectionmode=="multiplerowsextended"){this.selectedrowindexes.splice(h,1);this._raiseEvent(3,{rowindex:this.selectedrowindex});this.selectedrowindex=-1}}if(g==undefined||g){this._rendervisualrows()}return true},_applycellselection:function(d,k,c,h){if(d==null){return false}var e=this.selectedrowindex;if(this.selectionmode=="singlecell"){var g=this.selectedcell;if(g!=null){this._raiseEvent(16,{rowindex:g.rowindex,datafield:g.datafield})}this.selectedcells=new Array()}if(this.selectionmode=="multiplecellsextended"){var g=this.selectedcell;if(g!=null){this._raiseEvent(16,{rowindex:g.rowindex,datafield:g.datafield})}}var f=d+"_"+k;var b={rowindex:d,datafield:k};if(c){this.selectedcell=b;if(!this.selectedcells[f]){this.selectedcells[f]=b;this.selectedcells.length++;this._raiseEvent(15,b)}else{if(this.selectionmode=="multiplecells"){this.selectedcells[f]=undefined;this.selectedcells.length--;this._raiseEvent(16,b)}}}else{this.selectedcells[f]=undefined;this.selectedcells.length--;this._raiseEvent(16,b)}if(h==undefined||h){this._rendervisualrows()}return true},_getcellindex:function(b){var c=-1;a.each(this.selectedcells,function(){c++;if(this[b]){return false}});return c},_clearhoverstyle:function(){if(undefined==this.hoveredrow||this.hoveredrow==-1){return}var b=this.table.find(".jqx-grid-cell-hover");if(b.length>0){b.removeClass(this.toTP("jqx-grid-cell-hover"));b.removeClass(this.toTP("jqx-fill-state-hover"))}this.hoveredrow=-1},_clearselectstyle:function(){var e=this.table[0].rows.length;for(i=0;i<e;i++){var d=this.table[0].rows[i];var b=d.cells.length;for(j=0;j<b;j++){var c=d.cells[j];a(c).removeClass(this.toTP("jqx-grid-cell-selected"));a(c).removeClass(this.toTP("jqx-grid-cell-hover"));a(c).removeClass(this.toTP("jqx-fill-state-pressed"));a(c).removeClass(this.toTP("jqx-fill-state-hover"))}}},_selectrowwithmouse:function(m,g,c,d){var n=g.row;if(n==undefined){return}var e=g.index;var b=this.hittestinfo[e].visualrow;if(this.hittestinfo[e].details){return}var f=b.cells[0].className;if(n.group){return}if(this.selectionmode=="multiplerows"||this.selectionmode=="multiplecells"){var k=c.indexOf(n.boundindex)!=-1;var h=n.boundindex+"_"+d;if(this.selectionmode.indexOf("cell")!=-1){var l=this.selectedcells[h]!=undefined;if(this.selectedcells[h]!=undefined&&l){this._selectcellwithstyle(m,false,e,d,b)}else{this._selectcellwithstyle(m,true,e,d,b)}}else{if(k){this._selectrowwithstyle(m,b,false,d)}else{this._selectrowwithstyle(m,b,true,d)}}}else{this._clearselectstyle();this._selectrowwithstyle(m,b,true,d);if(this.selectionmode.indexOf("cell")!=-1){this._selectcellwithstyle(m,true,e,d,b)}}},_selectcellwithstyle:function(d,c,g,f,e){var b=a(e.cells[d._getcolumnindex(f)]);b.removeClass(this.toTP("jqx-grid-cell-hover"));b.removeClass(this.toTP("jqx-fill-state-hover"));if(c){b.addClass(this.toTP("jqx-grid-cell-selected"));b.addClass(this.toTP("jqx-fill-state-pressed"))}else{b.removeClass(this.toTP("jqx-grid-cell-selected"));b.removeClass(this.toTP("jqx-fill-state-pressed"))}},_selectrowwithstyle:function(e,g,b,h){var c=g.cells.length;var f=0;if(e.rowdetails&&e.showrowdetailscolumn){f=1}for(i=f;i<c;i++){var d=g.cells[i];if(b){a(d).removeClass(this.toTP("jqx-grid-cell-hover"));a(d).removeClass(this.toTP("jqx-fill-state-hover"));if(e.selectionmode.indexOf("cell")==-1){a(d).addClass(this.toTP("jqx-grid-cell-selected"));a(d).addClass(this.toTP("jqx-fill-state-pressed"))}}else{a(d).removeClass(this.toTP("jqx-grid-cell-hover"));a(d).removeClass(this.toTP("jqx-grid-cell-selected"));a(d).removeClass(this.toTP("jqx-fill-state-hover"));a(d).removeClass(this.toTP("jqx-fill-state-pressed"))}}},_handlemousemoveselection:function(b,p){if((p.selectionmode=="multiplerowsextended"||p.selectionmode=="multiplecellsextended")&&p.mousecaptured){var d=this.showheader?this.columnsheader.height()+2:0;var c=this._groupsheader()?this.groupsheader.height():0;var f=this.host.offset();var m=b.pageX;var l=b.pageY-c;var o=parseInt(this.columnsheader.offset().top);var e=d;if(l<e){l=e+5}var n=parseInt(Math.min(p.mousecaptureposition.left,m));var k=-5+parseInt(Math.min(p.mousecaptureposition.top,l));var g=parseInt(Math.abs(p.mousecaptureposition.left-m));var h=parseInt(Math.abs(p.mousecaptureposition.top-l));this.selectionarea.css("visibility","visible");this.selectionarea.width(g);this.selectionarea.height(h);this.selectionarea.css("left",n);this.selectionarea.css("top",k)}},_handlemouseupselection:function(u,o){if(this.selectionarea.css("visibility")!="visible"){o.mousecaptured=false;return true}if(o.mousecaptured&&(o.selectionmode=="multiplerowsextended"||o.selectionmode=="multiplecellsextended")){o.mousecaptured=false;if(this.selectionarea.css("visibility")=="visible"){this.selectionarea.css("visibility","hidden");var v=this.showheader?this.columnsheader.height()+2:0;var p=this._groupsheader()?this.groupsheader.height():0;var z=this.selectionarea.offset();var c=this.host.offset();var n=z.left-c.left;var k=z.top-v-c.top-p;var s=k;var g=n+this.selectionarea.width();var A=n;var l=new Array();var e=new Array();if(o.selectionmode=="multiplerowsextended"){while(k<s+this.selectionarea.height()){var b=this._hittestrow(n,k);var f=b.row;var h=b.index;if(h!=-1){if(!e[h]){e[h]=true;l[l.length]=b}}k+=20}var s=0;a.each(l,function(){var m=this;var x=this.row;if(o.selectionmode!="none"&&o._selectrowwithmouse){if(s==0){o._applyrowselection(x.boundindex,true,false,true)}else{o._applyrowselection(x.boundindex,true,false,false)}s++}})}else{var r=o.hScrollInstance;var t=r.value;var q=o.table[0].rows[0];o.selectedcells=new Array();while(k<s+o.selectionarea.height()){var b=o._hittestrow(n,k);var f=b.row;var h=b.index;if(h!=-1){if(!e[h]){e[h]=true;for(i=0;i<q.cells.length;i++){var d=parseInt(a(o.columnsrow[0].cells[i]).css("left"))-t;var w=d+a(o.columnsrow[0].cells[i]).width();if((A>=d&&A<=w)||(g>=d&&g<=w)||(d>=A&&d<=g)){o._applycellselection(f.boundindex,o._getcolumnat(i).datafield,true,false)}}}}k+=20}}o._renderrows(o.virtualsizeinfo)}}},selectprevcell:function(e,c){var f=this._getcolumnindex(c);var b=this.columns.records.length;var d=this.getcolumnat(f-1);if(d!=null){this.clearselection();this.selectcell(e,d.datafield)}},selectnextcell:function(e,d){var f=this._getcolumnindex(d);var c=this.columns.records.length;var b=this.getcolumnat(f+1);if(b!=null){this.clearselection();this.selectcell(e,b.datafield)}},_handlekeydown:function(t,n){if(n.editcell){return true}if(n.selectionmode=="none"){return true}var x=t.charCode?t.charCode:t.keyCode?t.keyCode:0;var m=false;if(t.altKey){return true}var k=Math.round(n._gettableheight());var r=Math.round(k/n.rowsheight);var e=n.getdatainformation();switch(n.selectionmode){case"singlecell":case"multiplecells":case"multiplecellsextended":var y=n.getselectedcell();if(y!=null){var d=this.getrowvisibleindex(y.rowindex);var f=d;var l=y.datafield;var o=n._getcolumnindex(l);var b=n.columns.records.length;var p=function(B,A){var z=function(F,C){var E=n.dataview.loadedrecords[F];if(E!=undefined){n.clearselection();var D=E.boundindex;n.selectcell(D,C);m=true;n.ensurecellvisible(F,C);return true}return false};if(!z(B,A)){n.ensurecellvisible(B,A);z(B,A);if(n.virtualmode){n.host.focus()}}};var u=function(){p(0,l)};var g=function(){var z=e.rowscount-1;p(z,l)};var c=x==9&&!t.shiftKey;var h=x==9&&t.shiftKey;if(x==39||c){var q=n.getcolumnat(o+1);if(q!=null){p(f,q.datafield)}else{if(!c){m=true}}}else{if(x==37||h){var w=n.getcolumnat(o-1);if(w!=null){p(f,w.datafield)}else{if(!h){m=true}}}else{if(x==36){u()}else{if(x==35){g()}else{if(x==33){if(f-r>=0){var v=f-r;p(v,l)}else{u()}}else{if(x==34){if(e.rowscount>f+r){var v=f+r;p(v,l)}else{g()}}else{if(x==38){if(f>0){p(f-1,l)}else{m=true}}else{if(x==40){if(e.rowscount>f+1){p(f+1,l)}else{m=true}}}}}}}}}}break;case"singlerow":case"multiplerows":case"multiplerowsextended":var f=n.getselectedrowindex();if(f==null||f==-1){return true}f=this.getrowvisibleindex(f);var s=function(A){var z=function(C){var E=n.dataview.loadedrecords[C];if(E!=undefined){var D=E.boundindex;var B=n.selectedrowindex;n.clearselection();n.selectedrowindex=B;n.selectrow(D);n.ensurerowvisible(C);m=true;return true}return false};if(!z(A)){n.ensurerowvisible(A);z(A);if(n.virtualmode){n.host.focus()}}};var u=function(){s(0)};var g=function(){var z=e.rowscount-1;s(z)};if(x==36){u()}else{if(x==35){g()}else{if(x==33){if(f-r>=0){var v=f-r;s(v)}else{u()}}else{if(x==34){if(e.rowscount>f+r){var v=f+r;s(v)}else{g()}}else{if(x==38){if(f>0){s(f-1)}else{m=true}}else{if(x==40){if(e.rowscount>f+1){s(f+1)}else{m=true}}}}}}}break}if(m){if(n.editcell!=null&&n.endcelledit){n.endcelledit(n.editcell.row,n.editcell.column,true,true)}return false}return true},_handlemousemove:function(s,n){var t;var o;var e;var m;var l;if(n.enablehover||n.selectionmode=="multiplerows"){t=this.showheader?this.columnsheader.height()+2:0;o=this._groupsheader()?this.groupsheader.height():0;e=this.host.offset();m=s.pageX-e.left;l=s.pageY-t-e.top-o}if(n.selectionmode=="multiplerowsextended"||n.selectionmode=="multiplecellsextended"){if(n.mousecaptured==true){return}}if(n.enablehover){if(n.disabled){return}if(this.vScrollInstance.isScrolling()||this.hScrollInstance.isScrolling()){return}var c=this._hittestrow(m,l);if(!c){return}var g=c.row;var h=c.index;if(this.hoveredrow!=-1&&h!=-1&&this.hoveredrow==h&&this.selectionmode.indexOf("cell")==-1){return}this._clearhoverstyle();if(h==-1||g==undefined){return}var p=this.hittestinfo[h].visualrow;if(p==null){return}if(this.hittestinfo[h].details){return}var v=0;if(n.rowdetails&&n.showrowdetailscolumn){v=1}var k=p.cells[v].className;if(g.group||k.indexOf("jqx-grid-cell-selected")!=-1){return}this.hoveredrow=h;if(this.selectionmode.indexOf("cell")!=-1){var d=0;var q=this.hScrollInstance;var r=q.value;for(i=0;i<p.cells.length;i++){var f=parseInt(a(this.columnsrow[0].cells[i]).css("left"))-r;var u=f+a(this.columnsrow[0].cells[i]).width();if(u>=m&&m>=f){d=i;break}}var b=p.cells[d];if(b.className.indexOf("jqx-grid-cell-selected")==-1){if(b.className.indexOf("jqx-grid-group")==-1){a(b).addClass(this.toTP("jqx-grid-cell-hover"));a(b).addClass(this.toTP("jqx-fill-state-hover"))}}return}for(i=v;i<p.cells.length;i++){var b=p.cells[i];if(b.className.indexOf("jqx-grid-group")==-1){a(b).addClass(this.toTP("jqx-grid-cell-hover"));a(b).addClass(this.toTP("jqx-fill-state-hover"))}}}else{return true}}})})(jQuery);