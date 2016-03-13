#!/bin/sh


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
