#ifndef CPPJIBEA_HMMSEGMENT_H
#define CPPJIBEA_HMMSEGMENT_H

namespace CppJieba
{
	class HMMSegment
	{
		public:
			HMMSegment();
			~HMMSegment();
		public:
			bool init();
			bool dispose();
		public:
			bool loadModel();
	};
}

#endif
