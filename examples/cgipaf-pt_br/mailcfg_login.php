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
<form action="/cgi-bin/viewmailcfg.cgi" method="post">
<table width="80%">
<tr>
<td bgcolor="#b0b0b0" width="20%">Usuário:</td>
<td align="left"><input name="name" type="text" size="30" maxlength="30" value="<? echo $name ?>"></td>
<td width="20%">senha :</td>
<td><input name="passwd" type="password" size="20" maxlength="16"></td>
</tr>
</table>
<hr>
<center>
<table>
<tr>
<td>
<input type="reset" value="Limpar">
</td>
<td>
<input type="submit" value="Enviar">
</td>
</tr>
</table>
</center>
</form>
</body>
