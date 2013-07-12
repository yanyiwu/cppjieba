#ifndef CPPJIEBA_KEYWORDEXT_H
#define CPPJIEBA_KEYWORDEXT_H

#include "Segment.h"

namespace CppJieba
{
	class KeyWordExt
	{
		private:
			Segment _segment;
		public:
			KeyWordExt();
			~KeyWordExt();
			bool init(const char * const filePath);
			bool destroy();

	};

}

#endif
