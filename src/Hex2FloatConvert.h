#ifndef _HEX2FLOATCONVERT_H
#define _HEX2FLOATCONVERT_H
// hexToFloatConvert.cpp : 定义控制台应用程序的入口点。
//
//***************************************************************************************************
//***************************************************************************************************
/*
IEEE标准754短实数R32.23： 
R32.23{小数，指数，符号}  共4个字节 
    2-16                小数                      2-23 
    2-8                  小数                      2-15 
20 2-1              小数                      2-7 
S 27              指数                        21 

小数=F:=UI23[1—23]〈0—1-2-23〉 
指数=E:=UI8[24—31]〈0—255〉 
符号=S:=BS1[32] 
S〈0〉:=正 
S〈1〉:=负 
所表示的实际数值为： 
  （1-2S）*(1+F)*2( E - 127 ) 

备注：数值采用IEEE754浮点短实数，数据位宽度为4，低字节在前，高位在后，符号位(S)1位，阶码(E)8位，尾数(F)23位。
计算公式为（1-2S）X (1+F) X 2的(E-127)次幂
以76.36 对应的那组为例  
（1）d7 a3 98 42 应翻译为  42          98          a3        d7
（2）对应2进制数为         0100 0010 1001 1000 1010 0011 1101 0111
（3）第1位为符号位S        1-2S= 1代表正数
（4）第2位到第9位为次幂E   100 0010 1 对应133 次幂为133-127=6  
（5）第10到第32为数值F      1+F=1.01010001010001111010111  （2进制）
 (6)对应值为        1 X  1.00110001010001111010111  X 2的6次方  = 1001100.01010001111010111 
（7）换算成10进制 约为 76.36

*/
//***************************************************************************************************
//***************************************************************************************************



#include <stdlib.h>     /* strtol */
#include <cmath>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <utils/Logging.h>

using namespace std;

namespace dbdky
{

const uint32_t s = 0x80000000;	//求符号位
const uint32_t r = 0x7F800000;	//求阶码位
const uint32_t m = 0x007FFFFF; //求尾数位
const uint32_t h = 0x00800000; //隐含位

//xinsy20140324
static int32_t getBinExp(uint32_t &data)
{
	uint32_t tmp = ((r&data)>>23);
	int32_t ret;
	if(tmp == 0)
	{
		ret = 1-127;
	}
	else
	{
		ret = tmp - 127;
	}
	string sRet = boost::lexical_cast<string>(ret);
 //       LOG_INFO << "Hex2FloatConvert  sRet : " << sRet;
	return ret;
}
//xinsy20140324
static uint32_t getBinMantissa(uint32_t &data)
{
	return m&data;
}
//xinsy20140324
static uint32_t getBinSign(uint32_t &data)
{
	return (s&data)>>31;
}
//xinsy20140324
static uint32_t getDecInt(uint32_t &data)
{
	int32_t exp		= 0;
	uint32_t mantissa	= 0;
	uint32_t decInt		= 0;
	uint32_t high		= 1;
    	exp = getBinExp(data);
    	mantissa = getBinMantissa(data);
    	//decInt = (m&mantissa)>>(23-exp);
	if(exp>=0)
	{
		decInt = (m&mantissa)>>(23-exp);
		high = high<<exp;
	}
	else
	{
		decInt = 0;
		high = 0;
	}
	decInt |= high;
	string sDecInt = boost::lexical_cast<string>(decInt);
   //     LOG_INFO << "Hex2FloatConvert  sDecInt : " << sDecInt;
    	//decInt |= (high = high<<exp);
    	return decInt;

}
//xinsy20140324
static float getDecfraction(uint32_t &data)
{
	uint32_t distillBit	= 1;					
	int32_t exp		= getBinExp(data) ;				//提取位 保存阶码;
	uint32_t mantissa    	= getBinMantissa(data);
    	float 	result		= 0.0000;					// 保存计算出来的结果
	int32_t loop		= 22-exp;
	int32_t absExp		= abs(exp);

	string sMantissa = boost::lexical_cast<string>(mantissa);
     //   LOG_INFO << "Hex2FloatConvert  sMantissa111 : " << sMantissa;

	if(exp!=(-126))
	{
		mantissa |= h;							//小数前边默认带一位1即1.m

		if(exp<0)
		{
			if(absExp>8)						//当阶码小于零，在小数前需要补零时，需要保证补零后的小数不能超过32位
			{
				exp = absExp-8-1;
				mantissa = mantissa/(pow(2.0000,exp+0.0000));
			}
		}
	}
	sMantissa = boost::lexical_cast<string>(mantissa);
     //  LOG_INFO << "Hex2FloatConvert  sMantissa222 : " << sMantissa;
	if(loop>=32)	//当loop 超过32位时，位左移或右移时实际移动的位是loop%32位，和预想效果不符需要修正。
	{
		loop = 31;
	}
    	for(int i=0;i<=loop;i++)
	{
        	if((mantissa&(distillBit<<(loop-i)))>>(loop-i)) 
		{
			result = result+1/pow(2.0000,i+1+0.0000);//计算x的y次幂
		}
	}
	string sResult = boost::lexical_cast<string>(result);
      //  LOG_INFO << "Hex2FloatConvert  sResult : " << sResult;
    	return result;

}
//xinsy20140324
static float getDecNumber(uint32_t &data)
{
	float result = getDecInt(data)+ getDecfraction(data);
	uint32_t sign = getBinSign(data);
    	return (1-2*sign)?result:-result;	
}


}

#endif /*_HEX2FLOATCONVERT_H*/
