#!/bin/sh

Usage () {

	echo
	echo "Usage: `basename $0` manpage.man"
	echo

	exit 1

}

if [ $# != "1" ]; then

	Usage

fi

groff -t -e -mandoc -Tascii $1
