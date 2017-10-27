#include <iostream>
#include <map>
#include "wrapper/mysql_wrapper.h"
//#include "mysql_driver.h"
//#include "cppconn/variant.h"
using namespace std;
  
void print_error(const char* str)
{
	cout << str;
}

void cpp_mysql_test()
{
	SqlConnectionPtr conn = MysqlWrapper::GetConnect("localhost", "root", "", 0, "hebei_center");
	if (!conn)
	{
		print_error("conn fail");
		return;
	}

	M_SQL_TRY()
		StatementScopedPtr stmt(conn->createStatement());
		ResultSetScopedPtr res(stmt->executeQuery("select count(*) from user"));
		
		while (res->next())
		{
			cout << res->getInt("count(*)2") << endl;
		}
	M_SQL_EXCEPTION();
}

void cpp_mysql_test2()
{
	SqlConnectionPtr conn = MysqlWrapper::GetConnect("localhost", "root", "", 0, "test");
	if (!conn)
	{
		print_error("conn fail");
		return;
	}

	M_SQL_TRY()
		StatementScopedPtr stmt(conn->createStatement());
		ResultSetScopedPtr res(stmt->executeQuery("select * from test"));
		
		while (res->next())
		{
			std::string value;
			GetFieldWrapper(res.get(), "id", value);
			cout << value << " " << res->getInt("age") << " " << res->getInt("headframe") << endl;
		}
	M_SQL_EXCEPTION();
}

void cpp_mysql_test3()
{
	SqlConnectionPtr conn = MysqlWrapper::GetConnect("localhost", "root", "", 0, "test");
	if (!conn)
	{
		print_error("conn fail");
		return;
	}

	try
	{
		PreparedStatementScopedPtr pstmt(conn->prepareStatement("select * from test where age>=?"));
		SetFieldWrapper(pstmt.get(), 1, 2);
		ResultSetScopedPtr res(pstmt->executeQuery());

		while (res->next())
		{
			std::string value;
			GetFieldWrapper(res.get(), "id", value);
			cout << value << " " << res->getInt("age") << " " << res->getInt("headframe") << endl;
		}
	}
	M_MYSQL_CATCH_EXCEPTION;
}

void cpp_mysql_test4()
{
	sql::Driver* dri1 = MysqlWrapper::GetDriver();
	sql::Driver* dri2 = MysqlWrapper::GetDriver();
	if (dri2 == dri1)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
}

int main()
{
	cpp_mysql_test4();
	return 0;
}


int c_mysql_test()
{
	return 0;
	//MYSQL* pmysql = mysql_init(0);
	//if (!pmysql)
	//{
	//	print_error("init error\n");
	//	return 0;
	//}

	//// Á¬½Ó
	//MYSQL* pconn = mysql_real_connect(pmysql, "localhost", "root", "", "hebei_center", 3306, NULL, 0);
	//if (!pconn)
	//{
	//	print_error("connect fail\n");
	//	return 0;
	//}
	//else
	//	print_error("connect success\n");

	//// query
	//const char* pSql = "select count(*) from user";
	//int ret = mysql_real_query(pconn, pSql, strlen(pSql));

	//MYSQL_RES* pres = mysql_store_result(pconn);
	//if (!pres)
	//{
	//	print_error("query error\n");
	//	return 0;
	//}

	//while (MYSQL_ROW row = mysql_fetch_row(pres))
	//{
	//	cout << row[0] << endl;
	//}
	//return 0;
}
