//
//  StretchableImage.cpp
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Image/StretchableImage.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(StretchableImage)
		
		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
		DEFINE_PROPERTY(BaseTextureAtlasID);
		DEFINE_PROPERTY(CentreTouchConsumption);
		
		//--------------------------------------------------------
        /// Constructor
        /// 
        /// Empty
        //---------------------------------------------------------
        StretchableImage::StretchableImage()
		: HeightMaintain(false), WidthMaintain(false), CentreTouchConsumption(false)
        {
	
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        StretchableImage::StretchableImage(const Core::ParamDictionary& insParams) 
		: GUIView(insParams), HeightMaintain(false), WidthMaintain(false), CentreTouchConsumption(false)
        {
			CentreTouchConsumption = IsTouchConsumptionEnabled();
			
            std::string strValue;
            
            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Texture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetTexture(resourcePool->LoadResource<Rendering::Texture>(eTextureLocation, strValue));
            }
            
            //---Sprite sheet
            Core::StorageLocation eTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureAtlasLocation", strValue))
            {
                eTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("TextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eTextureAtlasLocation, strValue));
			}
			//---Sprite sheet base name
			if(insParams.TryGetValue("BaseTextureAtlasID", strValue))
            {
				SetBaseTextureAtlasID(strValue);
			}
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::ParseBool(strValue);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2Old vSize = Core::ParseVector2Old(strValue);
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2Old vSize = Core::ParseVector2Old(strValue);
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
			//---Centre Touch Consumption
			if(insParams.TryGetValue("CentreTouchConsumption", strValue))
			{
				CentreTouchConsumption = Core::ParseBool(strValue);
			}
        }
        //---------------------------------------------------------
        /// Set Texture
        ///
        /// @param Texture containing the nine patches
        //---------------------------------------------------------
        void StretchableImage::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureCSPtr& StretchableImage::GetTexture() const
        {
            return Texture;
        }
        //---------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet containing the nine patches
        //---------------------------------------------------------
        void StretchableImage::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            TextureAtlas = inpTextureAtlas;
        }
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the nine patches
		//---------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& StretchableImage::GetTextureAtlas() const
		{
			return TextureAtlas;
		}
		//---------------------------------------------------------
		/// Set Base Sprite Sheet Index ID
		///
		/// Set the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		void StretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
                
            BaseTextureAtlasID = instrID;
            
            std::string atlasId = instrID + "TOP_LEFT";
            m_panels.m_topLeftSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_topLeftUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "TOP_CENTRE";
            m_panels.m_topCentreSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_topCentreUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "TOP_RIGHT";
            m_panels.m_topRightSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_topRightUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "BOTTOM_LEFT";
            m_panels.m_bottomLeftSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_bottomLeftUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "BOTTOM_CENTRE";
            m_panels.m_bottomCentreSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_bottomCentreUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "BOTTOM_RIGHT";
            m_panels.m_bottomRightSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_bottomRightUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "MIDDLE_LEFT";
            m_panels.m_leftCentreSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_leftCentreUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "MIDDLE_CENTRE";
            m_panels.m_middleCentreSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_middleCentreUVs = TextureAtlas->GetFrameUVs(atlasId);
            
            atlasId = instrID + "MIDDLE_RIGHT";
            m_panels.m_rightCentreSize = TextureAtlas->GetFrameSize(atlasId);
            m_panels.m_rightCentreUVs = TextureAtlas->GetFrameUVs(atlasId);
		}
		//---------------------------------------------------------
		/// Get Base Sprite Sheet Index ID
		///
		/// Get the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		const std::string& StretchableImage::GetBaseTextureAtlasID() const
		{
			return BaseTextureAtlasID;
		}
        //---------------------------------------------------------
        /// Draw
        /// 
        /// Draw the image constructed from the nine patches
        ///
        /// @param Canvas renderer pointer
        //---------------------------------------------------------
        void StretchableImage::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::Vector2Old vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2Old vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && TextureAtlas && Texture)
            {
                Core::Vector2Old vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::Vector2Old vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
                Core::Vector2Old vPatchPos;
                
                Core::Colour AbsColour = GetAbsoluteColour();
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::Matrix3x3Old matTransform;
                Core::Matrix3x3Old matPatchTransform;
                Core::Matrix3x3Old matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::Vector2Old(1, 1), GetAbsoluteRotation());
				
				// Retrieve each bit's size
				PatchSize sPatchSize;
				CalculatePatchSize(sPatchSize);
                
                // Render ourself
				
                // Draw the top left corner
                matPatchTransform.Translate(vTopLeft);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   sPatchSize.vSizeTopLeft, 
								   Texture,
                                   m_panels.m_topLeftUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Draw the top right corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeTopRight,  
								   Texture,
                                   m_panels.m_topRightUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                // Draw the bottom left corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft));
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomLeft, 
								   Texture,
                                   m_panels.m_bottomLeftUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                // Draw the bottom right corner
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight));
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomRight,  
								   Texture,
                                   m_panels.m_bottomRightUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomRight);
                
                // Draw the top
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeTopLeft.x;
				vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeTopCentre, 
								   Texture,
                                   m_panels.m_topCentreUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
				
                // Draw the bottom
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeBottomLeft.x;
				vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomCentre).y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeBottomCentre, 
								   Texture,
                                   m_panels.m_bottomCentreUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                // Draw the left
				vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleLeft).x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopLeft.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeLeftCentre, 
								   Texture,
                                   m_panels.m_leftCentreUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Draw the right
				vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleRight).x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopRight.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   sPatchSize.vSizeRightCentre, 
								   Texture,
                                   m_panels.m_rightCentreUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                // Draw the centre
				vPatchPos.x = vTopLeft.x + sPatchSize.vSizeTopLeft.x;
				vPatchPos.y = vTopLeft.y - sPatchSize.vSizeTopLeft.y;
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3Old::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   sPatchSize.vSizeMiddleCentre, 
								   Texture,
                                   m_panels.m_middleCentreUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                // Render subviews
                GUIView::Draw(inpCanvas);
            }
		}
		//--------------------------------------------------------
		/// Set Width Maintaining Aspect
		///
		/// Change the width of the image and resize the height
		/// to maintain the aspect ratio
		///
		/// @param Unified width
		//--------------------------------------------------------
		void StretchableImage::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
		{
            Core::Vector2Old vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
			SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
		}
		//--------------------------------------------------------
		/// Set Height Maintaining Aspect
		///
		/// Change the height of the image and resize the width
		/// to maintain the aspect ratio
		///
		/// @param Unified height
		//--------------------------------------------------------
		void StretchableImage::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
		{
            Core::Vector2Old vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
			SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
			SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
		}
		//--------------------------------------------------------
		/// Enable Height Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableHeightMaintainingAspect(bool inbEnabled)
		{
			HeightMaintain = inbEnabled;
		}
		//--------------------------------------------------------
		/// Enable Width Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableWidthMaintainingAspect(bool inbEnabled)
		{
			WidthMaintain = inbEnabled;            
		}
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return auto scaling of the Width to maintain the aspect ratio
		//--------------------------------------------------------
		bool StretchableImage::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool StretchableImage::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
		
		void StretchableImage::CalculatePatchSize(PatchSize& outPatchSize)
		{
			Core::Vector2Old vPanelSize = GetAbsoluteSize();

			//Get the patch sizes
			outPatchSize.vSizeTopLeft = m_panels.m_topLeftSize;
			outPatchSize.vSizeTopRight = m_panels.m_topRightSize;
			outPatchSize.vSizeBottomLeft = m_panels.m_bottomLeftSize;
			outPatchSize.vSizeBottomRight = m_panels.m_bottomRightSize;
			
			//Check to see if they are going to fit in the bounds of the view
			f32 fTotal = outPatchSize.vSizeTopLeft.y + outPatchSize.vSizeBottomLeft.y;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				outPatchSize.vSizeTopLeft.y *= fScale;
				outPatchSize.vSizeBottomLeft.y *= fScale;
			}
			fTotal = outPatchSize.vSizeTopRight.y + outPatchSize.vSizeBottomRight.y ;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				outPatchSize.vSizeTopRight.y *= fScale;
				outPatchSize.vSizeBottomRight.y *= fScale;
			}
			fTotal = outPatchSize.vSizeTopLeft.x + outPatchSize.vSizeTopRight.x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				outPatchSize.vSizeTopLeft.x *= fScale;
				outPatchSize.vSizeTopRight.x *= fScale;
			}
			fTotal = outPatchSize.vSizeBottomLeft.x + outPatchSize.vSizeBottomRight.x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				outPatchSize.vSizeBottomLeft.x *= fScale;
				outPatchSize.vSizeBottomRight.x *= fScale;
			}
			
			// Calculate the top
			outPatchSize.vSizeTopCentre.x = vPanelSize.x - (outPatchSize.vSizeTopLeft.x + outPatchSize.vSizeTopRight.x);
			outPatchSize.vSizeTopCentre.y = outPatchSize.vSizeTopLeft.y;
			
            // Calculate the bottom
			outPatchSize.vSizeBottomCentre.x = vPanelSize.x - (outPatchSize.vSizeBottomLeft.x + outPatchSize.vSizeBottomRight.x);
			outPatchSize.vSizeBottomCentre.y = outPatchSize.vSizeBottomLeft.y;
			
			// Calculate the left
			outPatchSize.vSizeLeftCentre.y = vPanelSize.y - (outPatchSize.vSizeTopLeft.y + outPatchSize.vSizeBottomLeft.y);
			outPatchSize.vSizeLeftCentre.x = outPatchSize.vSizeTopLeft.x;
			
			// Calculate the right
			outPatchSize.vSizeRightCentre.y = vPanelSize.y - (outPatchSize.vSizeTopRight.y + outPatchSize.vSizeBottomRight.y);
			outPatchSize.vSizeRightCentre.x = outPatchSize.vSizeTopRight.x;
			
			// Calculate the centre
			outPatchSize.vSizeMiddleCentre.x = vPanelSize.x - (outPatchSize.vSizeLeftCentre.x + outPatchSize.vSizeRightCentre.x);
			outPatchSize.vSizeMiddleCentre.y = vPanelSize.y - (outPatchSize.vSizeTopCentre.y + outPatchSize.vSizeBottomCentre.y);
		}
		
		//--------------------------------------------------------
		/// Enable Centre Touch Consumption
		///
		/// Enables the touch to go through the middle part of the image
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void StretchableImage::EnableCentreTouchConsumption(bool inbEnabled)
		{
			CentreTouchConsumption = inbEnabled;
		}
		
		//--------------------------------------------------------
		/// Is Centre Touch Consumption Enabled
		///
		/// @return whether the touch though the middle is enabled or not
		//--------------------------------------------------------
		bool StretchableImage::IsCentreTouchConsumptionEnabled() const
		{
			return CentreTouchConsumption;
		}
        //--------------------------------------------------------
		//--------------------------------------------------------
		bool StretchableImage::OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
		{
			bool bConsumed = GUIView::OnPointerDown(in_pointer, in_timestamp, in_inputType);
			
			if (!CentreTouchConsumption && bConsumed && IsTouchConsumptionEnabled(TouchType::k_began))
			{
				// If the patch contains the touch
				if(Contains(in_pointer.m_location))
				{
					// Retrieve the patch sizes
					PatchSize sPatchSize;
					CalculatePatchSize(sPatchSize);
					
					// Get the AABB of the centre
					Core::Rectangle sMiddleAABB(GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft) + sPatchSize.vSizeBottomLeft, sPatchSize.vSizeMiddleCentre);
					
					// If the touch is not located in the centre, we consume it
					bool bContainsMiddle = sMiddleAABB.Contains(in_pointer.m_location);
					
					bConsumed = !bContainsMiddle;
				}
			}
			
			return bConsumed;
		}
		
        //--------------------------------------------------------
		//--------------------------------------------------------
		bool StretchableImage::OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp)
		{
			// Special treatment if the middle image has to let the touch through
			if (!CentreTouchConsumption && IsTouchConsumptionEnabled(TouchType::k_moved))
			{
				if(UserInteraction)
				{
					if(mbIsBeingDragged)
					{
						if(!AlignedWithParent)
						{
							SetPosition(Core::UnifiedVector2Old(Core::Vector2Old::ZERO, in_pointer.m_location));
						}
						else
						{
							SetOffsetFromParentAlignment(Core::UnifiedVector2Old(Core::Vector2Old::ZERO, in_pointer.m_location));
						}
					}
					
					mSubviewsCopy = mSubviews;
					
					for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
					{
						if((*it)->OnPointerMoved(in_pointer, in_timestamp))
						{
							mSubviewsCopy.clear();
							return true;
						}
					}
					
					//Check for input events
					//If we contain this touch we can consume it
					if(mInputEvents.OnPointerMoved(this, in_pointer) && IsTouchConsumptionEnabled(TouchType::k_moved))
					{
						// Retrieve the patch sizes
						PatchSize sPatchSize;
						CalculatePatchSize(sPatchSize);
						
						// Get the AABB of the centre
						Core::Rectangle sMiddleAABB(GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft) + sPatchSize.vSizeBottomLeft, sPatchSize.vSizeMiddleCentre);
						
						// If the touch is not located in the centre, we consume it
						bool bContainsMiddle = sMiddleAABB.Contains(in_pointer.m_location);
						
						if (bContainsMiddle)
						{
							mSubviewsCopy.clear();
						}
						
						return !bContainsMiddle;
					}
				}
				
				//We have no user interaction enabled
				mSubviewsCopy.clear();
				return false;
			}
			
			return GUIView::OnPointerMoved(in_pointer, in_timestamp);
		}
    }
}
