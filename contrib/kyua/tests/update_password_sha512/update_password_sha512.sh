#!/bin/sh


. `dirname $0`/../config.sh

requiredHashType="sha512"

update_passwords -c sha512
