#include "common.h"
#include "index.h"

int giveIndexBlcok (){                       //分配新的索引块
    superNodeBlock sn = readSuperNode ();
    if (sn.emptyIndexBlock == -1) {		//索引块空间不足
		return -1;
	}
	int res = sn.emptyIndexBlock;
	if (sn.emptyIndexBlock == sn._emptyIndexBlock) {	//索引块刚好用完
		sn.emptyIndexBlock = -1;
		sn._emptyIndexBlock = -1;
	}
	else {
		indexBlock ib = readIndex(sn.emptyIndexBlock);	//读取空目索引信息
		sn.emptyIndexBlock = ib.nextIndexID;			//该块的下一块作为空索引块的首块
	}
	return res;
 }

indexBlock readIndex (int id){			//根据索引块id读取索引块信息
	indexBlock ib;
	ifstream fin (disk.c_str (), std::ios::binary);
	fin.seekg (indexSegOffset+sizeof (ib)*id, ios::beg);
	ib = fin.read ((char *)&ib, sizeof ib);
	fin.close ();
	return ib;
}

void writeIndex (indexBlock ib, int id){ //将索引块信息写入索引块
	ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
	fout.seekp (indexSegOffset+sizeof (ib)*id, ios::beg);
	fout.write ((char *)&ib, sizeof ib);
	fout.close ();
}

void releaseIndex (int indexID){		//释放索引块
    superNodeBlock sn = readSuperNode ();
    indexBlock ib;
    ib.used = 0;
    ib.nextIndexID = -1;
    if (sn.emptyIndexBlock = -1){
        sn.emptyIndexBlock = indexID;
        sn._emptyDirBlock = indexID;
    }
    else{
        indexBlock ib1 = readIndex(sn._emptyIndexBlock);
        ib1.nextIndexID = indexID;
        writeIndex(ib1,sn._emptyIndexBlock);
        sn._emptyDirBlock = indexID;
    }
    writeSuperNode(sn);
    writeIndex(ib,indexID);
}

