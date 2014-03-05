#include "cma_ptlrender.h"
#include <utils/Logging.h>


#include "cma_ptl_fbjc.h"
#include "cma_ptl_dxwd.h"			//xinsy20140213
#include "cma_ptl_gt.h"				//xinsy20140213
#include "cma_ptl_qx.h"				//xinsy20140213
#include "cma_ptl_zdtz.h"			//xinsy20140220
#include "cma_ptl_hc.h"				//xinsy20140220
#include "cma_ptl_fp.h"				//xinsy20140227
#include "cma_ptl_wh.h"				//xinsy20140227
#include "cma_ptl_wdtz.h"			//xinsy20140227

namespace dbdky
{
namespace cma_server
{
    cma_ptlrender* cma_ptlrender::instance_ = NULL;
   
    cma_ptlrender* cma_ptlrender::getInstance()
    {
        if (NULL == instance_)
        {
            instance_ = new cma_ptlrender();
        }

        return instance_;
    }

    CmaSqlInsertStringMaker cma_ptlrender::renderSqlInsertStringMaker(const cma_frame& frm)
    {
        switch (frm.getPtype())
        {
            	case cma_frame::CMA_PTYPE_QX:		//xinsy20140213
		{
			return dbdky::cma_qx_parser::sqlInsertStringMaker;
		}
            	case cma_frame::CMA_PTYPE_GT:		//xinsy20140213
		{
			return dbdky::cma_gt_parser::sqlInsertStringMaker;
		}
            	case cma_frame::CMA_PTYPE_ZDTZ:		//xinsy20140220
		{
			return dbdky::cma_zdtz_parser::sqlInsertStringMaker;
		}
            	case cma_frame::CMA_PTYPE_ZDBX:
		{
			break;
		}
            	case cma_frame::CMA_PTYPE_DXHC:		//xinsy20140220
		{
			return dbdky::cma_dxhc_parser::sqlInsertStringMaker;
		}
            	case cma_frame::CMA_PTYPE_WD:		//xinsy20140213
		{
			return dbdky::cma_dxwd_parser::sqlInsertStringMaker;
		}
           	case cma_frame::CMA_PTYPE_FP:		//xinsy20140227
		{
			return dbdky::cma_fp_parser::sqlInsertStringMaker;
		}
           	case cma_frame::CMA_PTYPE_WDTZ:		//xinsy20140227
		{
			return dbdky::cma_wdtz_parser::sqlInsertStringMaker;
		}
            	case cma_frame::CMA_PTYPE_WDGJ:
		{
			break;
		}
            	case cma_frame::CMA_PTYPE_WH:		//xinsy20140227
            	{
                	return dbdky::cma_wh_parser::sqlInsertStringMaker;
            	}
            	case cma_frame::CMA_PTYPE_FB:
            	{
                	return dbdky::cma_fbjc_parser::sqlInsertStringMaker;
            	}
            	default:
            	{
                	return defaultSqlInsertStringMaker;
            	} 
        }

        return defaultSqlInsertStringMaker;
    }
 
    CmaFrameParserFunc cma_ptlrender::renderParser(const cma_frame& frm)
    {
        switch (frm.getPtype())
        {
	    	case cma_frame::CMA_PTYPE_QX:
		{
			return dbdky::cma_qx_parser::frameParserFunc;
		}
	    	case cma_frame::CMA_PTYPE_GT:		//xinsy20140213
		{
			return dbdky::cma_gt_parser::frameParserFunc;
		}
	    	case cma_frame::CMA_PTYPE_ZDTZ:
		{
			return dbdky::cma_zdtz_parser::frameParserFunc;
		}
	    	case cma_frame::CMA_PTYPE_ZDBX:
		{
            		return defaultParseFunc;
           	}
	    	case cma_frame::CMA_PTYPE_DXHC:		//xinsy20140220
		{
            		return dbdky::cma_dxhc_parser::frameParserFunc;
           	}
	    	case cma_frame::CMA_PTYPE_WD:		//xinsy20140213
		{
			return dbdky::cma_dxwd_parser::frameParserFunc;
		}
        	case cma_frame::CMA_PTYPE_FP:		//xinsy20140227
		{
			return dbdky::cma_fp_parser::frameParserFunc;
		}
        	case cma_frame::CMA_PTYPE_WDTZ:		//xinsy20140227
		{
			return dbdky::cma_wdtz_parser::frameParserFunc;
		}
        	case cma_frame::CMA_PTYPE_WDGJ:
		{
            		return defaultParseFunc;
           	}
        	case cma_frame::CMA_PTYPE_WH:		//xinsy20140227
        	{
			return dbdky::cma_wh_parser::frameParserFunc;
		}
        	case cma_frame::CMA_PTYPE_FB:
        	{
            		return dbdky::cma_fbjc_parser::frameParserFunc;
        	}
        	default:
        	{
           	 	return defaultParseFunc;
        	}
        }

        return defaultParseFunc; 
    } 

    map<string,string> defaultParseFunc(cma_frame& frm)
    {
        LOG_INFO << "DEFAULTPARSEFUNC";
        map<string,string> ret;
        return ret;
    }

    string defaultSqlInsertStringMaker(const cma_frame& frm)
    {
        LOG_INFO << "DEFAULTSQLINSERTSTRINGMAKER";
        return "";
    }
    
}
}
