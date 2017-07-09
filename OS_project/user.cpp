#include "common.h"
#include "user.h"

bool userLogin () {			//刚开始时的用户登录
	return 0;
}

userBlock readUser (int id){				//根据用户块id读取用户块信息
	userBlock ub;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (userSegOffset+sizeof (ub)*id, ios::beg);
	ub = fin.read ((char *)&ub, sizeof ub);
	fin.close ();
	return ub;
}

void writeUser (userBlock ub, int id){  	//将用户块信息写入用户块
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (userSegOffset+sizeof (ub)*id, ios::beg);
	fout.write ((char *)&ub, sizeof ub);
	fout.close ();
}
//访盘，注意保护原有数据