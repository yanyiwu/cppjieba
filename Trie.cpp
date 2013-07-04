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

    Trie::Trie()
    {
        _root = NULL;
    }
    
    Trie::~Trie()
    {
        destroy();
    }
    
    bool Trie::init(const char* const filepath)
    {
        char msgBuf[512];
        ChUnicode chUniBuf[512];
        if(NULL != _root)
        {
            LogError("already initted!");
            return false;
        }
        _root = new TrieNode;
        ifstream ifile(filepath);
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
            size_t uniLen = utf8ToUnicode(chWord.c_str(), chWord.size(), chUniBuf);
            _insert(chUniBuf, uniLen, count, tag);
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
    }

    void Trie::display()
    {
        _display(_root, 0);
    }

    bool Trie::find(const ChUnicode* chUniStr, size_t len)
    {
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

    bool Trie::cut(const ChUnicode* chUniStr, size_t len, vector< vector<size_t> >& res)
    {
        res.clear();
        cout<<len<<endl;
        for(size_t i = 0; i < len; i++)
        {
            cout<<__LINE__<<","<<chUniStr[i]<<endl;
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

    bool Trie::_insert(const ChUnicode* chUniStr, size_t len, unsigned int cnt, const string& tag)
    {
        if(0 == len)
        {
            LogError("input args illegal: len == 0");
            return false;
        }
        TrieNode* p = _root;
        for(int i = 0; i < len; i++)
        {
            ChUnicode cu = chUniStr[i];
            if(p->hmap.end() == p->hmap.find(cu))
            {
                TrieNode * next = new TrieNode;
                p->hmap[cu] = next;
                p = next;
            }
            else
            {
                p = p->hmap[cu];
            }
        }
        p->isLeaf = true;
        return true;
    }
}

#ifdef TRIE_UT
using namespace CppJieba;
int main()
{
    Trie trie;
    trie.init("dict.utf8");
    //trie.init("test/dict.txt");
    //cout<<trie.begin()->count<<endl;
    
    //return 0;
    //trie.init("dict.txt");
    //trie.display();
    //const char * utf = "B";
    //ChUnicode chUniStr[16];
    //int uniLen = utf8ToUnicode(utf, sizeof(utf), chUniStr);
    //cout<<trie.find(chUniStr, uniLen)<<endl;
    char utf[1024] = "我来到北京清华大学";
    char buf[1024];
    ChUnicode chUniStr[1024];
    //cout<<sizeof(utf)<<endl;
    int uniLen = utf8ToUnicode(utf, strlen(utf), chUniStr);
    vector< vector<size_t> > res;
    cout<<trie.cut(chUniStr, uniLen, res)<<endl;
    PRINT_MATRIX(res);
    FOR_VECTOR(res, i)
    {
        FOR_VECTOR(res[i], j)
        {
            unicodeToUtf8(chUniStr + i, res[i][j] - i + 1, buf);
            cout<<buf<<endl;
        }
    }
    trie.destroy();
    //hash_map<ChUnicode, int> hmap;
    //hmap[136]=1;
    return 0;
}
#endif
