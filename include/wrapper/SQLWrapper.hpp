#pragma once

#include <string>
#include <vector>
#include <mysql_connection.h>

#include "SQL/Column.hpp"
#include <memory>

class SQLWrapper{
public:
  SQLWrapper();
  ~SQLWrapper();
  void load();

private:
  std::shared_ptr<sql::Connection> _sql_connection ;
  void create_table() ;
  void execute_insertion_request();
protected:
  std::string _table_name ;
  std::vector<std::shared_ptr<sql::Column>> _columns;

public:
  inline void setTableName(std::string table_name) { _table_name = table_name ; };
  inline std::string setTableName(void) const { return _table_name ;} ;
  inline void setSqlConnection(std::shared_ptr<sql::Connection> sql_connection){ _sql_connection = sql_connection ;} ;
  inline std::shared_ptr<sql::Connection> getSqlConnection(void) const { return _sql_connection ;} ;

  void addColumn(std::shared_ptr<sql::Column> column);
  std::string getSchema() const;

protected:
  virtual std::string create_insertion_request() = 0;
};

std::shared_ptr<sql::Column> create_column(std::vector<std::string> splits);
