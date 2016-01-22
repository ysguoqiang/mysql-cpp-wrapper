/*
 * mysql_wrapper.cpp
 *
 *  Created on: Jan 21, 2016
 *  Author: Xiaofeng.Wang
 */

#include "mysql_wrapper.h"

using namespace MysqlWrapper;

MyWrapper::MyWrapper()
{
    SetConnected(false);
    result_ = NULL;
    mysql_init(&conn_);
}

MyWrapper::~MyWrapper()
{
	FreePreResult();
	CloseConnect();
}

bool MyWrapper::CreateTable(const char *sql)
{
	if (mysql_real_query(&conn_, sql, strlen(sql))) {
		ERRMSG("%s", mysql_error(&conn_));
		return false;
	}

	return true;
}

bool MyWrapper::DeleteTable(const char *sql)
{
	if (mysql_real_query(&conn_, sql, strlen(sql))) {
		ERRMSG("%s", mysql_error(&conn_));
		return false;
	}

	return true;
}

int MyWrapper::Connect(const char *host, const char *user, const char *password,
        const char *db)
{
	SaveParam(host, user, password, db);

	if (IsConnected()) {
		printf("Connected..\n");
		return 0;
	}

	if (!mysql_real_connect(&conn_, host, user, password, db, 0, NULL, 0)) {
		ERRMSG("%s", mysql_error(&conn_));
		return -1;
	}
	printf("\tMysql Connection to %s [User:%s] Success!!\n\n", host, user);
	SetConnected(true);

	return 0;
}

void MyWrapper::CloseConnect()
{
	mysql_close(&conn_);
	SetConnected(false);
}

bool MyWrapper::SelectQuery(const char *sql)
{
	if (NULL == sql) {
		ERRMSG("%s", "NULL Sql");
		return false;
	}

	if (!IsConnected()) {
		ERRMSG("%s", "Not Connected");
		return false;
	}

	try {
		if (mysql_real_query(&conn_, sql, strlen(sql))) {
			ERRMSG("%s", mysql_error(&conn_));
			printf("%s\n", "Reconnect is called, Select!!");
			int ret = ReConnect();
			if (ret)
				return false;

			if (mysql_real_query(&conn_, sql, strlen(sql)))
				return false;
		}
		FreePreResult();
		if (NULL == (result_ = mysql_store_result(&conn_))) {
			ERRMSG("%s", mysql_error(&conn_));
			return false;
		}
	} catch (...) {
		printf("ReConnect() is called, Select!!\n");
		ReConnect();
		return false;
	}
	fileds_ = mysql_num_fields(result_); //number of columns in a result set
	map_filedname_index_.clear();

	MYSQL_FIELD *mysql_fields = NULL;

	mysql_fields = mysql_fetch_fields(result_);	//An array of all field structures
	for (unsigned int i = 0; i < fileds_; i++)
		map_filedname_index_[mysql_fields[i].name] = i;

	return true;
}

bool MyWrapper::ModifyQuery(const char *sql)
{
	if (NULL == sql) {
		ERRMSG("%s", "NULL sql");
		return false;
	}

	if (!IsConnected()) {
		ERRMSG("%s", "Not Connected");
		return false;
	}

	try {
		if (mysql_real_query(&conn_, sql, strlen(sql))) {
			ERRMSG("%s", mysql_error(&conn_));
			return false;
		}
	} catch (...) {
		printf("ReConnect() is called, Modify!!\n");
		ReConnect();
		return false;
	}

	return true;
}

char **MyWrapper::FetchRow()
{
	if (NULL == result_) {
		printf("NULL result_\n");
		return NULL;
	}

	row_ = mysql_fetch_row(result_);

	return row_;
}

char *MyWrapper::GetField(const char *field_name)
{
	map<string, int>::iterator iter = map_filedname_index_.find(field_name);

	if (iter != map_filedname_index_.end())
		return GetFiled(iter->second);
	else {
		printf("field_name: %s not exist!!\n", field_name);
		return NULL;
	}
}

char *MyWrapper::GetFiled(unsigned int field_index)
{
	if (field_index >= fileds_) {
		printf("Index overflow!!\n");
		return NULL;
	}

	return row_[field_index];
}

const char *MyWrapper::GetErrMsg()
{
	return err_msg_;
}

bool MyWrapper::IsConnected()
{
	return is_connected_;
}

void MyWrapper::SetConnected(bool flag)
{
	is_connected_ = flag;
}

void MyWrapper::SaveParam(const char *host, const char *user,
        const char *password, const char *db)
{
	hostname_ = host;
	username_ = user;
	password_ = password;
	db_ = db;
}

bool MyWrapper::ReConnect()
{
	CloseConnect();

	if (!mysql_real_connect(&conn_, hostname_.c_str(), username_.c_str(),
	        password_.c_str(), db_.c_str(), 0, NULL, 0)) {
		ERRMSG("%s", mysql_error(&conn_));
		return false;
	}
	SetConnected(true);

	return true;
}

void MyWrapper::FreePreResult()
{
	if (result_ != NULL) {
		mysql_free_result(result_);
		result_ = NULL;
	}
}
