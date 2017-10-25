#ifndef M_MYSQL_WRAPPER_INCLUDE
#define M_MYSQL_WRAPPER_INCLUDE

#include "mysql_wrapper_config.h"
#include "mysql_wrapper_mutex.hpp"
#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include <stdio.h>

#ifdef M_PLATFORM_WIN
#pragma comment(lib, "mysqlcppconn.lib")
#pragma comment(lib, "libmysql.lib")
#endif

#define M_PRINT_HANDLER printf

#define M_SQL_TRY() \
try{

#define M_SQL_EXCEPTION() \
}\
catch (sql::SQLException& e){\
	M_PRINT_HANDLER("ERROR , sql exception : what : %s, err_code : %d, sql_state : %s \n", e.what(), e.getErrorCode(), e.getSQLState().c_str()); \
}\
catch (std::runtime_error &e){\
	M_PRINT_HANDLER("ERROR , runtime_error in %s, waht : %s \n", __FUNCTION__, e.what()); \
}\
catch (...){\
	M_PRINT_HANDLER("ERROR , unknow error in %s \n", __FUNCTION__);\
}

template<int N>
class MyqlWrapperBase
{
protected:
	static MysqlWrapperDetail::MutexLock _s_mutex;
};

template<int N>
MysqlWrapperDetail::MutexLock MyqlWrapperBase<N>::_s_mutex;

// 简单的包装类
class MyqlWrapper : public MyqlWrapperBase<0>
{
public:
	static sql::Driver* GetDriver()
	{
		// get_mysql_driver_instance is not thread-safe
		MysqlWrapperDetail::ScopedLock scoped(_s_mutex);
		M_SQL_TRY()
			return sql::mysql::get_mysql_driver_instance();
		M_SQL_EXCEPTION();
		return 0;
	}

	static sql::Connection * GetConnect(const std::string& hostName, const std::string& userName, const std::string& password,unsigned short port=0)
	{
		sql::ConnectOptionsMap options;
		options["OPT_RECONNECT"] = true;
		options["hostName"] = hostName;
		options["userName"] = userName;
		options["password"] = password;
		if (port != 0)
			options["port"] = port;
		
		return GetConnect(options);
	}

	static sql::Connection * GetConnect(const std::string& hostName, const std::string& userName, const std::string& password, unsigned short port, const std::string& db)
	{
		sql::ConnectOptionsMap options;
		options["OPT_RECONNECT"] = true;
		options["hostName"] = hostName;
		options["userName"] = userName;
		options["password"] = password;
		options["schema"] = db;
		if (port != 0)
			options["port"] = port;

		return GetConnect(options);
	}

	static sql::Connection * GetConnect(const std::string& hostName, const std::string& userName, const std::string& password, unsigned short port, const std::string& db,
		const std::string& characterset)
	{
		sql::ConnectOptionsMap options;
		options["OPT_RECONNECT"] = true;
		options["hostName"] = hostName;
		options["userName"] = userName;
		options["password"] = password;
		options["schema"] = db;
		options["characterSetResults"] = characterset;
		if (port != 0)
			options["port"] = port;

		return GetConnect(options);
	}

	static sql::Connection * GetConnect(sql::ConnectOptionsMap & options)
	{
		sql::Driver* driver = GetDriver();
		if (!driver)
			return 0;

		M_SQL_TRY()
			return driver->connect(options);
		M_SQL_EXCEPTION();
		return 0;
	}
};

#endif