#include "concol.h"
#include <windows.h>
#include <iostream>
using namespace std;
#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)
int textcolor()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return a%16;
}

int backcolor()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return (a/16)%16;
}


ostream& operator<<(ostream& os,concol c)
{os.flush();setcolor(c,backcolor());return os;}
istream& operator>>(istream& is,concol c)
{cout.flush();setcolor(c,backcolor());return is;}
