<?php
$module_path = drupal_get_path('module', 'tol');
?>


<div id="page">

    <div id="content-wrapper" class="content-mis-datos">
      <h1><?php print t('Mis Datos') ?></h1>
      <div id="article">
          
          <div id="mis-datos">
            <div class="items">
              <ul>
                <li>
                  <span class="label"><?php print t('Nombre'); ?></span>
                  <span class="value"><?php print $nombre; ?></span>
                </li>
                <li>
                  <span class="label"><?php print t('Apellido'); ?></span>
                  <span class="value"><?php print $apellido; ?></span>
                </li>
                <li>
                  <span class="label"><?php print t('Tipo de documento'); ?></span>
                  <span class="value"><?php print $tipo_dni; ?></span>
                </li>
                <li>
                  <span class="label"><?php print t('Número de documento'); ?></span>
                  <span class="value"><?php print $nro_dni; ?></span>
                </li>
                <li>
                  <span class="label"><?php print t('Direccion'); ?></span>
                  <span class="value"><?php print $direccion; ?></span>
                </li>
               <!--
                Solicitan ocultar el campo departamento y ciudad para todos los paises
                <li>
                  <span class="label"><?php/* print t('Departamento'); */?></span>
                  <span class="value"><?php /*print $departamento; */?></span>
                </li>
                <li>
                  <span class="label"><?php/* print t('Ciudad');*/ ?></span>
                  <span class="value"><?php/* print $ciudad; */?></span>
                </li>
                -->
                <li>
                  <span class="label"><?php print t('Telefono de contacto'); ?></span>
                  <span class="value"><?php print $telefono; ?></span>
                </li>
                <li>
                  <span class="label"><?php print t('Correo'); ?></span>
                  <span class="value"><?php print $correo; ?></span>
                </li>
              </ul>
            </div>
            
            <div class="items-buttons">
				<?php if ($showEditarDatosCliente){?>
              <a href="/online/mis-lineas/mis-datos/editar" class="cambiar-datos button-gray"><?php print t('Cambiar datos')?></a>
               <?php }?>
            <?php if ($showSegundaContrasena){?>
              <a href="/online/editar/codigo-seguridad?width=460&height=400&dest=clave" class="cambiar-pass colorbox-node button-gray"><?php print t('Generar 2da contrase&ntilde;a')?></a>
            <?php }?>
            </div>
          </div>
        
      </div> <!-- end article -->
    </div> <!-- end content -->
</div> <!-- end page -->
