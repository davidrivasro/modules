<?php
/**
 * Description of tigo-destacado-home
 * @author Andrea Romero
 */
?>
<style type="text/css">
  p {
    color: #000;
  }
</style>
<table width="100%" cellspacing="0" cellpadding="0" border="0" bgcolor="transparent"><tbody><tr><td>
        <table width="100%" height="68" cellspacing="0" cellpadding="0" border="0" style="background:#5060ba"><tbody><tr><td align="center" style="vertical-align:bottom">
                <table width="560" cellspacing="0" cellpadding="0" border="0" align="center"><tbody><tr><td width="550" height="40" background="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/header.gif" style="background-repeat:no-repeat"></td>
                    </tr></tbody></table></td>
            </tr></tbody></table><table width="560" cellspacing="0" cellpadding="0" border="0" align="center"><tbody><tr><td style="font-family:Arial,Helvetica,sans-serif">
                <table width="100%" cellspacing="0" cellpadding="0" border="0" style="border-right:1px solid rgb(214,214,214);border-left:1px solid rgb(214,214,214);background:#fcfefc"><tbody><tr style="padding:0pt 36px;display:block"><td width="25%" style="vertical-align:top">
                        <img width="100%" alt="logo" src="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/logo_tigo.jpg"></td>
                      <td width="20" height="239" style="background-repeat:no-repeat"> </td>
                      <td valign="top">
                        <table width="100%" cellspacing="0" cellpadding="0" border="0"><tbody><tr><td style="color:#5060ba;font-size:1.5em;font-weight:bold"><?php print $mail; ?>,</td>
                            </tr></tbody>
                        </table><p>Hola</p>
																<p>Gracias por usar el servicio de pago de factura Tigo!</p>
																<p>Estado de tu solicitud:</p>
																<table width="100%" border="0" cellspacing="0" cellpadding="0">
																  <tr>
																    <td>Número Transacción:</td>
																    <td><?php print $transaccion_id; ?></td>
																  </tr>
																  <tr>
																    <td>Estado:</td>
																    <td><?php print $state; ?></td>
																  </tr>
																  <tr>
																    <td>Valor:</td>
																    <td><?php print $valor; ?></td>
																  </tr>
																  <tr>
																    <td>Fecha:</td>
																    <td><?php print $fecha; ?></td>
																  </tr>
																  <tr>
																    <td>Telefono:</td>
																    <td><?php print $telefono; ?></td>
																  </tr>
																  <tr>
																    <td>Email:</td>
																    <td><?php print $mail; ?></td>
																  </tr>
											 					</table>
																<p>Si tienes cualquier duda con respecto a tu pedido, por favor llamar a *300 desde tu movil Tigo.</p>
                        				<p>IMPORTANTE: EL PAGO DE TU FACTURA SE PODRÁ PROCESAR EN UN TIEMPO MAXIMO DE 24 HORAS.</p>
																<p>-- El equipo de <a href="http://www.tigo.com.co/">tigo.com.co</a></p>
																<p>&nbsp;</p>
                      					<p>&nbsp;</p></td>
                    </tr><tr><td colspan="4" style="border-top:1px dashed rgb(204,204,204)"><img src="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/spacer.gif" alt="" width="5" height="5"></td>
                    </tr><tr><td width="100%" align="center" colspan="4">
                        <table cellspacing="0" cellpadding="0" border="0" align="center"><tbody><tr><td valign="middle" background="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/facebook.png" style="background-repeat:no-repeat"><a style="display:block;font-size:11px;color:#a0a0a0;font-weight:bold;text-indent:20px;text-decoration:none" href="http://facebook.com/Tigo.Col" target="_blank">Siguenos en Facebook</a></td>
                              <td width="40">&nbsp;</td>
                              <td valign="middle" background="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/twitter.png" style="background-repeat:no-repeat"><a style="display:block;font-size:11px;color:#a0a0a0;font-weight:bold;text-indent:20px;text-decoration:none" href="http://www.twitter.com/@quieromitigo" target="_blank">Siguenos en Twitter</a></td>
                            </tr></tbody></table></td>
                    </tr><tr><td colspan="4"><img src="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/spacer.gif" alt="" width="2" height="2"></td>
                    </tr></tbody></table></td>
            </tr></tbody></table><table width="560" cellspacing="0" cellpadding="0" border="0" align="center"><tbody><tr><td>
                <img src="<?php echo $base_url; ?>/sites/all/themes/tigo/imagenescorreo/footer_grad.gif"></td>
            </tr></tbody></table><table width="560" cellspacing="0" cellpadding="0" border="0" align="center" style="padding:5px 19px"><tbody><tr><td style="font-size:0.85em;text-shadow:1px 1px 1px rgb(255,255,255);color:rgb(135,135,135)">
                Copyright© Tigo. Colombia Móvil S.A E.S.P. Todos los derechos reservados<br><br></td>
            </tr></tbody></table></td>
    </tr></tbody>
</table>
