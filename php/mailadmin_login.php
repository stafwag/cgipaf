<hr>
<p>
<form action="/cgi-bin/mailadmin.cgi" method="post">
<table width="80%">
<tr>
<td bgcolor="#b0b0b0" width="20%">Login name:</td>
<td align="center"><input name="admin" type="text" size="10" maxlength="15" value="<? echo $name ?>"></td>
<td width="20%">password :</td>
<td><input name="passwd" type="password" size="8" maxlength="16"></td>
</tr>
</table>
<hr>
<center>
<table>
<tr>
<td>
<input type="reset" value="Clear">
</td>
<td>
<input type="submit" value="Submit">
</td>
</tr>
</table>
</center>
</form>
</body>
