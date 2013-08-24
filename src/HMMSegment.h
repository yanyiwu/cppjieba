#ifndef CPPJIBEA_HMMSEGMENT_H
#define CPPJIBEA_HMMSEGMENT_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include "cppcommon/headers.h"
#include "globals.h"
#include "TransCode.h"

namespace CppJieba
{
	using namespace CPPCOMMON;
	class HMMSegment
	{
		private:
			/*
			 * STATUS:
			 * 0:B, 1:E, 2:M, 3:S
			 * */
			enum {STATUS_SUM = 4};
			double _startProb[STATUS_SUM];
			double _transProb[STATUS_SUM][STATUS_SUM];
			EmitProbMap _emitProbB;
			EmitProbMap _emitProbE;
			EmitProbMap _emitProbM;
			EmitProbMap _emitProbS;
			
		public:
			HMMSegment();
			~HMMSegment();
		public:
			bool init();
			bool dispose();
		public:
			bool loadModel(const char* const filePath);
		private:
			bool _getLine(ifstream& ifile, string& line);
			bool _loadEmitProb(const string& line, EmitProbMap& mp);
			bool _decodeOne(const string& str, uint16_t& res);
	};
}

#endif
