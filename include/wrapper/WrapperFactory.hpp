#pragma once

#include <memory>
#include "wrapper/SQLWrapper.hpp"
#include "wrapper/SQLWrapperFromCSV.hpp"
#include "wrapper/SQLWrapperFromXML.hpp"
#include "wrapper/SQLWrapperFromJSON.hpp"

#include <fstream>

class SQLWrapperFactory
{
public:
  static SQLWrapper* createWrapper(std::string conf_path);
private:
  static SQLWrapperFromCSV * createWrapperFromCSV(std::ifstream &ifs);
  static SQLWrapperFromXML * createWrapperFromXML(std::ifstream &ifs);
  static SQLWrapperFromJSON * createWrapperFromJSON(std::ifstream &ifs);
  static void init_sql_table(SQLWrapper* wrapper, std::ifstream &ifs);
  static void init_sql_columns(SQLWrapper* wrapper, std::ifstream &ifs);
};
