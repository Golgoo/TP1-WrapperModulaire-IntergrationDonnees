#include "wrapper/SQLWrapperFromJSON.hpp"

#include <iostream>
#include <fstream>

SQLWrapperFromJSON::SQLWrapperFromJSON(): SQLWrapper()
{}

SQLWrapperFromJSON::~SQLWrapperFromJSON()
{}


using json = nlohmann::json;

void display_info(json json_item)
{
  std::cout << std::boolalpha;
  std::cout << "Null       -> " << (json_item.type() == json::value_t::null) << '\n';
  std::cout << "Boolean    -> " << (json_item.type() == json::value_t::boolean) << '\n';
  std::cout << "Number     -> " << (json_item.type() == json::value_t::number_integer) << '\n';
  std::cout << "Uns Number -> " << (json_item.type() == json::value_t::number_unsigned) << '\n';
  std::cout << "Float      -> " << (json_item.type() == json::value_t::number_float) << '\n';
  std::cout << "Object     -> " << (json_item.type() == json::value_t::object) << '\n';
  std::cout << "Array      -> " << (json_item.type() == json::value_t::array) << '\n';
  std::cout << "String     -> " << (json_item.type() == json::value_t::string) << '\n';
  std::cout << "-------------------" << std::endl;
}

std::string SQLWrapperFromJSON::create_insertion_request()
{

  std::ifstream ifs(_filename);
  json j_base;
  ifs >> j_base;

  std::string values = iterate_json_file(j_base);
  values.pop_back();
  values.pop_back();

  std::string request = "INSERT INTO " + _table_name + " VALUES " + values + ";";

  return request ;
}


std::string SQLWrapperFromJSON::iterate_json_file(json current_json)
{
  std::string values ;
  switch (current_json.type()) {
    case json::value_t::array :
      for(auto it = current_json.begin() ; it != current_json.end() ; it++ ){
        values += iterate_json_file(*it);
      }
    break;
    case json::value_t::object :
      for(auto it = current_json.begin() ; it != current_json.end() ; it++ ){
        if(it.key() == "fields"){
          return "(" + read_json_row(*it) + "),\n";
        }else{
          values += iterate_json_file(*it);
        }
      }
    break;
    default:

    break;
  }
  return values;
}

std::string SQLWrapperFromJSON::read_json_row(json json_row)
{
  std::string value, tmp_item ;
  json json_item ;
  int item_counter = 0 ;
  for(auto it = json_row.begin() ; it != json_row.end() && item_counter < _columns.size(); it ++, item_counter ++){
    if(it.value().type() == nlohmann::json::value_t::string){
      tmp_item = it.value();
    }else {
      tmp_item = to_string(it.value());
    }
    value += _columns[item_counter]->format(tmp_item) + ",";
  }
  for( ; item_counter < _columns.size() ; item_counter ++){
    value += "NULL,";
  }
  value.pop_back();

  return value ;
}
