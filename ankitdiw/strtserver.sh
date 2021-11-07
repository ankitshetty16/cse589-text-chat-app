#!/bin/bash
 
echo "Enter server type (s/c): "
read x
echo "port number: "
read y
kill -9 `lsof -t -i:${y}` ; make clean ; make ; ./assignment1 ${x} ${y}
