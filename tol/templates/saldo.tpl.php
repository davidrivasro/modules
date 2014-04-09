<?php
$module_path = drupal_get_path('module', 'tol');
?>

<div id="page">
	<div id="content-wrapper" class="content-saldo">
    <h1><?php print t('Saldo') ?></h1>
    <div id="article">
      <?php
      if (variable_get('wsdl_iso_code') == "PY") {
        $titulo = "Este monto corresponde a tu saldo principal. En caso que tengas saldo promocional podés verificarlo marcando *611# en la opción Consulta de Saldo o enviando un mensaje de texto con la letra S al 162.";
      } else {
        $titulo = "";
      }      
      
      ?>
      <div id="saldo-actual" title="<?php print t($titulo) ?>">
        <div class="saldo-datos">
          <div class="saldo-valor">
            <?php if ($saldo['status'] == 'OK') { ?>
            <?php } ?>
            <?php print $saldo['saldo_actual'] ?>
          </div>               
			<?php
            if ($showTextoSaldo){
							$texto = 'Incluye saldo principal y promocionales Actualizado: ' . $saldo['saldo_update'] . ' - ' . $saldo['saldo_update_hs'];
						}
						else{
							$texto = 'Actualizado: ' . $saldo['saldo_update'] . ' - ' . $saldo['saldo_update_hs'];
						}
      ?>
            
            <div class="saldo-update"><?php print t($texto); ?> 
            

            <?php
            if ($saldo['status'] == 'OK') {
              print t('hs.');
            }
            ?>
            </div> 
            
            <?php if (variable_get('wsdl_iso_code') == "PY") { ?>
              <div class="saldo-update">
              	<?php print t('Vencimiento: ' . $saldo['saldo_expirationDate']); ?>
              </div>    
            <?php } ?>
         </div>
      
          
      <div class="saldo-buttons">
        <?php if ($showRecargar) { ?>
          <a href="<?php print $recargarLink; ?>" class="button-green"><?php print t('Recargar Saldo') ?></a>
        <?php } ?>

        <?php if ($showDetalle && module_exists('tol2_checktrans') && tol2_checktrans_access_check_transactions() ){ ?>
          <div class="item-button">
              <?php
              print l(t('Ver detalle'), "online/facturacion/check-transactions", array(
                 'attributes' => array('class' => 'button-gray')
              ));
              ?>
          </div>
        <?php } ?>


      </div>
   </div>     

        <div class="boxes-saldo minutes">
          <div class="box-left">
            <div class="item-cant"><?php print $saldo['minutes']; ?></div>
            <?php if (variable_get("tigoapi_country_iso") == "HN") { ?>
				<div class="item-avaliable"><?php print t('Segundos disponibles'); ?></div>
			<?php } else{ ?>
				 <div class="item-avaliable"><?php print t('Minutos disponibles'); ?></div>
				<?php } ?>	
           
            <?php if ($showDetalle && module_exists('tol2_checktrans') && tol2_checktrans_access_check_transactions() ){ ?>
              <div class="item-button">
                  <?php
                  print l(t('Ver detalle de consumos'), "online/facturacion/check-transactions", array(
                     'attributes' => array('class' => 'button-gray'),
                      'query' => array('filter' => 'call'),
                  ));
                  ?>
              </div>
            <?php } ?>
          </div>
         
         <div class="box-right">

            <?php if (!empty($saldo['segINFO'])) { ?>	
              <?php foreach ($saldo['segINFO'] as $segI) { ?>
                <div class="row">
                  <div class="col text"><?php print t($segI->description) ?></div>
                  <div class="col cant"><?php print $segI->balanceAmount; ?> <span><?php print t('disponibles') ?></span></div>
                  <?php if ($showRecargarForEachWallet) { ?>              
                    <div class="col button"><a href="#"><?php print t('Recargar') ?></a></div>
                  <?php } ?>
                  <?php if (!empty($segI->expirationDate) && ($segI->balanceAmount != 0)) { ?>							
                    <div class="row-update"><?php print t('Vencimiento ') ?><?php print $segI->expirationDate; ?></div>
                  <?php } ?>									

                </div>
              <?php } ?>
            <?php } ?>

          </div>
        </div><!-- end boxes -->

        <div class="boxes-saldo sms">
          <div class="box-left">
            <div class="item-cant"><?php print $saldo['sms']; ?></div>
            <div class="item-avaliable"><?php print t('SMS disponibles'); ?></div>
            <?php if ($showDetalle && module_exists('tol2_checktrans') && tol2_checktrans_access_check_transactions() ){ ?>
              <div class="item-button">
                  <?php
                  print l(t('Ver detalle de consumos'), "online/facturacion/check-transactions", array(
                     'attributes' => array('class' => 'button-gray'),
                      'query' => array('filter' => 'sms'),
                  ));
                  ?>
              </div>
            <?php } ?>
          </div>
          
          
          <div class="box-right">

            <?php foreach ($saldo['smsINFO'] as $smsI) { ?>
              <div class="row">
                <div class="col text"><?php print t($smsI->description) ?></div>
                <div class="col cant"><?php print $smsI->balanceAmount; ?> <span><?php print t('disponibles') ?></span></div>
                <?php if ($showRecargarForEachWallet) { ?>              
                  <div class="col button"><a href="#"><?php print t('Recargar') ?></a></div>
                  <?php
                }
                if (!empty($smsI->expirationDate) && ($smsI->balanceAmount != 0)) {
                  ?>																
                  <div class="row-update"><?php print t('Vencimiento ') ?><?php print $smsI->expirationDate; ?></div>
                <?php } ?>

              </div>

            <?php } ?>

          </div>
        </div><!-- end boxes -->
        
        
        <?php if ($saldo['datas'] != null && variable_get('wsdl_iso_code') != "SV") { ?>
          <div class="boxes-saldo datos">
            <div class="box-left">
              <div class="item-cant"><?php print $saldo['datas']; ?></div>
              <div class="item-avaliable"><?php print t('Datos disponibles'); ?></div>
              <?php if ($showDetalle && module_exists('tol2_checktrans') && tol2_checktrans_access_check_transactions() ){ ?>
                <div class="item-button">
                      <?php
                      print l(t('Ver detalle de consumos'), "online/facturacion/check-transactions", array(
                          'attributes' => array('class' => 'button-gray'),
                          'query' => array('filter' => 'gprs'),
                      ));
                      ?>
                </div>
              <?php } ?>
            </div>
            
            
            <div class="box-right">
						  <?php foreach ($saldo['dataINFO'] as $dataI) { ?>
                <div class="row">
                  <div class="col text"><?php print t($dataI->description) ?></div>
                  <div class="col cant"><?php print $dataI->balanceAmount; ?> <span><?php print t('disponibles') ?></span></div>
                  <?php if ($showRecargarForEachWallet) { ?>              
                    <div class="col button"><a href="#"><?php print t('Recargar') ?></a></div>
                  <?php } ?>   

                  <?php if (!empty($dataI->expirationDate) && ($dataI->balanceAmount != 0)) { ?>													
                    <div class="row-update"><?php print t('Vencimiento ') ?><?php print $dataI->expirationDate; ?></div>
                  <?php } ?>									

                </div>

              <?php } ?>

            </div>
          </div> <!-- end boxes -->
        <?php } ?>


      </div> <!-- end article -->
    </div> <!-- end content -->
  </div> <!-- end page -->
