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
<HR>
<P>
<FORM ACTION="/cgi-bin/mailcfg.cgi" METHOD=POST>
<TABLE WIDTH="100%">
<TR>
<TD>Login name:</TD>
<TD ALIGN="LEFT"><INPUT NAME=name VALUE="<? echo $name ?>" TYPE=TEXT SIZE=10 MAXLENGTH=30></TD>
<TD ALIGN="LEFT">password :</TD>
<TD ALIGN="LEFT"><INPUT NAME=passwd TYPE=PASSWORD SIZE=8 MAXLENGTH=10> (RETYPE YOUR PASSWORD!!!)</TD>
</TR>
</TABLE>
<HR>
<TABLE>
<TR>
<TD>Forward mailbox:</TD>
<TD>
<SELECT NAME="forward">
<OPTION value="<? echo $forward ?>"><? echo $forward ?> 
<OPTION value="<? echo $not_forward ?>"><? echo $not_forward ?>
</SELECT></TD>
<TD>Forward to:</TD>
<TD><INPUT NAME=forwardname VALUE="<? echo $forwardto ?>" TYPE=TEXT SIZE=30 MAXLENGTH=40></TD>
</TR>
<TR>
<TD>Keep messages:</TD>
<TD>
<SELECT NAME=keepforward>
<OPTION value="<? echo $keep_msg ?>"><? echo $keep_msg ?>
<OPTION value="<? echo $keep_msg ?>"><? echo $keep_msg ?>
</SELECT><TD>
</TR>
</TABLE>
<HR>
<TABLE>
<TR>
<TD>Enable autoReply:</TD>
<TD>
<SELECT NAME="reply">
<OPTION value="<? echo $autoreply ?>"><? echo $autoreply ?>
<OPTION value="<? echo $not_autoreply ?>"><? echo $notautoreply ?>
</SELECT>
</TD>
</TABLE>
Autoreply message:<BR>
<TEXTAREA NAME="<? echo $autoreply_msg ?>" ROWS=10 COLS=60>
</TEXTAREA>
<P>
<CENTER>
<TABLE>
<TR>
<TD>
<INPUT TYPE=SUBMIT VALUE="Submit">
<INPUT TYPE=RESET VALUE="Clear">
</TD></TR></TABLE>
</CENTER>
</FORM>
