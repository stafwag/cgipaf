#!/bin/sh
# creates cgipaf.conf			        staf wagemakers
#                                               staf@patat.org
#                                               http://staf.patat.org

prefix="/usr/local"
exec_prefix="${prefix}"
exec_suffix="@exec_suffix@"
cgi_suffix="@cgi_suffix@"
top_srcdir="."
bindir="${exec_prefix}/bin"
libexecdir="${exec_prefix}/libexec"
sysconfdir="${prefix}/etc"
localstatedir="${prefix}/var"
srcdir="."
datadir="${prefix}/share"


if [ ! -x ./passwd.cgi ]; then
   echo "type make to compile cgipaf"
   exit 1
fi

CFGFILE="./cgipaf.conf"
CGIBIN="${bindir}"
WWW="${datadir}"
   echo "#" > $CFGFILE
   echo "# sample cgipaf config file" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# set the pam service name, if not set passwd is used" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "# pam_service		 cgipaf" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# enable syslog messages" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "syslog			 on" >> $CFGFILE
   echo "loglevel		 6" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# Enable Access Control lists" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "# ACL example" >> $CFGFILE
   echo >> $CFGFILE
   echo "# AclOrder              Deny,Allow" >> $CFGFILE
   echo "# DenyUsers             *" >> $CFGFILE
   echo "# DenyGroups            *" >> $CFGFILE
   echo "# AllowUsers            user1 user2 user3" >>$CFGFILE
   echo "# AllowGroups           group1 group2 group3" >>$CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# accessdb configuration" >> $CFGFILE
   echo >> $CFGFILE
   echo "accessdb                /etc/cgipaf/accessdb" >> $CFGFILE
   echo "max_invalid             10" >> $CFGFILE
   echo "invalid_timeout         60" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# set the minimum uid & maximum uid, if min_uid isn't set the" >> $CFGFILE
   echo "# default (10) is used. if max_uid isn't set there is no" >> $CFGFILE
   echo "# maximum" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "min_uid                 101" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# passwd.cgi configuration" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "<passwd>" >> $CFGFILE
   echo >> $CFGFILE
   echo "# syntax:      document      document_name" >> $CFGFILE
   echo "#              document      redirect          location" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "document_root           $WWW" >> $CFGFILE
   echo "login_document          cgipasswd.php" >> $CFGFILE
   echo "error_loginname         cgipasswd_errlogin.php" >> $CFGFILE
   echo "error_access            cgipasswd_erraccess.php" >> $CFGFILE
   echo "error_invalid           cgipasswd_errinvalid.php" >> $CFGFILE
   echo "error_newpassword       cgipasswd_errnewpass.php" >> $CFGFILE
   echo "error_match             cgipasswd_errmatch.php" >> $CFGFILE
   echo "error_unchanged         cgipasswd_errunchanged.php" >> $CFGFILE
   echo "error_tooshort          cgipasswd_errshort.php" >> $CFGFILE
   echo "error_locked            cgipasswd_errlocked.php" >> $CFGFILE
   echo "# run_locked            /usr/local/bin/notify_script %{name}" >> $CFGFILE
   echo "error_toolong           cgipasswd_errlong.php" >> $CFGFILE
   echo "error_cracklib          cgipasswd_errbadpassword.php" >> $CFGFILE
   echo "error_illegalword       cgipasswd_errbadpassword.php" >> $CFGFILE
   echo "error_pam               cgipasswd_errpam.php" >> $CFGFILE
   echo "msg_success             cgipasswd_msgchanged.php" >> $CFGFILE
   echo "# run_success             \"/usr/bin/smbpasswd -s -U %{name} > /dev/null 2>&1\" 	\"%{password}\n%{password}\n\"" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# set the minimum & maximum password length" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "min_length              5" >> $CFGFILE
   echo "max_length              16" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# enable cracklib support" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "# cracklib on" >> $CFGFILE
   echo "# cracklib_dictpath    /var/cache/cracklib/cracklib_dict" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# illegal words ( words that are not allowed in a new password" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# illegal_words         foo bar" >> $CFGFILE 
   echo >> $CFGFILE
   echo "</passwd>" >> $CFGFILE
   echo >> $CFGFILE
   echo "#" >> $CFGFILE
   echo "# mailcfg configuration" >> $CFGFILE
   echo "#" >> $CFGFILE
   echo >> $CFGFILE
   echo "<mailcfg>" >> $CFGFILE
   echo >> $CFGFILE
   echo "cookie_timeout          300" >> $CFGFILE
   echo "document_root           $WWW" >> $CFGFILE
   echo "login_document          viewmailcfg.php" >> $CFGFILE
   echo "mailcfg_document        mailcfg.php" >> $CFGFILE
   echo "error_readname		 mailcfg_errreadname" >> $CFGFILE
   echo "error_accessdb		 mailcfg_erraccessdb.php" >> $CFGFILE
   echo "error_loginname         mailcfg_errloginname.php" >> $CFGFILE
   echo "error_access		 mailcfg_erraccess.php" >> $CFGFILE
   echo "error_invalid           mailcfg_errinvalid.php" >> $CFGFILE
   echo "error_locked            mailcfg_errlocked.php" >> $CFGFILE
   echo "error_readforward	 mailcfg_errforward.php" >> $CFGFILE
   echo "error_forwardto	 mailcfg_errforwardto.php" >> $CFGFILE
   echo "error_invalidforwardto	 mailcfg_errinvalidforwardto.php" >> $CFGFILE
   echo "error_readkeepmsg	 mailcfg_errkeepmsg.php" >> $CFGFILE
   echo "error_readautoreply	 mailcfg_errautoreply.php" >> $CFGFILE
   echo "error_autoreplymsg      mailcfg_errautoreplymsg.php" >> $CFGFILE
   echo "error_cookie		 mailcfg_errcookie.php" >> $CFGFILE
   echo "error_cookietimeout	 mailcfg_errcookietimeout.php" >> $CFGFILE
   echo "error_deldotforward	 mailcfg_errdeldotforward.php" >> $CFGFILE
   echo "error_deldotprocmailrc  mailcfg_errdeldotprocmailrc.php" >> $CFGFILE
   echo "error_openvacations	 mailcfg_erropenvacations.php" >> $CFGFILE
   echo "msg_success		 mailcfg_msgupdated.php" >> $CFGFILE
   echo "# sendmail              /usr/lib/sendmail" >> $CFGFILE
   echo "# domain		 your_domain_name" >> $CFGFILE
   echo >> $CFGFILE
   echo "# create a backup of the user's .procmailrc and restore it" >> $CFGFILE
   echo "# when mail forwarding and autoreply is disabled" >> $CFGFILE
   echo >> $CFGFILE
   echo "# run_before_mailcfg      \"cp %{homedir}/.procmailrc %{homedir}/.procmailrc_backup\"" >> $CFGFILE
   echo "# run_after_mailcfg       \"cp %{homedir}/.procmailrc_backup %{homedir}/.procmailrc\"" >> $CFGFILE
   echo >> $CFGFILE
   echo "</mailcfg>" >> $CFGFILE
   chmod 644 $CFGFILE
