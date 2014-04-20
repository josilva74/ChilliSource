/*
 * File: RenderComponent.cpp
 * Date: 23/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(RenderComponent);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		RenderComponent::RenderComponent() : mfSortValue(0), mbVisible(true), mbShouldCull(true), mbCastsShadows(false)
		{
		}
		//----------------------------------------------------
		/// Is Visible
		//----------------------------------------------------
		bool RenderComponent::IsVisible() const
		{
            return GetEntity() ? mbVisible && (GetEntity()->IsVisible() && GetEntity()->GetTransform().GetWorldOpacity() != 0) : mbVisible;
		}
        //----------------------------------------------------
        /// Should Cull
        //----------------------------------------------------
        bool RenderComponent::IsCullingEnabled() const
        {
            return mbShouldCull;
        }
        //----------------------------------------------------
        /// Set Cull Enabled
        //----------------------------------------------------
        void RenderComponent::SetCullingEnabled(bool inbCull)
        {
            mbShouldCull = inbCull;
        }
		//----------------------------------------------------
		/// Set Visible
		//----------------------------------------------------
		void RenderComponent::SetVisible(bool inbVisible)
		{
			mbVisible = inbVisible;
		}
		//-----------------------------------------------------
		/// Get Transformation Matrix
		//-----------------------------------------------------
		const Core::Matrix4x4Old& RenderComponent::GetTransformationMatrix()
		{
			return GetEntity()->GetTransform().GetWorldTransform();
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void RenderComponent::SetMaterial(const MaterialCSPtr &inpMaterial)
		{
			mpMaterial = inpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		const MaterialCSPtr& RenderComponent::GetMaterial() const
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool RenderComponent::IsTransparent()
		{
			return mpMaterial->IsTransparencyEnabled();
		}
        //-----------------------------------------------------
        /// Set Shadow Casting Enabled
        //-----------------------------------------------------
        void RenderComponent::SetShadowCastingEnabled(bool inbEnabled)
        {
            mbCastsShadows = inbEnabled;
        }
        //-----------------------------------------------------
        /// Is Shadow Casting Enabled
        //-----------------------------------------------------
        bool RenderComponent::IsShadowCastingEnabled() const
        {
            return mbCastsShadows;
        }
	}
}
