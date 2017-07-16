#include "file.h"
#include "common.h"
#include "dir.h"
#include "user.h"
//#include "vim.h"
#include "filestruct.h"
void init () { //重新初始化磁盘块
	//磁盘文件名//
	//写入超级节点块
	superNodeBlock sn;
	sn.root = 0;
	sn.emptyUserBlock = 1;
	sn.emptyDirBlock = 1; sn._emptyDirBlock = DIRSIZE-1;
	sn.emptyFileBlock = 0; sn._emptyFileBlock = FILESIZE-1;
	sn.emptyIndexBlock = 0; sn._emptyIndexBlock = INDEXSIZE-1;
	ofstream fout (disk.c_str (), std::ios::binary);
	fout.write ((char *)&sn, sizeof sn);
	//cout << sizeof (superNodeBlock) << " " << userSegOffset << endl; while (1) {}
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
	//cout << dirSegOffset << " " << sizeof(superNodeBlock)+sizeof(userBlock)*USERSIZE << endl; while (1) {}
	//ub = readUser (0); cout << ub.userName << " " << ub.userPassword << endl; while (1) {}
	//写入目录块
	dirBlock db;
	strcpy (db.dirName, "root");
	strcpy (db.dirOwner, "admin");
	db.type = 1;
	db.faDirID = -1;
	db.sonDirID = -1;
	db.nextDirID = -1;
	db.dirMod = 2;
	db.used = true;
	fout.write ((char *)&db, sizeof db);
//	cout << ub.userName << endl;
//	cout <<ub.userPassword << endl;
//	while (1) {}
	db.used = false;
	for (int i = 1; i < DIRSIZE; i++) {
		if (i != DIRSIZE-1) db.nextDirID = i+1;
		else db.nextDirID = -1;
		fout.write ((char *)&db, sizeof db);
	}
	//写入文件块
	fileBlock fb;
	Clear (fb.text, 0);
	for (int i = 0; i < FILESIZE; i++) {
		if (i != FILESIZE-1) fb.nextFileID = i+1;
		else fb.nextFileID = -1;
		fout.write ((char *)&fb, sizeof fb);
	}
	//写入索引块
	indexBlock ib;
	for (int i = 0; i < INDEXSIZE; i++) {
		if (i != INDEXSIZE-1) ib.nextIndexID = i+1;
		else ib.nextIndexID = -1;
		fout.write ((char *)&ib, sizeof ib);
	}
	fout.close ();
	curDirID = 0;
	curPath.clear (); curPath.pb ("root");
	return ;
}

void load () {			//载入磁盘文件
    cout << disk << endl;
	ifstream fin (disk.c_str ());
	if (fin.is_open ()) {
        curDirID = 0;
        curPath.clear (); curPath.pb ("root");
        fin.close ();
		return ;
	}
	else {
        cout << "init" << endl;
		init ();		//磁盘块丢失，重新初始化
	}
}

void bash () {			//命令行模式操作文件系统
    //cout << curUserID << ".." << endl;
	string op1, op2, op3;
	showCurPath (0, curPath);	//命令行显示当前路径用户
	while (cin >> op1) {
		if (op1 == "ls") {
            //cout << "ls" << endl;
			showAllSonDir ();	//显示当前路径下所有子目录
		}
		else if (op1 == "mkdir") { 			//创建单级子目录
			string newDirName;
			cin >> newDirName;	//想新创建的目录名
			char ch; string newDirMod = "a"; //新文件的权限输入
			while ((ch = getchar ()) != '\n') {
				if (ch == '-') {
					cin >> newDirMod;
				}
			}
			mkDir (newDirName, newDirMod);	//在当前目录下创建子目录
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
			mkdirs (newDirPath, newDirMod);	//在当前目录下创建多级子目录
		}
		else if (op1 == "cd") {
			char ch = getchar ();
			if (ch == '\n') {		//跳转到根目录
				curPath.clear ();
				curPath.pb ("root");
				curDirID = 0;
				goto out;
			}
			string tarPath; cin >> tarPath;
			if (tarPath == "..") {	//跳转到上一级目录
				if (!gotoFaDir ()) {
					cout << "already in root!" << endl;
				}
			}
			else {
				if (!gotoDir (tarPath)) {	//跳转到目标目录
					cout << "no such path!" << endl;
					goto out;
				}
			}
		}
		else if (op1 == "pwd") {
			showCurPath (1, curPath);
		}
		else if (op1 == "touch") {
			string fileName; cin >> fileName;
			string newDirMod = "a"; char ch;
			ch = getchar ();
			if (ch == ' ') {
                getchar ();
                cin >> newDirMod;
			}
			//cout << fileName << " " << newDirMod << endl;
			touch (fileName, newDirMod);
		}
		else if (op1 == "rm") {
			string tmp, path; cin >> tmp;
			if (tmp[0] == '-') {
				cin >> path;
				if (tmp != "-rf") {
					cout << "parameter error" << endl;
					goto out;
				}
			}
			else {
				path = tmp;
			}
			delDir (curDirID, path, tmp == "-rf" ? 0 : 1);
		}
		else if (op1 == "cp") {
			string fromPath, toPath;
			cin >> fromPath >> toPath;
		}
		else if (op1 == "move") {
			string fromPath, toPath;
			cin >> fromPath >> toPath;
		}
		else if (op1 == "find") {
			string target; cin >> target;
			find (curDirID, target, curPath);
		}
		else if (op1 == "state") {
			state ();
		}
		else if (op1 == "rename") {
			string oldFilname,newFilename;
                        cin >>oldFilname>>newFilename;
                        rename(oldFilname,newFilename);
		}
		else if (op1 == "cat") {
			string filename; cin >> filename;
			cat (filename);
		}
		else if (op1 == "chuser") {
			cout << "\n\n" << endl;
			string name, passwd;
			cout << "        input username:"; cin >> name;
			cout << "        input password:"; cin >> passwd;
			if (!chuser (name, passwd))
				cout << "can't change user" << endl;
		}
		else if (op1 == "createuser") {
			string newUserMod = "-a";
			char ch; if ((ch = getchar ()) == ' ') cin >> newUserMod;
			int userMod = (newUserMod[0] == 'r' ?
				(newUserMod == "r" ? 3 : 2) :
				(newUserMod == "p" ? 0 : 1));
			userBlock ub = readUser (curUserID);
			if (userMod < ub.userMod) {
				cout << "Mod fault" << endl;
			}
			else {
				cout << "\n\n" << endl;
				string name, passwd;
				cout << "        input username:"; cin >> name;
				cout << "        input password:"; cin >> passwd;
				if (!createuser (name, passwd, userMod)) {
					cout << "can't crate user!" << endl;
				}
			}
		}
		else if (op1 == "open") {
			string fileName; cin >> fileName;
			int id = openFile (fileName);		//打开文件的目录块 返回对应的文件内容块编号
			if (id == -1) {
				cout << "filename error!" << endl;
				goto out;
			}
			//runVim(id);		//对文件内容进行编辑
		}
		else if (op1 == "exit") {
            return ;
		}
		else if (op1 == "link") {

		}
		else if (op1 == "formet") {
            init ();
		}
		else {
			cout << "instruction error!" << endl;
			goto out;
		}
		out: ;
		showCurPath (0, curPath);
	}
}


int main () {
	load ();			//载入磁盘文件
	if (!userLogin ()) {//用户登录
	    cout << "userID2" << curUserID << endl;
		system ("CLS");
		cout << "fail to longin!" << endl;
		exit (0);
	}
	bash ();			//命令行模式操作文件系统
    return 0;
}









//test by morejarphone
