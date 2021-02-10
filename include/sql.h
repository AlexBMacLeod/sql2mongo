#pragma once
void finish_with_error(MYSQL *con);
void fromMySQL(int volRow, const char username[], const char password[], const char database[], const char query[]);