#include "common.h"
#include "file.h"

fileBlock readFile (int id){	//根据文件块id读取文件内容
	fileBlcok fb;
	ifstream fin (dis.c_str (), std::ios::binary);
	fin.seekg (fileSegOffset+sizeof (fb)*id, ios::beg);
	fb = fin.read ((char *)&fb, sizeof fb);
	fin.close ();
	return fb;
}

void writeFile (fileBlock db, int id){  	//将文件内容写入文件块
	ofstream fout (dis.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (fileSegOffset+sizeof (fb)*id, ios::beg);
	fout.write ((char *)&fb, sizeof fb);
	fout.close ();
}

int openFile (string fileaName){ 			//打开文件的目录块 返回对应的文件内容块编号
	dirBlcok db;
	db = readDir (curDirID); 
	if (db.sonDirID == -1)
		return -1;
	db = readDir (db.sonDirID);
	while (!((string)db.dirName == fileaName && db.type == 2)) {
		db = readDir (db.nextDirID);
	}
	if ((string)db.dirName == fileaName && db.type == 2) {
		indexBlock ib = readIndex (db.sonDirID);
		return ib.diskOffset;
	}
	else 
		return -1;
}

void vim (int id){				//对文件内容进行编辑
	fileBlock fb = readFile (id);
	string buffer = (string)fb.text;
	system ("CLS");
	cout << "---修改模式---" << endl;
	cout << buffer << endl;
	char ch, op;
	while (true) {
		//用getch实现的小型vim 待完善
	}
	write (fb, id);
}
//参数表示文件内容块的索引

void releaseFile(int fileID){              //释放文件块
     superNodeBlock sn = readSuperNode();
     fileBlock fb = readFile(fileID);
     memset(fb.text,0,sizeof(fb.text));
     fb.used = 0;
     fb.nextFileID = -1;
     if(sn.emptyFileBlock = -1){
         sn.emptyFileBlock = fileID;
         sn._emptyFileBlock = fileID;
    }
     else{
         fileBlock fb1 = readFile(sn._emptyFileBlock);
         fb1.nextFileID = fileID;
         writeFile(ib1,sn._emptyIndexBlock);
         sn._emptyFileBlock = fileID;
    }
    writeSuperNode(sn);
    writeFile (fb,fileID);
}

int  giveFileBlock(){           //分配文件块
    superNodeBlock sn = readSuperNode ();
    if (sn.emptyFileBlock == -1) {		//索引块空间不足
		return -1;
	}
	int res = sn.emptyFileBlock;
	if (sn.emptyFileBlock == sn._emptyFileBlock) {	//索引块刚好用完
		sn.emptyFileBlock = -1;
		sn._emptyFileBlock = -1;
	}
	else {
		fileBlock fb = readFile(sn.emptyFileBlock);	//读取空目索引信息
		sn.emptyFileBlock = fb.nextFileID;			//该块的下一块作为空索引块的首块
	}
	return res;
 }

bool touch (string fileName,string newDirMod){           //当前目录下新建文件
    if (!checkMod (curUserID, curDirID, 2)) {	//权限检查没通过
		cout << "权限错误！" << endl;
		return false;
	}
	if (newDirMod == "p") {
		userBlcok ub = readUser (curUserID);
		if ((string)ub.userName != "admin") {
			cout << "权限错误！" << endl;
			return false;
		}
	} 
	if (!checkDirName (newFileName)) { 	//文件名检查没通过
		cout << "文件名错误！" << endl;
		return false;
	}
	int dirID = giveDirBlock ();	//分配新的目录块
	int indexID = giveIndexBlcok ();//分配新的索引块
	int fileID = giveFileBlock();
	if (dirID == -1 || indexID == -1||fileID == -1) {
		cout << "磁盘空间不足!" << endl;
		return false;
	}

	fileBlock fb = readFile (fileID);
	fb.used = true;
	fb.nextFileID = -1;
	writeFile(fb,fileID);

	indexBlock ib = readIndex (indexID);
	ib.used = 1;
	ib.diskOffset = fileID;
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
	userBlock ub = readUser (curUserID);
	strcpy (db.fileName, newFileName.c_str ());
	strcpy (db.dirOwner, ub.name);
	db.dirSize = 0;
	db.dirCreateTime = getTime ();
	db.dirChangeTime = db.dirCreateTime;
	db.type =2;
	db.textLocation =indexID;
	db.faDirID = curDirID;
	db.sonDirID = -1;
	db.nextDirID = -1;
	db.dirMod = (newDirMod[0] == 'r' ?
		(newDirMod == "rw" ? 2 : 1) :
		(newDirMod == "a" ? 3: 0));
	db.used = true;
	writeDir (db, dirID);	//将目录信息写入目录块
	return true;
}
