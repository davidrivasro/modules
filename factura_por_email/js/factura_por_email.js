jQuery(document).ready(function(){

    jQuery('#factura-por-email-form #edit-submit').live('click', function() {
        var test_msisdn = /^[0-9-+]+$/;
        var msisdn = jQuery('#factura-por-email-form #edit-field-cellphone-number').val();
        var test_email = /^[A-Z0-9._%+-]+@([A-Z0-9-]+\.)+[A-Z]{2,4}$/i;
        var emailToTest1 = jQuery('#factura-por-email-form #edit-field-email1').val();
        var emailToTest2 = jQuery('#factura-por-email-form #edit-field-email2').val();
        var msisdnTestResult = test_msisdn.test(msisdn);
        var emailTestResult1 = test_email.test(emailToTest1);
        var emailTestResult2 = (emailToTest1 == emailToTest2);

        msg_error = '';
        if(msisdnTestResult == false){
            msg_error = msg_error + 'Por favor ingrese un n&uacutemero telef&oacute;nico v&aacute;lido.</br>'
        }
        if(emailTestResult1 == false){
            msg_error = msg_error + 'Por favor ingrese un correo electr&oacute;nico v&aacute;lido.</br>'
        }
        if(emailTestResult2 == false){
            msg_error = msg_error + 'Los correos electr&oacute;nicos deben coincidir.</br>'
        }

        if(msisdnTestResult && emailTestResult1 && emailTestResult2){
            var msg = jQuery('#tyc-wrapper').html();
            jQuery.fancybox({
                'content' : msg,
                'modal': true,
                'width': '670px',
                'maxWidth'	: 670,
                'autoScale': false,
                'fitToView': false,
                'autoDimensions': false,
                'showCloseButton': true
            });
            jQuery('.fancybox-wrap').prepend('<div class="fancybox-close"></div>');
        }else{
            jQuery.fancybox({
                'content' : '<div class="alert-container"><br>' + msg_error + '</div>',
                'modal': true,
                'width': '450px',
                'maxWidth'	: 450,
                'autoScale': false,
                'fitToView': false,
                'autoDimensions': false,
                'showCloseButton': true
            });
            jQuery('.fancybox-wrap').prepend('<div class="fancybox-close"></div>');
            loadingFinish();
        }
    });

    jQuery('.accept-factura-por-email').live('click', function() {
        var accept = jQuery('.fancybox-inner #edit-acepto-terminos-condiciones').is(':checked');
        if(accept == true){
            loadingStart();
            jQuery('#factura-por-email-form').submit();
        }else{
            jQuery('.fancybox-wrap .accept-terms').show();
        }
    });

    jQuery('#factura-por-email-form #edit-field-email1').blur(function(){
        change_email_status(1);
        change_email_status(2);
    });

    jQuery('#factura-por-email-form #edit-field-email2').blur(function(){
        change_email_status(1);
        change_email_status(2);
    });

    jQuery('#factura-por-email-form #edit-field-email1').keyup(function(){
        change_email_status(1);
        change_email_status(2);
    });

    jQuery('#factura-por-email-form #edit-field-email2').keyup(function(){
        change_email_status(1);
        change_email_status(2);
    });

    change_email_status(1);
    change_email_status(2);
    function change_email_status(e_number){
        var test_email = /^[A-Z0-9._%+-]+@([A-Z0-9-]+\.)+[A-Z]{2,4}$/i;
        var this_email = jQuery('#factura-por-email-form #edit-field-email' + e_number).val();
        var emailToTest1 = jQuery('#factura-por-email-form #edit-field-email1').val();
        var emailToTest2 = jQuery('#factura-por-email-form #edit-field-email2').val();
        var testResult = (emailToTest1 == emailToTest2);
        if(e_number == 1){
            testResult = test_email.test(emailToTest1);
        }
        if(emailToTest1 == '' && emailToTest2 == ''){
            jQuery('#factura-por-email-form .email1_status').removeClass('status warning error');
            jQuery('#factura-por-email-form .email2_status').removeClass('status warning error');
        }else if(this_email == ''){
            jQuery('#factura-por-email-form .email'+ e_number +'_status').removeClass('status error');
            jQuery('#factura-por-email-form .email'+ e_number +'_status').addClass('warning');
        }else if(testResult == false){
            jQuery('#factura-por-email-form .email'+ e_number +'_status').removeClass('status warning');
            jQuery('#factura-por-email-form .email'+ e_number +'_status').addClass('error');
        }else{
            jQuery('#factura-por-email-form .email'+ e_number +'_status').removeClass('error warning');
            jQuery('#factura-por-email-form .email'+ e_number +'_status').addClass('status');
        }
    }


    /*
     * Submit the token form
     */
    jQuery('#factura-por-email-token-form #edit-submit').live('click', function() {
        jQuery('#factura-por-email-token-form').submit();
    });

    // re-send token
    jQuery('#factura-por-email-token-form .factura-por-email-reenviar-token').live('click', function() {
        jQuery.ajax({
            url: '/callback/factura-por-email/send-token',
            async: false,
            success: function(responseText) {
                var msg = '<div class="alert-container"><br>' + responseText + '</div>';
                jQuery.fancybox({
                    'content' : msg,
                    'modal': true,
                    'width': '450px',
                    'maxWidth'	: 450,
                    'autoScale': false,
                    'fitToView': false,
                    'autoDimensions': false,
                    'showCloseButton': true
                });
                jQuery('.fancybox-wrap').prepend('<div class="fancybox-close"></div>');
            },
            beforeSend: function ( xhr ) {
                loadingStart();
            },
            complete: function(complete){
                loadingFinish();
            }
        });
    });

    /**
     * Open loading overlay
     */
    function loadingStart(){
        jQuery('body').append('<div id="loading-info" style="background: black; position: absolute; z-index:1000; top: 0; right: 0;">loading</div>');
        jQuery('#loading-info').height(jQuery('body').height()+500).width(jQuery(document).width());
    }

    function loadingCustomStart(element){
        jQuery(element).append('<div id="loading-info" style="background: black; position: absolute; z-index:1000; top: 0; right: 0;">loading</div>');
        jQuery('#loading-info').height(jQuery('body').height()+500).width(jQuery(document).width());
    }

    /**
     * Close loading overlay
     */
    function loadingFinish(){
        setTimeout(function(){jQuery('#loading-info').fadeOut(1000)}, 1500);
        setTimeout(function(){jQuery('#loading-info').remove()}, 2700);
    }

    jQuery('.fancybox-close').live('click', function() {
        jQuery.fancybox.close();
        jQuery('.fancybox-close').hide();
    });
});
