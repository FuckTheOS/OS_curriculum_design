#include "common.h"
#include "filestruct.h"
vector <string> curPath;//��ǰ�ľ���·��
int curUserID;			//��ǰ�û�
int curDirID;			//��ǰĿ¼��ID
string disk = "disk";	//���̿��ļ���

long long getTime (){               //��ȡ��ǰ��ʱ��
    SYSTEMTIME sys;
    GetLocalTime( &sys );
    long long year, month, day, hour, minute;
    year = sys.wYear, month = sys.wMonth, day = sys.wDay, hour = sys.wHour;
    minute = sys.wMinute;
    long long ans = year*100000000LL+month*1000000LL+day*10000LL+hour*100+minute;
    return ans;
}
//����������ʱ�� ����*100000000+��*1000000+��*10000+ʱ*100+��
void printTime (long long num){     //����ʱ��ֵ��ӡʱ�䴮
    cout << num/100000000LL << "-"; num %= 100000000LL;
    cout << num/1000000LL << "-"; num %= 1000000LL;
    cout << num/10000LL << " "; num %= 10000LL;
    printf ("%02d:", (int)num/100);
    printf ("%02d\n", (int)num%100);
}




superNodeBlock readSuperNode (){    //���볬���ڵ���Ϣ
    superNodeBlock sn;
    ifstream fin (disk.c_str (), std::ios::binary);
    fin.seekg (superNodeSegOffset, ios::beg);
    fin.read ((char *)&sn, sizeof sn);
    fin.close ();
    return sn;
}

void writeSuperNode (superNodeBlock sn) {   //д�볬���ڵ���Ϣ
    ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
    fout.seekp (superNodeSegOffset, ios::beg);
    fout.write ((char *)&sn, sizeof sn);
    fout.close ();
}

#define PATH_PRASER_ERROR  path.clear();path.push_back("error");break;
void showCurPath (int type, vector <string> curPath) {	//�����ǰ·��
    if (type == 0) {
        cout << "root>";
        for (int i = 1; i < curPath.size (); i++) {
            cout << curPath[i] << (i == curPath.size ()-1 ? ' ' : '/');
        }
        userBlock ub = readUser (curUserID);
        cout << " " << ub.userName << "$ ";
    }
    else if (type == 1) {
        for (int i = 0; i < curPath.size (); i++) {
            cout << curPath[i] << '/';
        }
        cout << endl;
    }
}

void pathPrase(string tarPath, vector <string>& path) {	//���Զ�������·��
	path.clear();
	size_t x_point = 0; //δ������tarPath���ַ��±�
	if (tarPath[0] == '/') //����·��
	{
		path.push_back("/root");
		x_point = 1;
	}
	else
		path.push_back("/CUR");
	while (x_point != tarPath.npos)
	{
		size_t tmp_pos = tarPath.find_first_of('/', x_point);
		if (tmp_pos == x_point) // �������ۺŶѵ���һ�� ���Ƿ�
		{
			PATH_PRASER_ERROR;
		}
		string tmpString = tarPath.substr(x_point, (tmp_pos != tarPath.npos) ? tmp_pos - x_point : tmp_pos);
		if (tmpString == "*") //ͨ�������ĩβʱ���Ƿ�
			if (tmp_pos == tarPath.npos) tmpString = "/TOT";
			else
			{
				PATH_PRASER_ERROR;
			}
		if (tmpString == ".") tmpString = "/CUR";//��ǰĿ¼
		if (tmpString == "..") tmpString = "/BACK";//�����ϼ�Ŀ¼
		if (tmpString == "null")
			if (path.size() != 1 || tmp_pos != tarPath.npos) { PATH_PRASER_ERROR; }
			else break;
			path.push_back(tmpString);
			if (tmp_pos == tarPath.npos) break;
			x_point = tmp_pos + 1;
	}
}

bool visitPath(dirBlock& cur, string target, int& curID)
{
	//Ȩ�޼�������
	//indexBlock ib;
	if (target == "/CUR") return true;
	if (target == "/root")
	{
		cur = readDir(0);
		curID = 0;
		return true;
	}
	if (target == "/BACK")
	{
		if (cur.faDirID <= 0) return false;//�ѵ���Ŀ¼���޷�����
		else
		{
			curID = cur.faDirID;
			cur = readDir(curID);
		}
		return true;
	}
	curID = cur.sonDirID;
	if (curID < 0) return false;
	cur = readDir(curID);
	while (strcmp(cur.dirName, target.c_str()) != 0)
	{
		if (cur.nextDirID == -1) return false; //��ǰĿ¼�²��Ҳ���ָ����Ŀ¼
		curID = cur.nextDirID;
		cur = readDir(curID);
	}
	return true;
}

