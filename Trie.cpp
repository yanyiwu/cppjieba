#include "Trie.h"

namespace CppJieba
{
    Trie::Trie()
    {
    }
    
    Trie::~Trie()
    {
    }
    
    bool Trie::init(const char* const filepath)
    {
        ifstream ifile(filepath);
        string line;
        vector<string> vecBuf;
        while(getline(ifile, line))
        {
            vecBuf.clear();
            splitStr(line, vecBuf, " ");
            PRINT_VECTOR(vecBuf);
            getchar();
            uint16_t strbuf[1024];

            size_t unilen = utf8ToUnicode(line.c_str(), line.size(), strbuf);
            for(int i = 0; i < unilen; i++)
            {
                // printf("%x\n", strbuf[i]);
            }
            char utf8str[512]={0};
            unicodeToUtf8(strbuf, unilen, utf8str);
            //cout<<strlen(utf8str);
            cout<<utf8str<<endl;
            
        }
    }
    
}

#ifdef TRIE_UT
using namespace CppJieba;
int main()
{
    Trie trie;
    trie.init();
    return 0;
}
#endif
