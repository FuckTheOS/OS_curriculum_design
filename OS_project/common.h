#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include "dir.h"
#include "user.h"
#include "file.h"
#include "user.h"
#include "index.h"
#include "global.h"

superNodeBlock readSuperNode ();	//���볬���ڵ���Ϣ
void writeSuperNode (superNodeBlock sn);        //���볬���ڵ���Ϣ
void showCurPath (int type, vector <string> curPath);	//����������ָ��ǰ��ʾ��ǰ·���û� ��ʽΪroot>a/b admin$[space]
//0��ʾ����������ָ��ǰ��ʾ��ǰ·���û� ��ʽΪroot>a/b admin$[space]
//1��ʾ�����ǰ����·��λ�� ��ʽΪroot>a/b[enter]
//����ȫ�ֱ���globalֱ�Ӱ��ո�ʽ���
vector <string>& pathPrase (string tarPath);	//���Զ�������·��
//������ʾĿ��·��
//���Ŀ��·����root��ͷ��ʾ����·��  ���������·��
//����·���ϸ����ڵ��Ŀ¼�����ɵ�����
//  *��ʾ�����ļ� ��������TOT��ʾ  .��ʾ��ǰ·�� ��������CUR��ʾ
bool visitPath(dirBlock& cur, string target, int& curID); //���ʵ�ǰĿ¼�µ�ָ���¼�Ŀ¼
//������ʱ���ڵ�Ŀ¼��ָ���Լ��¼�Ŀ¼������
//����޷����ʻ��߲������򷵻�false
//���ʳɹ�ʱ�޸ĵ�ǰ�ı���curDirID;
int findNextDir (int dirID, string target, int dirType = 1);//����dirID�µ�targetĿ¼
//��������¼�Ŀ¼��ID �����ڷ���-1
long long getTime ();				//��ȡ��ǰ��ʱ��
//����������ʱ�� ����*100000000+��*1000000+��*10000+ʱ*100+��
void printTime (long long num);		//����ʱ��ֵ��ӡʱ�䴮
bool checkMod (int userID, int dirID, int type);	//Ȩ���ж�
//������ʾ�û�id Ŀ¼id ��Ϊ��Ȩ������
//�û��ܷ���ļ�����Ȩ��Ϊtype�Ĳ���
//���Է���1 ���򷵻�0
void find (int curDirID, string target, vector <string> path);	//�ӵ�ǰ·��������Ŀ���ļ���ע�����ļ���
//������ʾĿ���ļ����ļ���
//������п��ܵĽ��·��
void state ();						//��ʾ�ڴ�ʹ�����
//����û��� Ŀ¼�� �ļ��� ������ʣ�µĿ�����ʹ����









#endif // COMMON_H_INCLUDED
