#include <stdlib.h> 
#include <iostream> 
#include <sstream> 
#include <stdexcept> 
#include<math.h> 
#include "mysql_connection.h" 
#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h> 
#include<string.h>
#include<algorithm> 
using namespace std;
int main(int argc, const char **argv)
{
string username,password;
std::auto_ptr<sql::Connection> con;
cout<<"\nThis table is for presentation purpose\n";
//clearances
map<string,int> clearance;
clearance["admin"] = 4; //Admins@1234
clearance["branch_head"] = 3;	//Branch@123
clearance["branch_member"] = 2;//Member@123

map<string,string> slevel; 
slevel["admin"] = "TS"; 
slevel["branch_head"] = "S"; 
slevel["branch_member"] = "U";

map<string,int> ltoint;
ltoint["TS"] = 4;
ltoint["S"] = 3;
ltoint["U"] =2;

map<int,string> itostr;
itostr[4] = "TS";
itostr[3] = "S";
itostr[2] = "U";
cout<<"subjects/users and corresponding security levels are\n";
for(auto it:slevel)
{
cout<<it.first<<"  "<<it.second<<"\n";
}
cout<<"\n\n\n";
while(1)
{
cout<<"Please enter your username\n";
cin>>username;
cout<<"Enter password\n";
cin>>password;
sql::Driver* driver = get_driver_instance();
try
{
con.reset(driver->connect("127.0.0.1",username, password));

if(!con->isClosed())
{
cout<<"logged in\n";
con->setSchema("csisdb");

}
}
catch(sql::SQLException &e)
{
cout<<"wrong password\n";
cerr << e.what()<<endl;
continue;

}

while(1)
{
sql::Statement *stmt;
stmt = con->createStatement();
cout<<"\nWhat operation would you like to perform on emp_details\ntype stop to login as another user\n";
string op;

cin>>op;
if(op == "stop") break;

if(op == "show_details")
{
sql::ResultSet *res;
sql::PreparedStatement *prep_stmt;


if(slevel[username] == "TS")
{
prep_stmt = con->prepareStatement("select emp_id,Cid,Sid,Did,salary,dept,TC from emp_details");
res = prep_stmt->executeQuery();
cout<<"emp_id Cid  sal  Cs  dept  Cd  TC\n";
while(res->next())
{
int id = res->getInt("emp_id");
int sal = res->getInt("salary");
string dep = res->getString("dept");
string slev = res->getString("TC");
cout<<id<<"     "<<res->getString("Cid")<<"    "<<sal<<"    "<<res->getString("Sid")<<"   "<<dep<<"   "<<res->getString("Did")<<"  "<<slev<<"\n";
}
}
else if(slevel[username] == "S")
{
prep_stmt = con->prepareStatement("select emp_id,salary,dept,TC from emp_details where TC <> ?");
prep_stmt->setString(1,slevel["admin"]);
res = prep_stmt->executeQuery();
cout<<"emp_id sal dept Cd \n";
while(res->next())
{
int id = res->getInt("emp_id");
int sal = res->getInt("salary");
string dep = res->getString("dept");
string slev = res->getString("TC");
cout<<id<<"      "<<sal<<"  "<<dep<<"  "<<slev<<"\n";
}
}
else
{
prep_stmt = con->prepareStatement("select emp_id,salary,dept,TC from emp_details where TC = ?");
prep_stmt->setString(1,slevel[username]);
res = prep_stmt->executeQuery(); 
cout<<"emp_id sal  dept  Cd \n";
while(res->next())
{
int id = res->getInt("emp_id");
int sal = res->getInt("salary");
string dep = res->getString("dept");
string slev = res->getString("TC");
cout<<id<<"      "<<sal<<"  "<<dep<<"  "<<slev<<"\n";
}
}
}
else if(op == "insert")
{
sql::ResultSet *res;
sql::PreparedStatement *prep_stmt;
cout<<"\nKindly enter the required data\n";
cout<<"emp_id\n";
int eid;
cin>>eid;
cout<<"salary\n";
int salary;
cin>>salary;
cout<<"dept\n";
string dept;
cin>>dept;
prep_stmt = con->prepareStatement("insert into emp_details values(?,?,?,?,?,?,?)");
prep_stmt->setInt(1,eid);
prep_stmt->setString(2,slevel[username]);
prep_stmt->setInt(3,salary);
prep_stmt->setString(4,slevel[username]);
prep_stmt->setString(5,dept);
prep_stmt->setString(6,slevel[username]);
prep_stmt->setString(7,slevel[username]);

prep_stmt->execute();
cout<<"\nDONE\n";
}

else if(op == "update")
{
sql::ResultSet *res;
sql::PreparedStatement *prep_stmt,*ps;
cout<<"\nKindly enter the employee id to be updated\n";
int eid;
cin>>eid;
cout<<"\nwhich field would you like to change salary/dept\n";
string ch;
cin>>ch;
cout<<"\nEnter new value for "<<ch<<"\n";
int newch;
string newdept; if(ch == "salary")
{
cin>>newch;
}
else cin>>newdept;
prep_stmt = con->prepareStatement("select Cid,salary,Sid,Did,dept from emp_details where emp_id=?");
prep_stmt->setInt(1,eid);
res = prep_stmt->executeQuery();
while(res->next())
{
string curr = slevel[username];
ps = con->prepareStatement("insert into emp_details values(?,?,?,?,?,?,?)");
ps->setInt(1,eid);
string tcid,tsid,tdid;
tcid = res->getString("Cid");
tsid = res->getString("Sid");
tdid = res->getString("Did");
int i_tcid = ltoint[tcid];
int i_tsid = ltoint[tsid];
int i_tdid = ltoint[tdid];

if(ch == "salary")
{
tsid = slevel[username];
i_tsid = ltoint[tsid];
int mx = -1;
mx = max(i_tcid,i_tsid);
mx = max(mx,i_tdid);
string tuple_level = itostr[mx];
ps->setString(2,tcid);
ps->setInt(3,newch);
ps->setString(4,tsid);
ps->setString(5,res->getString("dept"));
ps->setString(6,tdid);
ps->setString(7,tuple_level);
ps->execute();
cout<<"\nDONE\n";
}
else if(ch == "dept")
{
tdid = slevel[username];
i_tdid = ltoint[tdid];
int mx = -1;
mx = max(i_tcid,i_tsid);
mx = max(mx,i_tdid);
string tuple_level = itostr[mx];
ps->setString(2,tcid);
ps->setInt(3,res->getInt("salary"));
ps->setString(4,tsid);
ps->setString(5,newdept);
ps->setString(6,tdid);
ps->setString(7,tuple_level);
ps->execute();
cout<<"\nDONE\n";
}
}
}
}
}
return 0;
}
