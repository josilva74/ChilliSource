//
//  GUIEditableLabel.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_GUI_EDITABLE_LABEL
#define _MO_FLO_GUI_GUI_EDITABLE_LABEL

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>

namespace ChilliSource
{
	namespace GUI
	{
		class EditableLabel : public Label
		{
		public:

			DECLARE_META_CLASS(EditableLabel)

			typedef std::function<void(EditableLabel*)> TextChangeEventDelegate;

			static const s8 kbySecureEntryCharacter = '*';

			EditableLabel();
			EditableLabel(const Core::ParamDictionary& insParams);
			virtual ~EditableLabel();

			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is enabled.
			//-------------------------------------------------
			Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& GetTextInputEnabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is disabled.
			//-------------------------------------------------
			Core::IConnectableEvent<Input::Keyboard::KeyboardEventDelegate>& GetTextInputDisabledEvent();
			//-------------------------------------------------
			/// @author S Downie
            ///
            /// @return A delegate that is called when text
            /// input is received.
			//-------------------------------------------------
			Core::IConnectableEvent<TextChangeEventDelegate>& GetTextInputReceivedEvent();
            //-------------------------------------------------
			/// Set Keyboard
			///
			/// @param Virtual keyboard
			//-------------------------------------------------
			void SetKeyboard(Input::Keyboard* inpKeyboard);
			//-------------------------------------------------
			/// Get Keyboard Ptr
			///
			/// @return Virtual keyboard
			//-------------------------------------------------
			Input::Keyboard* GetKeyboardPtr();
            //-------------------------------------------------
			/// Show Keyboard
			//-------------------------------------------------
			void ShowKeyboard();
            //-------------------------------------------------
			/// Hide Keyboard
			//-------------------------------------------------
			void HideKeyboard();
			//-------------------------------------------------
			/// Enable Secure Entry
			///
			/// Enabling this will prevent the label from
			/// showing any user input. Each character will
			/// be replaced by a * for security
			///
			/// @param Enable/disable
			//--------------------------------------------------
			void EnableSecureEntry(bool inbEnabled);
			//-------------------------------------------------
			/// Enable Secure Entry
			///
			/// Enabling this will prevent the label from
			/// showing any user input. Each character will
			/// be replaced by a * for security
			///
			/// @param Enable/disable
			//--------------------------------------------------
			bool IsSecureEntryEnabled() const;
			//--------------------------------------------------
			/// Set Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @param Limit
			//--------------------------------------------------
			void SetCharacterLimit(u32 inudwLimit);
			//--------------------------------------------------
			/// Get Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @return Limit
			//--------------------------------------------------
			u32 GetCharacterLimit() const;
			//--------------------------------------------------
			/// Get Input Limit
			///
			/// The maximum number of characters the user
			/// can enter. Zero is infinite
			///
			/// @return Limit
			//--------------------------------------------------
			u32 GetInputLimit() const;
            //-------------------------------------------------------
			/// Clears the text
			///
			/// 
			///
			///
			//-------------------------------------------------------
            void ClearText();
            //-------------------------------------------------
			/// SetKeyboardInputTypeNumeric
            ///
            /// change displayed keys to numberpad
			//-------------------------------------------------
			void SetKeyboardInputTypeNumeric();
            //-------------------------------------------------
			/// SetKeyboardInputTypeText
            ///
            /// change displayed keys to text entry
			//-------------------------------------------------
			void SetKeyboardInputTypeText();
            //------------------------
            /// Set Keyboard Capitalisation Method
            ///
            /// @param Capitalisation Type
            //------------------------
            void SetKeyboardCapitalisationMethod(Input::Keyboard::Capitalisation ineCapitalisationType);
            //-------------------------------------------------------
			/// SetTextSeparator
			///
			/// Sets the separator for text display
			/// @param The string to use as a separator
			//-------------------------------------------------------
            void SetTextSeparator(const Core::UTF8String& inutf8strSeparator);
            //-------------------------------------------------------
			/// SetTextSeparatorSpacing
			///
			/// Sets the characters between separators for diplay
			/// @param The number of spaces between seperators
			//-------------------------------------------------------
            void SetTextSeparatorSpacing(u32 inu32Spacing);
            //-------------------------------------------------------
			/// SetText
			///
			/// Overridden set text method, updates cached separator string
            /// @param instrText text to set as entry
			//-------------------------------------------------------
            void SetText(const Core::UTF8String& instrText);
		protected:

			//-------------------------------------------------
			/// Called when Text Input is enabled on the
            /// keyboard.
            ///
            /// @author S Downie
			//-------------------------------------------------
			void OnKeyboardTextInputEnabled();
            //-------------------------------------------------
			/// Called when the keyboard text input changes.
			///
            /// @author S Downie
            ///
			/// @param Contents of the keyboard.
			/// @param [Out] Whether to accept the input.
			//----------------------------------------------------
			void OnKeyboardTextInputReceived(const Core::UTF8String& in_text, bool* out_rejectText);
			//-------------------------------------------------
			/// Called when Text Input is disabled on the
            /// keyboard.
            ///
            /// @author S Downie
			//-------------------------------------------------
			void OnKeyboardTextInputDisabled();
            //-----------------------------------------------------------
            /// Called when the window receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            ///
            /// @return Whether touch has been consumed
            //-----------------------------------------------------------
			bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer) override;
			//-----------------------------------------------------------
			/// Called when the view stops receiving cursor/touch input
            ///
            /// @author S Downie
			///
			/// @param The poniter
			//-----------------------------------------------------------
			void OnPointerUp(const Input::PointerSystem::Pointer& in_pointer) override;
			//-------------------------------------------------------
			/// Draw
			///
			/// Draw the text using the canvas then draw any
			/// subviews
			///
			/// @param Canvas renderer
			//-------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			//-------------------------------------------------------
			/// Update
			///
			/// Used for animation
			///
			/// @param Time between frames
			//-------------------------------------------------------
			void Update(f32 infDt);
            //-------------------------------------------------------
			/// GetTextWithSeparators
			///
			/// Returns the stored text separated by set separators
			//-------------------------------------------------------
            Core::UTF8String GetTextWithSeparators() const;

		private:

			Input::Keyboard* mpKeyboard;

			Core::Event<Input::Keyboard::KeyboardEventDelegate> m_textInputEnabledEvent;
			Core::Event<Input::Keyboard::KeyboardEventDelegate> m_textInputDisabledEvent;
			Core::Event<TextChangeEventDelegate> m_textInputReceivedEvent;
            
            Core::ConnectionUPtr m_keyboardShownConnection;
            Core::ConnectionUPtr m_keyboardHiddenConnection;
            Core::ConnectionUPtr m_keyboardTextChangedConnection;

			f32 mfTimeToShow;
			bool mbShowKeyboard;

			DECLARE_PROPERTY_A(bool, SecureEntry, EnableSecureEntry, IsSecureEntryEnabled);
			DECLARE_PROPERTY_A(u32, CharacterLimit, SetCharacterLimit, GetCharacterLimit);

			static EditableLabel* pKeyboardListener;
            
            Core::UTF8String mutf8strTextWithSeparators;
            Core::UTF8String mutf8strSeparator;
            u32 mu32SeparatorSpacing;
            bool mbSelected;
		};
	}
}

#endif