#ifndef __CMA_CALLBACKS_H_ 
#define __CMA_CALLBACKS_H_

#include <boost/function.hpp>
#include <map>
#include <string>

using std::string;
using std::map;

namespace dbdky
{
namespace cma_server
{
class cma_frame;
typedef boost::function<map<string,string> (const cma_frame&)> CmaFrameParserFunc;

map<string,string> defaultParseFunc(const cma_frame& frm);
}
}

#endif    //__CMA_CALLBACKS_H_
