//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_APPLYMESHBATCHRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_APPLYMESHBATCHRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/RenderMeshBatch.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>

#include <vector>

namespace ChilliSource
{
    /// A render command for batching the given set of meshes into a single mesh and applying
    /// it to the context state.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class ApplyMeshBatchRenderCommand final : public RenderCommand
    {
    public:
        /// @return The render mesh batch that should be applied.
        ///
        const RenderMeshBatch* GetRenderMeshBatch() const noexcept { return m_renderMeshBatch.get(); }
        
    private:
        friend class RenderCommandList;
        
        /// Creates a new instance with the given render mesh batch.
        ///
        /// @param renderMeshBatch
        ///     The render mesh batch that should be applied. Must be moved.
        ///
        ApplyMeshBatchRenderCommand(RenderMeshBatchUPtr renderMeshBatch) noexcept;
        
        RenderMeshBatchUPtr m_renderMeshBatch;
    };
}

#endif
