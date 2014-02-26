//
//  StandardMacros.h
//  Chilli Source
//
//  Created by Ian Copland on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_
#define _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_


//TODO: Remove the array deleter and put it in a more sutiable place. This is a functor for deleting shared arrays.
template <typename T> class ArrayDeleter
{
public:
    void operator () (T* in_arrayObject) const
    {
        delete[] in_arrayObject;
    }
};

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

//---Logging Levels
#define LOG_VERBOSE		(0)
#define LOG_WARNING		(1)
#define LOG_ERROR		(2)

#ifdef DEBUG
//---Active Log Level
#define LOG_LEVEL		(LOG_VERBOSE)
//---Debug Flags
#define DEBUG_DRAWING	(1)
#else
//---Active Log Level
#define LOG_LEVEL		(LOG_ERROR)
//---Debug Flags
#define DEBUG_DRAWING	(0)
#endif


//---Macros
#define SAFE_DELETE(x)						{if(x) delete(x); x = nullptr;}
#define SAFE_DELETE_ARRAY(x)				{if(x) delete[] (x); x = nullptr;}

#define PARSE_FLOAT(x)                      (Core::ParseF32(x))
#define PARSE_INT(x)                        (Core::ParseS32(x))
#define PARSE_UINT(x)                       (Core::ParseU32(x))
#define PARSE_BOOL(x)                       (Core::ParseBool(x))
#define PARSE_VECTOR2(x)                    (Core::ParseVector2(x))
#define PARSE_VECTOR3(x)                    (Core::ParseVector3(x))
#define PARSE_VECTOR4(x)                    (Core::ParseVector4(x))
#define PARSE_MATRIX4X4(x)                  (Core::ParseMatrix4(x))
#define PARSE_QUATERNION(x)                 (Core::ParseQuaternion(x))
#define PARSE_COLOUR(x)                     (Core::ParseColour(x))

#define STRING_HASH(x)						ChilliSource::Core::CHashCRC32::GenerateHashCode(x)
#define GET_LOCALISED_TEXT(x)               ChilliSource::Core::CLocalisedText::GetText(x)

#define DEBUG_LOG(x)						(ChilliSource::Core::CLogging::LogVerbose(x))
#define WARNING_LOG(x)						(ChilliSource::Core::CLogging::LogWarning(x))
#define ERROR_LOG(x)						(ChilliSource::Core::CLogging::LogError(x))
#define FATAL_LOG(x)						(ChilliSource::Core::CLogging::LogFatal(x))

#ifdef DEBUG
#define MOFLOW_ASSERT(x, msg) if((x) == 0){ChilliSource::Core::CLogging::LogFatal(msg);}
#else
#define MOFLOW_ASSERT(x, msg)
#endif

#ifdef DEBUG
#define SMART_CAST(to, from) dynamic_cast<to >(from); assert(dynamic_cast<to >(from) != nullptr);
#else
#define SMART_CAST(to, from) static_cast<to >(from);
#endif

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef TARGET_ANDROID
typedef char GLchar;
#endif



#endif
