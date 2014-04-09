<div id="block-menu-perfil">
    <h2 class="open">
        <span class="arrow"></span>

        <span class="arrow-blue"></span>
        <?php 
       
        foreach($lineas as $linea){
        foreach($linea as $items){ ?>
            <?php if($items['active']=='predeterminado') { ?>
                <?php print $items['nombre'] ?> 
                <span class="dni"><?php print $items['numero'] ?></span>
            <?php } ?>
        <?php   }} ?>
    </h2>
    <div class="menu menu-lineas" style="display:none;">
        <ul>
            <?php 
            foreach($lineas as $linea){
            foreach($linea as $items){ ?>
               
               <a href="/online/resumen/camb-pred?numcp=<?php echo $items['numero']; ?>">
	               <li class="<?php echo $items['active'];?>">
		             
		                <?php echo $items['nombre']; ?>
		                <br>
		                <span><?php echo $items['numero']; ?></span>
	                
	                </li>
                </a>
            <?php } } ?> 
        </ul>
        
        <?php if ($showAsociarLinea){?>
        <div class="menu-lineas-buttons">
            <div class="button-agregar"><a class="colorbox-node" href="/online/editar/asociar-linea?width=440&height=380"><?php print t('Agregar')?></a></div>
            
            <!--
            <div class="button-editar"><a href="#"><?php print t('Editar')?></a></div>
            -->
        </div>
        <?php }?>
        
    </div>
</div>
