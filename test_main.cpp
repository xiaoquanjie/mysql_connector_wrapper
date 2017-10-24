#include <iostream>
#include <map>
#include "mysql_wrapper.h"
using namespace std;

void print_error(const char* str)
{
	cout << str;
}

void cpp_mysql_test()
{
	sql::Driver* driver = sql::mysql::get_mysql_driver_instance();
	if (!driver)
	{
		print_error("get driver fail\n");
		return;
	}

	sql::Connection* conn = driver->connect("localhost", "root", "");
	if (!conn)
	{
		print_error("conn fail");
		return;
	}

	conn->setSchema("hebei_center");
	sql::Statement* stmt = conn->createStatement();
	sql::ResultSet* res = stmt->executeQuery("select count(*) from user");
	if (!res)
	{
		print_error("query fail\n");
		return;
	}

	while (res->next())
	{
		cout << res->getInt(1) << endl;
	}

}

int main()
{
	cpp_mysql_test();
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
