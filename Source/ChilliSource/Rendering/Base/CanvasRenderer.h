/*
 *  CanvasRenderer.h
 *  moFlo
 *
 *  Created by Scott Downie on 12/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_CANVAS_H_
#define _MO_FLO_RENDERING_CANVAS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Core/Math/Matrix3x3Old.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		class CanvasRenderer
		{
		public:

            struct PlacedCharacter
            {
                Core::Rectangle sUVs;
                Core::Vector2Old vSize;
                Core::Vector2Old vPosition;
            };
            
            typedef std::vector<PlacedCharacter> CharacterList;
            
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Render system
            //----------------------------------------------------------
			CanvasRenderer(RenderSystem* in_renderSystem);
            //----------------------------------------------------------
            /// @author S Downie
            //----------------------------------------------------------
            void Init();
			//----------------------------------------------------------
			/// Render
			///
			/// Draw UI 
			//----------------------------------------------------------
			void Render(GUI::GUIView* inpRootSurface, f32 infNearClipDistance);
            //----------------------------------------------------------
            /// Enable Clipping To Bounds
            ///
            /// Set the bounds beyond which any subviews will clip
            /// Pushes to a stack which tracks when to enable and
            /// disable scissoring
            ///
            /// @param Position of the bottom left corner of the rect
            /// @param Size of the clip region
            //---------------------------------------------------------
            void EnableClippingToBounds(const Core::Vector2Old& invPosition, const Core::Vector2Old& invSize);
            //----------------------------------------------------------
            /// Disable Clipping To Bounds
            ///
            /// Pop the scissor tracking stack
            //----------------------------------------------------------                            
            void DisableClippingToBounds();
            //-----------------------------------------------------------
            /// Draw Box
            ///
            /// Build a sprite box and batch it ready for rendering
            ///
            /// @param Transform
            /// @param Dimensions
            /// @param Texture
            /// @param Colour
            /// @param Box origin alignment
            //-----------------------------------------------------------
			void DrawBox(const Core::Matrix3x3Old& inmatTransform, const Core::Vector2Old & invSize, const TextureCSPtr & inpTexture, const Core::Rectangle& inUVs,
                         const Core::Colour & insTintColour, AlignmentAnchor ineAlignment = AlignmentAnchor::k_middleCentre);
            //-----------------------------------------------------------
            /// Draw String
            ///
            /// Build a sprite font and batch it ready for rendering
            ///
            /// @param Text
            /// @param Transform
            /// @param Font
            /// @param Built character cache
            /// @param Colour
            /// @param Bounds
            /// @param Letter spacing
            /// @param Line spacing
			/// @param Justification (Horizontal)
			/// @param Justification (Vertical)
            /// @param Whether or not to flip vertically.
            /// @param Max num lines (0 = infinite)
            /// @param Bool pointer. Bool set true if string is clipped, false if not, untouched if string not rebuilt
            /// @param Bool pointer. Bool set true if invalid character is found.
            //-----------------------------------------------------------
			void DrawString(const Core::UTF8String & insString, const Core::Matrix3x3Old& inmatTransform, f32 infSize, const FontCSPtr& inpFont, CharacterList& outCharCache,
							const Core::Colour & insColour, const Core::Vector2Old & invBounds, f32 infCharacterSpacing, f32 infLineSpacing, 
							GUI::TextJustification ineHorizontalJustification, GUI::TextJustification ineVerticalJustification, bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour, u32 inudwNumLines, bool * outpClipped = nullptr, bool * outpInvalidCharacterFound = nullptr);
            //-----------------------------------------------------------
            /// Calculate String Width
            ///
            /// Calculate the length of a string based on the font
            /// and attributes
            ///
            /// @param String
            /// @param Font
            /// @param Size
            /// @param Character spacing
            ///
            /// @return String length
            //------------------------------------------------------------
            f32 CalculateStringWidth(const Core::UTF8String& insString, const FontCSPtr& inpFont, f32 infSize, f32 infCharSpacing, bool inbIgnoreLineBreaks);
            //-----------------------------------------------------------
            /// Calculate String Height
            ///
            /// Calculate the height of a string based on the font, width
            /// and attributes
            ///
            /// @param String
            /// @param Font
            /// @param Width
            /// @param Size
            /// @param Character spacing
            /// @param Line spacing
            /// @param Number of lines
            ///
            /// @return String height
            //------------------------------------------------------------
            f32 CalculateStringHeight(const Core::UTF8String& insString, const FontCSPtr& inpFont, f32 infWidth, f32 infSize, f32 infCharSpacing, f32 infLineSpacing, u32 inudwNumLines);
			//-------------------------------------------
			/// Build String
			///
			/// Construct a list of character sprites
			/// from the given string
			///
			/// @param Font
			/// @param String to display
			/// @param List of characters to be filled
            /// @param Text size
            /// @param Spacing between each letter
            /// @param Spacing between each line
            /// @param Wrap bounds
            /// @param Max number of lines
			/// @param Justification (Horizontal)
			/// @param Justification (Vertical)
            /// @param Whether or not to flip the text
            /// vertically.
			//-------------------------------------------
			static void BuildString(const FontCSPtr& inpFont, const Core::UTF8String &inText, CharacterList &outCharacters, f32 infTextSize, f32 infCharacterSpacing, f32 infLineSpacing,
							const Core::Vector2Old& invBounds, u32 inudwNumLines, GUI::TextJustification ineJustification, GUI::TextJustification ineVerticalJustification,
							bool inbFlipVertical, GUI::TextOverflowBehaviour ineBehaviour,  bool * outpClipped = nullptr, bool * outpInvalidCharacterFound = nullptr);
			//----------------------------------------------------
			/// Build Character
			///
			/// Build a sprite from the given character
            /// @param Font
			/// @param Character
            /// @param Next Character (used for kerning)
			/// @param Cursor pos
            /// @param Text scale
            /// @param Character spacing
			/// @param The width of the new character
			/// @param List of characters that we can add too
			//----------------------------------------------------
			static void BuildCharacter(const FontCSPtr& inpFont, Core::UTF8String::Char inCharacter, Core::UTF8String::Char inNextCharacter,
                                                   const Core::Vector2Old& invCursor, f32 infTextScale, f32 infCharSpacing,
                                                   f32 &outfCharacterWidth, CharacterList &outCharacters, bool * outpInvalidCharacterFound = nullptr);
            //----------------------------------------------------
            /// Wrap
            ///
            /// @param Text justification (Horizontal)
            /// @param Y - offset
            /// @param Bounds 
            /// @param Character list with characters in this line
            /// @param Out: Cursor Pos
            /// @param Out: Character list
            //----------------------------------------------------
            static void Wrap(GUI::TextJustification ineHorizontalJustification, f32 infLineSpacing, const Core::Vector2Old& invBounds, CharacterList &inCurrentLine,
					  Core::Vector2Old& outvCursor, CharacterList &outCharacters);
			
		private:
			
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return Cached or new GUI material that has the given
            /// texture
            //----------------------------------------------------------
            MaterialCSPtr GetGUIMaterialForTexture(const TextureCSPtr& in_texture);
            
			//-----------------------------------------------------
			/// Update Sprite Data
			///
			/// Rebuild the sprite data
			//-----------------------------------------------------
			void UpdateSpriteData(const Core::Matrix4x4Old& inTransform, const Core::Vector2Old & invSize, const Core::Rectangle& inUVs, const Core::Colour & insTintColour, AlignmentAnchor ineAlignment);
            
		private:
			
			SpriteComponent::SpriteData msCachedSprite;

            DynamicSpriteBatch mOverlayBatcher;
            
			std::vector<Core::Vector2Old> mScissorPos;
            std::vector<Core::Vector2Old> mScissorSize;
            
            std::unordered_map<TextureCSPtr, MaterialCSPtr> m_materialGUICache;
            
            Core::ResourcePool* m_resourcePool;
            MaterialFactory* m_materialFactory;
            
            f32 mfNearClippingDistance;
		};
	}
}

#endif
