#include "dir.h"
#include "user.h"
#include "common.h"
using namespace std;
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
		dirBlock db = readDir (sn.emptyDirBlock);	//读取空目录块信息
		sn.emptyDirBlock = db.nextDirID;			//该块的下一块作为空目录块的首块
	}
	writeSuperNode (sn);
	return res;
}
//参数表示目录块类型
//如果分配成功返回目录块ID 否则返回-1

bool checkDirName (string newDirName, int dirType){	//检查目录名是否和当前其他目录冲突
	dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1) return true;
	db = readDir (db.sonDirID);
	if (db.dirName == newDirName) return false;
	while (db.nextDirID != -1) {
		db = readDir (db.nextDirID);
		if (db.dirName == newDirName)
			return false;
	}
	return true;
}
//参数表示希望新建的文件名
//如果不冲突返回1 否则返回0

void showAllSonDir (){	//显示当前路径下所有子目录
	dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1) { 			//空目录
		cout << endl;
		return ;
	}
	db = readDir (db.sonDirID);
	cout << db.dirName << " ";
	while (db.nextDirID != -1) {
		db = readDir (db.nextDirID);
		cout << db.dirName << " ";
	}
	cout << endl;
}
//当前路径直接用全局变量
//按照a b.txt c d.cpp 的格式输出

bool mkDir (string newDirName, string newDirMod, int _curDirID) {	//在当前目录下创建子目录
	if(_curDirID == -1) _curDirID = curDirID;
	if (!checkMod (curUserID, curDirID, 2)) {	//权限检查没通过
		cout << "Mod fault!" << endl;
		return false;
	}
	if (newDirMod == "p") {
		userBlock ub = readUser (curUserID);
		if ((string)ub.userName != "admin") {
			cout << "Mod fault!" << endl;
			return false;
		}
	}
	if (!checkDirName (newDirName, 1)) { 	//目录名检查没通过
		cout << "filename error!" << endl;
		return false;
	}
	int dirID = giveDirBlock ();	//分配新的目录块
	//int indexID = giveIndexBlock ();//分配新的索引块
	if (dirID == -1/* || indexID == -1*/) {
		cout << "no more disk space!" << endl;
		return false;
	}
	/*indexBlock ib = readIndex (indexID);
	ib.used = 1;
	ib.offsetID = dirID;
	writeIndex (ib, indexID);*/

	dirBlock db = readDir (_curDirID);
	if (db.sonDirID == -1) {
		db.sonDirID = dirID;
		writeDir (db, _curDirID);
	}
	else {
		int tmp = db.sonDirID;
		db = readDir (db.sonDirID);
		while (db.nextDirID != -1) {		//找到当前路径的最后一个目录
            tmp = db.nextDirID;
			db = readDir (db.nextDirID);
		}
		db.nextDirID = dirID;
		writeDir (db, tmp);
	}

	db = readDir (dirID);
	userBlock ub = readUser (curUserID);
	strcpy (db.dirName, newDirName.c_str ());
	strcpy (db.dirOwner, ub.userName);
	db.dirSize = 0;
	db.dirCreateTime = getTime ();
	db.dirChangeTime = db.dirCreateTime;
	db.type = (newDirMod[0] == 'r' ?
		(newDirMod == "r" ? 1 : 2) :
		(newDirMod == "p" ? 0 : 3));
	db.textLocation = -1;
	db.faDirID = _curDirID;
	db.sonDirID = -1;
	db.nextDirID = -1;
	db.dirMod = (newDirMod[0] == 'r' ?
		(newDirMod == "rw" ? 2 : 1) :
		(newDirMod == "a" ? 3: 0));
	db.used = 1;
	writeDir (db, dirID);	//将目录信息写入目录块
	return true;
}

bool mkdirs (string newDirPath, string newDirMod){	//在当前目录下创建多级子目录
	vector <string> tarDirPath;
	pathPrase (newDirPath, tarDirPath);	//用自动机解析路径
	int dirID = (tarDirPath[0] == "/root" ? 0 : curDirID);
	for (int i = 0; i < tarDirPath.size (); i++) {
		string tmp = tarDirPath[i];
		if (tmp == "/TOT") {
			return 0;
		}
		if (i == 0 && tmp == "/root") continue;
		if (tmp == "/CUR") continue;
		int nextID = findNextDir (dirID, tarDirPath[i]);
		if (nextID == -1) {	//不存在需要新创建
			if (!mkDir (tarDirPath[i], (string)"a",  dirID));
				return false;
		}
		else {
			dirID = nextID;
		}
	}
	return true;
}
//相对或绝对路径 新目录的权限(仅针对最末级)
//路径是一个原始串 需要路径解析自动机解析
//创建成功返回1 否则返回0

