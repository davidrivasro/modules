<?php
	define("RUTA", drupal_get_path('module', 'tol'));
?>  

<div id="page">
    <div id="content-wrapper">
    	<h1><?php print t('Mis L&iacute;neas') ?></h1>
    	<div id="article">
	<?php if ($showAsociarLinea){?>		
      	<div class="article-accion"><?php print t('Asociar L&iacute;nea');?></div>
    		<div class="article-description"><?php print t('Desde este panel podr&aacute;s Asociar o Editar tus lineas.');?></div>
    <?php }?>
    		<div id="article-table">
        <?php foreach($lineas as $linea){ ?>
    			<div class="row <?php echo $linea['active']; ?>">
    				
    				<div class="column-selected col-linea">

                <?php
                $pred=''; $pred_text='';
									if(!empty($linea['active'])) { 
                    $pred='checked="checked"';
                    $pred_text=t('Predeterminado');
                 }; 
                ?>
			
                <form name="declaration" method="get" action="/online/mis-lineas?lineaNum=<?php echo $linea['numero']; ?>&lineaId=<?php echo $linea['lineaId']; ?>" >
                    <label><input type="checkbox" onClick="submit();" name="predeterminado" <?php print $pred; ?> ><?php print $pred_text?></label>
                    <input type="hidden" name="lineaNum" value="<?php echo $linea['numero']; ?>" >
                </form>
                        
    				</div>
    				<div class="column-user col-linea"><?php echo $linea['nombre']; ?></div>
    				<div class="column-dni col-linea"><?php echo $linea['numero']; ?></div>
    					
    				<div class="column-buttons col-linea">
    					<div class="button-editar"><a href="#"><?php print t('Editar');?></a></div>    					
    					<?php if ($showAsociarLinea){?>
    					<div class="button-eliminar"><a href="/online/mis-lineas/eliminar?width=370&height=210&id=<?php echo $linea['lineaId']; ?>&name=<?php echo $linea['nombre']; ?>&num=<?php echo $linea['numero']; ?>" class="colorbox-node"><?php print t('Eliminar');?></a>
    					</div>
    					<?php }?>    					
    				</div>
    				
    				<div class="row-edit">
                <div class="column-form"><?php print $linea['ediform']; ?></div>
            </div>
    			</div>
    			 
        <?php } ?>
<?php if ($showAsociarLinea){?>
    			<div class="row row-last">
    				<div class="column-opcion-asociar">
    					<span class="opcion-question"><?php print t('&iquest;Deseas asociar una nueva l&iacute;nea a tu cuenta?')?></span>
    					<span class="opcion-description"><?php print t('Puedes asociar la cantidad de l&iacute;neas que desees.'); ?></span>
    				</div>
    				
    				<div class="column-button-asociar">
                  <a href="/online/editar/asociar-linea?width=440&height=410" class="colorbox-node"><?php print t('Asociar L&iacute;nea');?></a>
    				</div>
    				
    			</div>
    			<?php }?>
    		</div>
    	</div>
    </div>
</div>
