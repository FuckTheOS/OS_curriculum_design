#include "common.h"
#include "dir.h"
#include "index.h"

dirBlock readDir (int id) {				//根据文件块id读取文件块信息
	if (id >= DIRSIZE || id < 0) {
		cout << "段错误！" << endl;
		exit (0);
	}
	dirBlock db;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (dirSegOffset+sizeof (db)*id, ios::beg); 	//定位到目标便宜
	db = fin.read ((char *)&db, sizeof db);
	fin.close ();
}

void writeDir (dirBlock db, int id){	//将目录块信息写入目录块
	if (id >= DIRSIZE || id < 0) {
		cout << "段错误！" << endl;
		exit (0);
	}
	ofstream fout (dis.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (userSegOffset+sizeof (db)*id, ios::beg);
	fout.write ((char *)&db, sizeof db);
	fout.close ();
}

int giveDirBlock (int dirType){			//分配新的目录块 
	return -1;
}

bool checkDirName (string newDirName){	//检查目录名是否和当前其他目录冲突
	return true;
}
//参数表示希望新建的文件名
//如果不冲突返回1 否则返回0
//当前路径直接是全局变量

void showAllSonDir (){	//显示当前路径下所有子目录

}
//当前路径直接用全局变量
//按照a b.txt c d.cpp 的格式输出

bool mkdir (string newDirName, string newDirMod) {	//在当前目录下创建子目录
	if (!checkMod (3)) {	//权限检查没通过
		cout << "权限错误！" << endl;
		return false;
	}
	if (!checkDirName (newDirName)) { 	//目录名检查没通过
		cout << "文件名错误！" << endl;
		return false;
	}
	int dirID = giveDirBlock (1);	//分配新的目录块
	int indexID = giveIndexBlcok ();//分配新的索引块
	if (dirID == -1 || indexID == -1) {
		cout << "磁盘空间不足!" << endl;
		return false;
	}
	return true;
}

bool mkdirs (string newDirPath, string newDirMod){	//在当前目录下创建多级子目录
	vector <string> tarDirPath = pathPrase (newDirPath);	//用自动机解析路径
	int dirID = (tarDirPath[0] == "root" ? 0 : curDirID);
	for (int i = 0; i < tarDirPath.size (); i++) {
		string tmp = tarDirPath[i];
		if (tmp == "TOT") {
			return 0;
		}
		if (i == 0 && tmp == root) continue;
		if (tmp == "CUR") continue;
		int nextID = findNextDir (dirID, tmp);	//找到下一级目录
		if (nextID == -1) {
			if (!mkdir (tmp, newDirMod)) {
				return false;
			}
		}
		else {}
	} 
	return true;
}
//相对或绝对路径 新目录的权限(仅针对最末级)
//路径是一个原始串 需要路径解析自动机解析
//创建成功返回1 否则返回0

bool gotoDir (string tarPath){			//跳转到新的目录
	vector <string> path = pathPrase (tarPath);
	return true;
}
//参数是相对或绝对路径 需要路径解析自动机解析路径
//跳转成功返回1 否则返回0

