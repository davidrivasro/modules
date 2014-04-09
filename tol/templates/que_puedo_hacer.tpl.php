<div class="resumen-subtitile">&iquest;Qu&eacute; puedo hacer ?</div>

    <div id="blocks-date">
<?php $block = module_invoke('menu', 'block_view', 'menu-tol---saldo');
if (!empty($block['content'])){ ?>
        <div class="box-date">
            <?php
                //$block = module_invoke('menu', 'block_view', 'menu-tol---saldo');
                print '<div class="box-date-title saldo">'.render($block['subject']).'</div>';
                print render($block['content']);

            ?>
        </div>
<?php }?>

<?php $block = module_invoke('menu', 'block_view', 'menu-tol---facturacion');
if (!empty($block['content'])){ ?>
        <div class="box-date">
            <?php
                //$block = module_invoke('menu', 'block_view', 'menu-tol---facturacion');
                print '<div class="box-date-title facturacion">'.render($block['subject']).'</div>';
                print render($block['content']);

            ?>
        </div>
<?php }?>
        <!-- //RVS 20130701 a peticion de TIGO CO
        <div class="box-date">
            <?php
                $block = module_invoke('menu', 'block_view', 'menu-tol---linea');
                print '<div class="box-date-title linea">'.render($block['subject']).'</div>';
                print render($block['content']);

            ?>
        </div>
        <div class="box-date box-last">
            <?php
                $block = module_invoke('menu', 'block_view', 'menu-tol---comprar');
                print '<div class="box-date-title comprar">'.render($block['subject']).'</div>';
                print render($block['content']);

            ?>
        </div>
        -->
    </div>
