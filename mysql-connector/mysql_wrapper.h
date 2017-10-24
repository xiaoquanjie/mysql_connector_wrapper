#ifndef M_MYSQL_WRAPPER_INCLUDE
#define M_MYSQL_WRAPPER_INCLUDE

#include "mysql_wrapper_config.h"
#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"

#ifdef M_PLATFORM_WIN
#pragma comment(lib, "mysqlcppconn.lib")
#pragma comment(lib, "libmysql.lib")
#endif

#endif