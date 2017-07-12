#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED
#include "common.h"
#include "global.h"

int giveIndexBlock ();		//分配新的索引块
//如果分配成功返回索引块的ID 否则输出-1
indexBlock readIndex (int id);				//根据索引块id读取索引块信息
//访盘，注意保护原有数据
void writeIndex (indexBlock db, int id);  	//将索引块信息写入索引块
//访盘，注意保护原有数据
void releaseIndex (int indexID);		//释放索引块

#endif // INDEX_H_INCLUDED

