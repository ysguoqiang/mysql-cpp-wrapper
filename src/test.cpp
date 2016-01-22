/*
 * test.cpp
 *
 *  Created on: Jan 21, 2016
 *  Author: Xiaofeng.Wang
 */

#include "mysql_wrapper.h"

using MysqlWrapper::MyWrapper;

int main(int argc, char **argv)
{
    MyWrapper *con = new MyWrapper();

    if (-1 == con->Connect("localhost", "root", "passwd", "student")) {
        cout << con->GetErrMsg() << endl;
        return -1;
    }

    /*//create table
      if (!con->CreateTable("create table test (id int)")) {
      cout << con->GetErrMsg() << endl;
      return -1;
      }*/

    //update
    if (!con->ModifyQuery("update student_info set stu_score = '100'"
            "where stu_num = '2011031013'")) {
        cout << con->GetErrMsg() << endl;
        return -1;
    }

    //select
    if (!con->SelectQuery("select * from student_info")) {
        cout << con->GetErrMsg() << endl;
        return -1;
    }
    //Execute selectQuery
    MYSQL_ROW res = NULL;
    while (NULL != (res = con->FetchRow())) {
        printf("%s\t%s\t%s\t%s\t%s\n", res[0], res[1], res[2], res[3], res[4]);
        printf("Column stu_score: %s\n\n", con->GetField("stu_score"));
    }

    /*//delete
      if (!con->DeleteTable("drop table test")) {
      cout << con->GetErrMsg() << endl;
      return -1;
      }*/

    return 0;
}
