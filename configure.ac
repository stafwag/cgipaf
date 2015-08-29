dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT(cgipaf,1.4.0pre1,staf@wagemakers.be)
AC_CANONICAL_SYSTEM
AC_CONFIG_HEADER(config.h)
AC_SUBST(ARG)
AC_SUBST(NEED_COMPAT_H)

dnl
dnl OS Checks
dnl

AC_CANONICAL_SYSTEM

AC_MSG_CHECKING(Is your system compatible with CGIpaf?);

ARG="WITHPAM"

case $host_os in

 freebsd*) 
    		AC_DEFINE(BSDHOST,1,BSD system)
		BSDHOST=1
    		AC_DEFINE(FREEBSDHOST,1,FreeBSD system)
		AC_DEFINE(MODERNCRYPT,1,FreeBSD has a modern crypt)
		FREEBSDHOST=1
		MODERNCRYPT=1
 		AC_MSG_RESULT(yes)

		AC_MSG_CHECKING(FreeBSD release)
		AC_MSG_RESULT($host_os)
		MAJORREL=`echo $host_os | sed -e 's/freebsd\(.*\)/\1/' | cut -f 1 -d \.`
		AC_MSG_CHECKING(Major FreeBSD Release)
		AC_MSG_RESULT($MAJORREL)

		AC_MSG_CHECKING(Minor FreeBSD Release)
		MINORREL=`echo $host_os | sed -e 's/freebsd\(.*\)/\1/' | cut -f 2 -d \.`
		AC_MSG_RESULT($MINORREL)

		AC_MSG_CHECKING(Verifing if the pam password changer should work)

		if test "$MAJORREL" -lt 7; then

			AC_MSG_RESULT("Major version below 7 disabling PAM")
    			ARG="NOPAM"

		else

			if test "$MAJORREL" -eq 7; then

				if test "$MINORREL" -lt 3; then

					AC_MSG_RESULT("Version below 7.3 disabling PAM")
    					ARG="NOPAM"

				fi
				AC_MSG_CHECKING(Verifing if modern crypt shoud support sha2 protocols)
				AC_DEFINE(MODERNCRYPT_SHA2,1,FreeBSD 7.3 should support sha2)

			fi

		fi

		if test "$ARG" = "WITHPAM"; then

			AC_MSG_RESULT("yes")

		fi

		;;
   linux*) 
		AC_DEFINE(LINUXHOST,1,GNU Linux system)
   		AC_MSG_RESULT(yes)

		AC_MSG_CHECKING(Linux kernel release)
		LINUXREL=`uname -r`
		AC_MSG_RESULT($LINUXREL)
		AC_MSG_CHECKING(Major Linux kernel Release)
		MAJORREL=`echo $LINUXREL | cut -f1 -d\.`
		AC_MSG_RESULT($MAJORREL)
		AC_MSG_CHECKING(Minor Linux kernel Release)
		MINORREL=`echo $LINUXREL | cut -f2 -d\.`
		AC_MSG_RESULT($MINORREL)

		AC_MSG_CHECKING(Verifing that a modern crypt is present)

		if test "$MAJORREL" -ge 3; then

			AC_MSG_RESULT(Major Release is 3 or higher assume a modern crypt)
			AC_DEFINE(MODERNCRYPT,1,Linux kernel version is high enough assume a modern crypt)
			MODERNCRYPT=1
		else

			if test "$MAJORREL" -lt 2; then

				AC_MSG_RESULT(Major Release is lower thean 2 or higher assume  DESonly crypt)

			else

				AC_MSG_RESULT(Major Release is 2)
				AC_MSG_CHECKING(Verifing minor release)

				if test "$MINORREL" -ge 6; then

					AC_MSG_RESULT(Kernel version is higher than 2.6 assume modern crypt)
					AC_DEFINE(MODERNCRYPT,1,Linux kernel version is high enough assume a modern crypt)

					MODERNCRYPT=1

				else

					AC_MSG_RESULT(Kernel version is lower than 2.6 assume DESonly crypt)

				fi

			fi


		fi

		if test "$MODERNCRYPT"; then

			AC_DEFINE(MODERNCRYPT_SHA2,1,Linux 2.6 should support sha2)

		fi

		;;
  netbsd*) 
    		AC_DEFINE(BSDHOST,1,BSD system)
		BSDHOST=1
    		AC_DEFINE(NETBSDHOST,1,NetBSD system)
		NETBSDHOST=1
  		AC_MSG_RESULT(yes)

		AC_MSG_CHECKING(NetBsd Release)
		AC_MSG_RESULT($host_os)

		AC_MSG_CHECKING(Major NetBSD Release)

		MAJORREL=`echo $host_os | sed -e 's/netbsd\(.*\)/\1/' | cut -f 1 -d \.`
		
		AC_MSG_RESULT($MAJORREL)

		AC_MSG_CHECKING(If we can enable PAM on this NetBsd Release)

		if test "$MAJORREL" -lt 6; then

    			ARG="NOPAM"
			AC_MSG_RESULT(no)

		else
			
			AC_MSG_RESULT(yes)

		fi

		AC_MSG_CHECKING(Verify if we can enable modern crypt)

		if test "$MAJORREL" -lt 6; then

			AC_MSG_RESULT(no)

		else
			
			AC_DEFINE(MODERNCRYPT,1,NetBSD 6 has built-in md5 support)
			MODERNCRYPT=1
			AC_MSG_RESULT(yes)

		fi

  		;;
 openbsd*) 
    		AC_DEFINE(BSDHOST,1,BSD system)
		BSDHOST=1
    		AC_DEFINE(OPENBSDHOST,1,OpenBSD system)
		OPENBSDHOST=1
		AC_MSG_CHECKING(OpenBsd Release)

		AC_MSG_RESULT($host_os)

		AC_MSG_CHECKING(Major OpenBSD Release)
		MAJORREL=`echo $host_os | sed -e 's/openbsd\(.*\)/\1/' | cut -f 1 -d \.`
		AC_MSG_RESULT($MAJORREL)

		AC_MSG_CHECKING(Minor OpenBSD Release)
		MINORREL=`echo $host_os | sed -e 's/openbsd\(.*\)/\1/' | cut -f 2 -d \.`
		AC_MSG_RESULT($MINORREL)
		AC_DEFINE(OPENBSD_BLOWFISH,1,openbsd supports blowfish)

		AC_MSG_CHECKING(Checking if this OpenBSD Release is supported)

		if test $MAJORREL -lt "5"; then

  	   		AC_MSG_ERROR($host_os is not supported)

		fi

		if test $MINORREL -lt "7"; then

  	   		AC_MSG_ERROR($host_os is not supported)

		fi
		AC_MSG_RESULT(yes)
		AC_MSG_CHECKING(Disabling PAM)
        	ARG="NOPAM"
		AC_MSG_RESULT(done)
 		;;
 solaris*) 
 		AC_MSG_RESULT(yes) 
    		AC_DEFINE(SOLARISHOST,1,solaris system)
		SOLARISHOST=1
		AC_MSG_CHECKING(Solaris release)
		AC_MSG_RESULT($host_os)
		AC_MSG_CHECKING(Solaris version)
		SOLARISVERSION=`echo $host_os | sed -e 's/^solaris//'`
		AC_MSG_RESULT($SOLARISVERSION)

		AC_MSG_CHECKING(Solaris major release)
		MAJORREL=`echo $SOLARISVERSION |sed -e 's/^\(.*\)\..*/\1/'`
		AC_MSG_RESULT($MAJORREL)
		AC_MSG_CHECKING(Solaris minor release)
		MINORREL=`echo $SOLARISVERSION |sed -e 's/^.*\.\(.*\)/\1/'`
		AC_MSG_RESULT($MINORREL)

		AC_MSG_CHECKING(Verifing if we can enable MODERNCRYPT)
		if test "$MAJORREL" -ge 2; then


			if test "$MINORREL" -ge 10; then
		
				AC_MSG_RESULT(Yes)
				MODERNCRYPT=1
				AC_DEFINE(MODERNCRYPT,1,solaris version is high enough assume a modern crypt)
				AC_DEFINE(MODERNCRYPT_SHA2,1,Solaris 10+ should support sha2)

			else

				AC_MSG_RESULT(No)

			fi

		else

			AC_MSG_RESULT(No)


		fi
		;;
        *) 
		AC_MSG_RESULT(unkown)
	   	AC_MSG_WARN(CGIpaf is not tested with your system: $host_os)
	   	AC_MSG_WARN(Please inform me about the result)
		;;

