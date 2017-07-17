#ifndef FILE_STRUCT_H_INCLUDED
#define FILE_STRUCT_H_INCLUDED

struct superNodeBlock {
	//超级节点块
	int root;			//根目录位置
	int emptyUserBlock;	//空用户块
	int emptyDirBlock;	//空目录块
	int _emptyDirBlock; //空目录块最后一块
	int emptyFileBlock;	//空文件块
	int _emptyFileBlock;//空文件块最后一块
	int emptyIndexBlock;//空索引块
	int _emptyIndexBlock;//空索引块最后一块
	//int curUser;		//目前的用户
};


struct userBlock {
	//用户块
	char userName[20];	//用户名
	char userPassword[20];//用户密码
	int userMod;		//用户权限
	bool used;			//是否被占用
};

struct dirBlock {
	//目录块 包含目录或文件的信息
	char dirName[20];	//目录名
	char dirOwner[20];	//目录创建者
	int dirSize;		//如果是目录则为0 如果是文件则表示文件大小
	int dirCreateTime;	//目录创建时间
	int dirChangeTime;	//目录上一次修改时间
	int type; 			//1:目录 2:文件 3:硬链接 4:软链接
	int textLocation;	//如果是文件 文件内容的位置所在的索引
	int faDirID;		//父目录id
	int sonDirID;		//下一级目录id(孩子)
	int nextDirID;		//下一个目录id(兄弟)  如果没有被占用，那么表示下一个空闲快 -1表示最后一个空块
	int dirMod;			//目录权限信息
	bool used;			//是否被占用
};

struct fileBlock {
	//文件块 保存文件的文本内容
	char text[1000];
	int nextFileID;		//下一个文件块id 如果没有被占用那么表示下一个空闲块
	bool used;			//是否被占用
};

struct indexBlock {
	int nextIndexID;	//下一个索引块id  如果没有被占用那么表示下一个索引块
	int diskOffset;		//索引指向的地址
	int used;			//是否被占用
};

#endif // FILE_STRUCT_H_INCLUDED
