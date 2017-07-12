#include "common.h"
#include "dir.h"
#include "user.h"
#include "file.h"
#include "user.h"
#include "index.h"

superNodeBlock readSuperNode (){    //读入超级节点信息
    superNodeBlock sn;
    ifstream fin (disk.c_str (), std::ios::binary);
    fin.seekg (superNodeSegOffset, ios::beg);
    fin.read ((char *)&sn, sizeof sn);
    fin.close ();
    return sn;
}
void writeSuperNode (superNodeBlock sn) {   //写入超级节点信息
    ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
    fout.seekp (superNodeSegOffset, ios::beg);
    fout.write ((char *)&sn, sizeof sn);
    fout.close ();
}

#define PATH_PRASER_ERROR  path.clear();path.push_back("error");break;
void showCurPath (int type) {	//输出当前路径
    if (type == 0) {
        cout << "root>";
        for (int i = 1; i < curPath.size (); i++) {
            cout << curPath[i] << (i == curPath.size ()-1 ? '/' : ' ');
        }
        userBlock ub = readUser (curUserID);
        cout << ub.userName << "$ ";
    }
    else if (type == 1) {
        for (int i = 0; i < curPath.size (); i++) {
            cout << curPath[i] << '/';
        }
        cout << endl;
    }
}

vector <string>& pathPrase (string tarPath){	//用自动机解析路径
	vector <string> path;path.clear();
	size_t x_point = 0; //未处理到的tarPath的字符下标
	if(tarPath[0]=='/') //绝对路径
    {
        path.push_back("/root");
        x_point = 1;
    }
    else
        path.push_back("/CUR");
    while(x_point != tarPath.npos)
	{
	    size_t tmp_pos = tarPath.find_first_of('/', x_point);
	    if(tmp_pos == x_point) // 两个破折号堆叠在一起 ，非法
        {
          PATH_PRASER_ERROR;
        }
        string tmpString = tarPath.substr(x_point, (tmp_pos != tarPath.npos)? tmp_pos-x_point : tmp_pos);
        if(tmpString == "*") //通配符不在末尾时，非法
            if(tmp_pos == tarPath.npos) tmpString = "/TOT";
            else
            {
                PATH_PRASER_ERROR;
            }
        if(tmpString == ".") tmpString = "/CUR";//当前目录
        if(tmpString == "..") tmpString = "/BACK";//返回上级目录
        if(tmpString == "null")
            if(path.size()!=1 || tmp_pos != tarPath.npos) {PATH_PRASER_ERROR;}
            else break;
        path.push_back(tmpString);
        if(tmp_pos == tarPath.npos) break;
        x_point = tmp_pos+1;
	}
	return path;
}

bool visitPath(dirBlock& cur, string target, int& curID)
//这里存在bug 访问下一级dir应该通过索引块
{
    //权限检查待更新
    //indexBlock ib;
    if(target == "/CUR") return true;
    if(target == "/root")
    {
        cur = readDir(0);
        curID = 0;
        return true;
    }
    if(target == "/BACK")
    {
        if(cur.faDirID <=0) return false;//已到根目录而无法返回
        else
        {
            curID = cur.faDirID;
            cur = readDir(curID);
        }
        return true;
    }
    while(strcmp(cur.dirName,target.c_str())!=0)
    {
        if(cur.nextDirID == -1) return false; //当前目录下并找不到指定的目录
        curID = cur.nextDirID;
        cur = readDir(curID);
    }
    curID = cur.sonDirID;
    cur = readDir(curID);//进入该目录
    return true;
}

int findNextDir (int dirID, string target, int dirType){     //访问dirID下的target目录 目录类型为1
    dirBlock db = readDir (dirID), tmp;
    if (db.sonDirID == -1) return -1;
    tmp = readDir (db.sonDirID);
    if (target == (string)tmp.dirName && tmp.type == dirType) {
        return db.sonDirID;
    }
    db = tmp;
    while (db.nextDirID != -1) {
        tmp = readDir (db.nextDirID);
        if (target == (string)tmp.dirName && tmp.type == dirType) {
            return db.nextDirID;
        }
        else
            db = tmp;
    }
    return -1;
}
//返回这个下级目录的ID 不存在返回-1

long long getTime (){               //获取当前的时间
    SYSTEMTIME sys;
    GetLocalTime( &sys );
    long long year, month, day, hour, minute;
    year = sys.wYear, month = sys.wMonth, day = sys.wDay, hour = sys.wHour;
    minute = sys.wMinute;
    long long ans = year*100000000LL+month*1000000LL+day*10000LL+hour*100+minute;
    return ans;
}
//按照年月日时分 即年*100000000+月*1000000+日*10000+时*100+分
void printTime (long long num){     //根据时间值打印时间串
    cout << num/100000000LL << "年"; num %= 100000000LL;
    cout << num/1000000LL << "月"; num %= 1000000LL;
    cout << num/10000LL << "日 "; num %= 10000LL;
    printf ("%02d:", (int)num/100);
    printf ("%02d\n", (int)num%100);
}

bool checkMod (int userID, int dirID, int type){    //权限判断
    userBlock ub = readUser (userID);
    dirBlock db = readDir (dirID);
    int utype = ub.userMod, dtype = db.dirMod;
    if (type == 1 || (type == 2 && utype < 3) || (type == 3 && utype < 2)) { //操作满足用户权限
        if ((type == 1 && dtype) || (type == 2 && dtype > 1) || (type == 3 && dtype > 2))
            return true;    //操作满足文件权限
        else
            return false;
    }
    else
        return false;
}

void find (int curDirID, string target, vector <string> path){  //从当前路径下搜索目标文件（注意是文件）
    //直接递归搜索
    dirBlock db = readDir (curDirID);
    if ((string)db.dirName == target && db.type == 2) {
        for (int i = 0; i < (int)path.size (); i++) {
            cout << path[i] << '/';
        }
        cout << target << endl;
        return ;
    }
    path.pb ((string)db.dirName);
    if (db.sonDirID == -1)
        return ;
    else {
        find (db.sonDirID, target, path);
    }
    path.pop_back ();
    if (db.nextDirID != -1) {
        find (db.nextDirID, target, path);
    }
}
//参数表示目标文件的文件名
//输出所有可能的结果路径

void state (){                      //显示内存使用情况
    superNodeBlock sn = readSuperNode ();
    int cnt, p;
    p = sn.emptyUserBlock;
    if (p == -1) p = USERSIZE+1;
    printf ("已经占用%d个用户块，使用率%.2f\n", USERSIZE-p+1, (USERSIZE-p+1)*1.0/USERSIZE);

    cnt = 0, p = sn.emptyDirBlock;
    dirBlock db;
    while (p != -1) {
        cnt++;
        db = readDir (p);
        p = db.nextDirID;
    }
    printf ("已经占用%d个目录块，使用率%.2f\n", cnt, cnt*1.0/DIRSIZE);

    cnt = 0, p = sn.emptyFileBlock;
    fileBlock fb;
    while (p != -1) {
        cnt++;
        fb = readFile (p);
        p = fb.nextFileID;
    }
    printf ("已经占用%d个文件块，使用率%.2f\n", cnt, cnt*1.0/FILESIZE);

    cnt = 0, p = sn.emptyIndexBlock;
    indexBlock ib;
    while (p != -1) {
        cnt++;
        ib = readIndex (p);
        p = ib.nextIndexID;
    }
    printf ("已经占用%d个索引块，使用率%.2f\n", cnt, cnt*1.0/INDEXSIZE);
}
//输出用户块 目录块 文件块 索引块剩下的块数和使用率