bool gotoDir (string tarPath){			//跳转到新的目录
	vector <string> path;
	pathPrase (tarPath, path);
	dirBlock curDirBlock = readDir(curDirID);
	int tmpDirID = curDirID;
    	for(auto op:path)
        	if(!visitPath(curDirBlock, op, tmpDirID)) return false;
    	curDirID = tmpDirID;
		return true;
}
//参数是相对或绝对路径 需要路径解析自动机解析路径
//跳转成功返回1 否则返回0

bool gotoFaDir () {						//跳转到父亲目录
	dirBlock db = readDir (curDirID);
	if (db.faDirID == -1) {	 	//已经处在最上级目录
		return false;
	}
	curPath.pop_back ();		//擦掉路径的最末尾
	curDirID = db.faDirID;	//修改当前目录块id
	return true;
}

bool delDir (int dirID, string dirPath, int type){	//删掉目录块
	vector <string> path;
	pathPrase (dirPath, path);
	if (path[0] == "error")
		return false;
	for (int i = 0; i < path.size ()-1; i++) {
		if (path[i] == "/CUR")
			continue;
		dirID = (findNextDir (dirID, path[i]));
		if (dirID == -1) 		//没有找到目标路径
			return false;
		if (!checkMod (curUserID, dirID, 2))
			return false;		//用户权限错误
	}
	string leaf = path[path.size ()-1];
	if (type == 0) {	//删除整个目录块
		dirID = findNextDir (dirID, leaf);
		if (dirID == -1)
			return -1;
		if (!checkMod (curUserID, dirID, 3))
			return false;
		if (!delAllDir (dirID))	//递归删除整个目录块
			return false;
	}
	else {
		dirBlock db = readDir (dirID);
		if (db.sonDirID == -1)
			return false;
		dirID = db.sonDirID;
		db = readDir (dirID);
		while (!((string)db.dirName != leaf && db.type == 2)) {
			if (db.nextDirID == -1)
				return false;
			dirID = db.nextDirID;
			db = readDir (dirID);
		}
		if (!checkMod (curUserID, dirID, 3))
			return false;
        int indexID = db.textLocation;
        indexBlock ib = readIndex (indexID);
        int fileID = ib.diskOffset;
        releaseIndex (indexID);
        releaseDir (dirID);
        releaseFile (fileID);
	}
	return true;
}
//当前目录块 路径串 删掉的是整个目录还是某个文件
//删除成功返回1 否则(路径不存在或者权限错误)是0

bool delAllDir (int dirID) {		//递归删掉整个目录块
	bool flag = true;
	dirBlock db = readDir (dirID);
	if (db.sonDirID == -1) {
		if (!checkMod (curUserID, dirID, 3))
			return false;
	}
	else {
		flag = delAllDir (db.sonDirID);
	}
	if (db.nextDirID != -1) {
		flag &= delAllDir (db.nextDirID);
	}
	return flag;
}

void releaseDir (int dirID) {		//释放一块目录块
	superNodeBlock sn = readSuperNode ();
	dirBlock db = readDir (dirID);
	db.used = 0;
	if (sn.emptyDirBlock == -1) {
		sn.emptyDirBlock = dirID;
		sn._emptyDirBlock = dirID;
	}
	else {
		dirBlock db = readDir (sn._emptyDirBlock);
		db.nextDirID = dirID;
		writeDir (db, sn._emptyDirBlock);
		sn._emptyDirBlock = dirID;
	}
	writeSuperNode (sn);
	dirBlock faDir = readDir (db.faDirID);
	if (db.nextDirID == -1) {
		faDir.sonDirID = -1;
	}
	else {
		faDir.sonDirID = db.nextDirID;
	}
	writeDir (db, dirID);
	writeDir (faDir, db.faDirID);
}


