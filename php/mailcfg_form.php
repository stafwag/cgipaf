<HR>
<P>
<FORM ACTION="/cgi-bin/mailcfg.cgi" METHOD=POST>
<TABLE>
<TR>
<TD>Login name:</TD>
<TD ALIGN="left"><? echo $name ?>
<input type="hidden" name="name" value="<? echo $name ?>"></TD>
</TR>
</TABLE>
<HR>
<TABLE>
<TR>
<TD>Forward mailbox:</TD>
<TD WIDTH=20>
<SELECT NAME="forward">
<OPTION value="<? echo $forward ?>"><? echo $forward ?> 
<OPTION value="<? echo $not_forward ?>"><? echo $not_forward ?>
</SELECT></TD>
<TD>Forward to:</TD>
<TD><INPUT NAME="forward_to" VALUE="<? echo $forward_to ?>" TYPE=TEXT SIZE=30 MAXLENGTH=40></TD>
</TR>
<TR>
<TD>Keep messages:</TD>
<TD>
<SELECT NAME="keep_msg">
<OPTION value="<? echo $keep_msg ?>"><? echo $keep_msg ?>
<OPTION value="<? echo $not_keep_msg ?>"><? echo $not_keep_msg ?>
</SELECT><TD>
</TR>
</TABLE>
<HR>
<TABLE>
<TR>
<TD>Enable autoReply:</TD>
<TD>
<SELECT NAME="autoreply">
<OPTION value="<? echo $autoreply ?>"><? echo $autoreply ?>
<OPTION value="<? echo $not_autoreply ?>"><? echo $not_autoreply ?>
</SELECT>
</TD>
</TABLE>
Autoreply message:<BR>
<TEXTAREA NAME="autoreply_msg" ROWS=10 COLS=60><? echo $autoreply_msg; ?></TEXTAREA>
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
</body>
</html>
