#ifndef VIM_H_INCLUDED
#define VIM_H_INCLUDED
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include "file.h"
#include "user.h"
#include "dir.h"
using namespace std;
// Edit by Kelukin to support the file modify operation in file system
void displayVim();
// �Ƿ�Ҫ�����ײ��Ĺ�������ʱ�д�����
// �����ڻ��������е����ݴ�ӡ���ն���Ļ��

void runVim(int fileBlockID);
//����Vim
//����Ĳ���Ϊ�ļ�������

void clearScreen(COORD pos);
//�����Ļ

void workChar(char &x);
//����������ַ��Ĳ�ͬ�����в�ͬ�Ĳ���
//��ʱ����Ĺ����ַ��пɼ��ַ�, ESC���˳�����ͨģʽ

void initBuffer(int fileBlockID);
//���ļ����������ݼ��ؽ�Buffer����

void saveVim();
//����ǰ�ļ�����Ϣд��ĳ���ļ��鵱��
//������ĵ���������ļ���С����ʱ����Ҫ��ԭ�����ļ�������ͷ�
//����ʱ��Ҫ�����µ��ļ���

void writeChar(int& curFileID,fileBlock& curFileBlock, char x, int& cnt);
//�������ַ�������ļ�����,���͵�ǰ�ַ���Ӧ���洢��λ��,�ļ���ID

void quitVim();
//�˳���ǰVim
#endif // VIM_H_INCLUDED
