#include "common.h"
#include "index.h"

int giveIndexBlcok ();		//分配新的索引块
//如果分配成功返回索引块的ID 否则输出-1
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
