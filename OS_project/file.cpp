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
         writeIndex(fb1,sn._emptyFileBlock);
         sn._emptyFileBlock = fileID;
    }
    writeSuperNode(sn);
    writeFile (fb,fileID);
}
