/**
 * Author: Ian Copland
 * Date: 17/03/2011
 * Description: A interface class for
 */

//includes
#include <ChilliSource/Platform/Android/JavaInterface/TouchInputJavaInterface.h>
#include <ChilliSource/Platform/Android/JavaInterface/CoreJavaInterface.h>

//function definitions
extern "C"
{
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchDown(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchUp(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchMoved(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
}

//--------------------------------------------------------------------------------------
/// Touch Down
///
/// Interface function called from java. This is called when a touch is first registered
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchDown(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	moFlo::AndroidPlatform::CTouchScreen* touchScreen = moFlo::AndroidPlatform::SCTouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != NULL)
	{
		touchScreen->OnTouchBegan(indwID, infX, infY);
	}
}
//--------------------------------------------------------------------------------------
/// Touch Up
///
/// Interface function called from java. This is called when a touch has finished
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchUp(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	moFlo::AndroidPlatform::CTouchScreen* touchScreen = moFlo::AndroidPlatform::SCTouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != NULL)
	{
		touchScreen->OnTouchEnded(indwID, infX, infY);
	}
}
//--------------------------------------------------------------------------------------
/// Touch Moved
///
/// Interface function called from java. This is called when a touch has moved.
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CTouchInputNativeInterface_TouchMoved(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	moFlo::AndroidPlatform::CTouchScreen* touchScreen = moFlo::AndroidPlatform::SCTouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != NULL)
	{
		touchScreen->OnTouchMoved(indwID, infX, infY);
	}
}


namespace moFlo
{
	namespace AndroidPlatform
	{
		//create static member
		moFlo::AndroidPlatform::CTouchScreen * SCTouchInputJavaInterface::mspTouchScreen = NULL;

		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		//--------------------------------------------------------------------------------------
		void SCTouchInputJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;
		}
		//--------------------------------------------------------------------------------------
		/// Register Touch Screen
		//--------------------------------------------------------------------------------------
		void SCTouchInputJavaInterface::RegisterTouchScreen(moFlo::AndroidPlatform::CTouchScreen* inpTouchScreen)
		{
			mspTouchScreen = inpTouchScreen;
		}
		//--------------------------------------------------------------------------------------
		/// Get Touch Screen
		//--------------------------------------------------------------------------------------
		moFlo::AndroidPlatform::CTouchScreen* SCTouchInputJavaInterface::GetTouchScreen()
		{
			return mspTouchScreen;
		}
	}
}