#include "vim.h"
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <string>
#define GLOBAL_Y begin_y+p_y //光标目前所处位置相对于Buffer而言的位置
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_DOWN 80
#define KEY_RIGHT 77
static short p_x = 0, p_y = 0;//光标目前所处的位置 仅为窗口中的相对位置
static int x_backup, y_backup;
static int begin_y;//窗口目前所处的位置相对于全部Buffer而言的位置
//存储时记得在每个不同行的bufferString末尾添加'\n'
std::vector<std::string> bufferString;
static CONSOLE_SCREEN_BUFFER_INFO screen;
const COORD left_top = { 0,0 };
static bool isArrow = false;
static enum WorkStatus
{
	normal = 0,
	edit = 1,
}curStatus;
static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);//获取输出窗口的句柄
//清空屏幕
void clearScreen(COORD &pos) {
	//HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, left_top, &written
	);

	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, left_top, &written
	);

}
void displayVim() // 将当期缓存区当中的内容打印在终端屏幕上
{
	COORD pos = { p_x, p_y };
	clearScreen(pos);
	SetConsoleCursorPosition(console, left_top);
	if(curStatus == edit)
	for (int i = begin_y; i - begin_y < screen.dwSize.Y&&i < bufferString.size() ; i++)
	{
		std::cout << bufferString[i] << std::endl;
	}
	else
	{
		std::cout << "指令模式" << std::endl;
		std::cout << "(S)ave (Q)uit" << std::endl << std::endl;
		for (int i = begin_y; i - begin_y < screen.dwSize.Y-4&&i < bufferString.size(); i++)
		{
			std::cout << bufferString[i] << std::endl;
		}
		p_x = 0;
		p_y = 2;
		pos = { p_x,p_y };
	}
	/*
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	SetClassLongPtr(GetStdHandle(STD_OUTPUT_HANDLE), GCLP_HBRBACKGROUND, (LONG)brush);
	*/
	SetConsoleCursorPosition(console, pos);//
}


void workChar(int &x)
{
	if (curStatus == edit && !isArrow && x >= 32 && x <= 126)
	{
		//输入会在当前位置所在地方之后添加
		//if(p_x == bufferString[GLOBAL_Y].size()-1)
		//bufferString[GLOBAL_Y].push_back(x);
		bufferString[GLOBAL_Y].insert(p_x, 1, char(x));
		p_x++;
		return;
	}
	int ty;
	if(curStatus == edit)
	switch (x)
	{
	case '\t': //处理制表符，一次添加4个空格
		bufferString[GLOBAL_Y].insert(p_x, 4, ' ');
		p_x += 4;
		break;
	case 13: //处理回车情况 开启一新行
		bufferString.push_back("");
		if (p_y<screen.dwSize.Y - 1) p_y++;
		else begin_y++;
		for (int i = bufferString.size() - 1; i > GLOBAL_Y; i--)
			bufferString[i] = bufferString[i - 1];
		bufferString[GLOBAL_Y] = bufferString[GLOBAL_Y-1].substr(p_x);
		bufferString[GLOBAL_Y - 1] = bufferString[GLOBAL_Y - 1].substr(0, p_x);
		p_x = 0;
		break;
	case 27: //更改当前所处的模式
		x_backup = p_x;
		y_backup = p_y;
		curStatus = normal;
		break;
	case '\b':
		if (p_x == 0)
		{
			//处理和上一行连接的情况
			if (GLOBAL_Y == 0) break;
			if (p_y) p_y--;
			else begin_y--;
			p_x = bufferString[GLOBAL_Y].size();
			bufferString[GLOBAL_Y] += bufferString[GLOBAL_Y + 1];
			bufferString.erase(bufferString.begin()+ GLOBAL_Y + 1);
		}
		else
		{
			bufferString[GLOBAL_Y].erase(p_x - 1, 1);
			p_x--;
		}
		break;
	//下方滚屏细节有待完善
	case KEY_UP: //UP
		if (p_y) p_y--;
		else if (begin_y) begin_y--;

		if (bufferString[GLOBAL_Y].size() <= p_x) p_x = bufferString[GLOBAL_Y].size() - 1;
		if (p_x < 0) p_x = 0;
		break;
	case KEY_DOWN: //Down
		if (GLOBAL_Y == bufferString.size() - 1) break;
		if (p_y<screen.dwSize.Y - 1) p_y++;
		else begin_y++;

		if (bufferString[GLOBAL_Y].size() <= p_x) p_x = bufferString[GLOBAL_Y].size() - 1;
		if (p_x < 0) p_x = 0;
		break;
	case KEY_LEFT:
		if (p_x) p_x--;
		else
			if (GLOBAL_Y == 0) break;
			else
			{
				if (p_y) p_y--;
				else begin_y--;
				p_x = bufferString[GLOBAL_Y].size() - 1;
			}
		break;
	case KEY_RIGHT:
		ty = bufferString[GLOBAL_Y].size();
		if (p_x <= ty - 1) p_x++;
		else
		{
			if (p_y == bufferString.size() - 1) break;
			if (p_y<screen.dwSize.Y - 1) p_y++;
			else begin_y++;
			p_x = 0;
		}
		break;
	default:
		break;
	}
	else switch (x) // 编辑模式下的处理
	{
	case 27:
		p_x = x_backup;
		p_y = y_backup;
		curStatus = edit;
		break;
	case 'Q':
	case 'q':
		quitVim();
		break;
	case 'S':
	case 's':
		saveVim();
		break;
	default:
		break;
	}
}
void initBuffer()
{
	bufferString.push_back("");
}
void runVim()
{
	int input_c;
	curStatus = edit;
	initBuffer();
	isArrow = false;
	while (1)
	{
		displayVim();
		input_c = getch();
		if (input_c == 0 || input_c == 224)
			isArrow = true;
		else
		{
			workChar(input_c);
			isArrow = false;
		}
		//std::cout.flush();
	}
}

