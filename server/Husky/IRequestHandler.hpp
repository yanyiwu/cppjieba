#ifndef HUSKY_IREQUESTHANDLER_HPP
#define HUSKY_IREQUESTHANDLER_HPP

#include "HttpReqInfo.hpp"

namespace Husky {
class IRequestHandler {
 public:
  virtual ~IRequestHandler() {};
 public:
  virtual bool do_GET(const HttpReqInfo& httpReq, string& res) const = 0;
  virtual bool do_POST(const HttpReqInfo& httpReq, string& res) const = 0;
};
}

#endif
