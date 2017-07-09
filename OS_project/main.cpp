#include "common.h"
#include "user.h"
#include "dir.h"
#include "file.h"
#include "index.h"

void init () { //重新初始化磁盘块
	//磁盘文件名
	//写入超级节点块
	superNodeBlcok sn;
	sn.root = 0;
	sn.emptyUserBlock = 1; 
	sn.emptyDirBlcok = 1; sn.emptyDirBlcok = DIRSIZE-1;
	sn.emptyFileBlock = 0; sn.emptyFileBLock = FILESIZE-1;
	sn.emptyIndexBlcok = 0; sn.emptyIndexBlcok = INDEXSIZE-1;
	ofstream fout (disk.c_str (), std::ios::binary);
	fout.write ((char *)&sn, sizeof sn);
	//写入用户块
	userBlock ub;
	strcpy (ub.userName, "admin");
	strcpy (ub.userPassword, "000000");
	ub.userMod = 0;
	ub.used = 1;
	fout.write ((char *)&ub, sizeof ub);
	ub.used = 0;
	for (int i = 1; i < USERSIZE; i++) {
		fout.write ((char *)&ub, sizeof ub);
	}
	//写入目录块
	dirBlcok db;
	strcpy (db.dirName, "root");
	strcpy (db.dirOwner, "admin");
	db.type = 1;
	db.faDirID = -1;
	db.used = true;
	fout.write ((char *)&db, sizeof db);
	for (int i = 1; i < DIRSIZE; i++) {
		if (db != DIRSIZE-1) db.nextDirID = i+1;
		else db.nextDirID = -1;
		fout.write ((char *)&db, sizeof db);
	}
	//写入文件块
	fileBlock fb;
	for (int i = 0; i < FILESIZE; i++) {
		if (fb != FILESIZE-1) fb.nextFileBlock = i+1;
		else fb.nextFileBlock = -1;
		fout.write ((char *)&fb, sizeof fb);
	}
	//写入索引块
	indexBlcok ib;
	for (int i = 0; i < INDEXSIZE; i++) {
		if (ib != INDEXSIZE-1) ib.nextIndexBlcok = i+1;
		else ib.nextIndexBlock = -1;
		fout.write ((char *)&ib, sizeof ib);
	}
	fout.close ();
	return ;
}

void load () {			//载入磁盘文件
	ifstream fin (disk.c_str ());
		fin.close ();
	if (fin.is_open ()) {
		return ;
	}
	else {
		init ();		//磁盘块丢失，重新初始化
	}
}

void bash () {			//命令行模式操作文件系统
	string op1, op2, op3;
	showCurPath (0);	//命令行显示当前路径用户
	while (cin >> op1) {
		if (op1 == "ls") {
			showAllSonDir ();	//显示当前路径下所有子目录
		}
		else if (op1 == "mkdir") { 			//创建单级子目录
			string newDirName;
			cin >> newDirName;	//想新创建的目录名
			char ch; string newDirMod; //新文件的权限输入
			while ((ch = getchar ()) != '\n') {
				if (ch == '-') {
					cin >> newDirMod;
				}
			}
			mkdir (newDirName, newDirMod);	//在当前目录下创建子目录
		}
		else if (op1 == "mkdirs") {			//创建多级子目录
			string newDirPath;
			cin >> newDirPath;	//想创建的多级子目录路径
			char ch; string newDirMod; //新文件的权限输入
			while ((ch = getchar ()) != '\n') {
				if (ch == '-') {
					cin >> newDirMod;
				}
			}
			mkdirs (newDirPath, newDirMod);	////在当前目录下创建多级子目录
		}
		else if (op1 == "cd") {
			char ch = getchar ();
			if (ch == '\n') {		//跳转到根目录
			}
			string tarPath; cin >> tarPath;
			if (tarPath == "..") {	//跳转到上一级目录
			}
			else {
				if (!gotoDir (tarPath)) {	//跳转到目标目录
					cout << "目标路径不存在" << endl;
					goto out;
				}
			}
		}
		else if (op1 == "pwd") {
			showCurPath (1);
		}
		else if (op1 == "touch") {
			string fileName; cin >> fileName;
			touch (fileName);
		}
		else if (op1 == "rm") {} 
		else if (op1 == "cp") {}
		else if (op1 == "move") {}
		else if (op1 == "find") {}
		else if (op1 == "state") {}
		else if (op1 == "rename") {}
		else if (op1 == "cat") {}
		else if (op1 == "chuser") {}
		else if (op1 == "createuser") {}
		else if (op1 == "open") {
			string fileNmae; cin >> fileName;
			int id = openFile (fileaName);		//打开文件的目录块 返回对应的文件内容块编号
			if (id == -1) {
				cout << "文件名有误！" << endl;
				goto out;
			}
			vim (id);		//对文件内容进行编辑
		}
		else {
			cout << "输入的指令有误！" << endl;
			goto out;
		}
	}
	out: ;
	showCurPath (0);
}


int main () {
	load ();			//载入磁盘文件
	if (!userLogin ()) {//用户登录
		system ("CLS");
		cout << "登录失败-。-,请重新登录" << endl;
		exit (0);
	}
	bash ();			//命令行模式操作文件系统
    return 0;
}









//test by morejarphone
