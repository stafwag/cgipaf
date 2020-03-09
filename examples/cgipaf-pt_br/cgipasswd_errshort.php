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
<?
include "cgipasswd_top.php"
?>
<p>
<h1><b><i><font color="#ff0000">Senha muito curta...</font></i></b></h1>
<p>comprimento mínimo: <? echo $min_length; ?></p>
<?
include "cgipasswd_login.php"
?>
