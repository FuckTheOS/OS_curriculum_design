#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include "common.h"
#include "dir.h"
#include "global.h"

bool userLogin ();	//�տ�ʼʱ���û���¼
					//����ѡ��admin����ͨ�û�������ģʽ(���û����������˺�����)
					//������½����1 ���򷵻�0
userBlock readUser (int id);				//�����û���id��ȡ�û�����Ϣ
//���̣�ע�Ᵽ��ԭ������
void writeUser (userBlock ub, int id);  	//���û�����Ϣд���û���
//���̣�ע�Ᵽ��ԭ������
bool chuser (string name, string passwd);	//�ڵ�ǰĿ¼���л��û�
//������ʾ�û������û�����
//�ɹ�����1 ���򷵻�0
bool createuser (string name, string passwd, int userMod);	//�����û�
//������ʾ�û������û����� �������û���Ȩ��
//�ɹ�����1 ���򷵻�0

#endif // USER_H_INCLUDED

