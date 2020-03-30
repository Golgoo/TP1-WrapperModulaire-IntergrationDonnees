#include "SQL/Column.hpp"
#include <exception>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <regex>

sql::Column::Column(std::string column_name): _column_name(column_name)
{}

sql::Column::~Column()
{}

std::string sql::Column::getSchema() const
{
  std::string str = _column_name;
  str += " " ;
  str.append(getConstraint());
  if(! _optional){
    str += " NOT NULL";
  }
  return str;
}

sql::VarcharColumn::VarcharColumn(std::string name, int size): Column(name), _size(size)
{}

sql::VarcharColumn::~VarcharColumn()
{}

std::string sql::VarcharColumn::getConstraint() const
{
  std::string str = "VARCHAR(";
  str.append(std::to_string(_size));
  str.append(")");
  return str;
}

std::string sql::VarcharColumn::format(std::string origin) const
{
  std::replace( origin.begin(), origin.end(), '\"', '\'');
  if(origin.size() - 1 > _size ){
    return "NULL";
  }
  return "\"" + origin + "\"";
}

sql::NumericColumn::NumericColumn(std::string name, int digit, int precision): Column(name), _digit(digit), _precision(precision)
{}

sql::NumericColumn::~NumericColumn()
{}

std::string sql::NumericColumn::getConstraint() const
{
  std::string str = "NUMERIC(";
  str.append(std::to_string(_digit));
  str.append(",");
  str.append(std::to_string(_precision));
  str.append(")");
  return str;
}

std::string sql::NumericColumn::format(std::string origin) const
{
  try{
    std::size_t n = origin.find_first_of("0123456789");
    if (n != std::string::npos)
    {
      //Checking contain a number
      std::size_t m = origin.find_first_not_of("0123456789.", n);
      origin = origin.substr(n, m != std::string::npos ? m-n : m);
      std::stod(origin, NULL) ;
      //Checking precision validity
      int point = origin.find_first_of(".");
      if(point == std::string::npos){
        point = origin.size();
      }
      if( ( point ) > ( _digit - _precision ) ){
        return "NULL";
      }else if( ( (origin.size() - point ) ) > _precision + 1){
        return "NULL";
      }
    }else{
      return "NULL";
    }
  }catch (const std::invalid_argument& ia) {
	  std::cerr << "Invalid numeric argument : " << origin << '\n';
    return "NULL";
  }
  return "\"" + origin + "\"";
}

sql::DateColumn::DateColumn(std::string name): Column(name)
{
    _date_regex = "^\\d{4}\\D{1}\\d{2}\\D{1}\\d{2}" ;
}

sql::DateColumn::~DateColumn()
{}

std::string sql::DateColumn::getConstraint() const
{
  std::string str = "DATE";
  return str;
}

std::string sql::DateColumn::format(std::string origin) const
{
  std::smatch match;
  if( ! regex_search(origin, match, _date_regex)){
    return "NULL";
  }
  return "\"" + match.str() + "\"";
}

sql::PointColumn::PointColumn(std::string name): Column(name)
{}
sql::PointColumn::~PointColumn()
{}

std::string sql::PointColumn::getConstraint() const
{
  std::string str = "GEOMETRY";
  return str;
}
std::string sql::PointColumn::format(std::string origin) const
{
  std::string formated ;
  try{
    double latitude, longitude ;
    std::size_t n = origin.find_first_of("0123456789");
    if (n != std::string::npos)
    {
      std::size_t m = origin.find_first_not_of("0123456789.", n);
      origin = origin.substr(n, m != std::string::npos ? m-n : m);
      latitude = std::stod(origin);
      n = origin.find_first_of("0123456789");
      if(n != std::string::npos){
        m = origin.find_first_not_of("0123456789.", n);
        origin = origin.substr(n, m != std::string::npos ? m-n : m);
        longitude = std::stod(origin);
        formated = std::to_string(latitude) + "," + std::to_string(longitude);
      }else{
        return "NULL";
      }
    }else{
      return "NULL";
    }
    formated = "GeomFromWKB(Point("+ std::to_string(latitude)+"," + std::to_string(longitude) + "))";
  }catch (const std::exception& e) {
	  std::cerr << "Invalid argument: " << e.what() << '\n';
    std::cerr << "Cannot convert : " + origin + " into MySql Point ( NULL returned ) " << '\n' ;
    return "NULL";
  }
  return formated;
}
