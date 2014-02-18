/*
 *  Colour.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace moFlo
{
	namespace Core
	{
		
		const CColour CColour::WHITE(1.0f,1.0f,1.0f,1.0f);
		const CColour CColour::TRANSPARENT(0.0f,0.0f,0.0f,0.0f);
		const CColour CColour::BLACK(0.0f,0.0f,0.0f,1.0f);
		const CColour CColour::GREY(0.5f,0.5f,0.5f,1.0f);
		const CColour CColour::RED(1.0f,0.0f,0.0f,1.0f);
		const CColour CColour::GREEN(0.0f,1.0f,0.0f,1.0f);
		const CColour CColour::BLUE(0.0f,0.0f,1.0f,1.0f);
		const CColour CColour::MAGENTA(1.0f,0.0f,1.0f,1.0f);
		const CColour CColour::YELLOW(1.0f,1.0f,0.0f,1.0f);
		const CColour CColour::CYAN(0.0f,1.0f,1.0f,1.0f);
		const CColour CColour::CORNFLOWER_BLUE(0.4f,0.5f,0.8f,1.0f);
		const CColour CColour::LIGHTGREY(0.8f,0.8f,0.8f,1.0f);
		const CColour CColour::TURQUOISE(0.25f,0.88f,0.82f,1.0f);
		const CColour CColour::ORANGE(0.9f,0.4f,0.12f,1.0f);
		const CColour CColour::PURPLE(0.5f,0.0f,0.5f,1.0f);
		const CColour CColour::PINK(0.94f,0.36f,0.47f,1.0f);
		
        //--------------------------------------------------
		/// Constructor
		///
        /// Default
		//--------------------------------------------------
		CColour::CColour() : r(1.0f), g(1.0f), b(1.0f), a(1.0f)
		{
		}
		//--------------------------------------------------
		/// Constructor
		///
		/// Initialised a white colour container
		/// @param Red component
		/// @param Green component
		/// @param Blue component
		/// @param Alpha component
		//--------------------------------------------------
		CColour::CColour(f32 inR, f32 inG, f32 inB, f32 inA) : r(inR), g(inG), b(inB), a(inA)
		{
		}
		
		CColour::CColour(u8 inR, u8 inG, u8 inB, u8 inA)
		:r((f32)inR / 255.0f), g((f32)inG / 255.0f),	b((f32)inB / 255.0f),	a((f32)inA / 255.0f)
		{
			
		}
        
        CColour::CColour(u32 inudwRGBA)
        {
            u8 byR = (inudwRGBA >> 24) & 255;
            u8 byG = (inudwRGBA >> 16) & 255;
            u8 byB = (inudwRGBA >> 8) & 255;
            u8 byA = (inudwRGBA) & 255;
            
            r = (f32)byR/255.0f;
            g = (f32)byG/255.0f;
            b = (f32)byB/255.0f;
            a = (f32)byA/255.0f;
        }

		CColour CColour::FromByteRGBA(u8 inR, u8 inG, u8 inB, u8 inA)
        {
			return CColour(inR,inG,inB,inA);
		}
        CColour::ByteColour CColour::ColourToByteColour(const CColour& inCol)
        {
            CColour::ByteColour Col;
            
            inCol.r <= 0.0f ? Col.r = 0 : Col.r = CMathUtils::Round(inCol.r * 255.0f);
            inCol.g <= 0.0f ? Col.g = 0 : Col.g = CMathUtils::Round(inCol.g * 255.0f);
            inCol.b <= 0.0f ? Col.b = 0 : Col.b = CMathUtils::Round(inCol.b * 255.0f);
            inCol.a <= 0.0f ? Col.a = 0 : Col.a = CMathUtils::Round(inCol.a * 255.0f);

            return Col;
        }
	}
}