#!/bin/sh
cjserver -c /etc/CppJieba/server.conf -k start >> /dev/null 2>&1 
if [ $? -ne 0 ]; then
    echo "start failed."
    exit 1
fi
echo "start ok."
