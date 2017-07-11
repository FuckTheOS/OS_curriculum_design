#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

fileBlock readFile (int id);	//根据文件块id读取文件内容
//访盘，注意保护原有数据
void writeFile (fileBlock db, int id);  	//将文件内容写入文件块
//访盘，注意保护原有数据
int openFile (string fileaName);//打开文件的目录块 返回对应的文件内容块编号
//参数表示当前路径下的文件名
void vim (int id);				//对文件内容进行编辑
//参数表示文件内容块的索引
void releaseFile (int fileID);	//释放文件块
void cat (string filename);		//输出当前目录下的文件内容
int  giveFileBlock();           //分配文件块
//如果成功返回分配的文件块的编号 否则返回-1
bool touch (string fileName, string newDirMod);	//当前目录下新建文件
//新建的文件名 新建的文件权限类型

#endif // FILE_H_INCLUDED

