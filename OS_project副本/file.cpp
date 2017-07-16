#include "file.h"
#include "dir.h"
#include "common.h"
#include "filestruct.h"
int openFile (string fileaName){ 			//打开文件的目录块 返回对应的文件内容块编号
	dirBlock db;
	db = readDir (curDirID);
	if (db.sonDirID == -1)
		return -1;
	db = readDir (db.sonDirID);
	while (!((string)db.dirName == fileaName && db.type == 2)) {
        if (db.nextDirID == -1) return -1;
		db = readDir (db.nextDirID);
        if (!db.used) return -1;
	}
	if ((string)db.dirName == fileaName && db.type == 2) {
		indexBlock ib = readIndex (db.textLocation);
		return ib.diskOffset;
	}
	else
		return -1;
}

void vim (int id){				//对文件内容进行编辑
	fileBlock fb = readFile (id);
	string buffer = (string)fb.text;
	system ("CLS");
	cout << "---edit---" << endl;
	cout << buffer << endl;
	char ch, op;
	while (true) {
		//用getch实现的小型vim 待完善
	}
	writeFile (fb, id);
}
//参数表示文件内容块的索引


bool touch (string fileName,string newDirMod){           //当前目录下新建文件
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
	if (!checkDirName (fileName)) { 	//文件名检查没通过
		cout << "filename error!" << endl;
		return false;
	}
	int dirID = giveDirBlock ();	//分配新的目录块
	int indexID = giveIndexBlock ();//分配新的索引块
	int fileID = giveFileBlock();
	if (dirID == -1 || indexID == -1||fileID == -1) {
		cout << "no more disk space!" << endl;
		return false;
	}

	fileBlock fb = readFile (fileID);
	fb.used = true;
	fb.nextFileID = -1;
	Clear (fb.text, 0);
	writeFile(fb,fileID);

	indexBlock ib = readIndex (indexID);
	ib.used = 1;
	ib.diskOffset = fileID;
	writeIndex (ib, indexID);

	dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1) {
		db.sonDirID = dirID;
		writeDir (db, curDirID);
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
	strcpy (db.dirName, fileName.c_str ());
	strcpy (db.dirOwner, ub.userName);
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

void cat (string filename){		//输出当前目录下的文件内容
	int dirID = findNextDir (curDirID, filename, 2);
	if (!checkMod (curUserID, dirID, 1)) {
		cout << "Mod fault!" << endl;
		return ;
	}
	dirBlock db = readDir (dirID);
	indexBlock ib = readIndex (db.textLocation);
	fileBlock fb = readFile (ib.diskOffset);
	cout << fb.text << endl;
}
bool rename(string oldFilename,string newFilename){
    dirBlock db = readDir (curDirID);
	if (db.sonDirID == -1){
        cout<<"can't find this filename!"<<endl;
        return false;
	}
    int tmp = db.sonDirID;
	db = readDir (db.sonDirID);
	if (strcmp(db.dirName,oldFilename.c_str ())==0){
        if (!checkDirName (newFilename)) { 	//文件名检查没通过
            cout << "newfilename exisit!" << endl;
		    return false;
	    }
	    strcpy(db.dirName,newFilename.c_str ());
	    writeDir(db,tmp);
        return true;
    }
	while (db.nextDirID != -1) {
        tmp =db.nextDirID;
		db = readDir (db.nextDirID);
		if (strcmp(db.dirName,oldFilename.c_str ())==0){
            if (!checkDirName (newFilename)) { 	//文件名检查没通过
                cout << "newfilename exisit!" << endl;
		        return false;
	        }
		   strcpy(db.dirName,newFilename.c_str ());
		   writeDir(db,tmp);
		   return true;
		}

	}
	cout<<"can't find this filename!"<<endl;
	return false;
}

