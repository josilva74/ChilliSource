//
//  JavaClass.cpp
//  ChilliSource
//  Created by Ian Copland on 21/04/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClass.h>

#include <ChilliSource/Core/String/StringUtils.h>

#include <regex>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        jmethodID JavaClass::GetMethodId(const std::string& in_methodName, JavaMethodSignature::ReturnType in_returnType, u32 in_numArguments) const
        {
            auto methodInfoIt = m_methods.find(in_methodName);
            if (methodInfoIt == m_methods.end())
            {
                CS_LOG_FATAL("Could not find method '" + in_methodName + "' in Java class '" + m_className + "'");
            }

            CS_ASSERT(methodInfoIt->second.m_returnType == in_returnType, "Cannot call method '" + in_methodName + "' in Java class '" + m_className + "' because the wrong return type was specified.");
            CS_ASSERT(methodInfoIt->second.m_numArguments == in_numArguments, "Cannot call method '" + in_methodName + "' in Java class '" + m_className + "' because an incorrect number of arguments were supplied.");

            return methodInfoIt->second.m_methodId;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaClass::~JavaClass()
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            environment->DeleteGlobalRef(m_javaObject);
            m_javaObject = nullptr;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        jobject JavaClass::GetJavaObject() const
        {
            return m_javaObject;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& JavaClass::GetClassName() const
        {
            return m_className;
        }
    }
}

#endif