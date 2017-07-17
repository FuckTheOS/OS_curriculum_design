#include "dir.h"
#include "user.h"
#include "common.h"
#include "filestruct.h"
#include "concol.h"
using namespace std;
int giveDirBlock() {					//分配新的目录块
	superNodeBlock sn = readSuperNode();
	if (sn.emptyDirBlock == -1) {		//目录块空间不足
		return -1;
	}
	int res = sn.emptyDirBlock;
	if (sn.emptyDirBlock == sn._emptyDirBlock) {	//目录块刚好用完
		sn.emptyDirBlock = -1;
		sn._emptyDirBlock = -1;
	}
	else {
		dirBlock db = readDir(sn.emptyDirBlock);	//读取空目录块信息
		sn.emptyDirBlock = db.nextDirID;			//该块的下一块作为空目录块的首块
	}
	writeSuperNode(sn);
	return res;
}
//参数表示目录块类型
//如果分配成功返回目录块ID 否则返回-1

bool checkDirName(string newDirName, int dirType) {	//检查目录名是否和当前其他目录冲突
	dirBlock db = readDir(curDirID);
	if (db.sonDirID == -1) return true;
	db = readDir(db.sonDirID);
	if (db.dirName == newDirName) return false;
	while (db.nextDirID != -1) {
		db = readDir(db.nextDirID);
		if (db.dirName == newDirName)
			return false;
	}
	return true;
}
//参数表示希望新建的文件名
//如果不冲突返回1 否则返回0
void setColorForType(int type)
{
    if(type == 1)
        setcolor(blue, dark_green);
    if(type == 2)
        setcolor(green,black);
    if(type == 3)
        setcolor(red, black);
    if(type == 4)
        setcolor(pink, black);
}
void showAllSonDir() {	//显示当前路径下所有子目录
    //cout << curUserID << " " << curDirID << endl;
	dirBlock db = readDir(curDirID);
	//cout << db.sonDirID << endl;
	if (db.sonDirID == -1 || !db.used) { 			//空目录
		cout << endl;
		return;
	}
	db = readDir(db.sonDirID);
	setColorForType(db.type);
	cout << db.dirName ;
	setcolor(gray, black);
	cout<<" ";
	while (db.nextDirID != -1 || !db.used) {
		db = readDir(db.nextDirID);
		setColorForType(db.type);
		cout << db.dirName ;
		setcolor(gray, black);
		cout<<" ";
	}
	setcolor(gray, black);
	cout << endl;
}
//当前路径直接用全局变量
//按照a b.txt c d.cpp 的格式输出

bool mkDir(string newDirName, string newDirMod, int _curDirID) {	//在当前目录下创建子目录
	if (_curDirID == -1) _curDirID = curDirID;
	if (!checkMod(curUserID, curDirID, 2)) {	//权限检查没通过
		cout << "Mod fault!" << endl;
		return false;
	}
	if (newDirMod == "p") {
		userBlock ub = readUser(curUserID);
		if ((string)ub.userName != "admin") {
			cout << "Mod fault!" << endl;
			return false;
		}
	}
	if (!checkDirName(newDirName, 1)) { 	//目录名检查没通过
		cout << "filename error!" << endl;
		return false;
	}
	int dirID = giveDirBlock();	//分配新的目录块
								//int indexID = giveIndexBlock ();//分配新的索引块
	if (dirID == -1/* || indexID == -1*/) {
		cout << "no more disk space!" << endl;
		return false;
	}
	/*indexBlock ib = readIndex (indexID);
	ib.used = 1;
	ib.offsetID = dirID;
	writeIndex (ib, indexID);*/

	dirBlock db = readDir(_curDirID);
	if (db.sonDirID == -1) {
		db.sonDirID = dirID;
		writeDir(db, _curDirID);
	}
	else {
		int tmp = db.sonDirID;
		db = readDir(db.sonDirID);
		while (db.nextDirID != -1) {		//找到当前路径的最后一个目录
			tmp = db.nextDirID;
			db = readDir(db.nextDirID);
		}
		db.nextDirID = dirID;
		writeDir(db, tmp);
	}

	db = readDir(dirID);
	userBlock ub = readUser(curUserID);
	strcpy(db.dirName, newDirName.c_str());
	strcpy(db.dirOwner, ub.userName);
	db.dirSize = 0;
	db.dirCreateTime = getTime();
	db.dirChangeTime = db.dirCreateTime;
	db.type = 1;
	db.textLocation = -1;
	db.faDirID = _curDirID;
	db.sonDirID = -1;
	db.nextDirID = -1;
	db.dirMod = (newDirMod[0] == 'r' ?
		(newDirMod == "rw" ? 2 : 1) :
		(newDirMod == "a" ? 3 : 0));
	db.used = 1;
	writeDir(db, dirID);	//将目录信息写入目录块
	return true;
}

