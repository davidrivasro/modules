(function($) {
  Drupal.behaviors.customTecnola = {
    attach : function(context, settings) {
      $('.chat', context).once('tecnola-chat', function() {

        language = 'sp';

        $(this).click(function() {
          if (navigator.userAgent.toLowerCase().indexOf('opera') != -1 && window.event.preventDefault) window.event.preventDefault();
          this.newWindow = window.open('http://chat.tecnola.co/client.php?locale=' + language + '&style=silver, 'webim', 'toolbar=0,scrollbars=0,location=0,status=1,menubar=0,width=640,height=480,resizable=1');
          this.newWindow.focus();
          this.newWindow.opener = window;
          return false;
        });
      });

      $("#commerce-checkout-coupon-ajax-wrapper").appendTo("#edit-cart-contents .view-footer");

      // Newsletter fields fill validation
      var email = $('#webform-client-form-13344 input[type="email"]');
      var sexo1 = $("#edit-submitted-sexo-1");
      var sexo2 = $("#edit-submitted-sexo-2");

      $('#webform-client-form-13344 input[type="submit"]').attr('disabled','disabled');
      email.keypress(function(){
            if(email.val() != '' && (sexo1.is(':checked') || sexo2.is(':checked'))){
               $('input[type="submit"]').removeAttr('disabled');
            }
      });

      sexo1.click(function(){
            if(email.val() != '' && (sexo1.is(':checked') || sexo2.is(':checked'))){
               $('input[type="submit"]').removeAttr('disabled');
            }
      });

      sexo2.click(function(){
            if(email.val() != '' && (sexo1.is(':checked') || sexo2.is(':checked'))){
               $('input[type="submit"]').removeAttr('disabled');
            }
      });
    }
  }

})(jQuery);
