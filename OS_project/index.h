#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED
#include "common.h"
#include "global.h"

int giveIndexBlock ();		//�����µ�������
//�������ɹ������������ID �������-1
indexBlock readIndex (int id);				//����������id��ȡ��������Ϣ
//���̣�ע�Ᵽ��ԭ������
void writeIndex (indexBlock db, int id);  	//����������Ϣд��������
//���̣�ע�Ᵽ��ԭ������
void releaseIndex (int indexID);		//�ͷ�������

#endif // INDEX_H_INCLUDED