bool mkdirs(string newDirPath, string newDirMod) {	//在当前目录下创建多级子目录
	vector <string> tarDirPath;
	pathPrase(newDirPath, tarDirPath);	//用自动机解析路径
	int dirID = (tarDirPath[0] == "/root" ? 0 : curDirID);
	for (int i = 0; i < tarDirPath.size(); i++) {
		string tmp = tarDirPath[i];
		if (tmp == "/TOT") {
			return 0;
		}
		if (i == 0 && tmp == "/root") continue;
		if (tmp == "/CUR") continue;
		int nextID = findNextDir(dirID, tarDirPath[i]);
		if (nextID == -1) {	//不存在需要新创建
			if (!mkDir(tarDirPath[i], (string)"a", dirID))
			return false;
			dirID = findNextDir(dirID, tarDirPath[i]);
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

bool gotoDir(string tarPath) {			//跳转到新的目录
	vector <string> path;
	pathPrase(tarPath, path);
	dirBlock curDirBlock = readDir(curDirID);
	int tmpDirID = curDirID;
	for (auto op : path)
		if (!visitPath(curDirBlock, op, tmpDirID)) return false;
	curDirID = tmpDirID;
	if (path[0] == "/root") {
		curPath.clear(); curPath.pb("/root");
		for (int i = 1; i < path.size(); i++) {
			if (path[i] == "/CUR" || path[i] == "/TOT") continue;
			curPath.pb(path[i]);
		}
	}
	else {
		for (int i = 0; i < path.size(); i++) {
			if (path[i] == "/CUR" || path[i] == "/TOT") continue;
			curPath.pb(path[i]);
		}
	}
	return true;
}
//参数是相对或绝对路径 需要路径解析自动机解析路径
//跳转成功返回1 否则返回0

bool gotoFaDir() {						//跳转到父亲目录
	dirBlock db = readDir(curDirID);
	if (db.faDirID == -1) {	 	//已经处在最上级目录
		return false;
	}
	curPath.pop_back();		//擦掉路径的最末尾
	curDirID = db.faDirID;	//修改当前目录块id
	return true;
}

bool delDir(int dirID, string dirPath, int type) {	//删掉目录块
	vector <string> path;
	pathPrase(dirPath, path);
	//for (int i = 0; i < path.size (); i++) cout << path[i] << " "; cout << endl; cout << type << endl;
	if (path[0] == "error")
		return false;
	for (int i = 0; i < path.size() - 1; i++) {
		if (path[i] == "/CUR")
			continue;
		dirID = (findNextDir(dirID, path[i]));
		if (dirID == -1) 		//没有找到目标路径
			return false;
		if (!checkMod(curUserID, dirID, 2))
			return false;		//用户权限错误
	}
	string leaf = path[path.size() - 1];
	//cout <<leaf << endl;
//	if (type == 0) {	//删除整个目录块
    int faID = dirID;
    if (type == 0) dirID = findNextDir(dirID, leaf);
    else dirID = findNextDir(dirID, leaf, 2);
    if(dirID == -1)
    {
        cout<<"No such file. Maybe it's a directory?"<<endl;
        return false;
    }
    dirBlock db = readDir (dirID);
    int broID = db.nextDirID;
    //cout << "dirID" << dirID << endl;
    if (dirID == -1)
        return -1;
    if (!checkMod(curUserID, dirID, 3)) {
       // cout << ".." << endl;
        return false;
    }
    dirBlock fadb = readDir(faID);
    if (fadb.sonDirID == dirID) {
        fadb.sonDirID = broID;
        writeDir(fadb, faID);
    }
    else {
        dirBlock tmp = readDir(fadb.sonDirID); int oldID = fadb.sonDirID;
        while (tmp.nextDirID != dirID) {
            //cout << oldID << " " << tmp.dirName << endl;
            oldID = tmp.nextDirID;
            tmp = readDir(tmp.nextDirID);
        }
        tmp.nextDirID = broID;
        writeDir(tmp, oldID);
    }
    if (type == 0) {
        if (!delAllDir(dirID, 1))	//递归删除整个目录块
            return false;
    }
    else {
        //cout << ".." << endl;
        db = readDir(dirID);
        if(db.type == 1) return false;
        indexBlock ib = readIndex(db.textLocation);
        releaseFile(ib.diskOffset);
        releaseIndex(db.textLocation);
        releaseDir(dirID);
    }

//	}
//	else {
//        //cout << "fuck" << endl;
//		dirBlock db = readDir(dirID), tmp;
//		if (db.sonDirID == -1)
//			return false;
//		//dirID = db.sonDirID;
//		tmp = db;
//		db = readDir(db.sonDirID);
//		bool flag = 1;
//		while ((string)db.dirName != leaf) {
//			if (db.nextDirID == -1 || !db.used)
//				return false;
//            if (flag) dirID = tmp.sonDirID, flag = 0;
//            else dirID = tmp.nextDirID;
//			tmp = db;
//			db = readDir(tmp.nextDirID);
//			if ((string)db.dirName == leaf) {   //找到了
//			    if (!checkMod(curUserID, dirID, 2))
//                    return false;
//                int indexID = db.textLocation;
//                indexBlock ib = readIndex(indexID);
//                int fileID = ib.diskOffset;
//                releaseIndex(indexID);
//                releaseDir(tmp.nextDirID);
//                releaseFile(fileID);
//                tmp.nextDirID = -1;
//                writeDir (tmp, dirID);
//                return true;
//			}
//		}
//		if (!checkMod(curUserID, dirID, 2)) {
//			return false;
//		}
//		int indexID = db.textLocation;
//		indexBlock ib = readIndex(indexID);
//		int fileID = ib.diskOffset;
//		releaseIndex(indexID);
//		releaseDir(tmp.sonDirID);
//		releaseFile(fileID);
//
//		tmp.sonDirID = -1;
//		writeDir (tmp, dirID);
//	}
	return true;
}
//当前目录块 路径串 删掉的是整个目录还是某个文件
//删除成功返回1 否则(路径不存在或者权限错误)是0

bool delAllDir(int dirID, int type) {		//递归删掉整个目录块
    //cout << dirID << ".." << endl;
	bool flag = true;
	dirBlock db = readDir(dirID); cout << db.dirName << endl;
	if (db.sonDirID == -1) {
        //cout << "id1" <<endl;
		if (!checkMod(curUserID, dirID, 3)) {
            //cout << "1error" <<endl;
			return false;
		}
	}
	else {
	    //cout << "id2" << endl;
		flag = delAllDir(db.sonDirID);
	}
	if (type == 1) {
        if (checkMod(curUserID, dirID, 3) && flag) {
            dirBlock db = readDir(dirID);
            if (db.type == 1) releaseDir(dirID);
            else if (db.type == 2) {
                indexBlock ib = readIndex(db.textLocation);
                releaseFile(ib.diskOffset);
                releaseIndex(db.textLocation);
                releaseDir(dirID);
            }
        }
        return flag;
	}
	if (db.nextDirID != -1) {
		flag &= delAllDir(db.nextDirID);
	}
	if (checkMod(curUserID, dirID, 3) && flag) {
        dirBlock db = readDir(dirID);
        if (db.type == 1) releaseDir(dirID);
        else if (db.type == 2) {
            indexBlock ib = readIndex(db.textLocation);
            releaseFile(ib.diskOffset);
            releaseIndex(db.textLocation);
            releaseDir(dirID);
        }
	}
	else flag = 0;
	return flag;
}

void releaseDir(int dirID) {		//释放一块目录块
	superNodeBlock sn = readSuperNode();
	dirBlock db = readDir(dirID);
	db.used = 0;
	if (sn.emptyDirBlock == -1) {
		sn.emptyDirBlock = dirID;
		sn._emptyDirBlock = dirID;
	}
	else {
		dirBlock tmp = readDir(sn._emptyDirBlock);
		tmp.nextDirID = dirID;
		writeDir(tmp, sn._emptyDirBlock);
		sn._emptyDirBlock = dirID;
	}
	writeSuperNode(sn);
	db.sonDirID = db.faDirID = -1;
	writeDir(db, dirID);
}

int getDirID (string path){         //从当前目录下按照这个路径访问目标 返回目标的dirID
    vector <string> Path; pathPrase(path, Path);
    if (Path[0] == "error")
        return -1;
    int dirID = (Path[0] == "/root"? 0 : curDirID);
    for (int i = 0; i < Path.size (); i++) {
        if (Path[i] == "/CUR") continue;
        int tmp = findNextDir(dirID, Path[i], 1);
        if (i == Path.size()-1 && tmp == -1) {
            tmp = findNextDir(dirID, Path[i], 2);
        }
        dirID = tmp;
    }
    return dirID;
}
//如果不存在返回-1

bool moveDir (string fromPath, string toPath) {     //移动文件
    int id1 = getDirID(fromPath), id2 = getDirID(toPath);
    dirBlock db1 = readDir(id1), db2 = readDir(id2);
    if (db2.type != 1) return false;
    cpDir(fromPath, toPath);
    delDir(curDirID, fromPath, db1.type-1);
    return true;
}

void cpAllDir (int id1, int id2) {                  //将ID2中的内容递归拷贝到ID1中
    //cout << id1 << " " << id2 << ".." << endl;
    dirBlock db1 = readDir(id1), db2 = readDir(id2), newdb;
    int newID = giveDirBlock(); newdb = readDir(newID);
    //cout << ".." << endl;
    newdb = db2; newdb.faDirID = id1; newdb.nextDirID = -1; newdb.sonDirID = -1;
    if (newdb.type == 2) {                          //拷贝文件
        int newi = giveIndexBlock(); indexBlock ib = readIndex(db2.textLocation), newib = ib;
        int newf = giveFileBlock(); fileBlock fb = readFile(ib.diskOffset), newfb = fb;
        newdb.textLocation = newi;
        newib.diskOffset = newf;
        writeIndex (newib, newi);
        writeFile(newfb, newf);
    }
    //cout << newID << " " << newdb.dirName << endl;
    writeDir(newdb, newID);
    //cout << "1.." << endl;
    if (db1.sonDirID == -1) {
        db1.sonDirID = newID;
        //cout << "2.." << endl;
        writeDir (db1, id1);
    }
    else {
        //cout << "3.." << endl;
        int dirID = db1.sonDirID; db1 = readDir (dirID);
        while (db1.nextDirID != -1) {
            dirID = db1.nextDirID;
            db1 = readDir(dirID);
        }
        db1.nextDirID = newID;
        writeDir(db1, dirID);
    }
    //cout << "4.." <<endl;
    if (db2.sonDirID == -1) return ;
    id2 = db2.sonDirID; db2 = readDir(id2);
    while (id2 != -1 && db2.used) {     //把目标文件夹下的东西都拷贝到新的文件夹下
        cpAllDir(newID, id2);
        id2 = db2.nextDirID;
        if (id2 == -1) break;
        db2 = readDir(id2);
    }
}

bool cpDir (string fromPath, string toPath){        //复制目录
    int id1 = getDirID(fromPath), id2 = getDirID(toPath);
    //cout << id1 << " " << id2 << endl;
    dirBlock db, db1 = readDir(id1), db2 = readDir (id2);
    if (db2.type != 1) return false;
    if (findNextDir(id2, (string)db1.dirName, 1) != -1 || findNextDir(id2, (string)db1.dirName, 2) != -1) {   //文件名冲突
        return false;
    }
    cpAllDir (id2, id1);
    return true;
}






