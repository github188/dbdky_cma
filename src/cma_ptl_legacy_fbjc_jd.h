#ifndef __CMA_PTL_LEGACY_FBJC_JD_H_
#define __CMA_PTL_LEGACY_FBJC_JD_H_

#include <map>
#include <string>
#include "cma_frame_legacy.h"

using std::string;
using std::map;
using dbdky::cma_server::cma_frame_legacy;

namespace dbdky
{
namespace cma_legacy_fbjc_jd_parser
{
	extern map<string,string> frameParserFunc(cma_frame_legacy& frm);
	extern string sqlInsertStringMaker(const cma_frame_legacy& frm);
}
}

#endif  //__CMA_PTL_LEGACY_FBJC_JD_H_