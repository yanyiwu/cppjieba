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

    Trie::Trie():_root(NULL), _totalCount(0)
    {
		_minWeight = numeric_limits<double>::max();
    }
    
    Trie::~Trie()
    {
        destroy();
    }

	bool Trie::init(const char* const filePath)
	{
		if(!checkFileExist(filePath))
		{
			LogError(string_format("cann't find fiel[%s].",filePath));
			return false;
		}
		bool res = false;
		res = _buildTree(filePath);
		if(!res)
		{
			LogError("_buildTree failed.");
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
    
    bool Trie::_buildTree(const char* const filePath)
    {
        if(NULL != _root)
        {
            LogError("already initted!");
            return false;
        }
        _root = new TrieNode;
        ifstream ifile(filePath);
        string line;
        vector<string> vecBuf;
        while(getline(ifile, line))
        {
            vecBuf.clear();
            splitStr(line, vecBuf, " ");
            if(3 != vecBuf.size())
            {
                LogError(string_format("line[%s] illegal.", line.c_str()));
                return false;
            }
            string chWord = vecBuf[0];
            uint count = atoi(vecBuf[1].c_str());
            const string& tag = vecBuf[2];

			//insert node
			TrieNodeInfo nodeInfo;
			nodeInfo.word = chWord;
			nodeInfo.count = count;
			nodeInfo.tag = tag;

			bool flag = _insert(nodeInfo);
			if(!flag)
			{
				LogError("insert node failed!");
				return false;
			}
			

        }
        return true;
    }

    bool Trie::destroy()
    {
        if(NULL == _root)
        {
            return false;
        }
        else
        {
            bool ret = _destroyNode(_root);
            _root = NULL;
            return ret;
        }
		_nodeInfoVec.clear();
    }

    void Trie::display()
    {
		for(uint i = 0; i < _nodeInfoVec.size(); i++)
		{
			cout<<_nodeInfoVec[i].word<<","
				<<_nodeInfoVec[i].count<<","
				<<endl;
		}
    }

	const TrieNodeInfo* Trie::findPrefix(const string& utf8Str)
	{
		if(NULL == _root)
		{
			LogFatal("trie not initted!");
			return NULL;
		}
		if(utf8Str.empty())
		{
			LogError("utf8Str is empty");
			return NULL;
		}
		string uniStr = utf8ToUnicode(utf8Str);
		if(uniStr.empty())
		{
			LogError("utf8ToUnicode return empty star");
			return NULL;
		}
		if(uniStr.size() % 2)
		{
			LogError("utf8ToUnicode return uniStr illegal");
			return NULL;
		}
		//find
		TrieNode* p = _root;
		TrieNodeInfo * res = NULL;
		for(uint i = 0; i < uniStr.size(); i+=2)
		{
			ChUnicode chUni = twocharToUint16(uniStr[0], uniStr[i+1]);
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

	const TrieNodeInfo* Trie::find(const string& uniStr)
	{
		ChUnicode* pUni = new ChUnicode[uniStr.size()];
		for(uint i = 0; i < uniStr.size(); i+=2)
		{
			pUni[i/2] = twocharToUint16(uniStr[i], uniStr[i+1]);
		}
		const TrieNodeInfo* res = find(pUni, uniStr.size()/2);
		delete [] pUni;
		return res;
	}

	const TrieNodeInfo* Trie::find(const ChUnicode* const chUniStr, size_t len)
	{
		if(NULL == _root)
		{
			LogFatal("trie not initted!");
			return NULL;
		}
		TrieNode* p = _root;
		for(uint i = 0; i < len; i++)
		{
			ChUnicode chUni = chUniStr[i];
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

	double Trie::getWeight(const ChUnicode* uniStr, size_t len)
	{
		const TrieNodeInfo* p = find(uniStr, len);
		if(NULL != p)
		{
			return p->weight;
		}
		else
		{
			return getMinWeight();
		}
	}

	double Trie::getWeight(const string& uniStr)
	{
		const TrieNodeInfo * p = find(uniStr);
		if(NULL != p)
		{
			return p->weight;
		}
		else
		{
			return getMinWeight();
		}
	}

	double Trie::getMinWeight()
	{
		return _minWeight;
	}

	int64_t Trie::getTotalCount()
	{
		return _totalCount;
	}

	/*
    bool Trie::cut(const ChUnicode* chUniStr, size_t len, vector< vector<size_t> >& res)
    {
        res.clear();
        //cout<<len<<endl;
        for(size_t i = 0; i < len; i++)
        {
            //cout<<__LINE__<<","<<chUniStr[i]<<endl;
            res.push_back(vector<size_t>());
            vector<size_t>& vec = res[i];
            for(size_t j = i; j < len; j++)
            {
                if(find(chUniStr + i, j - i + 1))
                {
                    vec.push_back(j);
                }
            }
        }
        return true;
    }
	*/


    bool Trie::_destroyNode(TrieNode* node)
    {
        for(TrieNodeMap::iterator it = node->hmap.begin(); it != node->hmap.end(); it++)
        {
            TrieNode* next = it->second;
            _destroyNode(next);
        }
        
        delete node;
        return true;
    }

	bool Trie::_insert(const TrieNodeInfo& nodeInfo)
	{
		_nodeInfoVec.push_back(nodeInfo);
		const string& word = nodeInfo.word;
		ChUnicode chUniStr[bufSize];
		memset(chUniStr, 0, sizeof(chUniStr));
		size_t len = utf8ToUnicode(word.c_str(), word.size(), chUniStr);
		if(0 == len)
		{
			return false;
		}
        TrieNode* p = _root;
        for(int i = 0; i < len; i++)
        {
            ChUnicode cu = chUniStr[i];
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
        p->isLeaf = true;
		if(!_nodeInfoVec.empty())
		{
			p->nodeInfoVecPos = _nodeInfoVec.size() - 1;
		}
		else
		{
			return false;
		}
        return true;
    }

	bool Trie::_countWeight()
	{
		if(_nodeInfoVec.empty() || 0 != _totalCount)
		{
			LogError("_nodeInfoVec is empty or _totalCount has been counted already.");
			return false;
		}
		
		//count total freq
		for(size_t i = 0; i < _nodeInfoVec.size(); i++)
		{
			_totalCount += _nodeInfoVec[i].count;
		}

		if(0 == _totalCount)
		{
			LogError("_totalCount == 0 .");
			return false;
		}
		
		//normalize
		for(uint i = 0; i < _nodeInfoVec.size(); i++)
		{
			TrieNodeInfo& nodeInfo = _nodeInfoVec[i];
			if(0 == nodeInfo.count)
			{
				LogFatal("nodeInfo.count == 0!");
				return false;
			}
			nodeInfo.weight = log(double(nodeInfo.count)/double(_totalCount));
			if(_minWeight > nodeInfo.weight)
			{
				_minWeight = nodeInfo.weight;
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
    trie.init("dicts/segdict.utf8.v2.1");
    //trie.init("dicts/jieba.dict.utf8");
    //trie.init("dict.100");
    //char utf[1024] = "我来到北京清华大学3D电视";
	//trie.display();
	//getchar();
	cout<<trie.getMinWeight()<<endl;
	cout<<trie.getTotalCount()<<endl;
    trie.destroy();
    return 0;
}
#endif
