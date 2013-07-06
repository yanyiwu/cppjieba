#include "Trie.h"

namespace CppJieba
{
    Trie::iterator Trie::begin()
    {
        return Trie::iterator(_root);
    }

    Trie::iterator Trie::end()
    {
        return Trie::iterator(NULL);
    }

    Trie::Trie():_root(NULL), _nodeInfoVec(), _totalWeight(0)
    {
    }
    
    Trie::~Trie()
    {
        destroy();
    }

	bool Trie::init(const char* const filePath)
	{
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
        char msgBuf[bufSize];
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
                sprintf(msgBuf, "line[%s] illegal.", line.c_str());
                LogError(msgBuf);
                return false;
            }
            string chWord = vecBuf[0];
            unsigned int count = atoi(vecBuf[1].c_str());
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
        _display(_root, 0);
    }

    bool Trie::find(const ChUnicode* chUniStr, size_t len)
    {
		int res = -1;
        TrieNode* p = _root;
        for(size_t i = 0; i < len; i++)
        {
            ChUnicode chUni = chUniStr[i];
            if(p->hmap.find(chUni) == p->hmap.end())
            {
                return false;
            }
            else
            {
                p = p->hmap[chUni];
            }
        }
        return p->isLeaf;
    }

	/*
	bool Trie::find(const vector<ChUnicode>& uniVec)
	{
		TrieNode * p = _root;
		for(size_t i = 0; i < uniVec.size(); i++)
		{
			ChUnicode chUni = uniVec[i];
			if(p->hmap.find(chUni) == p->hmap.end())
			{
				return false;
			}
			else
			{
				p = p-> hmap[chUni];
			}
		}
		return p->isLeaf;
	}
	*/

	int Trie::findMaxMatch(const ChUnicode* chUniStr, size_t len)
	{
		int res = -1;
		TrieNode * p = _root;
		for(int i = 0; i < len; i++)
		{
			ChUnicode chWord = chUniStr[i];
			TrieNodeHashMap::const_iterator iter = p->hmap.find(chWord);
			if(iter != p->hmap.end())
			{
				TrieNode * next = iter->second;
				if(next->isLeaf)
				{
					res = i + 1;
				}
				p = next;
			}
			else
			{
				break;
			}
		}
		cout<<__FILE__<<__LINE__<<res<<endl;
		return res;
	}

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

	bool Trie::cutUtf8(const string& str, vector< vector<size_t> >& res)
	{
		ChUnicode buf[ChUniMaxLen];
		size_t len = utf8ToUnicode(str.c_str(), str.size(), buf);
		if(0 ==  len)
		{
		  return false;
		}
		return cut(buf, len, res);
		/*
		PRINT_MATRIX(res);
		char buf[1024];
		FOR_VECTOR(res, i)
		{
			FOR_VECTOR(res[i], j)
			{
				unicodeToUtf8(chUniStr + i, res[i][j] - i + 1, buf);
				cout<<buf<<endl;
			}
		}*/
	}

    bool Trie::_destroyNode(TrieNode* node)
    {
        for(TrieNodeHashMap::iterator it = node->hmap.begin(); it != node->hmap.end(); it++)
        {
            TrieNode* next = it->second;
            _destroyNode(next);
        }
        
        delete node;
        return true;
    }

    void Trie::_display(TrieNode* node, int level)
    {
        if(NULL == node)
        {
            LogError("failed! node is null.");
            return;
        }
        for(TrieNodeHashMap::const_iterator it = node->hmap.begin(); it != node->hmap.end(); it++)
        {
            char utfBuf[8];
            ChUnicode chBuf[1];
            for(int i = 0; i < level; i++)
            {
                cout<<" ";
            }
            chBuf[0]=it->first;
            unicodeToUtf8(chBuf, 1, utfBuf);
            cout<<utfBuf<<endl;
            _display(it->second, level + 1);
        }
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
		if(_nodeInfoVec.empty() || 0 != _totalWeight)
		{
			LogError("_nodeInfoVec is empty or _totalWeight has been counted already.");
			return false;
		}
		
		//count total freq
		for(size_t i = 0; i < _nodeInfoVec.size(); i++)
		{
			_totalWeight += _nodeInfoVec[i].count;
			//cout<<_nodeInfoVec[i].word<<_nodeInfoVec[i].count<<endl;
		}
		
		//normalize
		for(size_t i = 0; i < _nodeInfoVec.size(); i++)
		{
			_nodeInfoVec[i].weight = log(double(_nodeInfoVec[i].count)/double(_totalWeight));
			//cout<<_nodeInfoVec[i].weight<<endl;
		}
		
		return true;
	}
}

#ifdef TRIE_UT
using namespace CppJieba;
int main()
{
    Trie trie;
    trie.init("jieba.dict.utf8");
    //char utf[1024] = "我来到北京清华大学3D电视";
    trie.destroy();
    return 0;
}
#endif
