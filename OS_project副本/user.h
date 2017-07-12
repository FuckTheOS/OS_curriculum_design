#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
#include <string>
using namespace std;

bool userLogin ();	//刚开始时的用户登录
					//可以选择admin，普通用户，访问模式(无用户不用输入账号密码)
					//正常登陆返回1 否则返回0

bool chuser (string name, string passwd);	//在当前目录下切换用户
//参数表示用户名和用户密码
//成功返回1 否则返回0
bool createuser (string name, string passwd, int userMod);	//创建用户
//参数表示用户名和用户密码 被创建用户的权限
//成功返回1 否则返回0

#endif // USER_H_INCLUDED

