#ifndef __CMA_PTL_LEGACY_DXWD_H_
#define __CMA_PTL_LEGACY_DXWD_H_

#include <map>
#include <string>
#include "cma_frame.h"

using std::string;
using std::map;
using dbdky::cma_server::cma_frame_legacy;

namespace dbdky
{
namespace cma_legacy_dxwd_parser
{
extern map<string,string> frameParserFunc(cma_frame_legacy& frm);
extern string sqlInsertStringMaker(const cma_frame_legacy& frm);
}
}


#endif    //__CMA_PTL_LEGACY_DXWD_H_
