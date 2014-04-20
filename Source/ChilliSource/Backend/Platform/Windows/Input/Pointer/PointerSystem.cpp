//
//  PointerSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 27/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>
#include <ChilliSource/Core/Base/Screen.h>

//This needs to be included after windows.h
#include <Platform/Windows/glfw3.h>

namespace ChilliSource
{
	namespace Windows
	{
		namespace
		{
			PointerSystem* g_pointerSystem = nullptr;

			//------------------------------------------------
			/// @author I Copland
			///
			/// @return The current mouse position.
			//------------------------------------------------
			Core::Vector2Old GetMousePosition()
			{
				f64 x = 0.0;
				f64 y = 0.0;

				GLFWManager::Get()->GetCursorPos(&x, &y);

				y = Core::Screen::GetOrientedHeight() - y;
				return Core::Vector2Old((f32)x, (f32)y);
			}
			//------------------------------------------------
			/// @author I Copland
			///
			/// @param The button ID.
			///
			/// @return The equivelent Press Type for the
			/// button Id
			//------------------------------------------------
			PointerSystem::InputType ButtonIdToInputType(s32 in_buttonId)
			{
				switch (in_buttonId)
				{
				case GLFW_MOUSE_BUTTON_LEFT:
					return PointerSystem::InputType::k_leftMouseButton;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					return PointerSystem::InputType::k_middleMouseButton;
				case GLFW_MOUSE_BUTTON_RIGHT:
					return PointerSystem::InputType::k_rightMouseButton;
				default:
					return PointerSystem::InputType::k_none;
				}
				
			}
		}
		CS_DEFINE_NAMEDTYPE(PointerSystem);
		//----------------------------------------------------
		//----------------------------------------------------
		bool PointerSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Input::PointerSystem::InterfaceID == in_interfaceId || PointerSystem::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseMoved(GLFWwindow* in_window, f64 in_xPos, f64 in_yPos)
		{
			CS_ASSERT(g_pointerSystem, "OnMouseMoved callback requires a pointer system.");

			Core::Vector2Old touchLocation((f32)in_xPos, Core::Screen::GetOrientedDimensions().y - ((f32)in_yPos));
			g_pointerSystem->AddPointerMovedEvent(g_pointerSystem->m_pointerId, touchLocation);
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseButtonPressed(GLFWwindow* in_window, s32 in_buttonID, s32 in_buttonAction, s32 in_modifierKeys)
		{
			CS_ASSERT(g_pointerSystem, "On Mouse Button Pressed callback requires a pointer system.");

			switch (GLFWManager::MouseButtonAction(in_buttonAction))
			{
				case GLFWManager::MouseButtonAction::k_press:
				{
					InputType type = ButtonIdToInputType(in_buttonID);
					if (type != InputType::k_none)
					{
						g_pointerSystem->AddPointerDownEvent(g_pointerSystem->m_pointerId, type);
					}
					break;
				}
				case GLFWManager::MouseButtonAction::k_release:
				{
					InputType type = ButtonIdToInputType(in_buttonID);
					if (type != InputType::k_none)
					{
						g_pointerSystem->AddPointerUpEvent(g_pointerSystem->m_pointerId, type);
					}
					break;
				}
				default:
				{
					break;
				}
			};
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnInit()
		{
			g_pointerSystem = this;

			//Register for glfw mouse callbacks
			GLFWManager::Get()->SetCursorPosDelegate(&PointerSystem::OnMouseMoved);
			GLFWManager::Get()->SetMouseButtonDelegate(&PointerSystem::OnMouseButtonPressed);

			//create the mouse pointer
			m_pointerId = AddPointerCreateEvent(GetMousePosition());
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnDestroy()
		{
			AddPointerRemoveEvent(m_pointerId);

			GLFWManager::Get()->SetCursorPosDelegate(nullptr);
			GLFWManager::Get()->SetMouseButtonDelegate(nullptr);

			g_pointerSystem = nullptr;
		}
	}
}
