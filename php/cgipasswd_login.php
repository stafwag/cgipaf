<p>
<table>
<tr>
<td>Login name</td><td>:</td><td><input name=name size=12 value="<?echo $name ?>"></td>
</tr>
<tr>
<td>Current password</td><td>:</td><td><input name=passwd type=password></td>
</tr>
<tr>
<td>New password</td><td>:</td><td><input name=newpass1 size="<? echo $max_length ?>" type=password></td>
</tr>
<tr>
<td>Re-enter new password</td><td>:</td><td><input name=newpass2 size="<? echo $max_length ?>" type=password></td>
</tr>
</table>
<p>
<input type=submit value="Change">
<input type=reset  value="Clear">
</p>
</center>
</form>
</body>
</html>
