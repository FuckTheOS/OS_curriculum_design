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

int giveDirBlock (){					//分配新的目录块 
	superNodeBlock sn = readSuperNode ();
	if (sn.emptyDirBlock == -1) {		//目录块空间不足
		return -1;
	}
	int res = sn.emptyDirBlock;
	if (sn.emptyDirBlock == sn._emptyDirBlock) {	//目录块刚好用完
		sn.emptyDirBlock = -1;
		sn._emptyDirBlock = -1;
	}
	else {
		dirBlcok db = readDir (sn.emptyDirBlock);	//读取空目录块信息
		sn.emptyDirBlock = db.nextDirID;			//该块的下一块作为空目录块的首块
	}
	return res;
}
//参数表示目录块类型
//如果分配成功返回目录块ID 否则返回-1

bool checkDirName (string newDirName){	//检查目录名是否和当前其他目录冲突
	dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1) return true;
	indexBlock ib = readIndex (db.sonDirID);
	db = readDir (ib.offsetID);
	if (db.dirName == newDirName) return false;
	while (db.nextDirID != -1) {
		ib = readIndex (db.nextDirID);
		db = readDir (ib.offsetID);
		if (db.dirName == newDirName) 
			return false;
	}
	return true;
}
//参数表示希望新建的文件名
//如果不冲突返回1 否则返回0

void showAllSonDir (){	//显示当前路径下所有子目录
	dirBlcok db = readDir (curDirID);
	if (db.sonDirID == -1) { 			//空目录
		cout << endl;
		return ;
	}
	indexBlock ib = readIndex (db.sonDirID);
	db = readDir (ib.offsetID);
	cout << db.dirName << " ";
	while (db.nextDirID != -1) {
		ib = readIndex (db.nextDirID);
		db = readDir (ib.offsetID);
		cout << db.dirName << " ";
	}
	cout << endl;
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
	indexBlock ib = readIndex (indexID);	//更新索引块信息
	ib.used = 1;
	ib.offsetID = dirID;
	writeIndex (ib, indexID);

	dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1) {
		db.sonDirID = indexID;
	}
	else {
		int tmp = db.sonDirID;
		while (db.nextDirID != -1) {		//找到当前路径的最后一个目录 
			db = readDir (db.nextDirID);
		}
		db.nextDirID = dirID;
	}

	db = readDir (dirID);
	strcpy (db.fileName, newDirName.c_str ());
	char dirOwner[20];	//目录创建者
	db.dirSize = 0;
	db.dirCreateTime;	//目录创建时间
	db.dirChangeTime;	//目录上一次修改时间
	db.type = 1;
	db.textLocation = -1;
	db.faDirID = curDirID;
	db.sonDirID = -1;
	db.nextDirID = -1;
	db.used = 1;
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
	dirBlock* curDirBlock = &readDir(curDirID);
    	for(auto op:path)
        	if(!visitPath(op)) return false;
    	curDirID = curDirBlock - &readDir(0);
		return true;
}
//参数是相对或绝对路径 需要路径解析自动机解析路径
//跳转成功返回1 否则返回0

