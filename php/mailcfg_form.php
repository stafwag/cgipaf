<hr>
<p>
<form action="<? echo $next_action_full >" method="post">
<table>
<tr>
<td>Login name:</td>
<td align="left"><? echo $name ?>
<input type="hidden" name="name" value="<? echo $name ?>"></td>
</tr>
</table>
<hr>
<table>
<tr>
<td>Forward mailbox:</td>
<td width="20">
<select name="forward">
<option value="<? echo $forward ?>"><? echo $forward ?> 
<option value="<? echo $not_forward ?>"><? echo $not_forward ?>
</select></td>
<td>Forward to:</td>
<td><input name="forward_to" value="<? echo $forward_to ?>" type="text" size="30" maxlength="40"></td>
</tr>
<tr>
<td>Keep messages:</td>
<td>
<select name="keep_msg">
<option value="<? echo $keep_msg ?>"><? echo $keep_msg ?>
<option value="<? echo $not_keep_msg ?>"><? echo $not_keep_msg ?>
</select><td>
</tr>
</table>
<hr>
<table>
<tr>
<td>Enable autoReply:</td>
<td>
<select name="autoreply">
<option value="<? echo $autoreply ?>"><? echo $autoreply ?>
<option value="<? echo $not_autoreply ?>"><? echo $not_autoreply ?>
</select>
</td>
</table>
Autoreply message:<br>
<textarea name="autoreply_msg" rows="10" cols="60"><? echo $autoreply_msg; ?></textarea>
<p>
<center>
<table>
<tr>
<td>
<input type="reset" value="Clear">
<input type="submit" value="Submit">
</td></tr></table>
</center>
</form>
</body>
</html>
