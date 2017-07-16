#ifndef DIR_H_INCLUDED
#define DIR_H_INCLUDED
#include <string>
using namespace std;

void showAllSonDir ();	//��ʾ��ǰ·����������Ŀ¼
//��ǰ·��ֱ����ȫ�ֱ���
//����a b.txt c d.cpp �ĸ�ʽ���
int giveDirBlock ();					//�����µ�Ŀ¼��
//������ʾĿ¼������
//�������ɹ�����Ŀ¼��ID ���򷵻�-1
bool checkDirName (string newDirName, int dirType = 1);	//���Ŀ¼���Ƿ�͵�ǰ����Ŀ¼��ͻ
//������ʾϣ���½����ļ���
//�������ͻ����1 ���򷵻�0
//��ǰ·��ֱ����ȫ�ֱ���
bool mkDir (string newDirName, string newDirMod, int _curDirID = -1);	//��Ŀ¼�´�����Ŀ¼
//����������ȱʡ��ʾĿǰ���ڵ�Ŀ¼ �����ʾ����Ŀ¼
//��Ŀ¼������ ��Ŀ¼��Ȩ��(�մ���ʾ-a)
//�����ɹ�����1 ���򷵻�0
bool mkdirs (string newDirPath, string newDirMod);	//�ڵ�ǰĿ¼�´����༶��Ŀ¼
//��Ի����·�� ��Ŀ¼��Ȩ��(�������ĩ��)
//·����һ��ԭʼ�� ��Ҫ·�������Զ�������
//�����ɹ�����1 ���򷵻�0
bool gotoDir (string tarPath);			//��ת���µ�Ŀ¼
//��������Ի����·�� ��Ҫ·�������Զ�������·��
//��ת�ɹ�����1 ���򷵻�0
//bool cd (string tarPath);               //��һ����ת����
bool gotoFaDir ();						//��ת������Ŀ¼
//��ǰĿ¼��ֱ�ӵ���ȫ�ֱ���
bool delDir (int dirID, string dirPath, int type);	//ɾ��Ŀ¼��
//��ǰĿ¼�� ·���� ɾ����������Ŀ¼����ĳ���ļ�
//ɾ���ɹ�����1 ����(·�������ڻ���Ȩ�޴���)��0
bool delAllDir (int dirID,int type=0);				//�ݹ�ɾ������Ŀ¼��
//ɾ���ɹ�����1 ���򷵻�0
void releaseDir (int dirID);			//�ͷ�һ��Ŀ¼��
int getDirID (string path);             //�ӵ�ǰĿ¼�°������·������Ŀ�� ����Ŀ���dirID
//��������ڷ���-1
bool moveDir (string fromPath, string toPath);      //�ƶ�Ŀ¼
void cpAllDir (int id1, int id2);                   //��ID2�е����ݵݹ鿽����ID1��
bool cpDir (string fromPath, string toPath);        //����Ŀ¼



#endif // DIR_H_INCLUDED

