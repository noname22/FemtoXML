#!/bin/bash

if [ -f tests/xslspec.xml ]; then
	echo "You have xslspec.xml"
else
	echo "You don't have xslspec.xml, downloading it"
	wget http://www.w3.org/TR/2001/REC-xsl-20011015/xslspec.xml -O tests/xslspec.xml
fi

echo "Starting test..."
zzuf --seed=0:100000000 ./verify -- tests/xslspec.xml
