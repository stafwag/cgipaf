<?
include "cgipasswd_top.php"
?>
<p>
<h1><b><font color="#ff0000"><i>N�mero m�ximo de tentativas excedido (<? echo $max_invalid; ?>)
<br>Voc� dever� aguardar <? echo $invalid_wait; ?> segundos.</i></font></b></h1>
<p>
<?
include "cgipasswd_login.php"
?>
