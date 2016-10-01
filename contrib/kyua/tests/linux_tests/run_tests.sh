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

exitCode=0

${solTestDir}/create_users/create_users.sh

exitCode=`expr $exitCode + $?`

${solTestDir}/update_password_noargs/update_password_noargs.sh
 
exitCode=`expr $exitCode + $?`
 
${solTestDir}/update_password_pam/update_password_pam.sh
 
exitCode=`expr $exitCode + $?`
 
${solTestDir}/update_password_nopam/update_password_nopam.sh
 
exitCode=`expr $exitCode + $?`
 
${solTestDir}/update_password_md5/update_password_md5.sh
 
exitCode=`expr $exitCode + $?`

if [ "$exitCode" = "0" ]; then

     echo "(linux tests) Tests were successful creating ${reportDir}/kyua_report.xml"

     cp ${reportDir}/../dummy_reports/kyua_report_dummy_ok.xml ${reportDir}/kyua_report.xml

else

     echo "ERROR: test failed unlinking ${reportDir}/kyua_report.xml"

     rm ${reportDir}/kyua_report.xml

fi

exit $exitCode

