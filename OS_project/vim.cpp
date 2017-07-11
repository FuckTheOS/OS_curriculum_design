#include "vim.h"
#include "stdafx.h"
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <string>
#define GLOBAL_Y begin_y+p_y //���Ŀǰ����λ�������Buffer���Ե�λ��
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_DOWN 80
#define KEY_RIGHT 77
static short p_x = 0, p_y = 0;//���Ŀǰ������λ�� ��Ϊ�����е����λ��
static int x_backup, y_backup;
static int begin_y;//����Ŀǰ������λ�������ȫ��Buffer���Ե�λ��
//�洢ʱ�ǵ���ÿ����ͬ�е�bufferStringĩβ����'\n'
std::vector<std::string> bufferString;
static CONSOLE_SCREEN_BUFFER_INFO screen;
const COORD left_top = { 0,0 };
static bool isArrow = false;
static enum WorkStatus
{
	normal = 0,
	edit = 1,
}curStatus;
static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ������ڵľ��
//�����Ļ
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
void displayVim() // �����ڻ��������е����ݴ�ӡ���ն���Ļ��
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
		std::cout << "ָ��ģʽ" << std::endl;
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
		//������ڵ�ǰλ�����ڵط�֮������
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
	case '\t': //�����Ʊ�����һ������4���ո�
		bufferString[GLOBAL_Y].insert(p_x, 4, ' ');
		p_x += 4;
		break;
	case 13: //�����س���� ����һ����
		bufferString.push_back("");
		if (p_y<screen.dwSize.Y - 1) p_y++;
		else begin_y++;
		for (int i = bufferString.size() - 1; i > GLOBAL_Y; i--)
			bufferString[i] = bufferString[i - 1];
		bufferString[GLOBAL_Y] = bufferString[GLOBAL_Y-1].substr(p_x);
		bufferString[GLOBAL_Y - 1] = bufferString[GLOBAL_Y - 1].substr(0, p_x);
		p_x = 0;
		break;
	case 27: //���ĵ�ǰ������ģʽ
		x_backup = p_x;
		y_backup = p_y;
		curStatus = normal;
		break;
	case '\b':
		if (p_x == 0)
		{
			//��������һ�����ӵ����
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
	//�·�����ϸ���д�����
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
	else switch (x) // �༭ģʽ�µĴ���
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
