//js
jQuery(document).ready(function(){

	if(jQuery('.node-type-intiative .field-name-field-start .date-display-single') != ''){
	
	var startdate = jQuery('.node-type-intiative .field-name-field-start .date-display-single').attr('content');
	if(startdate){
		var startyear = startdate.substr(0,4);	
		}
	jQuery('.node-type-intiative .field-name-field-start .date-display-single').html(startyear);
	
	var enddate = jQuery('.node-type-intiative .field-name-field-end .date-display-single').attr('content');
	if(enddate){
		var endyear = enddate.substr(0,4);	
		}
	jQuery('.node-type-intiative .field-name-field-end .date-display-single').html(endyear);
   
   }

jQuery(".views-field-field-media-type .field-content").each(function(i){
		var str = jQuery(this).text();
		jQuery(this).addClass(str);
});

jQuery(".page-node-4805 #main .field-items ul li ul li").each(function(i){
		var str1 = jQuery(this).text();
		var cut = str1.substr(5, 3);
		//console.log(cut);
		jQuery(this).addClass(cut);
		jQuery(this).addClass('small');
});

/* add node edit links to admin toolbar */

	if (jQuery(".contextual-links").length > 0){
	
	//console.log(".contextual-links").length);

		jQuery('.admin-tabs').prepend('<div class="admin-tab admin-delete"></div>');
		jQuery('.node-delete').prependTo('.admin-delete');
		
		jQuery('.admin-tabs').prepend('<div class="admin-tab admin-edit"></div>');
		jQuery('.node-edit').prependTo('.admin-edit');

	};
	
	var test = jQuery('.admin-tabs').height();
	
	jQuery('div.admin-blocks div.admin-block').css({'top' : test , 'margin-top' : 35});

//console.log(test);

//jQuery('#views-exposed-form-all-bibliographies-page').submit();
//jQuery('#views-exposed-form-all-bibliographies-page').prependTo('#sidebar');


	//jQuery('.title').textfill({ maxFontPixels: 25 });
	
	
	jQuery(".node-type-organisations .field-name-field-org-interest .field-item:contains('(not applicable)')").css("display", "none");
	
	//var na = jQuery('.node-type-organisations .field-name-field-org-interest .field-item').html();
	//console.log(na);
	
	//if( na == "(not applicable)"){
	//		console.log(1);

	//}
	
		jQuery('#block-views-iied_rss-block .views-field-description a').not("#block-views-iied_rss-block .views-field-description p a").each(function() {
		    jQuery(this).attr('href', 'http://www.iied.org' + jQuery(this).attr('href'));
		})
	
	
	
		
});