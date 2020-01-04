#!/bin/sh

#
# update_password_mv_pwd_mkdb.sh
#
# Copyright (C) 2016, 2020 Staf Wagemakers Belgie/Belgium
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#



. `dirname $0`/../config.sh

sudo mv /usr/sbin/pwd_mkdb /usr/sbin/pwd_mkdb_cgipaf_tst || {


	echo "ERROR: sudo mv /usr/sbin/pwd_mkdb /usr/sbin/pwd_mkdb_cgipaf_tst failed"
	exit 1

}

echo "tst1:qwerty" | $changepass -n

exitCode=$?

sudo mv /usr/sbin/pwd_mkdb_cgipaf_tst /usr/sbin/pwd_mkdb || {

	echo "ERROR: sudo mv /usr/sbin/pwd_mkdb_cgipaf_tst /usr/sbin/pwd_mkdb failed"
	exit 1	

}

if [ "$exitCode" = 0 ]; then

	echo "ERROR: changepass was successful but it should fail"
	exit 1


fi

exit 0

	

