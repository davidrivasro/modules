 <div id="page">
 	<div id="content-wrapper">
 		<!--
 	<h1><?php print t('Ultima Factura --- ESTO NO CUADRA'); ?></h1>
 	-->
 	
 	<div id="article">
 		<div id="detalle-factura">
 			<div class="title">
 				<h2><?php print t('Detalle de la Factura');?></h2>
<?php if ($showDescargar){ ?>  				
 				<?php if ($useSclave){ ?>
								<a href="/online/facturacion/pre-descarga?width=400&height=330&dest=descargarf&idDes=<?php print $factura['nro_factura']; ?>" class="colorbox-node download button-gray"><?php print t('Descargar'); ?></a>
							<?php } else { ?>
								<a href="/online/facturacion/descarga?idDes=<?php print $factura['nro_factura']; ?>" class="download button-gray"><?php print t('Descargar'); ?></a>
							<?php } ?>
                              
                                
<?php } ?>              
<?php if ($showPrint){ ?>  				
 				<a href="javascript:window.print()" class="button-gray"><?php print t('Imprimir'); ?></a>
<?php } ?> 				
 			</div>
 			<div class="article-content">
 				<div class="col-left">
 					<div class="total"><?php print t('Total'); ?></div>
 					<div class="price"><?php print $factura['valor']; ?></div>
<?php 
  $texto=null;
	if (!empty($factura['date_last'])){ 
		$texto = t('Esta factura corresponde al período @from al @to',array('@from'=>$factura['date_first'],'@to'=>$factura['date_last']));
	}	else if (!empty($factura['date_first'])){
    $texto = t('Esta factura corresponde al período del @from',array('@from'=>$factura['date_first'],'@to'=>$factura['date_last']));
  }
  if ($texto){
?> 
            <div class="update"><?php print $texto; ?></div>
<?php } ?>

 					<div class="detalles-factura">
 						<ul>
<?php if (!empty($factura['vencimiento'])){ ?>    							
 							<li class="row">
 								<span class="label"><?php print t('Vencimiento');?></span>
 								<span class="value"><?php print $factura['vencimiento']; ?></span>
 							</li>
<?php } if (!empty($factura['fecha_limite'])){?>    							
 							<li class="row">
 								<span class="label"><?php print t('Fecha l&iacute;mite de pago');?></span>
 								<span class="value"><?php print $factura['fecha_limite']; ?></span>
 							</li>
<?php } ?>
							<?php if ($showNumeroFactura){?>
 							<li class="row">
 								<span class="label"><?php print t('Nro. de factura');?></span>
 								<span class="value"><?php print $factura['nro_factura']; ?></span>
 							</li>
 							<?php }?>
<?php if (!empty($factura['nro_cuenta'])){?>
 							<li class="row">
 								<span class="label"><?php print t('Nro. de cuenta');?></span>
 								<span class="value"><?php print $factura['nro_cuenta']; ?></span>
 							</li>
<?php } 
if (!empty($factura['periodo'])){?>
 							<li class="row">
 								<span class="label"><?php print t('Per&iacute;odo');?></span>
 								<span class="value"><?php print $factura['periodo']; ?></span>
 							</li>
<?php } 

if (!empty($factura['emision'])){ ?>  							
 							<li class="row">
 								<span class="label"><?php print t('Emisi&oacute;n');?></span>
 								<span class="value"><?php print $factura['emision']; ?></span>
 							</li>
<?php } ?>      							
 						</ul>
 					</div>
 				</div>
 				<div class="col-right">
 					
 					<!--
 					
 					<div class="titulo-targeta"><?php print t('Ingresa los datos de tu Targeta de Cr&eacute;dito');?></div>
 					<div id="formulario-pago">
 						<?php print $formulario_pago; ?>
 					</div>
 					
 					 -->
 					 
 					 <?php if ($tipoPago==1) { ?>
					    <div class="titulo-targeta"><?php print t('Haz clic en el boton para realizar tu pago');?></div>
					    <!-- <div id="formulario-pago"><a href="#">Pagar</a></div> -->
					    <div id="formulario-pago"><a href="#" class="button-form-submit"><?php print t('Pagar')?></a></div>
					   <?php } elseif ($tipoPago==2) { ?>
					    <div class="titulo-targeta"><?php print t('Ingresa los datos de tu Targeta de Cr&eacute;dito');?></div>
					    <div id="formulario-pago">
					     <?php print $formulario_pago; ?>
					    </div>
					   <?php } else { ?>
					
					   <?php } ?>
 					
 				</div>
 			</div>
 		</div>
 	</div>
 	</div>
 </div>
