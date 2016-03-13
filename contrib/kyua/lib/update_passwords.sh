#!/bin/sh

#
# update_passwords.sh
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

getShadowLine() {

	os=`uname`
	isBSD=""

	echo $os | grep -i bsd >/dev/null && {

		echo "bsd system detected"

		isBSD="yes"
	} 


	user=$1

	if [ "$isBSD" = "yes" ]; then


		hashLine=`EDITOR=cat vipw | grep -E "^${user}:"` || {

			echo ""
			return 1


		}

	else

		hashLine=`cat /etc/shadow | grep -E "^${user}:"` || {


			echo ""
			return 1


			}

	fi

	echo $hashLine
}


getHashType() {

	user=$1

	shadowline=`getShadowLine $user`

	if [ "$shadowline" = "" ]; then

		echo "failed to find user shadowline"
		exit 1


	fi

	hashEntry=`echo $shadowline | cut -f2 -d:`

	hashType=`$hash2name $hashEntry | tr [A-Z] [a-z]`

	echo $hashType

}

update_passwords() {

changepass="$changepass $@"

echo "changepass set to \"$changepass\"" 
echo "chkpass set to \"$chkPass\"" 
echo "genpass set to \"$genPass\"" 

myCounter=0
max=10

while [ "$myCounter" -lt "$max" ]; do 

	tmpFile="${tmpDir}/tmp_${scriptName}_$$_${myCounter}"

	echo "Creating ${tmpFile} ..."

	> ${tmpFile} || {

		echo "ERROR: failed to create ${tmpFile}"
		return 1

	}

	passSize=$((8+$myCounter))

	for user in $users; do


		password=""
		password=`$genPass $passSize` || {

			echo "ERROR: Sorry $genPass $passSize failed"
			exit 1


		}

		password=`echo $password | tr ':' '_'`

		echo "$user:$password" >> ${tmpFile}

	done

	echo "done ..."

	echo "running changepass..."

	cat $tmpFile | $changepass || {

		exitCode=$?
		echo "ERROR: Sorry changepass failed"
		return  $exitCode

	} 

	echo "done ..."

	if [ -n "$requiredHashType" ]; then

		for user in $users; do

			userHashType=`getHashType $user`

			echo "user \"$user\" has hashType \"${userHashType}\""

			if [ "$requiredHashType" != "$userHashType" ];then

					echo "ERROR: \"${user}\" has hashType \"${userHashType}\" while \"${requiredHashType}\" is required"
					exit 1

			fi

		done


	fi 

	echo "running chkpass..."

	cat $tmpFile | $chkPass || {

		exitCode=$?
		echo "ERROR: Sorry chkPass failed"
		return $exitCode

	}

	echo "done ..."

	echo "unlinking ${tmpFile}..."

	rm $tmpFile || {

		exitCode=$?
		echo "ERROR: Sorry unlink $tmpFile failed"
		return $exitCode

	}

	myCounter=$(($myCounter+1))

done

}


