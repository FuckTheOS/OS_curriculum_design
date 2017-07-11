#ifndef VIM_H_INCLUDED
#define VIM_H_INCLUDED
#include <cstdlib>
#include <cstdio>
#include <windows.h>
using namespace std;
// Edit by Kelukin to support the file modify operation in file system
void displayVim();
// 是否要制作底部的工具栏暂时有待考虑
// 将当期缓存区当中的内容打印在终端屏幕上

void runVim(int fileBlockID);
//运行Vim
//传入的参数为文件块的序号

void clearScreen();
//清空屏幕

void workChar(char &x);
//根据输入的字符的不同，进行不同的操作
//暂时设想的工作字符有可见字符, ESC键退出到普通模式

void initBuffer(int fileBlockID);
//打开文件，将其内容加载进Buffer当中

void saveVim();
//将当前文件的信息写入某个文件块当中
//有问题的点在于如果文件大小缩短时，需要将原来的文件块进行释放
//过长时需要申请新的文件块

void quitVim();
//退出当前Vim
#endif // VIM_H_INCLUDED
