<p>
<table>
<tr>
<td>Loginnaam</td><td>:</td><td><input name=name size=12 value="<?echo $name ?>"></td>
</tr>
<tr>
<td>Huidige wachtwoord</td><td>:</td><td><input name=passwd type=password></td>
</tr>
<tr>
<td>Nieuwe wachtwoord</td><td>:</td><td><input name=newpass1 size="<? echo $max_length ?>" type=password></td>
</tr>
<tr>
<td>Controle nieuwe wachtwoord</td><td>:</td><td><input name=newpass2 size="<? echo $max_length ?>" type=password></td>
</tr>
</table>
<p>
<input type="reset"  value="Clear">
<input type="submit" value="Change">
</p>
</center>
</form>
</body>
</html>
