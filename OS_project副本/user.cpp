#include "user.h"
#include "common.h"
#include "filestruct.h"
bool userLogin () {			//�տ�ʼʱ���û���¼
    string userName, passwd;
    cout << "        input username: "; cin >> userName;
    cout << "        input password: "; cin >> passwd;
    system ("CLS");
    userBlock ub;
	superNodeBlock sn = readSuperNode ();
	for (int i = 0; i < sn.emptyUserBlock; i++) {
		ub = readUser (i);
		if (userName == (string)ub.userName && passwd == (string)ub.userPassword) {  //�ҵ����û�
		    curUserID = i;
		    cout << "userID" << curUserID << endl;
		    return true;
		}
	}
	return 0;
}


bool chuser (string name, string passwd){	//�ڵ�ǰĿ¼���л��û�
	userBlock ub;
	for (int i = 0; i < USERSIZE; i++) {
		ub = readUser (i);
		if ((string)ub.userName == name && (string)ub.userPassword == passwd) {
			int dirID = curDirID;
			dirBlock db;
			curUserID = i;
			while (dirID != -1) {		//����׷�� �����ǹ��������Ŀ¼��ͻ
				if (dirID == 0)
					return true;
				if (!checkMod (i, dirID, 1))
					return false;
				db = readDir (db.faDirID);
			}
			return true;
		}
	}
	return false;
}
//������ʾ�û������û�����
//�ɹ�����1 ���򷵻�0

bool createuser (string name, string passwd, int userMod){	//�����û�
	userBlock ub;
	superNodeBlock sn = readSuperNode ();
	for (int i = 0; i < sn.emptyUserBlock; i++) {
		ub = readUser (i);
		if (name == (string)ub.userName) 	//�û����Ѿ�����
			return false;
	}
	ub = readUser (sn.emptyUserBlock);
	strcpy (ub.userName, name.c_str ());
	strcpy (ub.userPassword, passwd.c_str ());
	ub.userMod = userMod;
	ub.used = 1;
	writeUser (ub, sn.emptyUserBlock);
	sn.emptyUserBlock = (sn.emptyUserBlock+1 == USERSIZE ? -1 : sn.emptyUserBlock+1);
	writeSuperNode (sn);
	return true;
}
//������ʾ�û������û����� �������û���Ȩ��
//�ɹ�����1 ���򷵻�0



