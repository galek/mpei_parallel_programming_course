#pragma once

#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

// types
// PrecFloatDoubleV
typedef float PFDV;
#define PFDV_ZERO 0.0f;

typedef std::string String;

#define ToString(x) std::to_string(x)

#ifndef USE_DEBUG
#define USE_DEBUG 1
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x){delete x; x=nullptr;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if(x){delete []x; x=nullptr;}
#endif

#ifndef TRACE_FUNCTION

#if USE_DEBUG
#define TRACE_FUNCTION printf((__FUNCTION__ + String("\n")).c_str());
#else
#define TRACE_FUNCTION
#endif

#endif

#define MT_ARRAY_SIZE( arr ) ( sizeof( arr ) / sizeof( arr)[0] ) )


#endif