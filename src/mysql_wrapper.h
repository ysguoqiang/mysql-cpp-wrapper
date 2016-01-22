/*
 * mysql_wrapper.h
 *
 *  Created on: Jan 21, 2016
 *  Author: Xiaofeng.Wang
 */

#ifndef MYSQL_CPP_WRAPPER_MYSQL_WRAPPER_H_
#define MYSQL_CPP_WRAPPER_MYSQL_WRAPPER_H_

/* Standard C/C++ Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>

/* MySQL C API */
#include <mysql/mysql.h>

using std::map;
using std::string;
using std::cout;
using std::endl;

#define ERRMSG(fmt, args...) do {	\
    snprintf(err_msg_, BUFF_SIZE, "[%s:%s:%d]->"fmt, \
        __FILE__, __FUNCTION__, __LINE__, ##args); \
} while (0)

#define BUFF_SIZE  1024

namespace MysqlWrapper {

class MyWrapper {
	typedef map<string, int> map_index;

public:
	MyWrapper();
	~MyWrapper();

public:
	int Connect(const char *host, const char *user, const char *password,
	        const char *db);
	void CloseConnect();

	bool SelectQuery(const char *sql);
	bool ModifyQuery(const char *sql);
	const char *GetErrMsg();
	char **FetchRow();
	char *GetField(const char *filed_name);

private:
	bool IsConnected();
	void SetConnected(bool flag);
	char *GetFiled(unsigned int filed_index);
	void FreePreResult();
	bool ReConnect();
	void SaveParam(const char *host, const char *user, const char *password,
	        const char *db);

public:
	bool is_connected_;	//数据库是否连接
	char err_msg_[BUFF_SIZE];	//出错信息
	unsigned int fileds_;	//字段个数
	map_index map_filedname_index_;	//key是字段名，value是字段对应的索引

public:
	MYSQL conn_;
	MYSQL_RES *result_;
	MYSQL_ROW row_;

private:
	string hostname_;
	string username_;
	string password_;
	string db_;
};
} //End of namespace MysqlWrapper

#endif /* MYSQL_CPP_WRAPPER_MYSQL_WRAPPER_H_ */
