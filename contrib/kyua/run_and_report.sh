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

myWhich () {


	cmd=$1

	cmdPath=`which $cmd`

	if [ -x "$cmdPath" ]; then


		return 0


	fi

	return 1


}



myWhich realpath > /dev/null 2>&1

if [ $? = "0" ]; then

	scriptName=`realpath $0`

else

	toolsDir="`dirname $0`/tools/"
	pyRealPath="${toolsDir}/realpath.py"

	if [ ! -x "${pyRealPath}" ]; then

		echo "ERROR: no realpath tool found"
		exit 1

	fi

	scriptName=`$pyRealPath $0` || {

		echo "ERROR: Sorry $pyRealPath $0 failed"
		exit 1

	}


fi

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
		kyuaMainDir="${scriptDir}/$os"
		;;
	"freebsd"|"netbsd")
		kyuaMainDir="${scriptDir}/${os}/${majorRelease}"
		;;
	"sunos"|"openbsd")
		kyuaMainDir="${scriptDir}/${os}/${release}"
		;;
	*)

		echo "ERROR: sorry no test define for \"$os\""
		exit 1

		;;

esac

if [ ! -d "$reportDir" ]; then

	echo "ERROR: reportDir \"$reportDir\"  doesn't exists"
	exit 1

fi

if [ -f ${kyuaMainDir}/Kyuafile ]; then

	cd $kyuaMainDir || {

		echo "ERROR: cd $kyuaMainDir failed"
		exit 1

	}

else


	if [ -x "${kyuaMainDir}/tst/run_tests.sh" ]; then


		${kyuaMainDir}/tst/run_tests.sh
		exit $?


	else

		echo "ERROR: Kyuafile \"${kyuaMainDir}/Kyuafile\" not found"
		echo "ERROR: script alternative \"${kyuaMainDir}/tst/run_tests.sh\" not found"
		exit 1

	fi

fi

myWhich kyua || {

	PATH=$PATH:/usr/local/bin

	myWhich kyua || {


		echo "kyua not found"

		if [ -x "${kyuaMainDir}/tst/run_tests.sh" ]; then


			${kyuaMainDir}/tst/run_tests.sh
			exit $?


		else

			echo "ERROR: Kyuafile \"${kyuaMainDir}/Kyuafile\" not found"
			echo "ERROR: script alternative \"${kyuaMainDir}/tst/run_tests.sh\" not found"
			exit 1

		fi

		exit 1


	}

}

kyua test

exitCode=$?

echo "Creating kyua report: \"kyua report-junit > ${reportDir}/kyua_report.xml\""
kyua report-junit > ${reportDir}/kyua_report.xml

exit $exitCode
