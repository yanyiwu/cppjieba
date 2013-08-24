#include "HMMSegment.h"

namespace CppJieba
{
	HMMSegment::HMMSegment()
	{
		memset(_startProb, 0, sizeof(_startProb));
		memset(_transProb, 0, sizeof(_transProb));
	}
	
	HMMSegment::~HMMSegment()
	{
		
	}

	bool HMMSegment::init()
	{
		return true;
	}
	
	bool HMMSegment::dispose()
	{
		return true;
	}

	bool HMMSegment::loadModel(const char* const filePath)
	{
		ifstream ifile(filePath);
		string line;
		vector<string> tmp;
		vector<string> tmp2;

		//load _startProb
		if(!_getLine(ifile, line))
		{
			return false;
		}
		splitStr(line, tmp, " ");
		if(tmp.size() != STATUS_SUM)
		{
			LogError("start_p illegal");
			return false;
		}
		for(uint j = 0; j< tmp.size(); j++)
		{
			_startProb[j] = atof(tmp[j].c_str());
			//cout<<_startProb[j]<<endl;
		}

		//load _transProb
		for(uint i = 0; i < STATUS_SUM; i++)
		{
			if(!_getLine(ifile, line))
			{
				return false;
			}
			splitStr(line, tmp, " ");
			if(tmp.size() != STATUS_SUM)
			{
				LogError("trans_p illegal");
				return false;
			}
			for(uint j =0; j < STATUS_SUM; j++)
			{
				_transProb[i][j] = atof(tmp[j].c_str());
				//cout<<_transProb[i][j]<<endl;
			}
		}

		//load _emitProbB
		if(!_getLine(ifile, line) || !_loadEmitProb(line, _emitProbB))
		{
			return false;
		}
		
		//load _emitProbE
		if(!_getLine(ifile, line) || !_loadEmitProb(line, _emitProbE))
		{
			return false;
		}
		
		//load _emitProbM
		if(!_getLine(ifile, line) || !_loadEmitProb(line, _emitProbM))
		{
			return false;
		}

		//load _emitProbS
		if(!_getLine(ifile, line) || !_loadEmitProb(line, _emitProbS))
		{
			return false;
		}

		return true;
	}

	bool HMMSegment::_getLine(ifstream& ifile, string& line)
	{
		while(getline(ifile, line))
		{
			trim(line);
			if(line.empty())
			{
				continue;
			}
			if(strStartsWith(line, "#"))
			{
				continue;
			}
			return true;
		}
		return false;
	}

	bool HMMSegment::_loadEmitProb(const string& line, EmitProbMap& mp)
	{
		if(line.empty())
		{
			return false;
		}
		vector<string> tmp, tmp2;
		uint16_t unico;
		splitStr(line, tmp, ",");
		for(uint i = 0; i < tmp.size(); i++)
		{
			splitStr(tmp[i], tmp2, ":");
			if(2 != tmp2.size())
			{
				LogError("_emitProb illegal.");
				return false;
			}
			if(!_decodeOne(tmp2[0], unico))
			{
				LogError("TransCode failed.");
				return false;
			}
			mp[unico] = atof(tmp2[1].c_str());
		}
		return true;
	}

	bool HMMSegment::_decodeOne(const string& str, uint16_t& res)
	{
		vector<uint16_t> ui16;
		if(!TransCode::strToVec(str, ui16) || ui16.size() != 1)
		{
			return false;
		}
		res = ui16[0];
		return true;
	}

}


#ifdef HMMSEGMENT_UT
using namespace CppJieba;

int main()
{
	TransCode::setUtf8Enc();
	HMMSegment hmm;
	hmm.loadModel("../dicts/hmm_model.utf8");
    //cout<<MIN_DOUBLE<<endl;
	
	return 0;
}

#endif
