#!/bin/sh

#
# run_and_report.sh
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

which kyua || {

	PATH=$PATH:/usr/local/bin

}

scriptName=`realpath $0`
scriptDir=`dirname $scriptName`
reportDir="${scriptDir}/reports/"

os=`uname| tr [A-Z] [a-z]`

if [ -f $scriptDir/$os/Kyuafile ]; then

	cd $scriptDir/$os || {

		echo "ERROR: cd $scriptDir/$os failed"
		exit 1

	}


else

	echo "ERROR: no test defined for $os"
	exit 1


fi

if [ ! -d "$reportDir" ]; then

	echo "ERROR: reportDir \"$reportDir\"  doesn't exists"
	exit 1

fi

kyua test
kyua report-junit > ${reportDir}/kyua_report.xml
