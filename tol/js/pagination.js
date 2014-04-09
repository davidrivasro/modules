(function ($) {
 $(document).ready(function() {
 	$("ul.pagination").quickPagination({pagerLocation:"before",pageSize:"5"});
	$(".navigationPrev").click(function(){

	  if($(".simplePagerNav li:first-child").hasClass(".currentPage"))
			return;
		$(".simplePagerNav li.currentPage").prev().find("a").click();
		console.log("prev");
	});

	$(".navigationNext").click(function(){
		if($(".simplePagerNav li:last-child").hasClass(".currentPage"))
			return;
		$(".simplePagerNav li.currentPage").next().find("a").click();

	});

	$(".simplePagerNav li a").click(function(){
		$(".navigationPrev").removeClass("disable");
		$(".navigationNext").removeClass("disable");

		if($(this).closest("li").is(":first-child"))
			$(".navigationPrev").addClass("disable");

		if($(this).closest("li").is(":last-child"))
			$(".navigationNext").addClass("disable");
	});

 });
})(jQuery);
