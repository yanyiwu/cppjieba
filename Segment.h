#ifndef SEGMENT_H
#define SEGMENT_H

#include "Trie.h"

namespace CppJieba
{
	class Segment
	{
		private:
			Trie _trie;
		public:
			Segment();
			~Segment();
		public:
			bool init(const char* const dictFilePath);
			bool destroy();
		public:
			bool cutRMM(const string& chStr, vector<string>& res);

		private:
			enum {bufSize = 1024};
			
	};
}

#endif
