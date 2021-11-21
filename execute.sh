#!/bin/bash

echo "Run GRADER for: "
read x

echo "COMPILING TAR FILE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
./assignment1_package.sh
cd grader
echo "TAR FILE COMPILED>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"

if [[ "$x" == "all" ]]
then
  echo "RUNNING GRADER FOR ALL COMMANDS"
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t startup
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t author
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t ip
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t port
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t _list
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t refresh
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t send
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t broadcast
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t block
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t blocked
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t unblock
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t logout
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t buffer
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exit
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t statistics
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exception_login
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exception_send
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exception_block
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exception_unblock
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t exception_blocked
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t bonus
else
  echo "RUNNING GRADER FOR ${x}"
  ./grader_controller -c ./grader.cfg -s ../ankitdiw_pa1.tar -t $x
fi

