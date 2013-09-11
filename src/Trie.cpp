/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#include "Trie.h"

namespace CppJieba
{
    Trie::iterator Trie::begin()
    {
        return _nodeInfoVec.begin();
    }

    Trie::iterator Trie::end()
    {
        return _nodeInfoVec.end();
    }

    Trie::Trie()
    {

        _root = NULL;
        _freqSum = 0;
        _minLogFreq = MAX_DOUBLE;
        _initFlag = false;
    }
    
    Trie::~Trie()
    {
        dispose();
    }
    
    bool Trie::_getInitFlag()
    {
        return _initFlag;
    }
    void Trie::_setInitFlag(bool on)
    {
        _initFlag = on;
    }

    bool Trie::init()
    {
        if(_getInitFlag())
        {
            LogError("already initted!");
            return false;
        }

        try
        {
            _root = new TrieNode;
        }
        catch(const bad_alloc& e)
        {
            return false;
        }
        if(NULL == _root)
        {
            return false;
        }
        _setInitFlag(true);
        return true;
    }

    bool Trie::loadDict(const char * const filePath)
    {
        if(!_getInitFlag())
        {
            LogError("not initted.");
            return false;
        }

        if(!checkFileExist(filePath))
        {
            LogError(string_format("cann't find fiel[%s].",filePath));
            return false;
        }
        bool res = false;
        res = _trieInsert(filePath);
        if(!res)
        {
            LogError("_trieInsert failed.");
            return false;
        }
        res = _countWeight();
        if(!res)
        {
            LogError("_countWeight failed.");
            return false;
        }
        return true;
    }

    bool Trie::_trieInsert(const char * const filePath)
    {
        
        ifstream ifile(filePath);
        string line;
        vector<string> vecBuf;
        
        TrieNodeInfo nodeInfo;
        while(getline(ifile, line))
        {
            vecBuf.clear();
            splitStr(line, vecBuf, " ");
            if(3 < vecBuf.size())
            {
                LogError(string_format("line[%s] illegal.", line.c_str()));
                return false;
            }
            if(!TransCode::decode(vecBuf[0], nodeInfo.word))
            {
                return false;
            }
            nodeInfo.freq = atoi(vecBuf[1].c_str());
            if(3 == vecBuf.size())
            {
                nodeInfo.tag = vecBuf[2];
            }

            //insert node
            if(!insert(nodeInfo))
            {
                LogError("insert node failed!");
            }
        }
        return true;
    }

    bool Trie::dispose()
    {
        if(!_getInitFlag())
        {
            return false;
        }
        bool ret = _deleteNode(_root);
        if(!ret)
        {
            LogFatal("_deleteNode failed!");
            return false;
        }
        _root = NULL;
        _nodeInfoVec.clear();

        _setInitFlag(false);
        return ret;
    }

    const TrieNodeInfo* Trie::findPrefix(const string& str)
    {
        if(!_getInitFlag())
        {
            LogFatal("trie not initted!");
            return NULL;
        }
        Unicode uintVec;
        
        bool retFlag = TransCode::decode(str, uintVec);
        if(retFlag)
        {
            LogError("TransCode::decode failed.");
            return NULL;
        }

        //find
        TrieNode* p = _root;
        TrieNodeInfo * res = NULL;
        for(uint i = 0; i < uintVec.size(); i++)
        {
            uint16_t chUni = uintVec[i];
            if(p->isLeaf)
            {
                uint pos = p->nodeInfoVecPos;
                if(pos < _nodeInfoVec.size())
                {
                    res = &(_nodeInfoVec[pos]);
                }
                else
                {
                    LogFatal("node's nodeInfoVecPos is out of _nodeInfoVec's range");
                    return NULL;
                }
                
            }
            if(p->hmap.find(chUni) == p->hmap.end())
            {
                break;
            }
            else
            {
                p = p->hmap[chUni];
            }
        }
        return res;
    }

    TrieNodeInfo* Trie::find(const string& str)
    {
        Unicode uintVec;
        bool retFlag = TransCode::decode(str, uintVec);
        if(!retFlag)
        {
            return NULL;
        }
        return find(uintVec);
    }

    TrieNodeInfo* Trie::find(const Unicode& uintVec)
    {
        if(uintVec.empty())
        {
            return NULL;
        }
        return find(uintVec.begin(), uintVec.end());
    }

