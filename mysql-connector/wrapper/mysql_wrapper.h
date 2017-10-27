#ifndef M_MYSQL_WRAPPER_INCLUDE
#define M_MYSQL_WRAPPER_INCLUDE

#include "mysql_wrapper_config.h"
#include "mysql_wrapper_mutex.hpp"
#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/prepared_statement.h"
#include <stdio.h>

#ifdef M_PLATFORM_WIN
#pragma comment(lib, "mysqlcppconn.lib")
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
 
#define M_MYSQL_CATCH_EXCEPTION \
catch (sql::SQLException& e){\
	M_PRINT_HANDLER("ERROR , sql exception : what : %s, err_code : %d, sql_state : %s \n", e.what(), e.getErrorCode(), e.getSQLState().c_str()); \
}\
catch (std::runtime_error &e){\
	M_PRINT_HANDLER("ERROR , runtime_error in %s, waht : %s \n", __FUNCTION__, e.what()); \
}\
catch (...){\
	M_PRINT_HANDLER("ERROR , unknow error in %s \n", __FUNCTION__);\
}


typedef boost::shared_ptr<sql::Connection>			SqlConnectionPtr;
typedef boost::scoped_ptr< sql::PreparedStatement > PreparedStatementScopedPtr;
typedef boost::scoped_ptr< sql::Statement >			StatementScopedPtr;
typedef boost::scoped_ptr< sql::ResultSet >			ResultSetScopedPtr;

template<int N>
class MyqlWrapperBase
{
protected:
	static MysqlWrapperDetail::MutexLock _s_mutex;
};

template<int N>
MysqlWrapperDetail::MutexLock MyqlWrapperBase<N>::_s_mutex;

// 简单的包装类
class MysqlWrapper : public MyqlWrapperBase<0>
{
public:
	static sql::Driver* GetDriver()
	{
		// get_driver_instance_by_name is not thread-safe
		MysqlWrapperDetail::ScopedLock scoped(_s_mutex);
		try
		{
			char id_str[10] = { 0 };
			sprintf(id_str, "%d",get_cur_threadid());
			return sql::mysql::get_driver_instance_by_name(id_str);
		}
		M_MYSQL_CATCH_EXCEPTION;

		return 0;
	}

	static SqlConnectionPtr GetConnect(const std::string& hostName, const std::string& userName, const std::string& password,unsigned short port=0)
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

	static SqlConnectionPtr GetConnect(const std::string& hostName, const std::string& userName, const std::string& password, unsigned short port, const std::string& db)
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

	static SqlConnectionPtr GetConnect(const std::string& hostName, const std::string& userName, const std::string& password, unsigned short port, const std::string& db,
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

	static SqlConnectionPtr GetConnect(sql::ConnectOptionsMap & options)
	{
		sql::Driver* driver = GetDriver();
		if (!driver)
			return SqlConnectionPtr();

		try
		{
			return SqlConnectionPtr(driver->connect(options));
		}
		M_MYSQL_CATCH_EXCEPTION;

		return SqlConnectionPtr();
	}
};

// 取域
struct GetFieldWrapper
{
public:

	// int32_t
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, int32_t& value)
	{
		value = set->getInt(index);
	}
	// int32_t
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, int32_t& value)
	{
		value = set->getInt(columnLabel);
	}

	// uint32_t
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, uint32_t& value)
	{
		value = set->getUInt(index);
	}
	// uint32_t
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, uint32_t& value)
	{
		value = set->getUInt(columnLabel);
	}

	// int64_t
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, int64_t& value)
	{
		value = set->getInt64(index);
	}
	// int64_t
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, int64_t& value)
	{
		value = set->getInt64(columnLabel);
	}

	// uint64_t
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, uint64_t& value)
	{
		value = set->getUInt64(index);
	}
	// uint64_t
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, uint64_t& value)
	{
		value = set->getUInt64(columnLabel);
	}

	// long double
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, long double& value)
	{
		value = set->getDouble(index);
	}
	// long double
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, long double& value)
	{
		value = set->getDouble(columnLabel);
	}

	// bool
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, bool& value)
	{
		value = set->getBoolean(index);
	}
	// bool
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, bool& value)
	{
		value = set->getBoolean(columnLabel);
	}

	// SQLString
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, sql::SQLString& value)
	{
		value = set->getString(index);
	}
	// SQLString
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, sql::SQLString& value)
	{
		value = set->getString(columnLabel);
	}

	// std::string
	GetFieldWrapper(sql::ResultSet* set, unsigned int index, std::string& value)
	{
		value = set->getString(index);
	}
	// std::string
	GetFieldWrapper(sql::ResultSet* set, const char* columnLabel, std::string& value)
	{
		value = set->getString(columnLabel);
	}
};

// 设域: index 必须是从1开始的，并且不能超出输入参数最大个数，否则会抛异常
struct SetFieldWrapper
{
	// int32_t
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const int32_t& value)
	{
		stmt->setInt(index, value);
	}

	// uint32_t
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const uint32_t& value)
	{
		stmt->setUInt(index, value);
	}

	// int64_t
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const int64_t& value)
	{
		stmt->setInt64(index, value);
	}

	// uint64_t
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const uint64_t& value)
	{
		stmt->setUInt64(index, value);
	}

	// double
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const double& value)
	{
		stmt->setDouble(index, value);
	}

	// bool
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const bool& value)
	{
		stmt->setBoolean(index, value);
	}

	// sql::SQLString
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const sql::SQLString& value)
	{
		stmt->setString(index, value);
	}

	// std::string
	SetFieldWrapper(sql::PreparedStatement* stmt, unsigned int index, const std::string& value)
	{
		stmt->setString(index, value);
	}
};

#endif