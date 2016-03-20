#!/bin/sh


. `dirname $0`/../config.sh

mv /usr/sbin/pwd_mkdb /usr/sbin/pwd_mkdb_cgipaf_tst || {


	echo "ERROR: mv /usr/sbin/pwd_mkdb /usr/sbin/pwd_mkdb_cgipaf_tst failed"
	exit 1

}

echo "tst1:qwerty" | $changepass -n

exitCode=$?

mv /usr/sbin/pwd_mkdb_cgipaf_tst /usr/sbin/pwd_mkdb || {

	echo "ERROR: mv /usr/sbin/pwd_mkdb_cgipaf_tst /usr/sbin/pwd_mkdb failed"
	exit 1	

}

if [ "$exitCode" = 0 ]; then

	echo "ERROR: changepass was successful but it should fail"
	exit 1


fi

exit 0

	

