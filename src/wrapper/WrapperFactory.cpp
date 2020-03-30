#include "wrapper/WrapperFactory.hpp"

#include <iostream>

SQLWrapper* SQLWrapperFactory::createWrapper(std::string conf_path)
{
    std::ifstream ifs(conf_path);
    std::string tmp ;
    std::getline(ifs, tmp);
    if(tmp == "WRAPPER-SQL"){
      std::getline(ifs, tmp);
      SQLWrapper *wrapper ;
      if(tmp == "CSV"){
        wrapper = createWrapperFromCSV(ifs);
      }else if(tmp == "XML"){
        wrapper = createWrapperFromXML(ifs);
      }else if(tmp == "JSON"){
        wrapper = createWrapperFromJSON(ifs);
      }else{
        return NULL;
      }
      SQLWrapperFactory::init_sql_table(wrapper, ifs);
      SQLWrapperFactory::init_sql_columns(wrapper, ifs);
      return wrapper ;
    }
    return NULL;
}

#include <sstream>
#include <vector>
#include <iterator>
SQLWrapperFromCSV * SQLWrapperFactory::createWrapperFromCSV(std::ifstream &ifs)
{
//  std::cout << "Lodaing csv" << std::endl;
  std::string line ;
  SQLWrapperFromCSV *wrapper = new SQLWrapperFromCSV();
  std::string item ;
  while(std::getline(ifs,line)){
    if(line == "SQL") break;
    else{
    //  std::cout << "Line : " << line << std::endl;
      std::stringstream ss(line);
      std::vector<std::string> splits(std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>());
      if(splits[0] == "PATH"){
        wrapper->setFilename(splits[1]);
      }else if(splits[0] == "COLDELIM"){
        wrapper->setColDelimiter(splits[1][0]);
      }
    }
  }
  return wrapper;
}

SQLWrapperFromXML * SQLWrapperFactory::createWrapperFromXML(std::ifstream &ifs)
{
//  std::cout << "Lodaing csv" << std::endl;
  std::string line ;
  SQLWrapperFromXML *wrapper = new SQLWrapperFromXML();
  std::string item ;
  while(std::getline(ifs,line)){
    if(line == "SQL") break;
    else{
    //  std::cout << "Line : " << line << std::endl;
      std::stringstream ss(line);
      std::vector<std::string> splits(std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>());
      if(splits[0] == "PATH"){
        wrapper->setFilename(splits[1]);
      }
    }
  }
  return wrapper;
}

SQLWrapperFromJSON * SQLWrapperFactory::createWrapperFromJSON(std::ifstream &ifs)
{
//  std::cout << "Lodaing csv" << std::endl;
  std::string line ;
  SQLWrapperFromJSON *wrapper = new SQLWrapperFromJSON();
  std::string item ;
  while(std::getline(ifs,line)){
    if(line == "SQL") break;
    else{
    //  std::cout << "Line : " << line << std::endl;
      std::stringstream ss(line);
      std::vector<std::string> splits(std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>());
      if(splits[0] == "PATH"){
        wrapper->setFilename(splits[1]);
      }
    }
  }
  return wrapper;
}

void SQLWrapperFactory::init_sql_table(SQLWrapper* wrapper, std::ifstream &ifs)
{
//  std::cout << "Loading table " << std::endl;
  std::string line ;
  while(std::getline(ifs,line)){
    if(line == "CONTENT") break;
    else{
      std::stringstream ss(line);
      std::vector<std::string> splits(std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>());
      if(splits[0] == "TABLE_NAME"){
        wrapper->setTableName(splits[1]);
      }
    }
  }
}

void SQLWrapperFactory::init_sql_columns(SQLWrapper* wrapper, std::ifstream &ifs)
{
  //std::cout << "Loading columns " << std::endl;
  std::string line ;
  while(std::getline(ifs,line)){
      std::stringstream ss(line);
    //  std::cout << "Line : " << line << std::endl;
      std::vector<std::string> splits(std::istream_iterator<std::string>{ss}, std::istream_iterator<std::string>());
      std::shared_ptr<sql::Column> column = create_column(splits);
      if(column){
        wrapper->addColumn(column);
      }
  }
}

std::shared_ptr<sql::Column> create_column(std::vector<std::string> splits)
{
  std::shared_ptr<sql::Column> column_ptr;
  std::string type = splits[1];
  if(type == "VARCHAR"){
    column_ptr = std::make_shared<sql::VarcharColumn>(splits[0], std::stoi(splits[2]));
  }else if(type == "DATE"){
    column_ptr = std::make_shared<sql::DateColumn>(splits[0]);
  }else if(type == "NUMERIC"){
    column_ptr = std::make_shared<sql::NumericColumn>(splits[0], std::stoi(splits[2]), std::stoi(splits[3]));
  }else if(type == "POINT"){
    column_ptr = std::make_shared<sql::PointColumn>(splits[0]);
  }
  return column_ptr ;
}
