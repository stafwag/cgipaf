<?

/*
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
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
<td bgcolor="#b0b0b0" width="20%">Login name:</td>
<td align="center"><input name="name" type="text" size="10" maxlength="15" value="<? echo $name ?>"></td>
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
