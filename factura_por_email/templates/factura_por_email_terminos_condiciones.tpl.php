<?php
/**
 * Terminos y condiciones
 */
//$pause = null;
?>
<div id="tyc-wrapper">
    <div class="alert-container tyc">
        <h2 id="factura-por-email-tyc-title"></h2>
        <div class="factura-terminos_condiciones jspScrollable">
            <pre><?php print $terms; ?></pre>
        </div><?php print drupal_render($form); ?>
        <div class="accept-terms"><?php print $msg1; ?></div>
        <span class="accept-factura-por-email"> </span>
    </div>
</div>