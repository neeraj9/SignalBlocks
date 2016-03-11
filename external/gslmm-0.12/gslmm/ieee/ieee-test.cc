//
// $Id: ieee-test.cc,v 1.6 2004-11-06 00:49:46 cholm Exp $ 
//  
//  gslmm::error
//  Copyright (C) 2002 Christian Holm Christensen <cholm@nbi.dk> 
//
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public License 
//  as published by the Free Software Foundation; either version 2.1 
//  of the License, or (at your option) any later version. 
//
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free 
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
//  02111-1307 USA 
//
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef __IOMANIP__
#include <iomanip>
#endif
#ifndef GSLMM_ieee
#include <gslmm/ieee/ieee.hh>
#endif
#ifndef GSLMM_ieee_representation
#include <gslmm/ieee/ieee_representation.hh>
#endif
#ifndef GSLMM_test_suite
#include <gslmm/test/test_suite.hh>
#endif
#ifndef FLT_MIN
#define FLT_MIN 1.17549435e-38f
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38f
#endif

#ifndef DBL_MIN
#define DBL_MIN 2.2250738585072014e-308
#endif

#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623157e+308
#endif

/** @file   ieee/ieee-test.cc
    @author Christian Holm
    @date   Mon Sep 16 13:03:50 2002
    @brief  Test of ieee classses.  */

//____________________________________________________________________  
int main(int argc, char** argv) 
{
  gslmm::test_suite ts("ieee", argc, argv);

  float zerof = 0.0f, minus_onef = -1.0f ;
  double zero = 0.0, minus_one = -1.0 ;

  // Check for +ZERO (float)
  {
    float f = 0.0f;
    std::bitset<23> mantissa(std::string("00000000000000000000000"));
    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = 0, sign is +");
    ts.test(r.exponent(), -127, "float x = 0, exponent is -127");
    ts.test(r.mantissa(), mantissa, "float x = 0, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::zero), 
	    "float x = 0, type is ZERO");
  }

  // Check for -ZERO (float)
  {
    float f = minus_onef;
    std::bitset<23> mantissa(std::string("00000000000000000000000"));
    
    while (f < 0) {
      f *= 0.1f;
    }

    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 1, "float x = -1*0, sign is -");
    ts.test(r.exponent(), -127, "float x = -1*0, exponent is -127");
    ts.test(r.mantissa(), mantissa, "float x = -1*0, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::zero), 
	    "float x = -1*0, type is ZERO");
  }

  // Check for a positive NORMAL number (e.g. 2.1) (float)
  {
    float f = 2.1f;
    std::bitset<23> mantissa(std::string("00001100110011001100110"));

    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = 2.1, sign is +");
    ts.test(r.exponent(), 1, "float x = 2.1, exponent is 1");
    ts.test(r.mantissa(), mantissa, "float x = 2.1, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal), 
	    "float x = 2.1, type is NORMAL");
  }


  // Check for a negative NORMAL number (e.g. -1.3304...) (float)
  {
    float f = -1.3303577090924210f ;
    std::bitset<23> mantissa(std::string("01010100100100100101001"));

    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 1, "float x = -1.3304..., sign is -");
    ts.test(r.exponent(), 0, "float x = -1.3304..., exponent is 0");
    ts.test(r.mantissa(), mantissa, "float x = -1.3304..., mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal),
	    "float x = -1.3304..., type is NORMAL");
  }

  // Check for a large positive NORMAL number (e.g. 3.37e31) (float)
  {
    float f = 3.37e31f;
    std::bitset<23> mantissa(std::string("10101001010110101001001"));
    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = 3.37e31, sign is +");
    ts.test(r.exponent(), 104, "float x = 3.37e31, exponent is 104");
    ts.test(r.mantissa(), mantissa, "float x = 3.37e31, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal), 
	    "float x = 3.37e31, type is NORMAL");
  }

  // Check for a small positive NORMAL number (e.g. 3.37e-31) (float)
  {
    float f = 3.37e-31f;
    std::bitset<23> mantissa(std::string("10110101011100110111011"));

    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = 3.37e-31, sign is +");
    ts.test(r.exponent(), -102, "float x = 3.37e-31, exponent is -102");
    ts.test(r.mantissa(), mantissa, "float x = 3.37e-31, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal),
	    "float x = 3.37e-31, type is NORMAL");
  }

  // Check for FLT_MIN (smallest possible number that is not denormal)
  {
    float f = FLT_MIN;	
    std::bitset<23> mantissa(std::string("00000000000000000000000"));
    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = FLT_MIN, sign is +");
    ts.test(r.exponent(), -126, "float x = FLT_MIN, exponent is -126");
    ts.test(r.mantissa(), mantissa, "float x = FLT_MIN, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal), 
	    "float x = FLT_MIN, type is NORMAL");
  }

  // Check for FLT_MAX (largest possible number that is not Inf)
  {
    float f = FLT_MAX;
    std::bitset<23> mantissa(std::string("11111111111111111111111"));

    gslmm::ieee_representation<float> r(f);

    ts.test(r.sign(), 0, "float x = FLT_MAX, sign is +");
    ts.test(r.exponent(), 127, "float x = FLT_MAX, exponent is 127");
    ts.test(r.mantissa(), mantissa, "float x = FLT_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::normal), 
	    "float x = FLT_MAX, type is NORMAL");
  }


  // Check for DENORMAL numbers (e.g. FLT_MIN/2^n)
