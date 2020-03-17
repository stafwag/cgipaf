<?

/*
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
 * Copyright (C) 2004 Koen de Jonge
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
