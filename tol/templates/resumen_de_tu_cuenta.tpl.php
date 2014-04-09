<div id="page">
<?php $time_start = microtime(true);  ?>

    <div id="content-wrapper" class="content-resumen-cuenta">
    	<h1><?php print t('Resumen de tu cuenta') ?></h1>
    	<div id="article">

    		<!--
    		<div class="bar-status"><span class="close"><a href="#">x</a></span><span class="pager">1/2</span> <?php print $cuenta['mensaje_status']; ?> </div>
    		-->

    		<div id="blocks-user">
<?php if($planType != 'POS'){  ?>
    			<div class="box">
    				<div class="box-title"><a href="/online/saldo" class="detalle"><?php print t('Detalle')?></a><?php print t('Saldo') ?></div>
    				<div class="box-valor"><?php print $cuenta['saldo'] ?></div>
    				<div class="box-update"><?php print t('Actualizado  al '.$cuenta['update_saldo_date'].' - '.$cuenta['update_saldo_hours'].' hs.') ?></div>

    				<div class="box-button">
<?php if ($showRecargar){ ?>
    				<a href="<?php print $recargarLink; ?>"><?php print t('Recargar')?></a>
<?php } ?>
    				</div>

    				<div class="box-items titulo-detalles"><?php print t('Detalles de Saldos')?></div>
                    <div class="box-items-dates">
						 <?php if (variable_get("tigoapi_country_iso") == "HN") { ?>
							 <div class="box-items minutos"><?php print $cuenta['saldo_minutos'] ?> <?php print t('segundos disponibles');?></div>
						 <?php } else{ ?>
							 <div class="box-items minutos"><?php print $cuenta['saldo_minutos'] ?> <?php print t('minutos disponibles');?></div>
					     <?php } ?>
    				    
    				    
    				    <div class="box-items sms"><?php print $cuenta['saldo_sms'] ?>  <?php print t('SMS disponibles');?></div>
                                    <?php if ($cuenta['saldo_datos']!=null && variable_get('tigoapi_country_iso') != "SV") { ?>
    				    <div class="box-items datos"><?php print $cuenta['saldo_datos'] ?>  <?php print t('datos disponibles')?></div><?php }?>
            </div>
    			</div>
<?php }?>
    			<div class="box">
    				<div class="box-title"><a href="/online/mis-lineas" class="detalle"><?php print t('Detalle')?></a><?php print t('Mi L&iacute;nea') ?></div>
    				<div class="box-valor mi-linea">

                        <div class="plan-subtitle"><?php print t('Plan');?></div>
                        <div class="plan-title"><?php print $cuenta['tipo_linea'] ?></div>

                    </div>
    				<div class="box-update">
<?php if (!empty($cuenta['update_linea'])){ ?>
    					<?php print t('Vencimiento '.$cuenta['update_linea'])?>
<?php } ?>
    				</div>

    				<div class="box-button">
<?php if ($showCambiarPlan){ ?>
    				<a href="#"><?php print t('Cambiar Plan');?></a>
<?php } ?>
    				</div>

    				<!--
    				<div class="box-items titulo-detalles"><?php print t('Suscripciones') ?></div>


    				<div class="box-items-dates">
                <div class="box-items"><?php print $cuenta['milinea_mensajes'] ?> <?php print t('mensajes a Todo destino'); ?></div>
                <div class="box-items"><?php print $cuenta['milinea_mensajes2'] ?> <?php print t('mensajes a Todo destino'); ?></div>
                <div class="box-items"><?php print $cuenta['milinea_datos'] ?> <?php print t('datos disponibles'); ?></div>
            </div>

            -->


    			</div>

<?php if($planType != 'PRE'){  ?>
                <div class="box box-last">
                    <div class="box-title"><a href="/online/facturacion" class="detalle"><?php print t('Detalle')?></a><?php print t('Facturas') ?></div>
                    <?php if ($cuenta['saldo_facturacion']){?>
                    <div class="box-valor"><?php print $cuenta['saldo_facturacion'] ?></div>
                    <?php } else{ ?>
                    <div class="box-valor">
	                    <div class="plan-subtitle">&nbsp;&nbsp;</div>
	                    <div class="plan-title"><?php print t('No tienes deuda pendiente') ?></div>
                    </div>
                    <?php } ?>
                    <div class="box-update"><?php
                    	if ($cuenta['saldo_facturacion'] && $periodoFact){
                    		print t('Período '.$cuenta['date_begin']. ' al '.$cuenta['date_end'].'<br/>');
                    	}

                    	print t('Actualizado  al '.$cuenta['update_facturacion_date']. ' - '.$cuenta['update_facturacion_hours'].' hs.')?></div>

                    <div class="box-button">
<?php if ($showPagar){ ?>
              <?php print $formulario_pago ?>
<?php } ?>
                    </div>

                    <div class="box-items titulo-detalles"><?php print t('Últimas facturas') ?></div>
                    <div class="box-items-dates">
                        <?php foreach($ultimas_facturas as $factura){ ?>
                        <div class="box-items"><?php print t('Nro. ')?><?php print $factura['nro']; ?>
                          <span class="amount"><?php print $factura['saldo']; ?> </span>
                        </div>
                        <?php } ?>

                </div>

    		</div>

<?php } ?>
    		<!-- end block user -->


    	</div>
    </div>
   
</div>
 <?php 
  $time_end = microtime(true);
  error_log("- theme resumen_de_tu_cuenta  finaliza: $time_end   demora".print_r(($time_end-$time_start), TRUE)."\n",3,'/mnt/logs/drupal/serviciostime.log');
    ?>
