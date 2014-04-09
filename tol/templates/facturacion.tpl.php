<?php
$module_path = drupal_get_path('module', 'tol');
?>
<div id="page">
    <div id="content-wrapper" class="content-facturacion">
      <h1><?php print t('Facturaci&oacute;n') ?></h1>
		<div id="article">
			<div id="ultima-factura">
				<div class="title">
					<h3><?php print t('Última factura'); ?></h3>
						<?php if ($showDescargar){ ?>
							<?php if ($useSclave){ ?>
								<a href="/online/facturacion/pre-descarga?width=400&height=330&dest=descargarf&idDes=<?php print $facturacion['nrofactura']; ?>" class="colorbox-node download button-gray"><?php print t('Descargar'); ?></a>
							<?php } else { ?>
								<a href="/online/facturacion/descarga?idDes=<?php print $facturacion['nrofactura']; ?>" class="download button-gray"><?php print t('Descargar'); ?></a>
							<?php } ?>
						<?php }
						if ($showPrint){ ?>
							<a href="javascript:window.print()" class="print button-gray" ><?php print t('Imprimir'); ?></a>
						<?php } ?>
				</div>
				<div class="valores">
					<?php if (!empty($facturacion['amount'])){ ?>
						<div class="pre-title"><?php print t('Total a pagar'); ?></div>
						<div class="amount"><?php print $facturacion['amount'];?></div>
					<?php }else{ ?>
						<div class="pre-title"><?php print t('No tienes deuda pendiente') ?></div>
						<?php } ?>
					<?php if ($showPagar){ ?>
						<?php print $formulario_pago; ?>
					<?php } ?>
				</div>
				<?php
				$texto=null;
				if (!empty($facturacion['date_end'])){
					$texto = t('Esta factura corresponde al período @from al @to',array('@from'=>$facturacion['date_begin'],'@to'=>$facturacion['date_end']));
				}	
				else if (!empty($facturacion['date_begin'])){
					$texto = t('Esta factura corresponde al período del @from',array('@from'=>$facturacion['date_begin'],'@to'=>$facturacion['date_end']));
				}
				if ($texto){
				?>
					<div class="post-title"><?php print $texto; ?></div>
				<?php } ?>
				<div class="items">
					<ul>
						<?php if (!empty($facturacion['vencimiento'])){ ?>
							<li>
							  <span class="label"><?php print t('Vencimiento'); ?></span>
							  <span class="value"><?php print $facturacion['vencimiento']; ?></span>
							</li>
						<?php } if (!empty($facturacion['fecha_limite'])){ ?>
							<li>
							  <span class="label"><?php print t('Fecha límite de pago'); ?></span>
							  <span class="value"><?php print $facturacion['fecha_limite']; ?></span>
							</li>
						<?php } ?>
							<li>
							  <span class="label"><?php print t('Total factura'); ?></span>
							  <span class="value"><?php print $facturacion['totalfactura']; ?></span>
							</li>
						<?php if ($showNumeroFactura){?>
							<li>
							  <span class="label"><?php print t('Número de factura'); ?></span>
							  <span class="value"><?php print $facturacion['nrofactura']; ?></span>
							</li>
						<?php }?>
						<?php if (!empty($facturacion['nrocuenta'])){ ?>
							<li>
							  <span class="label"><?php print t('Número de cuenta'); ?></span>
							  <span class="value"><?php print $facturacion['nrocuenta']; ?></span>
							</li>
						<?php } if (!empty($facturacion['periodo'])){ ?>
							<li>
							  <span class="label"><?php print t('Período'); ?></span>
							  <span class="value"><?php print $facturacion['periodo']; ?></span>
							</li>
						<?php } if (!empty($facturacion['emision'])){ ?>
							<li>
							  <span class="label"><?php print t('Emision'); ?></span>
							  <span class="value"><?php print $facturacion['emision']; ?></span>
							</li>
						<?php } ?>
					</ul>
				</div>
            </div><!-- end ultima factura -->
			<?php if ($showDatosFactura){?>
				<div id="datos-facturacion">
					<div class="title">
						<h3><?php print t('Datos de Facturación'); ?></h3>
							<?php if ($useSclave){ ?>
								<a href="/online/facturacion/pre-descarga?width=400&height=330&dest=editarf" class="colorbox-node editar button-gray"><?php print t('Editar'); ?></a>
							<?php } else { ?>
								<a href="/online/facturacion/editar" class="editar button-gray" ><?php print t('Editar'); ?></a>
							<?php } ?>
					</div>
					<div class="boxes factura-electronica">
						<input type="checkbox" id="electronica" disabled="true" <?php print $facturacion['recibir_elec'] ?> /><label><?php print t('Electronica');?></label>
						<div class="box">
						  <ul>
							<li><span class="label"><?php print t('Email')?></span><span class="value"><?php print $facturacion['email'] ?></span></li>
							<li><span class="label"><?php print t('Detalle de factura')?></span><span class="value"><?php print t($facturacion['electronica_detalle']) ?></span></li>
						  </ul>
						</div>
					</div>
					<?php if ($show_factura_fisica){?>
					<div class="boxes factura-fisica">
						<input type="checkbox" id="fisica" disabled="true" <?php print $facturacion['recibir_fis']; ?> /><label><?php print t('Fisica');?></label>
						<div class="box">
						<ul>
							<li><span class="label"><?php print t('Dirección')?></span><span class="value"><?php print t($facturacion['direccion']) ?></span></li>
							<?php if (!empty($facturacion['departamento'])){ ?>
								<li><span class="label"><?php print t('Departamento')?></span><span class="value"><?php print t($facturacion['departamento']) ?></span></li>
							<?php } ?>
							<?php if (!empty($facturacion['ciudad'])){ ?>
								<li><span class="label"><?php print t('Ciudad')?></span><span class="value"><?php print $facturacion['ciudad'] ?></span></li>
							<?php } ?>
							<?php  if (!empty($facturacion['telefono'])){ ?>
								<li><span class="label"><?php print t('Teléfono')?></span><span class="value"><?php print $facturacion['telefono'] ?></span></li>
							<?php }?>
								<li><span class="label"><?php print t('Detalle de la factura')?></span><span class="value"><?php print t($facturacion['fisica_detalle']) ?></span></li>
						</ul>
						</div>
					</div>
					<?php }?>
				</div><!-- end datos de facturacion -->
			<?php }?>
			<div class="clear"></div>
				<div id="historial-facturacion">
					<div class="title">
						<h3><?php print t('Historial de Facturaci&oacute;n'); ?></h3>
						   <!--
							<div class="dates-calendar"><?php print $formulario_historial ?></div>
							-->
					</div>
					<div class="table">
						<div class="table row-header">
							<?php if (!empty($ultimas['perdiodo_end'])){ ?>
								<div class="col col-periodo"><?php print t('Periodo')?></div>
							<?php } else { ?>
								<div class="col col-periodo1"><?php print t('Periodo')?></div>
							<?php } ?>
						<div class="col col-total"><?php print t('Total')?></div>
						<div class="col col-detalle"><?php print t('Detalle')?></div>
					</div>
					<?php foreach($ultimas_facturas as $ultimas) { ?>
						<div class="table row-body">
							<?php if (!empty($ultimas['perdiodo_end'])){ ?>
								<div class="col col-periodo"><?php print $ultimas['periodo_begin']; ?> <?php print t('al') ; ?> <?php print $ultimas['perdiodo_end']; ?></div>
							<?php } else { ?>
								<div class="col col-periodo1"><?php print $ultimas['periodo_begin']; ?></div>
							<?php } ?>
								<div class="col col-total"><?php print $ultimas['total']; ?></div>
								<div class="col col-detalle"><a href="facturacion/detalle?id=<?php print $ultimas['id_factura']; ?>" class="button-gray"><?php print t('Ver')?></a></div>
						</div>
					<?php } ?>
				</div>
			</div><!-- end historial de facturacion-->
      </div> <!-- end article -->
    </div> <!-- end content -->
</div> <!-- end page -->

<?php if($doFactDown){
	//para imprimir la factura
		print "<script>	window.location.href = 'facturacion/descarga?idDes=".$idDesParam."'; </script>";
}

?>
