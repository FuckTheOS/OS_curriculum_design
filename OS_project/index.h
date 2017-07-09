#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

int giveIndexBlcok ();		//分配新的索引块
//如果分配成功返回索引块的ID 否则输出-1
dirBlock readDir (int id);				//根据目录块id读取目录块信息
//访盘，注意保护原有数据
void writeDir (dirBlock db, int id);  	//将目录块信息写入目录块
//访盘，注意保护原有数据

#endif // INDEX_H_INCLUDED

