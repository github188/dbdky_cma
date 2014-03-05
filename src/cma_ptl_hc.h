#ifndef __CMA_PTL_HC_H_
#define __CMA_PTL_HC_H_

#include <map>
#include <string>
#include "cma_frame.h"

using std::string;
using std::map;
using dbdky::cma_server::cma_frame;

namespace dbdky
{
namespace cma_dxhc_parser
{
	extern map<string,string> frameParserFunc(cma_frame& frm);
	extern string sqlInsertStringMaker(const cma_frame& frm);
}
}


#endif    //__CMA_PTL_HC_H_
