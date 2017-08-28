#!/bin/sh

#
# config.sh
#
# Copyright (C) 2016, 2017 Staf Wagemakers Belgie/Belgium
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

createUserList() {

        n=1

        userPrefix="tst"
        users=""

        while [ "$n" -lt 50 ]; do


                if [ "$n" -eq "1" ]; then

                        users="${userPrefix}${n}"

                else
                        users="${users} ${userPrefix}${n}"

                fi

		n=`expr $n + 1`

        done

}

isLinux() {

        uname | grep -i linux > /dev/null && {

                return 0


        }

        return 1




}

isSolaris() {

	uname | grep -i sunos > /dev/null && {

		return 0


	}

	return 1


}

isBSD () {

	uname | grep -i bsd > /dev/null && {

		return 0

	}

	return 1


}

isFreeBsd () {

	uname | grep -i freebsd > /dev/null && {

		return 0

	}

	return 1


}

isNetBsd () {

	uname | grep -i netbsd > /dev/null && {

		return 0

	}

	return 1


}

isOpenBsd () {

	uname | grep -i netbsd > /dev/null && {

		return 0

	}

	return 1


}

scriptName=`basename $0`

baseDir=`dirname $0`

Echo="echo"

isSolaris && {

	Echo="/usr/ucb/echo"

	if [ ! -x "$Echo" ]; then

		echo "Sorry no valid echo found"
		exit 1

	fi

} 


len=`$Echo "\n" | wc -l`

if [ "$len" -ne "1" ]; then

	Echo="/bin/echo"

	len=`$Echo "\n" | wc -l`
	
	if [ "$len" -ne "1" ]; then

		echo "Sorry no valid echo found"
		exit 1

	fi

fi



changepassDir="${baseDir}/../../../../"
changepass="${changepassDir}/changepass"

if [ ! -x "$changepass" ]; then

	echo "ERROR: Sorry failed to find the changepass program \"$changepass\" "
	exit 1

fi

hash2name="${changepassDir}/hash2name"

if [ ! -x "$hash2name" ]; then

	echo "ERROR: Sorry failed to find the changepass hash2name \"$hash2name\" "
	exit 1

fi

toolDir="${baseDir}/../../tools"
chkPass="${toolDir}/chkpass.py"
genPass="${toolDir}/genpass.py"

if [ ! -x "$chkPass"  ]; then

	echo "ERROR: Sorry failed to find the chkpass program \"$chkPass\" "
	exit 1

fi

if [ ! -x "$genPass"  ]; then

	echo "ERROR: Sorry failed to find the chkpass program \"$genPass\" "
	exit 1

fi
createUserList

tmpDir="${baseDir}/../../tmp"

if [ ! -d "$tmpDir" ]; then

	echo "ERROR: Sorry tmpDir \"$tmpDir\" doesn't exists"
	exit 1

fi

libDir="${baseDir}/../../lib"

if [ ! -d "$libDir" ]; then

	echo "ERROR: Sorry tmpDir \"$libDir\" doesn't exists"
	exit 1

fi

. $libDir/update_passwords.sh
