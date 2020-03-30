#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#define HOST "localhost"
#define USER "tp1-user-gm"
#define PASS "123456"
#define DB "maurice_gautier_inte_donnees_tp1"

#include "wrapper/SQLWrapperFromCSV.hpp"
#include "SQL/Column.hpp"
#include "wrapper/WrapperFactory.hpp"

#include <nlohmann/json.hpp>

#include <regex>

using namespace std;


void load_wrapper(std::string config_path, std::shared_ptr<sql::Connection> connection)
{
  std::cout << "Initilializing config file : " << config_path << std::endl;
  std::unique_ptr<SQLWrapper> wrapper(SQLWrapperFactory::createWrapper(config_path));
  wrapper->setSqlConnection(connection);
  wrapper->load();
}

#include "rapidxml.hpp"
using namespace rapidxml;
#include <string.h>

int getFileSize(std::string filename) { // path to file
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}


int main(int argc, const char **argv)
{
  cout << "Connector/C++ - Wrapper Initialisation ..." << endl;
  cout << endl;

  sql::Driver* driver = get_driver_instance();
  std::shared_ptr<sql::Connection> con(driver->connect(HOST, USER, PASS));
  con->setSchema(DB);

  load_wrapper("mediator_config/tournages_json.conf", con);
  load_wrapper("mediator_config/velib_xml.conf", con);
  load_wrapper("mediator_config/cinemas.conf", con);
  load_wrapper("mediator_config/hotspot.conf", con);


  cout << "Done." << endl;
  return EXIT_SUCCESS;
}
