#!/bin/sh


. `dirname $0`/../config.sh

requiredHashType="blowfish"

update_passwords -c blowfish