    TrieNodeInfo* Trie::find(Unicode::const_iterator begin, Unicode::const_iterator end)
    {
        
        if(!_getInitFlag())
        {
            LogFatal("trie not initted!");
            return NULL;
        }
        if(begin >= end)
        {
            return NULL;
        }
        TrieNode* p = _root;
        for(Unicode::const_iterator it = begin; it != end; it++)
        {
            uint16_t chUni = *it;
            if(p->hmap.find(chUni) == p-> hmap.end())
            {
                return NULL;
            }
            else
            {
                p = p->hmap[chUni];
            }
        }
        if(p->isLeaf)
        {
            uint pos = p->nodeInfoVecPos;
            if(pos < _nodeInfoVec.size())
            {
                return &(_nodeInfoVec[pos]);
            }
            else
            {
                LogFatal("node's nodeInfoVecPos is out of _nodeInfoVec's range");
                return NULL;
            }
        }
        return NULL;
    }

    double Trie::getWeight(const string& str)
    {

        Unicode uintVec;
        TransCode::decode(str, uintVec);
        return getWeight(uintVec);
    }

    double Trie::getWeight(const Unicode& uintVec)
    {
        if(uintVec.empty())
        {
            return getMinLogFreq();
        }
        const TrieNodeInfo * p = find(uintVec);
        if(NULL != p)
        {
            return p->logFreq;
        }
        else
        {
            return getMinLogFreq();
        }
        
    }

    double Trie::getWeight(Unicode::const_iterator begin, Unicode::const_iterator end)
    {
        const TrieNodeInfo * p = find(begin, end);
        if(NULL != p)
        {
            return p->logFreq;
        }
        else
        {
            return getMinLogFreq();
        }
    }

    double Trie::getMinLogFreq()
    {
        return _minLogFreq;
    }

    int64_t Trie::getTotalCount()
    {
        return _freqSum;
    }

    bool Trie::_deleteNode(TrieNode* node)
    {
        for(TrieNodeMap::iterator it = node->hmap.begin(); it != node->hmap.end(); it++)
        {
            TrieNode* next = it->second;
            _deleteNode(next);
        }
        
        delete node;
        return true;
    }


    bool Trie::insert(const TrieNodeInfo& nodeInfo)
    {
        if(!_getInitFlag())
        {
            LogFatal("not initted!");
            return false;
        }

        
        const Unicode& uintVec = nodeInfo.word;
        TrieNode* p = _root;
        for(uint i = 0; i < uintVec.size(); i++)
        {
            uint16_t cu = uintVec[i];
            if(NULL == p)
            {
                return false;
            }
            if(p->hmap.end() == p->hmap.find(cu))
            {
                TrieNode * next = NULL;
                try
                {
                    next = new TrieNode;
                }
                catch(const bad_alloc& e)
                {
                    return false;
                }
                p->hmap[cu] = next;
                p = next;
            }
            else
            {
                p = p->hmap[cu];
            }
        }
        if(NULL == p)
        {
            return false;
        }
        if(p->isLeaf)
        {
            LogError("this node already inserted");
            return false;
        }

        p->isLeaf = true;
        _nodeInfoVec.push_back(nodeInfo);
        p->nodeInfoVecPos = _nodeInfoVec.size() - 1;

        return true;
    }

    bool Trie::_countWeight()
    {
        if(_nodeInfoVec.empty() || 0 != _freqSum)
        {
            LogError("_nodeInfoVec is empty or _freqSum has been counted already.");
            return false;
        }
        
        //freq total freq
        for(size_t i = 0; i < _nodeInfoVec.size(); i++)
        {
            _freqSum += _nodeInfoVec[i].freq;
        }

        if(0 == _freqSum)
        {
            LogError("_freqSum == 0 .");
            return false;
        }
        
        //normalize
        for(uint i = 0; i < _nodeInfoVec.size(); i++)
        {
            TrieNodeInfo& nodeInfo = _nodeInfoVec[i];
            if(0 == nodeInfo.freq)
            {
                LogFatal("nodeInfo.freq == 0!");
                return false;
            }
            nodeInfo.logFreq = log(double(nodeInfo.freq)/double(_freqSum));
            if(_minLogFreq > nodeInfo.logFreq)
            {
                _minLogFreq = nodeInfo.logFreq;
            }
        }
        
        return true;
    }
}

#ifdef TRIE_UT
using namespace CppJieba;
int main()
{
    Trie trie;
    trie.init();
    trie.loadDict("../dicts/segdict.gbk.v2.1");
    //trie.loadDict("tmp");
    cout<<trie.getMinLogFreq()<<endl;
    cout<<trie.getTotalCount()<<endl;
    trie.dispose();
    return 0;
}
#endif
