#include "wrapper/SQLWrapperFromCSV.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/statement.h>

SQLWrapperFromCSV::SQLWrapperFromCSV(): SQLWrapper()
{
}

SQLWrapperFromCSV::~SQLWrapperFromCSV()
{
}

using namespace std;

std::string SQLWrapperFromCSV::create_insertion_request()
{
  std::ifstream ifs(_filename);
  std::string tmp, values ;
  int col_index ;
  int _col_count = _columns.size();
  std::string request = "INSERT INTO " + _table_name + " VALUES";

  //Lecture des headers
  std::getline(ifs, tmp);

  long line = 1 ;
  while(std::getline(ifs, tmp)){
    line ++ ;
    std::stringstream ss(tmp);
    values += "(";
    col_index = 0 ;
    for (std::string item; std::getline(ss, item, _col_delimiter) && col_index < _col_count; col_index++) {
      if(!item.empty()){
        values += _columns[col_index]->format(item) + "," ;
      }else{
        values += "NULL,";
      }
    }
    for(;col_index < _col_count ; col_index++){
      values += "NULL,";
    }
    values.pop_back();
    values.append("),");

  }
  values.pop_back();
  request += values + ";";

  return request ;
}
