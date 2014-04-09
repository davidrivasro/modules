<div id="modal" class="asociar-linea codigo-seguridad">
	<h2><?php print t('Codigo de Seguridad'); ?></h2>
	<div class="modal-subtitle"><?php print t('Escribe el codigo de seguridad que recibiste por mensaje de texto. Si no recibiste el mensaje de texto, haz'); ?> <a class="colorbox-node load-ajax" href="/online/editar/codigo-seg-reenviar?width=410&height=390&dest=<?php echo $destination; ?>"><?php print t('click aquí'); ?></a> <?php print t('para solicitar otro.'); ?></div>
	<div id="formulario">
		<?php echo $formulario_codigo_seguridad; ?>
	</div>
</div>