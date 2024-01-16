#!/usr/bin/env python
# encoding: utf-8

"""
This module of native functions is implemented for
compatibility with 010 editor functions. Some of these functions
are nops, some are fully implemented.
"""

import math

from pfp.native import native
import pfp.errors as errors
import pfp.fields

# http://www.sweetscape.com/010editor/manual/FuncMath.htm

# double Abs( double x )
@native(name="Abs", ret=pfp.fields.Double)
def Abs(params, ctxt, scope, stream, coord):
    if params[0]._pfp__interp._generate:
        return 0.0
    raise NotImplementedError()


# double Ceil( double x )
# I split it to:
#  double CeilToDouble( double X )
#  int CeilToDouble( int X )
# To fix the ICO.bt template that does (int) casting
@native(name="CeilToInt", ret=pfp.fields.Int)
def CeilToInt(params, ctxt, scope, stream, coord):
    if len(params) < 1:
        raise errors.InvalidArguments(
            coord, "{} args".format(len(params)), "one arg"
        )
    value = PYVAL(params[0])
    return int(math.ceil(value))

@native(name="CeilToDouble", ret=pfp.fields.Double)
def CeilToDouble(params, ctxt, scope, stream, coord):
    if len(params) < 1:
        raise errors.InvalidArguments(
            coord, "{} args".format(len(params)), "one arg"
        )
    value = PYVAL(params[0])
    return float(math.ceil(value))


# double Cos( double a )
@native(name="Cos", ret=pfp.fields.Double)
def Cos(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Exp( double x )
@native(name="Exp", ret=pfp.fields.Double)
def Exp(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Floor( double x)
@native(name="Floor", ret=pfp.fields.Double)
def Floor(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Log( double x )
@native(name="Log", ret=pfp.fields.Double)
def Log(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Max( double a, double b )
@native(name="Max", ret=pfp.fields.Double)
def Max(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Min( double a, double b)
@native(name="Min", ret=pfp.fields.Double)
def Min(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Pow( double x, double y)
@native(name="Pow", ret=pfp.fields.Double)
def Pow(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# int Random( int maximum )
@native(name="Random", ret=pfp.fields.Int)
def Random(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Sin( double a )
@native(name="Sin", ret=pfp.fields.Double)
def Sin(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Sqrt( double x )
@native(name="Sqrt", ret=pfp.fields.Double)
def Sqrt(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# data_type SwapBytes( data_type x )
@native(name="SwapBytes", ret=pfp.fields.Int)
def SwapBytes(params, ctxt, scope, stream, coord):
    raise NotImplementedError()


# double Tan( double a )
@native(name="Tan", ret=pfp.fields.Double)
def Tan(params, ctxt, scope, stream, coord):
    raise NotImplementedError()
