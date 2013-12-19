#ifndef CPPJIEBA_TRIEMANAGER_H
#define CPPJIEBA_TRIEMANAGER_H

#include "Trie.hpp"
#include "Limonp/md5.hpp"
#include "Limonp/logger.hpp"

namespace CppJieba
{
    using namespace Limonp;
    class TrieManager
    {
        private:
            unordered_map<std::string, Trie*> _tries;
            TrieManager(){};
            TrieManager(TrieManager& tm){};
        public:
            Trie* getTrie(const char* dictpath) 
            {
                string md5;
                if (!md5File(dictpath, md5))
                {
                    LogError("error when getting md5 for file '%s'", dictpath);
                    return NULL;
                }
                else
                {
                    LogInfo("md5 for file '%s': %s", dictpath, md5.c_str());
                    if (_tries.find(md5) == _tries.end())
                    {
                        LogInfo("create a new trie for md5: '%s'", md5.c_str());
                        Trie* trie = NULL;
                        try
                        {
                            trie = new Trie();
                        }
                        catch (const bad_alloc& e)
                        {
                            LogError("error when new a trie for file '%s'", dictpath);
                            return NULL;
                        }
                        if (NULL == trie)
                            return NULL;

                        if (!trie->init())
                        { 
                            LogError("trie init error for file '%s'", dictpath);
                            return NULL;
                        }

                        LogInfo("trie->loadDict(%s) start...", dictpath);
                        if (!trie->loadDict(dictpath))
                        {
                            LogError("trie->loadDict(%s) failed...", dictpath);
                            return NULL;
                        }
                        LogInfo("trie->loadDict end...");

                        _tries[md5.c_str()] = trie;
                        return trie;
                    }
                    else
                    {
                        LogInfo("find a exits trie for md5: '%s'", md5.c_str());
                        return _tries[md5.c_str()];
                    }
                }
            }

            static TrieManager& getInstance()
            {
                static TrieManager _this;
                return _this;
            }
    };
}
#endif
