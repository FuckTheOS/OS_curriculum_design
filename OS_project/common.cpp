#include "common.h"

#define PATH_PRASER_ERROR  path.clear;path.push_back("error");break;
void showCurPath (int type) {	//输出当前路径
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
