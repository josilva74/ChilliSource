/*
 *  EmailCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		const std::string kstrTempAttachmentDirectory = "_AttachmentTemp/";
		//-------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------
		EmailCompositionActivity::EmailCompositionActivity()
		{
			//get the email java interface or create it if it doesn't yet exist.
			mpJavaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<EmailCompositionJavaInterface>();
			if (mpJavaInterface == nullptr)
			{
				mpJavaInterface = EmailCompositionJavaInterfaceSPtr(new EmailCompositionJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
			}
		}
		//-------------------------------------------------------
        /// Is Supported By Device
        ///
        /// @return whether email is supported on the current
        /// device
        //-------------------------------------------------------
        bool EmailCompositionActivity::IsSupportedByDevice() const
        {
			return true;
        }
        //-------------------------------------------------------
		/// Is A
		//-------------------------------------------------------
		bool EmailCompositionActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == EmailCompositionActivity::InterfaceID;
		}
        //-------------------------------------------------------
        /// Present
        //-------------------------------------------------------
		void EmailCompositionActivity::Present(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			Attachment emptyAttachment;
			emptyAttachment.m_filename = "";
			emptyAttachment.m_mimeType = "";
			emptyAttachment.m_storageLocation = Core::StorageLocation::k_none;
			PresentWithAttachment(inastrRecipientAddresses, instrSubject, instrContents, emptyAttachment, inCallback, inbFormatAsHtml);
		}
		//-------------------------------------------------------
		/// Present With Attachment
		//-------------------------------------------------------
		void EmailCompositionActivity::PresentWithAttachment(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			mCallback = inCallback;

			std::string strFilename;
			if (inAttachment.m_filename.size() > 0)
			{
				if (inAttachment.m_storageLocation == Core::StorageLocation::k_package || (inAttachment.m_storageLocation == Core::StorageLocation::k_DLC && Core::Application::Get()->GetFileSystem()->DoesFileExistInCachedDLC(inAttachment.m_filename) == false))
				{
					Core::Application::Get()->GetFileSystem()->CreateDirectoryPath(Core::StorageLocation::k_cache, kstrTempAttachmentDirectory);
					Core::Application::Get()->GetFileSystem()->CopyFile(inAttachment.m_storageLocation, inAttachment.m_filename, Core::StorageLocation::k_cache, kstrTempAttachmentDirectory + inAttachment.m_filename);
					strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache) + kstrTempAttachmentDirectory + inAttachment.m_filename;
				}
				else
				{
					strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(inAttachment.m_storageLocation) + inAttachment.m_filename;
				}
			}

			mpJavaInterface->Present(inastrRecipientAddresses, instrSubject, instrContents, strFilename, inbFormatAsHtml, Core::MakeDelegate(this, &EmailCompositionActivity::OnEmailClosed));
		}
        //-------------------------------------------------------
        /// Dismiss
        //-------------------------------------------------------
		void EmailCompositionActivity::Dismiss()
		{
			//We don't have any way of dismissing the activity on Android, so do nothing.
		}
        //-------------------------------------------------------
        /// On Email Closed
        //-------------------------------------------------------
		void EmailCompositionActivity::OnEmailClosed(s32 indwResultCode)
		{
			if(mCallback != nullptr)
			{
				switch (indwResultCode)
				{
					case EmailCompositionJavaInterface::kdwResultSuccess:
						mCallback(EmailCompositionActivity::SendResult::k_succeed);
						break;
					case EmailCompositionJavaInterface::kdwResultCancelled:
						mCallback(EmailCompositionActivity::SendResult::k_cancelled);
						break;
					default:
						mCallback(EmailCompositionActivity::SendResult::k_failed);
						break;
				}
			}
			mCallback = nullptr;
		}
		//-------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------
		EmailCompositionActivity::~EmailCompositionActivity()
		{
			Core::Application::Get()->GetFileSystem()->DeleteDirectory(Core::StorageLocation::k_cache, kstrTempAttachmentDirectory);
		}
	}
}
