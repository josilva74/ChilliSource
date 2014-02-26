/*
 *  RenderSystem.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 13/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(RenderSystem);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		RenderSystem::RenderSystem()
        : mpRenderFactory(nullptr), mpSpriteBatcher(nullptr)
		{
            Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMeshManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSkinnedAnimationManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mFontManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMaterialManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSpriteManager);
		}
        //----------------------------------------------------
        /// Get Dynamic Sprite Batch Pointer
        ///
        /// @return Pointer to dynamic sprite batcher
        //----------------------------------------------------
        CDynamicSpriteBatch* RenderSystem::GetDynamicSpriteBatchPtr()
        {
            if(mpSpriteBatcher == nullptr)
            {
                mpSpriteBatcher = new CDynamicSpriteBatch(this);
            }
            
            return mpSpriteBatcher;
        }
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32 RenderSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		/// Get Component Factory Pointer
		///
		/// 
		//-------------------------------------------------------
		Core::IComponentFactory* RenderSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
            if(mpRenderFactory == nullptr)
            {
                mpRenderFactory = new RenderComponentFactory(this);
            }
            
            return mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Get Component Factory
		///
		/// 
		//-------------------------------------------------------
		Core::IComponentFactory& RenderSystem::GetComponentFactory(u32 inudwIndex)
		{
            if(mpRenderFactory == nullptr)
            {
                mpRenderFactory = new RenderComponentFactory(this);
            }
            
            return *mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------
		RenderSystem::~RenderSystem()
		{
			CS_SAFE_DELETE(mpRenderFactory);
            CS_SAFE_DELETE(mpSpriteBatcher);
		}
	}
}
