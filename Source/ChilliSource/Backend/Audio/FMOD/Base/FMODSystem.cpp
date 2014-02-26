/*
 * File: FMODSystem.cpp
 * Date: 17/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioManager.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/FMODAudioListener.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/FMODAudioComponent.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioResource.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/FMODAudioComponentFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>

#include <fmodiphone.h>

namespace ChilliSource
{
	namespace iOS
	{
		//-------------------------------------------------------
		/// Constructor
		///
		/// Initialise FMOD
		//-------------------------------------------------------
		CFMODSystem::CFMODSystem() : mpFMODSystem(nullptr), mpFMODEventSystem(nullptr), mpFMODEventProject(nullptr)
		{
			mpAudioManager = new CFMODAudioManager();
            mpAudioComponentFactory = new CFMODAudioComponentFactory(this, mpAudioManager);
            
			// Subscribe to memory warnings so we can clear FMOD's cache
            Core::CApplicationEvents::GetLowMemoryEvent() += Core::ApplicationSystemDelegate(this, &CFMODSystem::OnApplicationMemoryWarning);
			
			//Create the FMOD event system
			ErrorCheck(FMOD::EventSystem_Create(&mpFMODEventSystem));
			
			//Get the FMOD system
			ErrorCheck(mpFMODEventSystem->getSystemObject(&mpFMODSystem));
			
			//Initialise the system with the number of virtual channels
#ifdef FMOD_IOS_PLAYANDRECORD
            FMOD_IPHONE_EXTRADRIVERDATA driverData;
            driverData.forceSpeakerOutput = true;
            driverData.forceMixWithOthers = false;
            driverData.sessionCategory = FMOD_IPHONE_SESSIONCATEGORY_PLAYANDRECORD;
			ErrorCheck(mpFMODEventSystem->init(kudwMaxFMODChannels, FMOD_INIT_NORMAL, &driverData));
#else
            ErrorCheck(mpFMODEventSystem->init(kudwMaxFMODChannels, FMOD_INIT_NORMAL, nullptr));
#endif
			
			//Set defaults
			mpFMODSystem->set3DSettings(Audio::kfDefaultDoppler, Audio::kfDefaultDistance, Audio::kfDefaultRolloff);
		}
		//-------------------------------------------------------
		/// Load Event Data
		///
		/// Tell FMOD to load event data generated by the
		/// fmod designer
		///
        /// @param Location
		/// @param File path
		//-------------------------------------------------------
		void CFMODSystem::LoadEventData(Core::StorageLocation ineLocation, const std::string& instrFilePath)
		{
            std::string strFilePath;
            Core::CApplication::GetFileSystemPtr()->GetBestPathToFile(ineLocation, instrFilePath, strFilePath);
            ErrorCheck(mpFMODEventSystem->load(strFilePath.c_str(), nullptr, &mpFMODEventProject));
		}
		//-------------------------------------------------------
		/// Unload Event Data
		///
		///	Unload all event data (useful for memory warnings)
		///
		/// @param File path
		//-------------------------------------------------------
		void CFMODSystem::UnloadEventData()
		{
            if(mpFMODEventProject)
            {
                FMOD::EventGroup * pcEventRelease = nullptr;
                u32 udwGroupID = 0;
                
                // get all groups and free them
                while(mpFMODEventProject->getGroupByIndex(udwGroupID++, false, &pcEventRelease) == FMOD_OK)
                {
                    ErrorCheck(pcEventRelease->freeEventData(nullptr, true));
                }
            }
		}
		//-------------------------------------------------------
		/// Unload Event Data
		///
		///	Unload all event data (useful for memory warnings)
		///
		/// @param File path
		//-------------------------------------------------------
		void CFMODSystem::UnloadEventData(const std::string& instrEventGroup)
		{
            if(mpFMODEventProject)
            {
                FMOD::EventGroup * pcEventRelease = nullptr;
                
                // get groups and free it
                if(mpFMODEventProject->getGroup(instrEventGroup.c_str(), false, &pcEventRelease) == FMOD_OK)
                {
                    ErrorCheck(pcEventRelease->freeEventData(nullptr, true));
                }
            }
		}
		//-------------------------------------------------------
		/// PreLoad Event Group
		///
		/// Preload events inside a specified group
		///
		/// @param The name of the group
		//-------------------------------------------------------
		void CFMODSystem::PreloadEventGroup(const std::string& instrGroupName)
		{
			if(mpFMODEventProject != nullptr)
			{	
				FMOD::EventGroup * pPreloadGroup = nullptr;
				mpFMODEventProject->getGroup(instrGroupName.c_str(), true, &pPreloadGroup);
			}
		}
		//-------------------------------------------------------
		/// Create Sound
		///
		/// Tell FMOD to create a new sound from the given
		/// file
		///
		/// @param File path
		/// @param Sound handle to be initialised with sound
		//-------------------------------------------------------
		void CFMODSystem::CreateSound(const std::string& instrFilePath, Audio::IAudioResource* inpAudio)
		{
			ErrorCheck(mpFMODSystem->createSound(instrFilePath.c_str(), FMOD_SOFTWARE, nullptr, &static_cast<CFMODAudioResource*>(inpAudio)->mpFMODSound));
		}
		//-------------------------------------------------------
		/// Create 3D Sound
		///
		/// Tell FMOD to create a new sound from the given
		/// file
		///
		/// @param File path
		/// @param Sound handle to be initialised with sound
		//-------------------------------------------------------
		void CFMODSystem::Create3DSound(const std::string& instrFilePath, Audio::IAudioResource* inpAudio)
		{
			ErrorCheck(mpFMODSystem->createSound(instrFilePath.c_str(), FMOD_SOFTWARE|FMOD_3D, nullptr, &static_cast<CFMODAudioResource*>(inpAudio)->mpFMODSound));
		}
		//-------------------------------------------------------
		/// Create Stream
		///
		/// Tell FMOD to stream from given file
		///
		/// @param File path
		/// @param Stream handle
		//-------------------------------------------------------
		void CFMODSystem::CreateStream(const std::string& instrFilePath, Audio::IAudioResource* inpAudio)
		{
			ErrorCheck(mpFMODSystem->createStream(instrFilePath.c_str(), FMOD_SOFTWARE|FMOD_LOOP_NORMAL, nullptr, &static_cast<CFMODAudioResource*>(inpAudio)->mpFMODSound));
		}
		//-------------------------------------------------------
		/// Create 3d Stream
		///
		/// Tell FMOD to stream from given file
		///
		/// @param File path
		/// @param Stream handle
		//-------------------------------------------------------
		void CFMODSystem::Create3DStream(const std::string& instrFilePath, Audio::IAudioResource* inpAudio)
		{
			ErrorCheck(mpFMODSystem->createStream(instrFilePath.c_str(), FMOD_SOFTWARE|FMOD_LOOP_NORMAL|FMOD_3D, nullptr, &static_cast<CFMODAudioResource*>(inpAudio)->mpFMODSound));
		}
		//-------------------------------------------------------
		/// Play Sound
		///
		/// Tell the FMOD system to play the sound with a free
		/// channel
		///
		/// @param FMOD sound handle
		//-------------------------------------------------------
		void CFMODSystem::PlaySound(Audio::CAudioComponent* inpAudioComponent)
		{
			//We let FMOD manages the channels 
			FMOD::Channel* pActiveChannel = nullptr;
			ErrorCheck(mpFMODSystem->playSound(FMOD_CHANNEL_FREE, std::static_pointer_cast<CFMODAudioResource>(inpAudioComponent->GetAudioSource())->mpFMODSound, false, &pActiveChannel));
			
			//Give the sound it's channel so we can query the state
			static_cast<CFMODAudioComponent*>(inpAudioComponent)->SetChannel(pActiveChannel);
		}
		//-------------------------------------------------------
		/// Play Event
		///
		/// Tell the FMOD system to start the sound event
		///
		/// @param name of event
		//-------------------------------------------------------
		FMOD::Event* CFMODSystem::PlayEvent(const std::string& instrEventName)
		{
			if(mpFMODEventProject != nullptr)
			{
				FMOD::Event * pEvent = nullptr;
				ErrorCheck(mpFMODEventProject->getEvent(instrEventName.c_str(), FMOD_EVENT_DEFAULT, &pEvent));
                
                if(pEvent)
                {
                    ErrorCheck(pEvent->start());
                }
                
				return pEvent; 
			}
			return nullptr;
		}
		//-------------------------------------------------------
		/// Update
		///
		/// Tell the system to update
		/// @param Time since last frame
		//-------------------------------------------------------
		void CFMODSystem::Update(f32 dt)
		{
			ErrorCheck(mpFMODEventSystem->update());
		}
		//----------------------------------------------------------------------------
		/// Create Audio Listener
		///
		/// @return Audio listener
		//----------------------------------------------------------------------------
		Audio::AudioListenerPtr CFMODSystem::CreateAudioListener()
		{
			return FMODAudioListenerPtr(new CFMODAudioListener(mpFMODSystem));
		}
		//-------------------------------------------------------
		/// Error Check
		///
		/// Log any FMOD errors
		/// @param Result of FMOD function return
		//-------------------------------------------------------
		void CFMODSystem::ErrorCheck(FMOD_RESULT ineResult)
		{
			if(ineResult != FMOD_OK)
			{
				FATAL_LOG("FMOD error: " + std::string(FMOD_ErrorString(ineResult)));
			}
		}
		//-------------------------------------------------------
        /// On Application Memory Warning
        ///
        /// Triggered when a message is received detailing
        /// that the OS has thrown a memory warning. We should
        /// release the FMOD event cache at this point
        //-------------------------------------------------------
		void CFMODSystem::OnApplicationMemoryWarning()
		{
			if(mpFMODEventProject!= nullptr)
			{
				mpFMODEventProject->stopAllEvents();
				UnloadEventData();
			}
		}
		//-------------------------------------------------------
		/// Destroy 
		///
		/// Release the FMOD system
		//-------------------------------------------------------
		void CFMODSystem::Destroy()
		{
            ErrorCheck(mpFMODEventSystem->release());
			mpFMODEventSystem = nullptr;
		}
		//-------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------
		CFMODSystem::~CFMODSystem()
		{
            SAFE_DELETE(mpAudioComponentFactory);
			SAFE_DELETE(mpAudioManager);
			Destroy();
		}
	}
}

