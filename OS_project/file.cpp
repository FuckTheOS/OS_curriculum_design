#include "common.h"
#include "file.h"

bool checkMod (int modType) {	//用户在当前路径下对欲使用此权限
	//用户和当前路径直接读取全局变量
	//如果可以使用此权限返回true 否则flase
	return true;
}

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
		if ((string)db.dirName == fileaName) {}
	}
	return -1;
}
//参数表示当前路径下的文件名
void vim (int id);				//对文件内容进行编辑
//参数表示文件内容块的索引

