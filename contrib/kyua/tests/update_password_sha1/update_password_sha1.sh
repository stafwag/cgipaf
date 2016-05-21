#!/bin/sh


. `dirname $0`/../config.sh

requiredHashType="sha1"

update_passwords -c sha1
