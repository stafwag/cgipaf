<?
include "cgipasswd_top.php"
?>
<P>
<H1><B><font color="#ff0000"><I>Maximum number of tries exceeded (<? echo $max_invalid; ?>)
<BR>You've to wait <? echo $invalid_wait; ?> seconds.</I></font></B></H1>
<P>
<?
include "cgipasswd_login.php"
?>
