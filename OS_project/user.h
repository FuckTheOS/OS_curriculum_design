#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

bool userLogin ();	//刚开始时的用户登录
					//可以选择admin，普通用户，访问模式(无用户不用输入账号密码)
					//正常登陆返回1 否则返回0
userBlock readUser (int id);				//根据用户块id读取用户块信息
//访盘，注意保护原有数据
void writeUser (userBlock ub, int id);  	//将用户块信息写入用户块
//访盘，注意保护原有数据

#endif // USER_H_INCLUDED

