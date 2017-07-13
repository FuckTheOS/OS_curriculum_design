#include "dir.h"
#include "user.h"
#include "common.h"
using namespace std;
int giveDirBlock() {					//�����µ�Ŀ¼��
	superNodeBlock sn = readSuperNode();
	if (sn.emptyDirBlock == -1) {		//Ŀ¼��ռ䲻��
		return -1;
	}
	int res = sn.emptyDirBlock;
	if (sn.emptyDirBlock == sn._emptyDirBlock) {	//Ŀ¼��պ�����
		sn.emptyDirBlock = -1;
		sn._emptyDirBlock = -1;
	}
	else {
		dirBlock db = readDir(sn.emptyDirBlock);	//��ȡ��Ŀ¼����Ϣ
		sn.emptyDirBlock = db.nextDirID;			//�ÿ����һ����Ϊ��Ŀ¼����׿�
	}
	writeSuperNode(sn);
	return res;
}
//������ʾĿ¼������
//�������ɹ�����Ŀ¼��ID ���򷵻�-1

bool checkDirName(string newDirName, int dirType) {	//���Ŀ¼���Ƿ�͵�ǰ����Ŀ¼��ͻ
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
//������ʾϣ���½����ļ���
//�������ͻ����1 ���򷵻�0

void showAllSonDir() {	//��ʾ��ǰ·����������Ŀ¼
	dirBlock db = readDir(curDirID);
	if (db.sonDirID == -1 || !db.used) { 			//��Ŀ¼
		cout << endl;
		return;
	}
	db = readDir(db.sonDirID);
	cout << db.dirName << " ";
	while (db.nextDirID != -1 || !db.used) {
		db = readDir(db.nextDirID);
		cout << db.dirName << " ";
	}
	cout << endl;
}
//��ǰ·��ֱ����ȫ�ֱ���
//����a b.txt c d.cpp �ĸ�ʽ���

bool mkDir(string newDirName, string newDirMod, int _curDirID) {	//�ڵ�ǰĿ¼�´�����Ŀ¼
	if (_curDirID == -1) _curDirID = curDirID;
	if (!checkMod(curUserID, curDirID, 2)) {	//Ȩ�޼��ûͨ��
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
	if (!checkDirName(newDirName, 1)) { 	//Ŀ¼�����ûͨ��
		cout << "filename error!" << endl;
		return false;
	}
	int dirID = giveDirBlock();	//�����µ�Ŀ¼��
								//int indexID = giveIndexBlock ();//�����µ�������
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
		while (db.nextDirID != -1) {		//�ҵ���ǰ·�������һ��Ŀ¼
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
	writeDir(db, dirID);	//��Ŀ¼��Ϣд��Ŀ¼��
	return true;
}

bool mkdirs(string newDirPath, string newDirMod) {	//�ڵ�ǰĿ¼�´����༶��Ŀ¼
	vector <string> tarDirPath;
	pathPrase(newDirPath, tarDirPath);	//���Զ�������·��
	int dirID = (tarDirPath[0] == "/root" ? 0 : curDirID);
	for (int i = 0; i < tarDirPath.size(); i++) {
		string tmp = tarDirPath[i];
		if (tmp == "/TOT") {
			return 0;
		}
		if (i == 0 && tmp == "/root") continue;
		if (tmp == "/CUR") continue;
		int nextID = findNextDir(dirID, tarDirPath[i]);
		if (nextID == -1) {	//��������Ҫ�´���
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
//��Ի����·�� ��Ŀ¼��Ȩ��(�������ĩ��)
//·����һ��ԭʼ�� ��Ҫ·�������Զ�������
//�����ɹ�����1 ���򷵻�0

bool gotoDir(string tarPath) {			//��ת���µ�Ŀ¼
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
//��������Ի����·�� ��Ҫ·�������Զ�������·��
//��ת�ɹ�����1 ���򷵻�0

bool gotoFaDir() {						//��ת������Ŀ¼
	dirBlock db = readDir(curDirID);
	if (db.faDirID == -1) {	 	//�Ѿ��������ϼ�Ŀ¼
		return false;
	}
	curPath.pop_back();		//����·������ĩβ
	curDirID = db.faDirID;	//�޸ĵ�ǰĿ¼��id
	return true;
}

bool delDir(int dirID, string dirPath, int type) {	//ɾ��Ŀ¼��
	vector <string> path;
	pathPrase(dirPath, path);
	if (path[0] == "error")
		return false;
	for (int i = 0; i < path.size() - 1; i++) {
		if (path[i] == "/CUR")
			continue;
		dirID = (findNextDir(dirID, path[i]));
		if (dirID == -1) 		//û���ҵ�Ŀ��·��
			return false;
		if (!checkMod(curUserID, dirID, 2))
			return false;		//�û�Ȩ�޴���
	}
	string leaf = path[path.size() - 1];
	if (type == 0) {	//ɾ������Ŀ¼��
		dirID = findNextDir(dirID, leaf);
		if (dirID == -1)
			return -1;
		if (!checkMod(curUserID, dirID, 3))
			return false;
		if (!delAllDir(dirID))	//�ݹ�ɾ������Ŀ¼��
			return false;
	}
	else {
        //cout << "fuck" << endl;
		dirBlock db = readDir(dirID), tmp;
		if (db.sonDirID == -1)
			return false;
		//dirID = db.sonDirID;
		tmp = db;
		db = readDir(db.sonDirID);
		bool flag = 1;
		while ((string)db.dirName != leaf) {
			if (db.nextDirID == -1 || !db.used)
				return false;
            if (flag) dirID = tmp.sonDirID, flag = 0;
            else dirID = tmp.nextDirID;
			tmp = db;
			db = readDir(tmp.nextDirID);
			if ((string)db.dirName == leaf) {   //�ҵ���
			    if (!checkMod(curUserID, dirID, 2))
                    return false;
                int indexID = db.textLocation;
                indexBlock ib = readIndex(indexID);
                int fileID = ib.diskOffset;
                releaseIndex(indexID);
                releaseDir(tmp.nextDirID);
                releaseFile(fileID);
                tmp.nextDirID = -1;
                writeDir (tmp, dirID);
                return true;
			}
		}
       // cout << ".." << dirID << endl;
        //cout << tmp.dirName << endl;
		if (!checkMod(curUserID, dirID, 2)) {
            //cout << "//" << endl;
			return false;
		}
		//state ();
		int indexID = db.textLocation;
		indexBlock ib = readIndex(indexID);
		int fileID = ib.diskOffset;
		releaseIndex(indexID);
		releaseDir(tmp.sonDirID);
		releaseFile(fileID);

        //state ();
		tmp.sonDirID = -1;
		writeDir (tmp, dirID);
		//tmp = readDir (dirID); cout << tmp.dirName << " " << tmp.sonDirID << "[[" << endl;
	}
	return true;
}
//��ǰĿ¼�� ·���� ɾ����������Ŀ¼����ĳ���ļ�
//ɾ���ɹ�����1 ����(·�������ڻ���Ȩ�޴���)��0

bool delAllDir(int dirID) {		//�ݹ�ɾ������Ŀ¼��
	bool flag = true;
	dirBlock db = readDir(dirID);
	if (db.sonDirID == -1) {
		if (!checkMod(curUserID, dirID, 3))
			return false;
	}
	else {
		flag = delAllDir(db.sonDirID);
	}
	if (db.nextDirID != -1) {
		flag &= delAllDir(db.nextDirID);
	}
	return flag;
}

void releaseDir(int dirID) {		//�ͷ�һ��Ŀ¼��
	superNodeBlock sn = readSuperNode();
	dirBlock db = readDir(dirID);
	db.used = 0;
	if (sn.emptyDirBlock == -1) {
		sn.emptyDirBlock = dirID;
		sn._emptyDirBlock = dirID;
	}
	else {
		dirBlock tmp = readDir(sn._emptyDirBlock);
		db.nextDirID = dirID;
		writeDir(tmp, sn._emptyDirBlock);
		sn._emptyDirBlock = dirID;
	}
	writeSuperNode(sn);
	db.sonDirID = db.faDirID = -1;
	writeDir(db, dirID);
}

