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
	db = readDir (curDirID); db = readDir (db.sonDirID);
	while (db.nextDirID != -1) {
		if ((string)db.dirName == fileaName && db.type == 2) {
			indexBlock ib = readIndex (db.textLcation); 	//读取文件内容所在的索引块
			return ib.offsetID;		//返回文件内容所在的偏移
		}
	}
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

