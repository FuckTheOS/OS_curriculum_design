#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#define Clear(x,y) memset (x,y,sizeof(x))
#define Close() ios::sync_with_stdio(0)
#define Open() freopen ("more.in", "r", stdin)
#define get_min(a,b) a = min (a, b)
#define get_max(a,b) a = max (a, b);
#define y0 yzz
#define y1 yzzz
#define fi first
#define se second
#define pii pair<int, int>
#define pli pair<long long, int>
#define pll pair<long long, long long>
#define pdi pair<double, int>
#define pdd pair<double, double>
#define vei vector<int>
#define vel vector<long long>
#define pb push_back
#define pl c<<1
#define pr (c<<1)|1
#define lson l,mid,pl
#define rson mid+1,r,pr
typedef unsigned long long ull;
template <class T> inline T lowbit (T x) {return x&(-x);}
template <class T> inline T sqr (T x) {return x*x;}
template <class T>
inline bool scan (T &ret) {
    char c;
    int sgn;
    if (c = getchar(), c == EOF) return 0; //EOF
    while (c != '-' && (c < '0' || c > '9') ) c = getchar();
    sgn = (c == '-') ? -1 : 1;
    ret = (c == '-') ? 0 : (c - '0');
    while (c = getchar(), c >= '0' && c <= '9') ret = ret * 10 + (c - '0');
    ret *= sgn;
    return 1;
}
const double pi = 3.14159265358979323846264338327950288L;
using namespace std;
#define mod 1000000007
#define INF 1e18
#define maxn 19
#define maxm 2000005
//-----------------morejarphone--------------------//

/*****************
������
���������õ������ݽṹ�ͻ�������
�����������е�ģ��������

���̿黮�֣������ڵ�|�û���|Ŀ¼��|�ļ���|������
*****************/
//�����ĸ���Ĵ�С
#define USERSIZE 50
#define DIRSIZE 100
#define FILESIZE 100
#define INDEXSIZE 200
//�����ĸ����ƫ��
#define superNodeSegOffset 0
#define userSegOffset sizeof(superNodeBlock)
#define dirSegOffset (userSegOffset+sizeof(userBlock)*USERSIZE)
#define fileSegOffset (dirSegOffset+sizeof(dirBlock)*DIRSIZE)
#define indexSegOffset (fileSegOffset+sizeof(fileBlock)*FILESIZE)
/*
�ļ�Ȩ�޹涨
���¹�Լ�������ȼ���Ӹߵ�������
	rootĿ¼����ɾ(����admin)
	1.Ȩ�޹涨��
		0(-p)����admin�ɼ� 1(-r)�������û�(�ÿ�)���� 2(-rw)�������û�(�ÿ�)�ɶ��ɱ༭������ɾ
		3(-a)�������û�(�ÿ�)���Զ��༭ɾ��ȱʡ��Ĭ�Ϸ�����
	2.admin��һ��Ȩ��(�����޸��κ��û������룬�޸��κ��ļ���Ȩ�ޣ��޸��κ��û���Ȩ��)
	3.����Ȩ��Ϊ2���ļ������ϼ��ļ�������Ϊ3

�û�Ȩ�޹涨��
	0��adminȨ��  1���ɶ��༭ɾ��ȱʡ��Ĭ��Ȩ�ޣ� 2���ɶ��༭������ɾ 3��ֻ�ɶ�
	��ôһ�����ҽ�����
		�û����������û�Ȩ�� && ���������ļ�Ȩ��ʱ���û����Զ��ļ������û�����

������1���� 2���༭���������ӣ��Ķ��� 3��ɾ��
*/

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

extern vector <string> curPath;//��ǰ�ľ���·��
extern int curUserID;			//��ǰ�û�
extern int curDirID;			//��ǰĿ¼��ID
extern string disk = "disk";	//���̿��ļ���

long long getTime ();				//��ȡ��ǰ��ʱ��
//����������ʱ�� ����*100000000+��*1000000+��*10000+ʱ*100+��
void printTime (long long num);		//����ʱ��ֵ��ӡʱ�䴮

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
bool checkMod (int userID, int dirID, int type);	//Ȩ���ж�
//������ʾ�û�id Ŀ¼id ��Ϊ��Ȩ������
//�û��ܷ���ļ�����Ȩ��Ϊtype�Ĳ���
//���Է���1 ���򷵻�0
void find (int curDirID, string target, vector <string> path);	//�ӵ�ǰ·��������Ŀ���ļ���ע�����ļ���
//������ʾĿ���ļ����ļ���
//������п��ܵĽ��·��
void state ();						//��ʾ�ڴ�ʹ�����
//����û��� Ŀ¼�� �ļ��� ������ʣ�µĿ�����ʹ����

userBlock readUser (int id);				//�����û���id��ȡ�û�����Ϣ
//���̣�ע�Ᵽ��ԭ������
void writeUser (userBlock ub, int id);  	//���û�����Ϣд���û���
//���̣�ע�Ᵽ��ԭ������
dirBlock readDir (int id);				//����Ŀ¼��id��ȡĿ¼����Ϣ
//���̣�ע�Ᵽ��ԭ������
void writeDir (dirBlock db, int id);  	//��Ŀ¼����Ϣд��Ŀ¼��
//���̣�ע�Ᵽ��ԭ������
fileBlock readFile (int id);	//�����ļ���id��ȡ�ļ�����
//���̣�ע�Ᵽ��ԭ������
void writeFile (fileBlock db, int id);  	//���ļ�����д���ļ���
//���̣�ע�Ᵽ��ԭ������
indexBlock readIndex (int id);				//����������id��ȡ��������Ϣ
//���̣�ע�Ᵽ��ԭ������
void writeIndex (indexBlock db, int id);  	//����������Ϣд��������
//���̣�ע�Ᵽ��ԭ������
int giveIndexBlock ();		//�����µ�������
//�������ɹ������������ID �������-1
void releaseIndex (int indexID);		//�ͷ�������
void releaseFile (int fileID);	//�ͷ��ļ���






#endif // COMMON_H_INCLUDED
