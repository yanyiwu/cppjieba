#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

namespace CppJieba
{
    class SegmentBase
    {
        public:
            SegmentBase();
            ~SegmentBase();
        private:
            bool _isInited;
        protected:
            bool _getInitFlag()const{return _isInited;};
            bool _setInitFlag(bool flag){return _isInited = flag;};
    };
}

#endif
