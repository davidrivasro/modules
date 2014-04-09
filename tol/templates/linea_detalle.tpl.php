 <?php $module_path = '/'.drupal_get_path('module', 'tol'); ?>
 
 <div id="page">
 	<div id="content-wrapper">
 		<div id="article">
 			<h1><?php print t('Detalle de cuenta');?></h1>
 			<div id="mi-equipo">
 				<h2><?php print t('Mi equipo');?></h2>
<?php /* 				
 				<div class="subtitle"><?php print t('AQUI VA ALGUN SUBTITULO/INFORMACION?');?></div>
*/ ?>
 				<div id="block-equipo">
 					<div class="movil-image"><img src="<?php print $movil['imagen'];?>" width="120" /></div>
 					<div class="equipo-detalles">
 						<ul>
 							<li>
 								<span class="label"><?php print t('Marca')?></span>
 								<span class="value"><?php print $movil['marca'] ; ?></span>
 							</li>
 						 	<li>
 								<span class="label"><?php print t('Modelo')?></span>
 								<span class="value"><?php print $movil['modelo'] ; ?></span>
 							</li>
 						 	<li>
 								<span class="label"><?php print t('IMEI')?></span>
 								<span class="value"><?php print $movil['imei'] ; ?></span>
 							</li>
 						</ul>
                                                <?php if ($showBloquearEquipo){?>
                                                        <div class="button-bloquear"><a href="/online/linea-detalle/bloquear-equipo?width=360&height=230" class="colorbox-node button-gray"><?php print t('Bloquear equipo');?></a></div>
                                                <?php }?>		
 						<!--
 						<div class="title-seguro"><?php print t('Seguro')?></div>

 						<ul class="mas-items">
 							<li>
 								<span class="label"><?php print t('Beneficiario')?></span>
 								<span class="value"><?php print $movil['beneficiario'] ; ?></span>
 							</li>
 							<li>
 								<span class="label"><?php print t('Valor deducible')?></span>
 								<span class="value"><?php print $movil['valor_deducible'] ; ?></span>
 							</li>
 							<li>
 								<span class="label"><?php print t('Contacto')?></span>
 								<span class="value"><?php print $movil['contacto'] ; ?></span>
 							</li>
 						</ul>
 						-->
 						
 					<div>
 				</div> <!-- end block equipo -->
 			</div> <!-- end mi equipo -->
 			</div>
 			</div>

			<?php if ($showInformacionPersonal){?>
 			<div id="info-personal">
 				<div class="title">
 					<h3><?php print t('Info. Personal')?></h3>
 					<?php if ($showEditarDatosCliente){?>
 					   <a href="/online/mis-lineas/mis-datos" class="editar button-gray"><?php print t('Editar')?></a>
 					<?php }?>
 				</div>
 				<div class="dates">
 					<ul>
 						<li>
 							<span class="label"><?php print t('Nombre')?></span>
 							<span class="value"><?php print $personal['nombre'] ; ?></span>
 						</li>
 						<li>
 							<span class="label"><?php print t('Apellido')?></span>
 							<span class="value"><?php print $personal['apellido'] ; ?></span>
 						</li>
 						<li>
 							<span class="label"><?php print t('Tipo de documento')?></span>
 							<span class="value"><?php print $personal['tipo_documento'] ; ?></span>
 						</li>
 						<li>
 							<span class="label"><?php print t('Nro. de documento')?></span>
 							<span class="value"><?php print $personal['nro_documento'] ; ?></span>
 						</li>
 						<li>
 							<span class="label"><?php print t('Direcci&oacute;n')?></span>
 							<span class="value"><?php print $personal['direccion'] ; ?></span>
 						</li>
 					</ul>
 				</div>
 			</div><!-- end info personal -->
			<?php }?>

 			<div id="detalle-cuenta">
 				
 					<h2><?php print t('Detalle de cuenta');?></h2>
<?php /* 					
 					<div class="subtitle"><?php print t('De nuevo aqui va algun SUBTITULO/INFORMACION');?></div>
*/ ?>
 				<div class="detalles">
 				<ul>
					<?php if (!empty($linea['numero'])){ ?>
					<?php if ($show_nrocuenta){?>
 					<li>
 						<span class="label"><?php print t('N&uacute;mero de cuenta')?></span>
 						<span class="value"><?php print $linea['numero'] ; ?></span>
 					</li>
 					<?php }?>
					<?php } ?>
 					<li>
 						<span class="label"><?php print t('PIN')?></span>
 						<span class="value"><?php print $linea['pin'] ; ?></span>
 					</li>
 					<li>
 						<span class="label"><?php print t('PUK')?></span>
 						<span class="value"><?php print $linea['puk'] ; ?></span>
 					</li>
 					<?php if ($show_pin2_punk2){?>
						<li>
							<span class="label"><?php print t('PIN2')?></span>
							<span class="value"><?php print $linea['pin2'] ; ?></span>
						</li>
						<li>
							<span class="label"><?php print t('PUK2')?></span>
							<span class="value"><?php print $linea['puk2'] ; ?></span>
						</li>
 					<?php }?>
 					<!--
 					<li>
 						<span class="label"><?php print t('Winmax')?></span>
 						<span class="value"><?php print $linea['winmax'] ; ?></span>
 					</li>
 					-->
 				<?php if ($showStatusLine){?>
 					<li>
 						<span class="label"><?php print t('Estado del servicio')?></span>
 						 <?php if ($linea['estado']=='Activa' || $linea['estado']=='Disponible'){?>
							<span class="value2"><?php print $linea['estado'] ; ?></span>
 						 <?php }
 						 else {?>
						    <span class="value3"><?php print $linea['estado'] ; ?></span>
						<?php }?>
								 
        
 					</li>
 				<?php }?>
 				<?php if ($showBloquearLine){?>
 					<li><a href="/online/linea-detalle/bloquear-linea?width=360&height=230" class="colorbox-node button-gray"><?php print t('Bloquear L&iacute;nea');?></a></li>
 				<?php }?>
 				</ul>

 			</div>
 	   </div>
 		</div>
 	</div>
 </div>
