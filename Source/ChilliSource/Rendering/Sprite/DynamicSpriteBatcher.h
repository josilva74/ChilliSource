/*
 *  SpriteBatch.h
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_DYNAMICSPRITEBATCH_H_
#define _MO_FLO_RENDERING_DYNAMICSPRITEBATCH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/SpriteBatch.h>

namespace ChilliSource
{
	namespace Rendering
	{
        const u32 kudwNumBuffers = 2;
        const u32 kudwBufferArrayBounds = kudwNumBuffers - 1;
        
        struct RenderCommand
        {
            CMaterial Material;
            u32 udwOffset;
            u32 udwStride;
        };
        
		class CDynamicSpriteBatch
		{
		public:
			CDynamicSpriteBatch(RenderSystem* inpRenderSystem);
            ~CDynamicSpriteBatch();
            
			//-------------------------------------------------------
			/// Render
			///
            /// Batch the sprite to be rendered later. Track the 
            /// render commands so that the correct subset of the 
            /// mesh buffer can be flushed and the correct material
            /// applied
            ///
            /// @param Render system
            /// @param Sprite data to batch
			//-------------------------------------------------------
			void Render(RenderSystem* inpRenderSystem, const CSpriteComponent::SpriteData& inpSprite, const Core::CMatrix4x4 * inpTransform = nullptr);
            //-------------------------------------------------------
            /// Force Command Change
            ///
            /// Force a render command change so that subsequent
            /// additons to the buffer will not be drawn in this call
            //-------------------------------------------------------
            void ForceCommandChange();
            //-------------------------------------------------------
			/// Force Render
			///
            /// Force the currently batched sprites to be rendered
            /// regardless of whether the batch is full
            ///
            /// @param Render system
			//-------------------------------------------------------
			void ForceRender(RenderSystem* inpRenderSystem);
			
		private:
            
            //----------------------------------------------------------
            /// Build and Flush Batch
            ///
            /// Map the batch into the mesh buffer and present the
            /// contents. This will then swap the active buffer
            /// so that it can be filled while the other one is 
            /// rendering
            /// 
            /// @param Render system
            //----------------------------------------------------------
            void BuildAndFlushBatch(RenderSystem* inpRenderSystem);
			
		private:
        
			CSpriteBatch* mpBatch[kudwNumBuffers];
			std::vector<CSpriteComponent::SpriteData> maSpriteCache;
            std::vector<RenderCommand> maRenderCommands;
            
            MaterialPtr mpLastMaterial;
            
            u32 mudwCurrentRenderSpriteBatch;
            u32 mudwSpriteCommandCounter;
		};
	}
}

#endif