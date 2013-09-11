/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
************************************/
#include "config.h"
#include "str_functs.h"

namespace CPPCOMMON
{
    Config::Config()
    {
        _isInit = false;
    }

    Config::~Config()
    {
    }

    bool Config::init(const string& configFile)
    {
        char msgBuf[1024];
        if(_isInit)
        {
            LogFatal("already have been initialized. ");
            return false;
        }
        ifstream ifile(configFile.c_str());
        if(!ifile)
        {
            sprintf(msgBuf, "open configFile[%s] failed.", configFile.c_str());
            LogFatal(msgBuf);
            return false;
        }
        string line, key, value;
        vector<string> vecBuf;
        while(getline(ifile, line))
        {
            line = _stripComment(line);
            if(line.empty())
            {
                continue;
            }
            vecBuf = splitStr(line, "=");
            if(2 != vecBuf.size())
            {
                sprintf(msgBuf, "line[%s] is illegal.", line.c_str());
                LogFatal(msgBuf);
                return false;
            }
            key = vecBuf[0];
            value = vecBuf[1];
            if(_map.end() != _map.find(key))
            {
                sprintf(msgBuf, "key[%s] already exists.", key.c_str());
                LogFatal(msgBuf);
                return false;
            }
            _map[key] = value;
        }
        ifile.close();
        _isInit = true;
        return true;
    }

    void Config::display()
    {
        for(map<string, string>::iterator it = _map.begin(); it != _map.end(); it++)
        {
            cout<<"("<<it->first<<","<<it->second<<")"<<endl;
        }
    }
    
    string Config::getByKey(const string& key)
    {
        if(_map.end() != _map.find(key))
        {
            return _map[key];
        }
        else
        {
            return "";
        }
    }

    string Config::_stripComment(const string& line)
    {
        string res = line;
        string::size_type pos = res.find('#');
        if(string::npos != pos)
        {
            res = res.substr(0, pos);
        }
        return stripStr(res);
    }

}

namespace CPPCOMMON
{
    Config gConfig;
}


#ifdef CONFIG_UT
using namespace CPPCOMMON;
int main()
{
    gConfig.init("1.conf");
    gConfig.init("1.conf");
    gConfig.display();
    cout<<gConfig.getByKey("a")<<endl;
    cout<<gConfig.getByKey("cm")<<endl;
    return 0;
}

#endif
