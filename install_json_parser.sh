#!/bin/bash

git clone https://github.com/nlohmann/json.git json_lib
cd json_lib
cmake .
make clean
make