int findNextDir (int dirID, string target, int dirType){     //����dirID�µ�targetĿ¼ Ŀ¼����Ϊ1
    dirBlock db = readDir (dirID), tmp;
    if (db.sonDirID == -1) return -1;
    tmp = readDir (db.sonDirID);
    if (target == (string)tmp.dirName && tmp.type == dirType) {
        return db.sonDirID;
    }
    db = tmp;
    while (db.nextDirID != -1) {
        tmp = readDir (db.nextDirID);
        if (target == (string)tmp.dirName && tmp.type == dirType) {
            return db.nextDirID;
        }
        else
            db = tmp;
    }
    return -1;
}
//��������¼�Ŀ¼��ID �����ڷ���-1

bool checkMod (int userID, int dirID, int type){    //Ȩ���ж�
    //cout << userID << " " << dirID << " " << type << endl;
    userBlock ub = readUser (userID);
    dirBlock db = readDir (dirID);
    int utype = ub.userMod, dtype = db.dirMod;
    if (type == 1 || (type == 2 && utype < 3) || (type == 3 && utype < 2)) { //���������û�Ȩ��
        if ((type == 1 && dtype) || (type == 2 && dtype > 1) || (type == 3 && dtype > 2))
            return true;    //���������ļ�Ȩ��
        else
            return false;
    }
    else
        return false;
}

void find (int curDirID, string target, vector <string> path){  //�ӵ�ǰ·��������Ŀ���ļ���ע�����ļ���
    //ֱ�ӵݹ�����
    dirBlock db = readDir (curDirID);
    cout << db.dirName << " " << target << " " << db.type << endl;
    if ((string)db.dirName == target && db.type == 2) {
        cout << "fuck" <<endl;
        for (int i = 1; i < path.size (); i++) {
            cout << path[i] << '/';
        }
        cout << target << endl;
        return ;
    }
    path.pb ((string)db.dirName);
    cout << db.dirName << " " << db.sonDirID << endl;
    if (db.sonDirID == -1) {}
    else {
        find (db.sonDirID, target, path);
    }
    path.pop_back ();
    cout << db.dirName << " " << db.nextDirID << " " << db.used << endl;
    if (db.used && db.nextDirID != -1) {
        find (db.nextDirID, target, path);
    }
}
//������ʾĿ���ļ����ļ���
//������п��ܵĽ��·��

void state (){                      //��ʾ�ڴ�ʹ�����
    superNodeBlock sn = readSuperNode ();
    int cnt, p;
    p = sn.emptyUserBlock;
    if (p == -1) p = USERSIZE+1;
    printf ("%d user blocks empty, unilization ratio %.2f\n", USERSIZE-p, (USERSIZE-p+1)*1.0/USERSIZE);

    cnt = 0, p = sn.emptyDirBlock;
    dirBlock db;
    while (p != -1) {
        cnt++;
        db = readDir (p);
        p = db.nextDirID;
    }
    printf ("%d dir blocks empty, unilization ratio %.2f\n", cnt, cnt*1.0/DIRSIZE);

    cnt = 0, p = sn.emptyFileBlock;
    fileBlock fb;
    while (p != -1) {
        cnt++;
        fb = readFile (p);
        p = fb.nextFileID;
    }
    printf ("%d file blocks empty, unilization ratio %.2f\n", cnt, cnt*1.0/FILESIZE);

    cnt = 0, p = sn.emptyIndexBlock;
    indexBlock ib;
    while (p != -1) {
        cnt++;
        ib = readIndex (p);
        p = ib.nextIndexID;
    }
    printf ("%d index blocks empty, unilization ratio %.2f\n", cnt, cnt*1.0/INDEXSIZE);
}
//����û��� Ŀ¼�� �ļ��� ������ʣ�µĿ�����ʹ����

userBlock readUser (int id){				//�����û���id��ȡ�û�����Ϣ
	userBlock ub;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (userSegOffset+sizeof (ub)*id, ios::beg);
	fin.read ((char *)&ub, sizeof ub);
	fin.close ();
	return ub;
}