esac

AC_C_BIGENDIAN(ENDIAN="BIG",ENDIAN="LITTLE",ENDIAN="UNKNOWN")
AC_MSG_CHECKING(checking endian)

if test $ENDIAN = "BIG"; then

	AC_MSG_RESULT("yes")
	AC_DEFINE(HIGHFIRST,1,big endian)

else
	AC_MSG_RESULT("no")

fi

AC_SUBST(host)
AC_SUBST(host_os)

AC_MSG_CHECKING(checking OS version)
HOSTREL=`uname -r 2> /dev/null`
AC_MSG_RESULT($HOSTREL)

if test "$ARG" = "WITHPAM"; then

	RESULT_MSG="I guess so"
	AC_MSG_CHECKING(authentication required at pam_chauthtok)

	if test "$SOLARISHOST" = "1";then

   		MAJORREL=`echo $HOSTREL | sed -e 's/\(.*\)\.\(.*\)/\1/'`
   		MINORREL=`echo $HOSTREL | sed -e 's/\(.*\)\.\(.*\)/\2/'`


   		if test "$MAJORREL" -eq "5"; then

      			if test "$MINORREL" -ge "8"; then

    				AC_DEFINE(PAM_CHPW_NOAUTH,1,do not auth at chpw)
   				RESULT_MSG="no"

      			fi

   		fi

	fi

