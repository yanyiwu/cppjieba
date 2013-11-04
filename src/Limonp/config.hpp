/************************************
 * file enc : utf8
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef LIMONP_CONFIG_H
#define LIMONP_CONFIG_H


#include <map>
#include <fstream>
#include <iostream>
#include "logger.hpp"
#include "str_functs.hpp"

namespace Limonp
{
    using std::map;
    using std::string;
    using std::cout;
    using std::endl;
    using std::ifstream;
    class Config
    {
        public:
            Config(){_isInit = false;};
            ~Config(){};
            bool init(const string& configFile)
            {
                if(_isInit)
                {
                    LogFatal("already have been initialized. ");
                    return false;
                }
                ifstream ifile(configFile.c_str());
                if(!ifile)
                {
                    LogFatal("open configFile[%s] failed.", configFile.c_str());
                    return false;
                }
                string line, key, value;
                vector<string> vecBuf;
                while(getline(ifile, line))
                {
                    //line = _stripComment(line);
                    if(line.empty())
                    {
                        continue;
                    }
                    vecBuf.clear();
                    if(!splitStr(line, vecBuf, "=") || 2 != vecBuf.size())
                    {
                        LogFatal("line[%s] is illegal.", line.c_str());
                        return false;
                    }
                    key = vecBuf[0];
                    value = vecBuf[1];
                    if(_map.end() != _map.find(key))
                    {
                        LogFatal("key[%s] already exists.", key.c_str());
                        return false;
                    }
                    _map[key] = value;
                }
                ifile.close();
                _isInit = true;
                return true;
            }
            void display()
            {
                for(map<string, string>::iterator it = _map.begin(); it != _map.end(); it++)
                {
                    cout<<"("<<it->first<<","<<it->second<<")"<<endl;
                }
            }
            bool get(const string& key, string& value)
            {
                if(_map.end() != _map.find(key))
                {
                    value = _map[key];
                    return true;
                }
                return false;
            }
        private:
            //bool _stripComment(const string& line, string& res)
            //{
            //    res = line;
            //    string::size_type pos = res.find('#');
            //    if(string::npos != pos)
            //    {
            //        res = res.substr(0, pos);
            //    }
            //    trim(res);
            //}
        private:
            map<string, string> _map;
            bool _isInit;

    };
}

namespace Limonp
{
    extern Config gConfig;
}

#endif
