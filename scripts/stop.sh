#!/bin/sh
cjserver -c /etc/CppJieba/server.conf -k stop
if [ $? -ne 0 ]; then
    echo "stop failed."
    exit 1
fi
echo "stop ok."
