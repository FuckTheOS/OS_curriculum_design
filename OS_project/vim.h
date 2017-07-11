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

void runVim();
//运行Vim

void clearScreen();
//清空屏幕

void workChar(char &x);
//根据输入的字符的不同，进行不同的操作
//暂时设想的工作字符有可见字符, ESC键退出到普通模式

void initBuffer();
//若打开文件，则将其内容加载进Buffer当中

void saveVim();
//将当前文件的信息写入某个系统中

void quitVim();
//退出当前Vim
#endif // VIM_H_INCLUDED
