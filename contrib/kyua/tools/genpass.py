#!/usr/bin/env python3

#
# genPass.py
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



import random
import string

import os
from sys import stdin,stderr,stdout,argv

prgName=os.path.basename(argv[0])


def usage():

        print("Usage: " + prgName + " length")
        exit(1)

if len(argv) !=2:

        usage()

l=int(argv[1])

if l < 4:
	print("Sorry, length not be < 4")
	exit(1)

maxDigitSpecialPlace=l-1

if l >  7:
	maxDigitSpecialPlace=7


digitPlace=random.randint(0,maxDigitSpecialPlace)
specialPlace=random.randint(0,maxDigitSpecialPlace)

while digitPlace == specialPlace:
	specialPlace=random.randint(0,maxDigitSpecialPlace)

normalPlace=random.randint(0,maxDigitSpecialPlace)

while (normalPlace == specialPlace) or (normalPlace == digitPlace):
	normalPlace=random.randint(0,maxDigitSpecialPlace)

pwArray=[random.choice(string.punctuation+string.digits+string.digits+string.ascii_letters+string.ascii_letters) for i in range(l)]

pwArray[normalPlace]=random.choice(string.ascii_letters)
pwArray[specialPlace]=random.choice(string.punctuation)
pwArray[digitPlace]=random.choice(string.digits)

pw= "".join(pwArray)

print(pw)

