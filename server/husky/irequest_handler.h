#ifndef HUSKY_IREQUESTHANDLER_HPP
#define HUSKY_IREQUESTHANDLER_HPP

#include "http_req_info.h"

namespace husky {
class IRequestHandler {
 public:
  virtual ~IRequestHandler() {
  }

  virtual bool DoGET(const HttpReqInfo& httpReq, string& res) = 0;
  virtual bool DoPOST(const HttpReqInfo& httpReq, string& res) = 0;
};
}

#endif