fi

AC_MSG_RESULT($RESULT_MSG)
AC_DEFINE_UNQUOTED(HOST,"$host",system host)
AC_DEFINE_UNQUOTED(HOSTOS,"$host_os",system host os)
AC_DEFINE_UNQUOTED(HOSTREL,"$HOSTREL",system os release)

NEED_COMPAT_H="COMPAT"

dnl
dnl looking for the systems lock directory
dnl

AC_MSG_CHECKING(verifing the lock directory)

if test -d /var/lock; then

   LOCKDIR=/var/lock

else

   if test -d /var/spool/locks; then
   
	LOCKDIR=/var/spool/locks
	

   else 

   	if test -d /var/spool/lock; then
	
      		LOCKDIR=/var/spool/lock

	   else
            
		AC_MSG_WARN("Can't find lock directory using default /var/lock")
   		LOCKDIR=/var/lock

	fi

    fi

fi

AC_MSG_RESULT($LOCKDIR);

AC_DEFINE_UNQUOTED(LOCKDIR,"$LOCKDIR",lock directory)
AC_DEFINE_UNQUOTED(TMPLOCK,"${LOCKDIR}/cgipaf",lock file)

AC_SUBST(OBJS)
AC_SUBST(CHPASSOBJS)
AC_SUBST(HAVE_CHPASS_CRYPT)

PRESET_CFLAGS="$CFLAGS"

OBJS="xstring.o ccgi.o configfile.o showmsg.o ephp.o accessdb.o createcookie.o searchdomain.o xmalloc.o out_of_memory.o run_cmd.o acl.o write_log.o cgipaf_func.o"

dnl
dnl Checks for programs.
dnl

AC_PROG_CC
AC_PROG_MAKE_SET

