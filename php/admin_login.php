<hr>
<p>
<form action="<? echo $_ENV[SCRIPT_NAME]; ?>" method="post">
<table width="80%">
<tr>
<td bgcolor="#b0b0b0" width="20%">Login name:</td>
<td align="center"><input name="admin" type="text" size="10" maxlength="15" value="<? echo $admin ?>"></td>
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
