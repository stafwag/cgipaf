<HR>
<P>
<FORM ACTION="/mailcfg.cgi" METHOD=POST>
<TABLE WIDTH="100%">
<TR>
<TD>Login naam:</TD>
<TD ALIGN="LEFT"><INPUT NAME=name VALUE="<? echo $name ?>" TYPE=TEXT SIZE=10 MAXLENGTH=15></TD>
<TD ALIGN="LEFT">wachtwoord :</TD>
<TD ALIGN="LEFT"><INPUT NAME=passwd TYPE=PASSWORD SIZE=8 MAXLENGTH=10> (TYPE UW WACHTWOORD OPNIEUW!!!)</TD>
</TR>
</TABLE>
<HR>
<TABLE>
<TR>
<TD>E-mail doorsturen:</TD>
<TD>
<SELECT NAME="forward">
<OPTION value="<? echo $forward ?>"><? echo $forward ?> 
<OPTION value="<? echo $not_forward ?>"><? echo $not_forward ?>
</SELECT></TD>
<TD>Doorsturen aan:</TD>
<TD><INPUT NAME=forwardname VALUE="<? echo $forwardto ?>" TYPE=TEXT SIZE=30 MAXLENGTH=40></TD>
</TR>
<TR>
<TD>Berichten ook zelf bewaren:</TD>
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
<TD>Afwezigheidsassistente aanzetten:</TD>
<TD>
<SELECT NAME="reply">
<OPTION value="<? echo $autoreply ?>"><? echo $autoreply ?>
<OPTION value="<? echo $not_autoreply ?>"><? echo $notautoreply ?>
</SELECT>
</TD>
</TABLE>
Afwezigheidsbericht:<BR>
<TEXTAREA NAME="<? echo $autoreply_msg ?>" ROWS=10 COLS=60>
</TEXTAREA>
<P>
<CENTER>
<TABLE>
<TR>
<TD>
<INPUT TYPE="RESET" VALUE="Clear">
<INPUT TYPE="SUBMIT" VALUE="Submit">
</TD></TR></TABLE>
</CENTER>
</FORM>
