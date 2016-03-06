#!/bin/sh


kyua test
kyua report-junit > create_users.xml

