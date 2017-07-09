#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

bool checkMod (int modType);	//用户在当前路径下对欲使用此权限
fileBlock readFile (int id);	//根据文件块id读取文件内容
//访盘，注意保护原有数据
void writeFile (fileBlock db, int id);  	//将文件内容写入文件块
//访盘，注意保护原有数据

#endif // FILE_H_INCLUDED

