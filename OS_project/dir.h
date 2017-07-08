#ifndef DIR_H_INCLUDED
#define DIR_H_INCLUDED

dirBlock readDir (int id);				//根据文件块id读取文件块信息
//访盘，注意保护原有数据
void showAllSonDir ();	//显示当前路径下所有子目录
//当前路径直接用全局变量
//按照a b.txt c d.cpp 的格式输出
int giveDirBlock (int dirType);			//分配新的目录块 
//参数表示目录块类型
//如果分配成功返回目录块ID 否则返回-1
bool checkDirName (string newDirName);	//检查目录名是否和当前其他目录冲突
//参数表示希望新建的文件名
//如果不冲突返回1 否则返回0
//当前路径直接是全局变量
bool mkdir (string newDirName, string newDirMod);	//在当前目录下创建子目录
//新目录的名字 新目录的权限(空串表示-a)
//创建成功返回1 否则返回0
bool mkdirs (string newDirPath, string newDirMod);	//在当前目录下创建多级子目录
//相对或绝对路径 新目录的权限(仅针对最末级)
//路径是一个原始串 需要路径解析自动机解析
//创建成功返回1 否则返回0
bool gotoDir (string tarPath);			//跳转到新的目录
//参数是相对或绝对路径 需要路径解析自动机解析路径
//跳转成功返回1 否则返回0



#endif // DIR_H_INCLUDED

