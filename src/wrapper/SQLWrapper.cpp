#include "wrapper/SQLWrapper.hpp"

SQLWrapper::SQLWrapper()
{

}

SQLWrapper::~SQLWrapper()
{

}

#include <cppconn/exception.h>
#include <cppconn/statement.h>

std::string SQLWrapper::getSchema() const
{
  std::string str = "CREATE TABLE " + _table_name + "(" + "\n" ;
  for(int i = 0 ; i < _columns.size() - 1 ; i ++){
    str += _columns[i]->getSchema() + ',' + "\n" ;
  }
  str += (_columns.back()->getSchema());
  str += ")" ;
  return str ;
}


void SQLWrapper::load()
{
  try{
    create_table();
    execute_insertion_request();
  }catch (sql::SQLException &e) {
    std::cerr << "# ERR: SQLException in " << __FILE__;
    std::cerr << "(" << __FUNCTION__ << ")" << std::endl;
    std::cerr << "# ERR: " << e.what();
    std::cerr << " (MySQL error code: " << e.getErrorCode();
    std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}

void SQLWrapper::execute_insertion_request()
{
  std::unique_ptr<sql::Statement> stmt(_sql_connection->createStatement());
  stmt->execute(create_insertion_request());
}

void SQLWrapper::create_table()
{
    std::unique_ptr<sql::Statement> stmt(_sql_connection->createStatement());
    stmt->execute("DROP TABLE IF EXISTS " + _table_name +";" );
    stmt->execute(getSchema());
}

void SQLWrapper::addColumn(std::shared_ptr<sql::Column> column)
{
  _columns.push_back(column) ;
}
