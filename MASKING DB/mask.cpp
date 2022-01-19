#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include<string.h>
using namespace std;
int main(int argc, const char **argv)
{

 sql::Driver* driver = get_driver_instance();
 std::auto_ptr<sql::Connection> con(driver->connect("127.0.0.1", "test", "Ankurt@1"));
 con->setSchema("ankurdb");
 sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *ps;
 stmt = con->createStatement();
 res = stmt->executeQuery("select empid,contactno from empinfo");
 while(res->next())
 {
  ps = con->prepareStatement("update empinfo set contactno=? where empid = ?");

  int oid=res->getInt("empid");

  int osc=res->getInt("contactno");
  
  int maskip = osc;

  maskip = maskip^11111111;


  string s = "update empinfo set contactno=? where empid = ?";

  
  ps->setInt(1,maskip);
  ps->setInt(2,oid);
  ps->execute();

cout<<"mask/unmask succesfully \n";
 }


   return 0;
}