dnl
dnl extra configure arguments
dnl

AC_ARG_ENABLE(pam,[  --enable-pam            Enable pam support (default) ],
   [ if test "$enableval" != "no"; then
        ARG="WITHPAM"
	if test "$enableval" = "yes"; then
	   ENABLE_PAM="yes"
	fi
      else
        ARG="NOPAM"
      fi
   ]
   ) 

AC_ARG_WITH(libcrack, [  --with-libcrack   try to use librack ])
AC_ARG_ENABLE(md5crypt, [  --enable-md5crypt       enable MD5 or higher (default) ])

dnl pam support test

if test "$ARG" = "WITHPAM"; then
   AC_MSG_CHECKING(checking for PAM support...)
   ls -l /etc/pam* > /dev/null 2>&1
   if test "$?" = "0"; then
      ENABLE_PAM="yes"
      AC_MSG_RESULT(yes);
      else
         AC_MSG_RESULT("no");
	 dnl only disable pam if it isnt enabled with --enable-pam
	 if test "$ENABLE_PAM" != "yes"; then
	    ARG="NOPAM"
            AC_MSG_WARN("PAM support is disabled...")
	 fi
   fi

   if test "$ARG" != "NOPAM"; then

      AC_CHECK_LIB(pam,pam_start,ARG="WITHPAM",ARG="")
      if test "$ENABLE_PAM"="yes"; then
         if test "$ARG" != "WITHPAM"; then
            AC_MSG_ERROR("PAM support enabled but pam_start function not found install the pam development package and rerun configure")
         fi
      fi

   fi


fi

if test "$ARG" != "WITHPAM"; then
   AC_MSG_WARN(***********************************************************)
   AC_MSG_WARN(you are compiling CGIpaf)
   AC_MSG_WARN(without pam support. systems without pam support are only)
   AC_MSG_WARN(supported if your system use the standard password file)
   AC_MSG_WARN(location /etc/passwd /etc/shadow and uses the crypt)
   AC_MSG_WARN(function to encrypt the passwords)
   AC_MSG_WARN(***********************************************************)


	AC_CHECK_FUNCS(crypt,CHPASSCRYPT="yes",NEED_CRYPTLIB="yes")


	if test "$NEED_CRYPTLIB" = "yes"; then

   		AC_CHECK_LIB(crypt,crypt,CHPASSCRYPT="yes",AC_MSG_ERROR(no pam and no crypt library found I am giving up))
		LIBS="$LIBS -lcrypt"

	fi

	HAVE_CHPASS_CRYPT="CHPASS_CRYPT"
dnl
dnl  most un*ces have crypt defined in unistd.h
dnl   if test "$HAVE_CRYPT_H" != "1"; then
dnl      AC_MSG_ERROR(pam support is disabled and crypt.h not found)
dnl   fi
dnl

   OBJS="$OBJS pass.o salt.o"
   
   if test "$enable_md5crypt" != "no"; then

dnl freebsd has built-in md5 support
 
     if test "$MODERNCRYPT" != "1"; then
        OBJS="$OBJS md5.o md5crypt.o"
     fi

     if test "$BSDHOST" != "1"; then
        OBJS="$OBJS fgetpwnam.o";
     else
        AC_MSG_WARN("BSD host detected vmail password are disabled")
     fi

      AC_DEFINE(MD5_CRYPT,1,md5 crypt functions required)
   fi

