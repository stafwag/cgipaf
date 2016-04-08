#!/bin/sh

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


solTestDir="$baseDir/../"

${solTestDir}/create_users/create_users.sh
${solTestDir}/update_password_noargs/update_password_noargs.sh
${solTestDir}/update_password_pam/update_password_pam.sh
${solTestDir}/update_password_nopam/update_password_nopam.sh
${solTestDir}/update_password_md5/update_password_md5.sh
