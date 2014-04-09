<?php
$module_path = drupal_get_path('module', 'tol');
?>

<div id="page">

    <div id="content-wrapper" class="content-mis-datos-editar">
      <h1><?php print t('Actualizar mis Datos') ?></h1>
      <div id="article">

          <div class="mis-datos-subtitle"><?php print t('Completa el siguiente formulario');?></div>
          <p><?php print t('Para actualizar los datos de cuenta necesitamos que modifiques los datos que quieres editar.<br> Recuerda que el documento y el correo no son editables.') ?></p>
          
          <div id="mis-datos-editar">
            <?php print $formulario_mis_datos; ?>
          </div>
        
      </div> <!-- end article -->
    </div> <!-- end content -->
</div> <!-- end page -->