dnl
dnl crypt_newhash if required for OpenBSD
dnl

	if test "$OPENBSDHOST" = "1"; then
		AC_CHECK_FUNCS(crypt_newhash)
	fi

	CHPASSOBJS=$OBJS

 else
	AC_CHECK_LIB(pam,pam_authenticate,PAMLIB_ERROR="no",PAMLIB_ERROR="yes")
   	AC_CHECK_LIB(pam,pam_chauthtok,PAMLIB_ERROR="no",PAMLIB_ERROR="yes")
	AC_CHECK_LIB(pam,pam_end,,PAMLIB_ERROR="yes")
	if test "$PAMLIB_ERROR" = "yes"; then
   		AC_MSG_ERROR(no valid pam library found)
   	fi
   	OBJS="$OBJS passpam.o"
	CHPASSOBJS="$OBJS pass.o salt.o"

   	AC_CHECK_LIB(crypt,crypt,CHPASSCRYPT="yes",AC_MSG_WARN(no crypt library ))

	if test "$CHPASSCRYPT" = "yes"; then

		HAVE_CHPASS_CRYPT="CHPASS_CRYPT"
		LIBS="$LIBS -lcrypt"

		dnl freebsd has built-in md5 support
 
   		if test "$MODERNCRYPT" != "1"; then

     			CHPASSOBJS="$CHPASSOBJS md5.o md5crypt.o"

   		fi

		if test "$BSDHOST" != "1"; then

     			CHPASSOBJS="$CHPASSOBJS fgetpwnam.o";

   		else

     			AC_MSG_WARN("BSD host detected vmail password are disabled")

   		fi

    		AC_DEFINE(MD5_CRYPT,1,md5 crypt functions required)
	else

		HAVE_CHPASS_CRYPT="NO_CHPASS_CRYPT"

	fi

fi

   		AC_CHECK_HEADERS(stdint.h,, [ HAVE_STDINT_H="no" ])

		if test HAVE_STDINT_H = "no"; then

     			AC_CHECK_HEADERS(sys/stdint.h,,[ HAVE_SYS_STDINT_H="no" ])

			if test HAVE_SYS_STDINT_H = "no"; then

    				AC_CHECK_HEADERS(sys/inttypes.h,,[ AC_MSG_ERROR(no valid stdint header file found) ])

			fi

		fi


if test "$with_libcrack" != "no"; then
   AC_CHECK_LIB(crack,FascistCheck,,AC_MSG_WARN(cracklib not found cracklib support is disabled!!!!))
fi

AC_CHECK_FUNCS(dbm_open dbm_fetch dbm_store dbm_close,,NEED_NDBMLIB="yes")

