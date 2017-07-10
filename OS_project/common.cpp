#include "common.h"
#include "dir.h"

superNodeBlock readSuperNode (){    //读入超级节点信息
    superNodeBlock sn;
    ifstream fin (disk.c_str (), std::ios::binary);
    fin.seekg (superNodeOffset, ios::beg);
    sn = fin.read ((char *)&sn, sizeof sn);
    fin.close ();
    return sn;
}
void writeSuperNode (superNodeBlcok sn) {   //吸入超级节点信息
    ofstream fout (disk.c_str (), std::ios::binary|ios::in|ios::out);
    fout.seekp (superNodeOffset, ios::beg);
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
    indexBlock ib;
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
            ib = readIndex(curID);
            cur = readDir(ib.diskOffset);
        }
        return true;
    }
    while(stricmp(cur.dirName,target.c_str())!=0)
    {
        if(cur.nextDirID == -1) return false; //当前目录下并找不到指定的目录
        curID = cur.nextDirID;
        ib = readIndex(curID);
        cur = readDir(ib.diskOffset);
    }
    curID = cur.sonDirID;
    ib = readIndex(curID);
    cur = readDir(ib.diskOffset);//进入该目录
    return true;
}

int findNextDir (int dirID, string target){     //访问dirID下的target目录
    dirBlock db = readDir (dirID), tmp;
    if (db.sonDirID == -1) return -1;
    indexBlock ib = readIndex (db.sonDirID);
    tmp = readDir (ib.diskOffset);
    if (target == (string)tmp.dirName) {
        return db.sonDirID;
    }
    db = tmp;
    while (db.nextDirID != -1) {
        ib = readIndex (db.nextDirID);
        tmp = readDir (ib.diskOffset);
        if (target == (string)tmp.dirName && tmp.type == 1) {
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
    printf ("%02d:", num/100);
    printf ("%02d\n", num%100);
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
        return false
}
