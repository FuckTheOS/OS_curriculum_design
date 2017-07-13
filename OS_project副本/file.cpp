#include "file.h"
#include "dir.h"
#include "common.h"
int openFile (string fileaName){ 			//���ļ���Ŀ¼�� ���ض�Ӧ���ļ����ݿ���
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

void vim (int id){				//���ļ����ݽ��б༭
	fileBlock fb = readFile (id);
	string buffer = (string)fb.text;
	system ("CLS");
	cout << "---edit---" << endl;
	cout << buffer << endl;
	char ch, op;
	while (true) {
		//��getchʵ�ֵ�С��vim ������
	}
	writeFile (fb, id);
}
//������ʾ�ļ����ݿ������

int  giveFileBlock(){           //�����ļ���
    superNodeBlock sn = readSuperNode ();
    if (sn.emptyFileBlock == -1) {		//�ļ���ռ䲻��
		return -1;
	}
	int res = sn.emptyFileBlock;
	if (sn.emptyFileBlock == sn._emptyFileBlock) {	//�ļ���պ�����
		sn.emptyFileBlock = -1;
		sn._emptyFileBlock = -1;
	}
	else {
		fileBlock fb = readFile(sn.emptyFileBlock);	//��ȡ���ļ���Ϣ
		sn.emptyFileBlock = fb.nextFileID;			//�ÿ����һ����Ϊ���ļ�����׿�
	}
	writeSuperNode (sn);
	return res;
 }

bool touch (string fileName,string newDirMod){           //��ǰĿ¼���½��ļ�
    if (!checkMod (curUserID, curDirID, 2)) {	//Ȩ�޼��ûͨ��
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
	if (!checkDirName (fileName)) { 	//�ļ������ûͨ��
		cout << "filename error!" << endl;
		return false;
	}
	int dirID = giveDirBlock ();	//�����µ�Ŀ¼��
	int indexID = giveIndexBlock ();//�����µ�������
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
		while (db.nextDirID != -1) {		//�ҵ���ǰ·�������һ��Ŀ¼
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
	writeDir (db, dirID);	//��Ŀ¼��Ϣд��Ŀ¼��
	return true;
}

void cat (string filename){		//�����ǰĿ¼�µ��ļ�����
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

