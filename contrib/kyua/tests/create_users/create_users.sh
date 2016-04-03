#!/bin/sh

set +x

#
# create_users.sh
#
# Copyright (C) 2016 Staf Wagemakers Belgie/Belgium
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

catPasswd() {


	isBSD && {

		EDITOR=cat vipw
		return $?

	}

	cat /etc/passwd


}

userAdd() {

	user=$1

	isFreeBsd && {

		pw useradd $user
		return $?

	}

	isNetBsd && {

		user add $user
		return $?

	}

	myUserAdd=`which useradd > /dev/null 2>&1` && {

		if [ -x "$myUserAdd" ]; then

			$myUserAdd $user
			return $?

		fi

	}

	if [ ! -x /usr/sbin/useradd ]; then

		echo "ERROR: no working useradd found"
		exit 13

	fi

	/usr/sbin/useradd $user

}


checkIfUserExists() {

	user=$1

	if [ $user = "" ]; then

		return -1

	fi

	
	catPasswd | sed -e 's/^\([^:]*:\).*$/\1/g' | grep "$user:" >/dev/null && {

		 return 0 

	}

	return 1

}

createUser() {

	user=$1

	checkIfUserExists $user || {

		userAdd $user || {

			return $?
		}

	}

	return 0

}

echo "Users = \"${users}\""

for user in $users; do

	createUser $user || {

		 exitCode=$?
		 echo "ERROR: createUser $user failed"

		 exit $exitCode

	}

done

exit 0
