#ifndef FILE_STRUCT_H_INCLUDED
#define FILE_STRUCT_H_INCLUDED

struct superNodeBlock {
	//�����ڵ��
	int root;			//��Ŀ¼λ��
	int emptyUserBlock;	//���û���
	int emptyDirBlock;	//��Ŀ¼��
	int _emptyDirBlock; //��Ŀ¼�����һ��
	int emptyFileBlock;	//���ļ���
	int _emptyFileBlock;//���ļ������һ��
	int emptyIndexBlock;//��������
	int _emptyIndexBlock;//�����������һ��
	//int curUser;		//Ŀǰ���û�
};


struct userBlock {
	//�û���
	char userName[20];	//�û���
	char userPassword[20];//�û�����
	int userMod;		//�û�Ȩ��
	bool used;			//�Ƿ�ռ��
};

struct dirBlock {
	//Ŀ¼�� ����Ŀ¼���ļ�����Ϣ
	char dirName[20];	//Ŀ¼��
	char dirOwner[20];	//Ŀ¼������
	int dirSize;		//�����Ŀ¼��Ϊ0 ������ļ����ʾ�ļ���С
	int dirCreateTime;	//Ŀ¼����ʱ��
	int dirChangeTime;	//Ŀ¼��һ���޸�ʱ��
	int type; 			//1:Ŀ¼ 2:�ļ� 3:Ӳ���� 4:������
	int textLocation;	//������ļ� �ļ����ݵ�λ�����ڵ�����
	int faDirID;		//��Ŀ¼id
	int sonDirID;		//��һ��Ŀ¼id(����)
	int nextDirID;		//��һ��Ŀ¼id(�ֵ�)  ���û�б�ռ�ã���ô��ʾ��һ�����п� -1��ʾ���һ���տ�
	int dirMod;			//Ŀ¼Ȩ����Ϣ
	bool used;			//�Ƿ�ռ��
};

struct fileBlock {
	//�ļ��� �����ļ����ı�����
	char text[1000];
	int nextFileID;		//��һ���ļ���id ���û�б�ռ����ô��ʾ��һ�����п�
	bool used;			//�Ƿ�ռ��
};

struct indexBlock {
	int nextIndexID;	//��һ��������id  ���û�б�ռ����ô��ʾ��һ��������
	int diskOffset;		//����ָ��ĵ�ַ
	int used;			//�Ƿ�ռ��
};

#endif // FILE_STRUCT_H_INCLUDED
