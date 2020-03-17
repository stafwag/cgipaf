<?

/*
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
 * Copyright (C) 2002 Fabricio Machado
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

?>
<hr>
<p>
<form action="/cgi-bin/mailcfg.cgi" method="post">
<table>
<tr>
<td>Login :</td>
<td align="left"><? echo $name ?>
<input type="hidden" name="name" value="<? echo $name ?>"></td>
</tr>
</table>
<hr>
<table>
<tr>
<td>Encaminhar e-mails:</td>
<td width="20">
<select name="forward">
<option value="<? echo $forward ?>"><? echo $forward ?> 
<option value="<? echo $not_forward ?>"><? echo $not_forward ?>
</select></td>
<td>Encaminhar para:</td>
<td><input name="forward_to" value="<? echo $forward_to ?>" type="text" size="30" maxlength="40"></td>
</tr>
<tr>
<td>Manter cópia das mensagens:</td>
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
<td>Habilitar resposta automática:</td>
<td>
<select name="autoreply">
<option value="<? echo $autoreply ?>"><? echo $autoreply ?>
<option value="<? echo $not_autoreply ?>"><? echo $not_autoreply ?>
</select>
</td>
</table>
Mensagem de Auto-Resposta:<br>
<textarea name="autoreply_msg" rows="10" cols="60"><? echo $autoreply_msg; ?></textarea>
<p>
<center>
<table>
<tr>
<td>
<input type="submit" value="Enviar">
<input type="reset" value="Limpar">
</td></tr></table>
</center>
</form>
</body>
</html>
