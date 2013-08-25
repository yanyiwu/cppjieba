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
			enum {B = 0, E = 1, M = 2, S = 3, STATUS_SUM = 4};
			char _statMap[STATUS_SUM];
			double _startProb[STATUS_SUM];
			double _transProb[STATUS_SUM][STATUS_SUM];
			EmitProbMap _emitProbB;
			EmitProbMap _emitProbE;
			EmitProbMap _emitProbM;
			EmitProbMap _emitProbS;
			vector<EmitProbMap* > _emitProbVec;
			
		public:
			HMMSegment();
			~HMMSegment();
		public:
			bool init();
			bool dispose();
		public:
			bool loadModel(const char* const filePath);
			bool cut(const string& str, vector<string>& res);
			bool viterbi(const vector<uint16_t>& unico, vector<uint>& status);
		private:
			bool _getLine(ifstream& ifile, string& line);
			bool _loadEmitProb(const string& line, EmitProbMap& mp);
			bool _decodeOne(const string& str, uint16_t& res);
			double _getEmitProb(const EmitProbMap& mp, uint16_t key, double defVal);
			double _getEmitProb(const EmitProbMap* ptMp, uint16_t key, double defVal);

			
	};
}

#endif
