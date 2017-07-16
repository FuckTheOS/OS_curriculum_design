#include "user.h"
#include "common.h"
#include "filestruct.h"
bool userLogin () {			//刚开始时的用户登录
    string userName, passwd;
    cout << "        input username: "; cin >> userName;
    cout << "        input password: "; cin >> passwd;
    system ("CLS");
    userBlock ub;
	superNodeBlock sn = readSuperNode ();
	for (int i = 0; i < sn.emptyUserBlock; i++) {
		ub = readUser (i);
		if (userName == (string)ub.userName && passwd == (string)ub.userPassword) {  //找到该用户
		    curUserID = i;
		    cout << "userID" << curUserID << endl;
		    return true;
		}
	}
	return 0;
}


bool chuser (string name, string passwd){	//在当前目录下切换用户
	userBlock ub;
	for (int i = 0; i < USERSIZE; i++) {
		ub = readUser (i);
		if ((string)ub.userName == name && (string)ub.userPassword == passwd) {
			int dirID = curDirID;
			dirBlock db;
			curUserID = i;
			while (dirID != -1) {		//往上追溯 看看是够和上面的目录冲突
				if (dirID == 0)
					return true;
				if (!checkMod (i, dirID, 1))
					return false;
				db = readDir (db.faDirID);
			}
			return true;
		}
	}
	return false;
}
//参数表示用户名和用户密码
//成功返回1 否则返回0

bool createuser (string name, string passwd, int userMod){	//创建用户
	userBlock ub;
	superNodeBlock sn = readSuperNode ();
	for (int i = 0; i < sn.emptyUserBlock; i++) {
		ub = readUser (i);
		if (name == (string)ub.userName) 	//用户名已经存在
			return false;
	}
	ub = readUser (sn.emptyUserBlock);
	strcpy (ub.userName, name.c_str ());
	strcpy (ub.userPassword, passwd.c_str ());
	ub.userMod = userMod;
	ub.used = 1;
	writeUser (ub, sn.emptyUserBlock);
	sn.emptyUserBlock = (sn.emptyUserBlock+1 == USERSIZE ? -1 : sn.emptyUserBlock+1);
	writeSuperNode (sn);
	return true;
}
//参数表示用户名和用户密码 被创建用户的权限
//成功返回1 否则返回0