#ifdef TEST_DENORMAL
  {
    float f = FLT_MIN;	
    char mantissa[] = "10000000000000000000000";

    int i;

    for (i = 0; i < 23; i++)
      {
	float x = f / (float)pow (2.0, 1 + (float) i);
	mantissa[i] = '1';
	gslmm::ieee_representation<float> r(x);

	ts.test(r.sign(), 0, "float x = FLT_MIN/2^%d, sign is +", i + 1);
	ts.test(r.exponent(), -127,
		"float x = FLT_MIN/2^%d, exponent is -127", i + 1);
	ts.test(r.mantissa(), mantissa,
		"float x = FLT_MIN/2^%d, mantissa", i + 1);
	ts.test(r.type(), int(gslmm::ieee_representation<float>::denormal),
		"float x = FLT_MIN/2^%d, type is DENORMAL", i + 1);
	mantissa[i] = '0';
      }
  }
#endif

  // Check for positive INFINITY (e.g. 2*FLT_MAX)
  {
    float f = FLT_MAX;	
    std::bitset<23> mantissa(std::string("00000000000000000000000"));


    float x;
    x = 2 * f;
    gslmm::ieee_representation<float> r(x);

    ts.test(r.sign(), 0, "float x = 2*FLT_MAX, sign is +");
    ts.test(r.exponent(), 128, "float x = 2*FLT_MAX, exponent is 128");
    ts.test(r.mantissa(), mantissa, "float x = 2*FLT_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::infinity), 
	    "float x = -2*FLT_MAX, type is INF");
  }

  // Check for negative INFINITY (e.g. -2*FLT_MAX)
  {
    float f = FLT_MAX;	
    std::bitset<23> mantissa(std::string("00000000000000000000000"));


    float x;
    x = -2 * f;
    gslmm::ieee_representation<float> r(x);

    ts.test(r.sign(), 1, "float x = -2*FLT_MAX, sign is -");
    ts.test(r.exponent(), 128, "float x = -2*FLT_MAX, exponent is 128");
    ts.test(r.mantissa(), mantissa, "float x = -2*FLT_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::infinity), 
	    "float x = -2*FLT_MAX, type is INF");
  }

  // Check for NAN (e.g. Inf - Inf) (float)
  {
    float x = 1.0f, y = 2.0f, z = zerof;

    x = x / z;
    y = y / z;
    z = y - x;

    gslmm::ieee_representation<float> r(z);

    /* We don't check the sign and we don't check the mantissa because
       they could be anything for a NaN */

    ts.test(r.exponent(), 128, "float x = NaN, exponent is 128");
    ts.test(r.type(), int(gslmm::ieee_representation<float>::not_a_number), 
	    "float x = NaN, type is NAN");
  }


  // Check for +ZERO
  {
    double d = 0.0;
    std::bitset<52> 
      mantissa(std::string("0000000000000000000000000000000000000000000000000000"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = 0, sign is +");
    ts.test(r.exponent(), -1023, "double x = 0, exponent is -1023");
    ts.test(r.mantissa(), mantissa, "double x = 0, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::zero), 
	    "double x = 0, type is ZERO");
  }

  // Check for -ZERO
  {
    double d =  minus_one;
    std::bitset<52> 
      mantissa(std::string("0000000000000000000000000000000000000000000000000000"));

    while (d < 0) {
      d *= 0.1;
    }

    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 1, "double x = -1*0, sign is -");
    ts.test(r.exponent(), -1023, "double x = -1*0, exponent is -1023");
    ts.test(r.mantissa(), mantissa, "double x = -1*0, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::zero), 
	    "double x = -1*0, type is ZERO");
  }

  // Check for a positive NORMAL number (e.g. 2.1)
  {
    double d = 2.1;
    std::bitset<52> 
      mantissa(std::string("0000110011001100110011001100110011001100110011001101"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = 2.1, sign is +");
    ts.test(r.exponent(), 1, "double x = 2.1, exponent is 1");
    ts.test(r.mantissa(), mantissa, "double x = 2.1, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal), 
	    "double x = 2.1, type is NORMAL");
  }


  // Check for a negative NORMAL number (e.g. -1.3304...)
  {
    double d = -1.3303577090924210146738460025517269968986511230468750;
    std::bitset<52> 
      mantissa(std::string("0101010010010010010100101010010010001000100011101110"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 1, "double x = -1.3304..., sign is -");
    ts.test(r.exponent(), 0, "double x = -1.3304..., exponent is 0");
    ts.test(r.mantissa(), mantissa, "double x = -1.3304..., mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal),
	    "double x = -1.3304..., type is NORMAL");
  }

  // Check for a large positive NORMAL number (e.g. 3.37e297)
  {
    double d = 3.37e297;
    std::bitset<52> 
      mantissa(std::string("0100100111001001100101111001100000100110011101000100"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = 3.37e297, sign is +");
    ts.test(r.exponent(), 988, "double x = 3.37e297, exponent is 998");
    ts.test(r.mantissa(), mantissa, "double x = 3.37e297, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal),
	    "double x = 3.37e297, type is NORMAL");
  }

  // Check for a small positive NORMAL number (e.g. 3.37e-297)
  {
    double d = 3.37e-297;
    std::bitset<52> 
      mantissa(std::string("0001101000011011101011100001110010100001001100110111"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = 3.37e-297, sign is +");
    ts.test(r.exponent(), -985, "double x = 3.37e-297, exponent is -985");
    ts.test(r.mantissa(), mantissa, "double x = 3.37e-297, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal),
	    "double x = 3.37e-297, type is NORMAL");
  }

  // Check for DBL_MIN (smallest possible number that is not denormal)
  {
    double d = DBL_MIN;
    std::bitset<52> 
      mantissa(std::string("0000000000000000000000000000000000000000000000000000"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = DBL_MIN, sign is +");
    ts.test(r.exponent(), -1022, "double x = DBL_MIN, exponent is -1022");
    ts.test(r.mantissa(), mantissa, "double x = DBL_MIN, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal),
	    "double x = DBL_MIN, type is NORMAL");
  }

  // Check for DBL_MAX (largest possible number that is not Inf)
  {
    double d = DBL_MAX;
    std::bitset<52> 
      mantissa(std::string("1111111111111111111111111111111111111111111111111111"));
    gslmm::ieee_representation<double> r(d);

    ts.test(r.sign(), 0, "double x = DBL_MAX, sign is +");
    ts.test(r.exponent(), 1023, "double x = DBL_MAX, exponent is 1023");
    ts.test(r.mantissa(), mantissa, "double x = DBL_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::normal),
	    "double x = DBL_MAX, type is NORMAL");
  }


#ifdef TEST_DENORMAL
  // Check for DENORMAL numbers (e.g. DBL_MIN/2^n)
  {
    double d = DBL_MIN;
    char mantissa[]
      = "1000000000000000000000000000000000000000000000000000";
    int i;

    for (i = 0; i < 52; i++)
      {
	double x = d / pow (2.0, 1 + (double) i);
	mantissa[i] = '1';
	gslmm::ieee_representation<double> r(x);

	ts.test(r.sign(), 0, "double x = DBL_MIN/2^%d, sign is +", i + 1);
	ts.test(r.exponent(), -1023,
		"double x = DBL_MIN/2^%d, exponent", i + 1);
	ts.test(r.mantissa(), mantissa,
		"double x = DBL_MIN/2^%d, mantissa", i + 1);
	ts.test(r.type(), int(gslmm::ieee_representation<double>::denormal),
		"double x = DBL_MIN/2^%d, type is DENORMAL", i + 1);
	mantissa[i] = '0';
      }
  }
#endif

  // Check for positive INFINITY (e.g. 2*DBL_MAX)
  {
    double d = DBL_MAX;
    std::bitset<52> 
      mantissa(std::string("0000000000000000000000000000000000000000000000000000"));

    double x;
    x = 2.0 * d;
    gslmm::ieee_representation<double> r(x);

    ts.test(r.sign(), 0, "double x = 2*DBL_MAX, sign is +");
    ts.test(r.exponent(), 1024, "double x = 2*DBL_MAX, exponent is 1024");
    ts.test(r.mantissa(), mantissa, "double x = 2*DBL_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::infinity), 
	    "double x = 2*DBL_MAX, type is INF");
  }

  // Check for negative INFINITY (e.g. -2*DBL_MAX)
  {
    double d = DBL_MAX;
    std::bitset<52> 
      mantissa(std::string("0000000000000000000000000000000000000000000000000000"));

    double x;
    x = -2.0 * d;
    gslmm::ieee_representation<double> r(x);

    ts.test(r.sign(), 1, "double x = -2*DBL_MAX, sign is -");
    ts.test(r.exponent(), 1024, "double x = -2*DBL_MAX, exponent is 1024");
    ts.test(r.mantissa(), mantissa, "double x = -2*DBL_MAX, mantissa");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::infinity),
	    "double x = -2*DBL_MAX, type is INF");
  }

  // Check for NAN (e.g. Inf - Inf)
  {
    double x = 1.0, y = 2.0, z = zero;

    x = x / z;
    y = y / z;
    z = y - x;

    gslmm::ieee_representation<double> r(z);

    // We don't check the sign and we don't check the mantissa because
    // they could be anything for a NaN
    ts.test(r.exponent(), 1024, "double x = NaN, exponent is 1024");
    ts.test(r.type(), int(gslmm::ieee_representation<double>::not_a_number), 
	    "double x = NaN, type is NAN");
  }


  return 0;
}

//
// EOF
//  
