#!/bin/sh


. `dirname $0`/../config.sh

requiredHashType="des"

update_passwords -c des
