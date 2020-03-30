#pragma once

#include "wrapper/SQLWrapper.hpp"

#include <string>

#include <nlohmann/json.hpp>

class SQLWrapperFromJSON : public SQLWrapper
{
public:
  SQLWrapperFromJSON();
  ~SQLWrapperFromJSON();

private:
  std::string _filename = "" ;
  std::string _path = "fields" ;

public:
  inline void setFilename(std::string filename){ _filename = filename ;} ;
  inline std::string getFilename(void) const { return _filename ;} ;

protected:
  std::string create_insertion_request() override;

private:
  std::string iterate_json_file(nlohmann::json current_json);
  std::string read_json_row(nlohmann::json json_row);
};
