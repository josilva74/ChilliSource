//
//  Device.mm
//  Chilli Source
//  Created by Ian Copland on 24/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#import <ChilliSource/Backend/Platform/iOS/Core/Base/Device.h>

#import <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <sys/types.h>
#import <sys/sysctl.h>
#import <AdSupport/ASIdentifierManager.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModel()
            {
                @autoreleasepool
                {
                    NSString * type = [[UIDevice currentDevice] model];
                    return [NSStringUtils newStringWithNSString:type];
                }
            }
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModelType()
            {
                size_t size = 0;
                sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
                s8* machine = new s8[size];
                sysctlbyname("hw.machine", machine, &size, nullptr, 0);
                std::string modelType(machine);
                CS_SAFEDELETE(machine);
                
                std::string output;
                bool record = false;
                for(std::string::const_iterator it = modelType.begin(); it != modelType.end(); ++it)
                {
                    if(isdigit(*it))
                    {
                        record = true;
                    }
                    
                    if(record)
                    {
                        output += (*it);
                    }
                }
                return output;
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetDeviceManufacturer()
            {
                return "Apple";
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetOSVersion()
            {
                @autoreleasepool
                {
                    NSString* version = [[UIDevice currentDevice] systemVersion];
                    return [NSStringUtils newStringWithNSString:version];
                }
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetLocale()
            {
                @autoreleasepool
                {
                    NSLocale* locale = [NSLocale currentLocale];
                    NSString* languageCode = [locale objectForKey:NSLocaleLanguageCode];
                    NSString* countryCode = [locale objectForKey:NSLocaleCountryCode];
                    if (countryCode != nil && [countryCode length] > 0)
                    {
                        return [NSStringUtils newStringWithNSString:languageCode] + "_" + [NSStringUtils newStringWithNSString:countryCode];
                    }
                    else
                    {
                        return [NSStringUtils newStringWithNSString:languageCode];
                    }
                }
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetLanguage()
            {
                @autoreleasepool
                {
                    NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
                    NSArray* supportedLanguages = [userDefaults objectForKey:@"AppleLanguages"];
                    NSString* userLocale = [supportedLanguages objectAtIndex:0];
                    std::string localeCode = [userLocale UTF8String];
                    
                    std::vector<std::string> localeBrokenUp = ChilliSource::Core::StringUtils::Split(localeCode, "-", 0);
                    if (localeBrokenUp.size() > 0)
                    {
                        return localeBrokenUp[0];
                    }
                    else
                    {
                        return "en";
                    }
                }
            }
            //-----------------------------------------------
            //-----------------------------------------------
            std::string GetUDID()
            {
                //TODO: address issues with Apple UDID policies
                @autoreleasepool
                {
                    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f)
                    {
                        NSUUID* uid = nil;
                        
                        if([ASIdentifierManager sharedManager].advertisingTrackingEnabled)
                        {
                            uid = [ASIdentifierManager sharedManager].advertisingIdentifier;
                        }
                        else
                        {
                            uid = [UIDevice currentDevice].identifierForVendor;
                        }
                        
                        return [NSStringUtils newStringWithNSString:[uid UUIDString]];
                    }
                    else
                    {
                        NSString* udid = [[UIDevice currentDevice] uniqueGlobalDeviceIdentifier];
                        return [NSStringUtils newStringWithNSString:udid];
                    }
                }
            }
            //-------------------------------------------------
            //-------------------------------------------------
            u32 GetNumberOfCPUCores()
            {
                u32 numCores = 1;
                size_t size = sizeof(numCores);
                
                if(sysctlbyname("hw.ncpu", &numCores, &size, nullptr, 0))
                {
                    return 1;
                }
                else
                {
                    return numCores;
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(Device);
        //----------------------------------------------------
        //----------------------------------------------------
        Device::Device()
            : m_locale("en_GB"), m_language("en")
        {
            m_model = iOS::GetDeviceModel();
            m_modelType = iOS::GetDeviceModelType();
            m_manufacturer = iOS::GetDeviceManufacturer();
            m_locale = iOS::GetLocale();
            m_language = iOS::GetLanguage();
            m_osVersion = iOS::GetOSVersion();
            m_udid = iOS::GetUDID();
            m_numCPUCores = iOS::GetNumberOfCPUCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModel() const
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModelType() const
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetManufacturer() const
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetLocale() const
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetLanguage() const
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetOSVersion() const
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetUDID() const
        {
            return m_udid;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        u32 Device::GetNumberOfCPUCores() const
        {
            return m_numCPUCores;
        }
    }
}


