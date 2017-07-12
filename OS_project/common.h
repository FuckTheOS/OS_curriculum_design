#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include "dir.h"
#include "user.h"
#include "file.h"
#include "user.h"
#include "index.h"
#include "global.h"

superNodeBlock readSuperNode ();	//读入超级节点信息
void writeSuperNode (superNodeBlock sn);        //吸入超级节点信息
void showCurPath (int type, vector <string> curPath);	//命令行输入指令前显示当前路径用户 格式为root>a/b admin$[space]
//0表示命令行输入指令前显示当前路径用户 格式为root>a/b admin$[space]
//1表示输出当前绝对路径位置 格式为root>a/b[enter]
//根据全局变量global直接按照格式输出
vector <string>& pathPrase (string tarPath);	//用自动机解析路径
//参数表示目标路径
//如果目标路径以root开头表示绝对路径  否则是相对路径
//返回路径上各个节点的目录名构成的向量
//  *表示所有文件 向量中用TOT表示  .表示当前路径 向量中用CUR表示
bool visitPath(dirBlock& cur, string target, int& curID); //访问当前目录下的指定下级目录
//传进当时所在的目录块指针以及下级目录的名臣
//如果无法访问或者不存在则返回false
//访问成功时修改当前的变量curDirID;
int findNextDir (int dirID, string target, int dirType = 1);//访问dirID下的target目录
//返回这个下级目录的ID 不存在返回-1
long long getTime ();				//获取当前的时间
//按照年月日时分 即年*100000000+月*1000000+日*10000+时*100+分
void printTime (long long num);		//根据时间值打印时间串
bool checkMod (int userID, int dirID, int type);	//权限判断
//参数表示用户id 目录id 行为的权限类型
//用户能否对文件进行权限为type的操作
//可以返回1 否则返回0
void find (int curDirID, string target, vector <string> path);	//从当前路径下搜索目标文件（注意是文件）
//参数表示目标文件的文件名
//输出所有可能的结果路径
void state ();						//显示内存使用情况
//输出用户块 目录块 文件块 索引块剩下的块数和使用率









#endif // COMMON_H_INCLUDED
