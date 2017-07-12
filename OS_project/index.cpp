#include "index.h"

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

void releaseIndex (int indexID){		//�ͷ�������
    superNodeBlock sn = readSuperNode ();
    indexBlock ib;
    ib.used = 0;
    ib.nextIndexID = -1;
    if (sn.emptyIndexBlock = -1){
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
