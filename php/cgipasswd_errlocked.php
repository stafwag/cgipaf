<?
include "cgipasswd_top.php"
?>
<p>
<h1><b><font color="#ff0000"><i>Maximum number of tries exceeded (<? echo $max_invalid; ?>)
<br>You've to wait <? echo $invalid_wait; ?> seconds.</i></font></b></h1>
<p>
<?
include "cgipasswd_login.php"
?>
