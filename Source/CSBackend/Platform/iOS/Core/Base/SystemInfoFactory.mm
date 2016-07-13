//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/Core/Base/DeviceInfo.h>
#import <ChilliSource/Core/Base/SystemInfo.h>
#import <ChilliSource/Core/String/StringUtils.h>

#import <CSBackend/Platform/iOS/Core/Base/SystemInfoFactory.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <sys/types.h>
#import <sys/sysctl.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            const std::string k_deviceManufacturer = "Apple";
            
            /// @return The current device model.
            ///
            std::string GetDeviceModel() noexcept
            {
                @autoreleasepool
                {
                    NSString * type = [[UIDevice currentDevice] model];
                    return [NSStringUtils newUTF8StringWithNSString:type];
                }
            }
            
            /// @return The current device model type.
            ///
            std::string GetDeviceModelType() noexcept
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
            
            /// @return The current OS version.
            ///
            std::string GetOSVersion() noexcept
            {
                @autoreleasepool
                {
                    NSString* version = [[UIDevice currentDevice] systemVersion];
                    return [NSStringUtils newUTF8StringWithNSString:version];
                }
            }
            
            /// @return The current locale.
            ///
            std::string GetLocale() noexcept
            {
                @autoreleasepool
                {
                    NSLocale* locale = [NSLocale currentLocale];
                    NSString* languageCode = [locale objectForKey:NSLocaleLanguageCode];
                    NSString* countryCode = [locale objectForKey:NSLocaleCountryCode];
                    if (countryCode != nil && [countryCode length] > 0)
                    {
                        return [NSStringUtils newUTF8StringWithNSString:languageCode] + "_" + [NSStringUtils newUTF8StringWithNSString:countryCode];
                    }
                    else
                    {
                        return [NSStringUtils newUTF8StringWithNSString:languageCode];
                    }
                }
            }
            
            /// @return The current language.
            ///
            std::string GetLanguage() noexcept
            {
                @autoreleasepool
                {
                    NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
                    NSArray* supportedLanguages = [userDefaults objectForKey:@"AppleLanguages"];
                    NSString* userLocale = [supportedLanguages objectAtIndex:0];
                    std::string localeCode = [userLocale UTF8String];
                    
                    std::vector<std::string> localeBrokenUp = ChilliSource::StringUtils::Split(localeCode, "-", 0);
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
            
            /// @return The unique device identifier.
            ///
            std::string GetUDID() noexcept
            {
                @autoreleasepool
                {
                    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f)
                    {
                        NSUUID* uid = [UIDevice currentDevice].identifierForVendor;
                        return [NSStringUtils newUTF8StringWithNSString:[uid UUIDString]];
                    }
                    
                    return "";
                }
            }
            
            /// @return The number of cores.
            ///
            u32 GetNumberOfCPUCores() noexcept
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
        
        //-------------------------------------------------------
        ChilliSource::SystemInfoCUPtr SystemInfoFactory::CreateSystemInfo() noexcept
        {
			// Create DeviceInfo.
			ChilliSource::DeviceInfo deviceInfo(GetDeviceModel(), GetDeviceModelType(), k_deviceManufacturer, GetUDID(), GetLocale(), GetLanguage(), GetOSVersion(), GetNumberOfCPUCores());

            // Create SystemInfo.
            ChilliSource::SystemInfoUPtr systemInfo(new ChilliSource::SystemInfo(deviceInfo));
            
            return std::move(systemInfo);
        }
        
    }
}


#endif