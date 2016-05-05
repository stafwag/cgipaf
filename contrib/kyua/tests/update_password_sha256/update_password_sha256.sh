#!/bin/sh


. `dirname $0`/../config.sh

requiredHashType="sha256"

update_passwords -c sha256
