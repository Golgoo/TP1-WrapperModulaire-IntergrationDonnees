#pragma once

#include "wrapper/SQLWrapper.hpp"

#include <string>

class SQLWrapperFromCSV : public SQLWrapper
{
public:
  SQLWrapperFromCSV();
  ~SQLWrapperFromCSV();

private:
  std::string _filename = "" ;
  char _col_delimiter = ',' ;

public:
  inline void setFilename(std::string filename){ _filename = filename ;} ;
  inline std::string getFilename(void) const { return _filename ;} ;
  inline void setColDelimiter(char col_delimiter) { _col_delimiter = col_delimiter ;} ;
  inline char getColDelimiter(void) const { return _col_delimiter ;} ;

protected:
  std::string create_insertion_request() override;
};
