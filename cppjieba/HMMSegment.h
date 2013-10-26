#ifndef CPPJIBEA_HMMSEGMENT_H
#define CPPJIBEA_HMMSEGMENT_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <str_functs.hpp>
#include <logger.hpp>
#include "globals.h"
#include "TransCode.hpp"
#include "ISegment.hpp"
#include "SegmentBase.hpp"

namespace CppJieba
{
    using namespace Limonp;
    class HMMSegment: public SegmentBase
    {
        public:
            /*
             * STATUS:
             * 0:B, 1:E, 2:M, 3:S
             * */
            enum {B = 0, E = 1, M = 2, S = 3, STATUS_SUM = 4};
        private:
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
            virtual ~HMMSegment();
        public:
            bool init(const char* const modelPath);
            bool dispose();
        public:
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<Unicode>& res)const ;
            bool cut(const string& str, vector<string>& res)const;
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const;
            //virtual bool cut(const string& str, vector<string>& res)const;

        private:
            bool _viterbi(Unicode::const_iterator begin, Unicode::const_iterator end, vector<uint>& status)const;
            bool _loadModel(const char* const filePath);
            bool _getLine(ifstream& ifile, string& line);
            bool _loadEmitProb(const string& line, EmitProbMap& mp);
            bool _decodeOne(const string& str, uint16_t& res);
            double _getEmitProb(const EmitProbMap* ptMp, uint16_t key, double defVal)const ;

            
    };
}

#endif
