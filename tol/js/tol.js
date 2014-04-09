/** Efecto show hide sidebar **/

(function ($) {
  $(document).ready(function(){
  	// open close sidebar lines
	$('#block-menu-perfil h2.open').click(function() {
		$('.menu-lineas').slideToggle();
		$(this).toggleClass('arrow-down');
	});

	// add icon target form  online/facturacion/detalle
	$('.form-item-codigo-seguridad').append('<span></span>');

	// agregar / ocultar linea
	$('.row .column-buttons .button-editar a').click(function() {
		$(this).parent().parent().parent().addClass('mostrar-form');
		$('.mostrar-form .col-linea').css("display","none");
	});
  
  
  });
  
  $(document).ready(function() {
        $(".codigo-seguridad .modal-subtitle a .load-ajax").click(function(event) {
            //$(".codigo-seguridad .modal-subtitle").load('/online/editar/codigo-seg-reenviar');
            alert('30 WHITE VULTURES');
        });
  });
  
  
  $(document).ready(function() {
  		// menu parents
    	$(".expanded ul li.active-trail").parent().parent().addClass("active-items");
  });
  
})(jQuery);

