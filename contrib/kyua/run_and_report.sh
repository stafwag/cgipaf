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
release=`uname -r | sed -e 's/^\([^.]*\.[^.]*\)\..*/\1/'`
majorRelease=`echo $release | cut -f1 -d\.`
minorRelease=`echo $release | cut -f2 -d\.`

echo "found \"$os\" with release \"$release\" (\"${majorRelease}\".\"${minorRelease}\")"

kyuaMainDir=""

case "$os" in

	"linux")
		kyuaMainDir="scriptDir/$os"
		;;
	"freebsd")
		kyuaMainDir="scriptDir/${os}/${majorRelease}"
		;;
	*)

		echo "ERROR: sorry no test define for \"$os\""
		exit 1

		;;

esac


if [ -f ${kyuaMainDir}/Kyuafile ]; then

	cd $kyuaMainDir || {

		echo "ERROR: cd $kyuaMainDir failed"
		exit 1

	}

else

	echo "ERROR: Kyuafile not found"
	exit 1

fi

if [ ! -d "$reportDir" ]; then

	echo "ERROR: reportDir \"$reportDir\"  doesn't exists"
	exit 1

fi

kyua test
kyua report-junit > ${reportDir}/kyua_report.xml
