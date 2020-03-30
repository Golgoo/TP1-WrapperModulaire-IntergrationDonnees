#pragma once

#include "wrapper/SQLWrapper.hpp"

#include <string>
#include <fstream>
#include <rapidxml.hpp>

class SQLWrapperFromXML : public SQLWrapper
{
public:
  SQLWrapperFromXML();
  ~SQLWrapperFromXML();

private:
  std::string _filename = "" ;
  char * getEffectiveData();
  int getFileSize(std::string filename);
public:
  inline void setFilename(std::string filename){ _filename = filename ;} ;
  inline std::string getFilename(void) const { return _filename ;} ;

protected:
  std::string create_insertion_request() override;
};
