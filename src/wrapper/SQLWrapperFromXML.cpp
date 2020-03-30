#include "wrapper/SQLWrapperFromXML.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

SQLWrapperFromXML::SQLWrapperFromXML(): SQLWrapper()
{}

SQLWrapperFromXML::~SQLWrapperFromXML()
{}

int SQLWrapperFromXML::getFileSize(std::string filename)
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

char * SQLWrapperFromXML::getEffectiveData()
{
  int size = getFileSize(_filename);
  char getdata[size];
  std::ifstream ifs(_filename);
  ifs.read(getdata, sizeof getdata);

  if (ifs.eof()) {
      size_t bytes_really_read = ifs.gcount();
  }
  else if (ifs.fail()) {
      std::cout << "Error" << std::endl;
  }
  char * start_c = strstr(getdata, "<Table");
  char * end_c = strstr(getdata, "</Table>");

  void * start_v = static_cast<void *>(start_c);
  void * end_v = static_cast<void *>(end_c);

  uint64_t start_i = (uint64_t)start_v;
  uint64_t end_i = (uint64_t)end_v;
  end_i += (sizeof(char) * 8); // 8 <=> sizeof("</Table>")

  uint64_t len = end_i - start_i ;

  char * effective_data = (char*)malloc(sizeof(char) * (len + 1));
  memcpy(effective_data, start_v, len );
  effective_data[ len ] = '\0';
  return effective_data ;
}

std::string SQLWrapperFromXML::create_insertion_request()
{
  int _col_count = _columns.size();

  char * rawDatas = getEffectiveData();

  std::string request = "INSERT INTO " + _table_name + " VALUES";

  rapidxml::xml_document<> doc;
  doc.parse<0>(rawDatas);
  rapidxml::xml_node<> *node = doc.first_node();

  //Ignoring first row ;
  node = node->first_node("Row")->next_sibling("Row");
  std::string values = "";
  std::string data_value;
  rapidxml::xml_node<> *cell_node, *data_node ;

  while(node){
    int col_index = 0 ;
    values.append("(") ;
    //Iterate over "Cell" node
    cell_node = node->first_node("Cell");
    while(cell_node && col_index < _col_count){
      //append the effective data in the cell to the requests values
      data_node = cell_node->first_node("Data");
      if(data_node){
        data_value = cell_node->first_node("Data")->value();
      }else{
        data_value = "NULL";
      }
      //std::cout << "DataValue :"<< data_value << std::endl;
      values += _columns[col_index]->format(data_value) + "," ;
      col_index ++ ;
      cell_node = cell_node->next_sibling("Cell");
    }
    //std::cout << " col_index " << col_index << " -- col count : " << _col_count << std::endl;
    for(int i = col_index ; i < _col_count ; i++){
      values += "NULL,";
    }
    values.pop_back();
    values.append("),\n");
    //Iterate over "Row" nodes
    node = node->next_sibling("Row");
  }
  values.pop_back();
  values.pop_back();
  request += values + ";";
  //std::cout << request << std::endl;

  free(rawDatas);
  return request;
}
/*
void SQLWrapperFromXML::fill_table()
{
  std::ifstream ifs(_filename);
  char * rawDatas = getEffectiveData();
  std::string tmp, values ;
  int  ;
  int _col_count = _columns.size();
  std::string request = "INSERT INTO " + _table_name + " VALUES";

  std::unique_ptr<sql::Statement> stmt(_sql_connection->createStatement());

  //Lecture des headers
  std::getline(ifs, tmp);

  long line = 1 ;
  while(std::getline(ifs, tmp)){
    line ++ ;
    std::stringstream ss(tmp);
    values += "(";
     = 0 ;
    for (std::string item; std::getline(ss, item, _col_delimiter) &&  < _col_count; ++) {
      if(!item.empty()){
        values += "\"" + item + "\"," ;
      }else{
        values += "NULL,";
      }
    }
    for(; < _col_count ; ++){
      values += "NULL,";
    }
    values.pop_back();
    values.append("),");

  }
  values.pop_back();
  request += values + ";";
  try{
      //std::cout << request << std::endl;
      stmt->execute(request);
  }catch (sql::SQLException &e) {
    cerr << _filename << " - LINE " << line << endl;
    cerr << "(MySQL error code: " << e.getErrorCode() << ")" << endl;
    cerr << "Invalid request : " << request << endl;
    cerr << "-----------------------------------" << endl;
  }
}
*/
