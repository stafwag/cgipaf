#! /usr/bin/env python3

#
# realpath.py
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

import os
from sys import stdin,stderr,stdout,argv

prgName=os.path.basename(argv[0])


def usage():

        print("Usage: <stdin> (username:password) " + prgName)
        exit(1)

if len(argv) !=2:

        usage()

myPath=argv[1]

print(os.path.realpath(myPath))