if test "$NEED_NDBMLIB" = "yes"; then

   AC_CHECK_LIB(ndbm,dbm_open,NDBM_LIB="yes",NDBM_LIB="no")
   AC_CHECK_LIB(ndbm,dbm_fetch,NDBM_LIB="yes",NDBM_LIB="no")
   AC_CHECK_LIB(ndbm,dbm_store,NDBM_LIB="yes",NDBM_LIB="no")
   AC_CHECK_LIB(ndbm,dbm_close,,NDBM_LIB="no")

   if test "$NDBM_LIB" = "no"; then 
  
      AC_CHECK_LIB(gdbm,dbm_open,GDBM_LIB="yes",GDBM_LIB="no")
      AC_CHECK_LIB(gdbm,dbm_fetch,GDBM_LIB="yes",GDBM_LIB="no")
      AC_CHECK_LIB(gdbm,dbm_store,GDBM_LIB="yes",GDBM_LIB="no")
      AC_CHECK_LIB(gdbm,dbm_close,,GDBM_LIB="no")
      
      if test "$GDBM_LIB" = "no"; then 

    	 AC_CHECK_LIB(gdbm_compat,dbm_open,GDBM_COMPAY="yes",GDBM_COMPAT_LIB="no")
     	 AC_CHECK_LIB(gdbm_compat,dbm_fetch,GDBM_COMPAY="yes",GDBM_COMPAT_LIB="no")
     	 AC_CHECK_LIB(gdbm_compat,dbm_store,GDBM_COMPAY="yes",GDBM_COMPAT_LIB="no")
     	 AC_CHECK_LIB(gdbm_compat,dbm_close,,GDBM_COMPAT_LIB="no")

    	 if test "$GDBM_COMPAT_LIB" = "no"; then 

            AC_CHECK_LIB(db1,dbm_open,DB1_LIB="yes",DB1_LIB="no")
            AC_CHECK_LIB(db1,dbm_fetch,DB1_LIB="yes",DB1_LIB="no")
            AC_CHECK_LIB(db1,dbm_store,DB1_LIB="yes",DB1LIB="no")
            AC_CHECK_LIB(db1,dbm_close,,DB1_LIB="no")

            if test "$DB1_LIB" = "no";then
  
               AC_CHECK_LIB(db,dbm_open,DB_LIB="yes",DB_LIB="no") 
               AC_CHECK_LIB(db,dbm_fetch,DB_LIB="yes",DB_LIB="no")
               AC_CHECK_LIB(db,dbm_store,DB_LIB="yes",DB_LIB="no") 
               AC_CHECK_LIB(db,dbm_close,,DB_LIB="no") 

	       if test "$DB_LIB" = "no";then

	          old_libs=$LIBS
	          LIBS="$LIBS -ldb"

	          AC_MSG_CHECKING("for dbm_open in -ldb with DB_DBM_HSEARCH 1")

	          AC_TRY_LINK([#define DB_DBM_HSEARCH 1
	                       #include <db.h>],
	                      [dbm_open(0,0,0);],
	                      [DB_COMPAT_LIB="yes"])

	          if test "$DB_COMPAT_LIB" != "yes"; then
	          
	             AC_MSG_RESULT(no)
		     LIBS="$LIBS -lpthread"
	          
		     AC_MSG_CHECKING("for dbm_open in -ldb -lphtread with DB_DBM_HSEARCH 1")

	             AC_TRY_LINK([#define DB_DBM_HSEARCH 1
	                          #include <db.h>],
	                         [dbm_open(0,0,0);],
	                         [DB_COMPAT_LIB="yes"])

	             if test "$DB_COMPAT_LIB" != "yes"; then

	                AC_MSG_RESULT(no)
	                AC_MSG_ERROR(no valid ndbm library found)

	             fi
	          fi

	          AC_DEFINE_UNQUOTED(HAVE_DB_H_DB_DBM_HSEARCH,"1",db4 with hsearch)
	          AC_MSG_RESULT(yes)

	       fi
            fi
	 fi
      fi
   fi
fi

dnl Checks for header files.

AC_HEADER_STDC

dnl freebsd has crypt() defined in unistd.h

if test "$FREEBSDHOST" != "1"; then

 AC_CHECK_HEADERS(crypt.h,,[ AC_MSG_WARN(crypt.h not found) ])

fi

if test "$HAVE_LIBCRACK" = "1"; then 

   AC_CHECK_HEADERS(crack.h,,AC_MSG_WARN(crack.h not found usign built-in FascistCheck define...))

fi

HAVE_GDBMDIR_NDBM_H="NO_GDBMDIR_NDBM_H"

if test "$DB_COMPAT_LIB" = "yes"; then

   AC_CHECK_HEADERS(db.h,, [ HAVE_DB_H="no" ])

else

    AC_CHECK_HEADERS(ndbm.h,,[ HAVE_NDBM_H="no" ]) 

    if test "$HAVE_NDBM_H" = "no"; then
       AC_CHECK_HEADERS(gdbm-ndbm.h,,[ HAVE_GDBM_NDBM_H="no" ])
      if test "$HAVE_GDBM_NDBM_H" = "no"; then
          unset HAVE_GDBM_NDBM_H
          AC_MSG_CHECKING(for gdbm/ndbm.h...)
          AC_EGREP_HEADER(gdbm/ndbm.h,gdbm/ndbm.h, \
          [ HAVE_GDBMDIR_NDBM_H="HAVE_GDBMDIR_NDBM_H" \
            AC_SUBST(HAVE_GDBMDIR_NDBM_H); AC_MSG_RESULT(yes)  ], \
          [ HAVE_GDBMDIR_NDBM_H="no"; AC_MSG_RESULT(no) ])
          if test "$HAVE_GDBMDIR_NDBM_H" = "no"; then
               HAVE_GDBMDIR_NDBM_H="NO_GDBMDIR_NDBM_H"
	        AC_CHECK_HEADERS(db1/ndbm.h,,AC_MSG_ERROR(no valid ndbm header file found))
          fi
      fi
    fi

fi


AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(sys/wait.h)
AC_CHECK_HEADERS(sys/types.h)

AC_HEADER_TIME

AC_HEADER_SYS_WAIT


AC_CHECK_HEADERS(unistd.h,,AC_MSG_ERROR(unistd.h not found))
AC_CHECK_HEADERS(stdlib.h,,AC_MSG_ERROR(stdlib.h not found))
AC_CHECK_HEADERS(stdio.h,,AC_MSG_ERROR(stdio.h not found))
AC_CHECK_HEADERS(string.h,,AC_MSG_ERROR(string.h not found))
AC_CHECK_HEADERS(ctype.h,,)
AC_CHECK_HEADERS(fcntl.h,,AC_MSG_ERROR(fcntl.h not found))
AC_CHECK_HEADERS(malloc.h,,)
AC_CHECK_HEADERS(errno.h,,AC_MSG_ERROR(errno.h not found))
AC_CHECK_HEADERS(sys/stat.h,,AC_MSG_ERROR(sys/stat.h not found))
AC_CHECK_HEADERS(pwd.h,,AC_MSG_ERROR(pwd.h not found))
AC_CHECK_HEADERS(stdarg.h,,AC_MSG_ERROR(stdarg.h not found))
AC_CHECK_HEADERS(syslog.h,,AC_MSG_ERROR(syslog.h not found))
AC_CHECK_HEADERS(libgen.h,,AC_MSG_ERROR(libgen.h not found))
AC_CHECK_HEADERS(shadow.h,,)

if test "$ARG" != "WITHPAM"; then
 
    ARG="NOPAM"
 
else
 
    AC_CHECK_HEADERS(security/pam_appl.h,,AC_MSG_ERROR(pam_appl.h not found))
 
fi

if test -z "$PRESET_CFLAGS"; then

   CFLAGS=`echo "$CFLAGS" | sed -e 's/-g//'`

fi

dnl Checks for typedefs, structures, and compiler characteristics.

AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.

AC_CHECK_FUNCS(free realloc strdup strerror strstr strcasecmp strncasecmp strrchr strchr strnchr va_start va_end vsnprintf strncat strncpy openlog syslog closelog setuid setreuid getuid getspnam gettimeofday memset l64a)
AC_CHECK_FUNCS(unsetenv,,NEED_COMPAT_H="yes")
AC_CHECK_FUNCS(setenv,,NEED_COMPAT_H="yes")

AC_CHECK_FUNCS(getdomainname,,NO_GETDOMAINNAME=yes)
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_FUNC_STAT
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_VPRINTF

if test "$NO_GETDOMAINNAME" = "yes"; then

   AC_CHECK_HEADERS(sys/systeminfo.h)
   AC_CHECK_FUNCS(sysinfo)
   NEED_COMPAT_H="yes"

fi

AC_CHECK_FUNCS(gethostname,,NO_GETHOSTNAME=yes)

if test "$NO_GETHOSTNAME" = "yes"; then

    AC_CHECK_HEADERS(sys/systeminfo.h)
    AC_CHECK_FUNCS(sysinfo,,AC_MSG_ERROR(your system dont support gethostname\n and i'm unable to emulate it))
    NEED_COMPAT_H="yes"

fi

if test "$NEED_COMPAT_H" = "yes"; then

    OBJS="$OBJS compat.o"
    CHPASSOBJS="$CHPASSOBJS compat.o"
    NEED_COMPAT_H="NEED_COMPAT_H"

fi

AC_OUTPUT(Makefile install.sh createconfig.sh)
