#!/bin/bash

#https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-installation-source-distribution.html
git clone https://github.com/mysql/mysql-connector-cpp.git
cd mysql-connector-cpp/
git checkout 1.1

#https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-installation-source-unix.html
cmake -DCMAKE_INSTALL_PREFIX=../mysql-conn-cpp/ -DBOOST_ROOT:STRING=../boost_1_72_0/
make clean
make
make install
