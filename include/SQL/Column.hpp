#pragma once

#include <string>
#include <regex>
namespace sql{

  class Column {
  public:
    Column(std::string column_name);
    ~Column();
    inline void isOptionnal(bool optionnal) { _optional = optionnal ;} ;
    virtual std::string getConstraint() const = 0 ;
    virtual std::string format(std::string origin) const = 0 ;
  protected:
    std::string _column_name ;
    bool _optional = true ;
  public:
    std::string getSchema() const;
  };

  class VarcharColumn : public Column{
    public:
      VarcharColumn(std::string name, int size);
      ~VarcharColumn();
    private:
      int _size ;
    public:
      std::string getConstraint() const override;
      std::string format(std::string origin) const override;
  };

  class NumericColumn: public Column {
  public:
    NumericColumn(std::string name, int digit, int precision);
    ~NumericColumn();
  private:
    int _digit;
    int _precision;
  public:
    std::string getConstraint() const override;
    std::string format(std::string origin) const override;
  };

  class DateColumn: public Column {
  public:
    DateColumn(std::string name);
    ~DateColumn();
  private:
    std::regex _date_regex;
  public:
    std::string getConstraint() const override;
    std::string format(std::string origin) const override;
  };

  class PointColumn: public Column{
  public:
    PointColumn(std::string name);
    ~PointColumn();
  public:
    std::string getConstraint() const override;
    std::string format(std::string origin) const override;
  };

}
