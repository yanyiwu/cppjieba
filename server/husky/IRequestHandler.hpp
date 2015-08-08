#ifndef HUSKY_IREQUESTHANDLER_HPP
#define HUSKY_IREQUESTHANDLER_HPP

#include "HttpReqInfo.hpp"

namespace husky {
class IRequestHandler {
 public:
  virtual ~IRequestHandler() {
  }

  virtual bool doGET(const HttpReqInfo& httpReq, string& res) = 0;
  virtual bool doPOST(const HttpReqInfo& httpReq, string& res) = 0;
};
}

#endif