void writeUser (userBlock ub, int id){  	//���û�����Ϣд���û���
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (userSegOffset+sizeof (ub)*id, ios::beg);
	fout.write ((char *)&ub, sizeof ub);
	fout.close ();
}
//���̣�ע�Ᵽ��ԭ������
dirBlock readDir (int id) {				//�����ļ���id��ȡ�ļ�����Ϣ
	if (id >= DIRSIZE || id < 0) {
		cout << "segment fault!" << endl;
		exit (0);
	}
	dirBlock db;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (dirSegOffset+sizeof (db)*id, ios::beg); 	//��λ��Ŀ��ƫ��
	fin.read ((char *)&db, sizeof db);
	fin.close ();
	return db;
}

void writeDir (dirBlock db, int id){	//��Ŀ¼����Ϣд��Ŀ¼��
	if (id >= DIRSIZE || id < 0) {
		cout << "segment fault!" << endl;
		exit (0);
	}
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (dirSegOffset+sizeof (db)*id, ios::beg);
	fout.write ((char *)&db, sizeof db);
	fout.close ();
}

fileBlock readFile (int id){	//�����ļ���id��ȡ�ļ�����
	fileBlock fb;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (fileSegOffset+sizeof (fb)*id, ios::beg);
	fin.read ((char *)&fb, sizeof fb);
	fin.close ();
	return fb;
}

void writeFile (fileBlock fb, int id){  	//���ļ�����д���ļ���
    if (id >= FILESIZE || id < 0) {
        cout <<"segment fault!" << endl;
        exit (0);
    }
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (fileSegOffset+sizeof (fb)*id, ios::beg);
	fout.write ((char *)&fb, sizeof fb);
	fout.close ();
}

indexBlock readIndex (int id){			//����������id��ȡ��������Ϣ
	indexBlock ib;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (indexSegOffset+sizeof (ib)*id, ios::beg);
	fin.read ((char *)&ib, sizeof ib);
	fin.close ();
	return ib;
}

void writeIndex (indexBlock ib, int id){ //����������Ϣд��������
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (indexSegOffset+sizeof (ib)*id, ios::beg);
	fout.write ((char *)&ib, sizeof ib);
	fout.close ();
}


int giveIndexBlock (){                       //�����µ�������
    superNodeBlock sn = readSuperNode ();
    if (sn.emptyIndexBlock == -1) {		//������ռ䲻��
		return -1;
	}
	int res = sn.emptyIndexBlock;
	if (sn.emptyIndexBlock == sn._emptyIndexBlock) {	//������պ�����
		sn.emptyIndexBlock = -1;
		sn._emptyIndexBlock = -1;
	}
	else {
		indexBlock ib = readIndex(sn.emptyIndexBlock);	//��ȡ��Ŀ������Ϣ
		sn.emptyIndexBlock = ib.nextIndexID;			//�ÿ����һ����Ϊ����������׿�
	}
	writeSuperNode (sn);
	return res;
 }

void releaseIndex (int indexID){		//�ͷ�������
    superNodeBlock sn = readSuperNode ();
    indexBlock ib;
    ib.used = 0;
    ib.nextIndexID = -1;
    if (sn.emptyIndexBlock == -1){
        sn.emptyIndexBlock = indexID;
        sn._emptyIndexBlock = indexID;
    }
    else{
        indexBlock ib1 = readIndex(sn._emptyIndexBlock);
        ib1.nextIndexID = indexID;
        writeIndex(ib1,sn._emptyIndexBlock);
        sn._emptyIndexBlock = indexID;
    }
    writeSuperNode(sn);
    writeIndex(ib,indexID);
}

void releaseFile(int fileID){              //�ͷ��ļ���
     superNodeBlock sn = readSuperNode();
     fileBlock fb = readFile(fileID);
     memset(fb.text,0,sizeof(fb.text));
     fb.used = 0;
     fb.nextFileID = -1;
     if(sn.emptyFileBlock == -1){
         sn.emptyFileBlock = fileID;
         sn._emptyFileBlock = fileID;
    }
     else{
         fileBlock fb1 = readFile(sn._emptyFileBlock);
         fb1.nextFileID = fileID;
         writeFile(fb1,sn._emptyFileBlock);
         sn._emptyFileBlock = fileID;
    }
    writeSuperNode(sn);
    writeFile (fb,fileID);
}
