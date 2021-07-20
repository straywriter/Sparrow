#include "RenderObject.h"

namespace bgfx
{

void EncoderImpl::submit(ViewId _id, ProgramHandle _program, OcclusionQueryHandle _occlusionQuery, uint32_t _depth,
                         uint8_t _flags)
{
    if (BX_ENABLED(BGFX_CONFIG_DEBUG_UNIFORM) && (_flags & BGFX_DISCARD_STATE))
    {
        m_uniformSet.clear();
    }

    if (BX_ENABLED(BGFX_CONFIG_DEBUG_OCCLUSION) && isValid(_occlusionQuery))
    {
        BX_ASSERT(m_occlusionQuerySet.end() == m_occlusionQuerySet.find(_occlusionQuery.idx),
                  "OcclusionQuery %d was already used for this frame.", _occlusionQuery.idx);
        m_occlusionQuerySet.insert(_occlusionQuery.idx);
    }

    if (m_discard)
    {
        discard(_flags);
        return;
    }

    if (0 == m_draw.m_numVertices && 0 == m_draw.m_numIndices)
    {
        discard(_flags);
        ++m_numDropped;
        return;
    }

    const uint32_t renderItemIdx =
        bx::atomicFetchAndAddsat<uint32_t>(&m_frame->m_numRenderItems, 1, BGFX_CONFIG_MAX_DRAW_CALLS);
    if (BGFX_CONFIG_MAX_DRAW_CALLS - 1 <= renderItemIdx)
    {
        discard(_flags);
        ++m_numDropped;
        return;
    }

    ++m_numSubmitted;

    UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
    m_uniformEnd = uniformBuffer->getPos();

    m_key.m_program = isValid(_program) ? _program : ProgramHandle{0};

    m_key.m_view = _id;

    SortKey::Enum type = SortKey::SortProgram;
    switch (s_ctx->m_view[_id].m_mode)
    {
    case ViewMode::Sequential:
        m_key.m_seq = s_ctx->getSeqIncr(_id);
        type = SortKey::SortSequence;
        break;
    case ViewMode::DepthAscending:
        m_key.m_depth = _depth;
        type = SortKey::SortDepth;
        break;
    case ViewMode::DepthDescending:
        m_key.m_depth = UINT32_MAX - _depth;
        type = SortKey::SortDepth;
        break;
    default:
        break;
    }

    uint64_t key = m_key.encodeDraw(type);

    m_frame->m_sortKeys[renderItemIdx] = key;
    m_frame->m_sortValues[renderItemIdx] = RenderItemCount(renderItemIdx);

    m_draw.m_uniformIdx = m_uniformIdx;
    m_draw.m_uniformBegin = m_uniformBegin;
    m_draw.m_uniformEnd = m_uniformEnd;

    if (UINT8_MAX != m_draw.m_streamMask)
    {
        uint32_t numVertices = UINT32_MAX;
        for (uint32_t idx = 0, streamMask = m_draw.m_streamMask; 0 != streamMask; streamMask >>= 1, idx += 1)
        {
            const uint32_t ntz = bx::uint32_cnttz(streamMask);
            streamMask >>= ntz;
            idx += ntz;
            numVertices = bx::min(numVertices, m_numVertices[idx]);
        }

        m_draw.m_numVertices = numVertices;
    }
    else
    {
        m_draw.m_numVertices = m_numVertices[0];
    }

    if (isValid(_occlusionQuery))
    {
        m_draw.m_stateFlags |= BGFX_STATE_INTERNAL_OCCLUSION_QUERY;
        m_draw.m_occlusionQuery = _occlusionQuery;
    }

    m_frame->m_renderItem[renderItemIdx].draw = m_draw;
    m_frame->m_renderItemBind[renderItemIdx] = m_bind;

    m_draw.clear(_flags);
    m_bind.clear(_flags);
    if (_flags & BGFX_DISCARD_STATE)
    {
        m_uniformBegin = m_uniformEnd;
    }
}

void EncoderImpl::dispatch(ViewId _id, ProgramHandle _handle, uint32_t _numX, uint32_t _numY, uint32_t _numZ,
                           uint8_t _flags)
{
    if (BX_ENABLED(BGFX_CONFIG_DEBUG_UNIFORM))
    {
        m_uniformSet.clear();
    }

    if (m_discard)
    {
        discard(_flags);
        return;
    }

    const uint32_t renderItemIdx =
        bx::atomicFetchAndAddsat<uint32_t>(&m_frame->m_numRenderItems, 1, BGFX_CONFIG_MAX_DRAW_CALLS);
    if (BGFX_CONFIG_MAX_DRAW_CALLS - 1 <= renderItemIdx)
    {
        discard(_flags);
        ++m_numDropped;
        return;
    }

    ++m_numSubmitted;

    UniformBuffer *uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
    m_uniformEnd = uniformBuffer->getPos();

    m_compute.m_startMatrix = m_draw.m_startMatrix;
    m_compute.m_numMatrices = m_draw.m_numMatrices;
    m_compute.m_numX = bx::max(_numX, 1u);
    m_compute.m_numY = bx::max(_numY, 1u);
    m_compute.m_numZ = bx::max(_numZ, 1u);

    m_key.m_program = _handle;
    m_key.m_depth = 0;
    m_key.m_view = _id;
    m_key.m_seq = s_ctx->getSeqIncr(_id);

    uint64_t key = m_key.encodeCompute();
    m_frame->m_sortKeys[renderItemIdx] = key;
    m_frame->m_sortValues[renderItemIdx] = RenderItemCount(renderItemIdx);

    m_compute.m_uniformIdx = m_uniformIdx;
    m_compute.m_uniformBegin = m_uniformBegin;
    m_compute.m_uniformEnd = m_uniformEnd;
    m_frame->m_renderItem[renderItemIdx].compute = m_compute;
    m_frame->m_renderItemBind[renderItemIdx] = m_bind;

    m_compute.clear(_flags);
    m_bind.clear(_flags);
    m_uniformBegin = m_uniformEnd;
}

void EncoderImpl::blit(ViewId _id, TextureHandle _dst, uint8_t _dstMip, uint16_t _dstX, uint16_t _dstY, uint16_t _dstZ,
                       TextureHandle _src, uint8_t _srcMip, uint16_t _srcX, uint16_t _srcY, uint16_t _srcZ,
                       uint16_t _width, uint16_t _height, uint16_t _depth)
{
    BX_WARN(m_frame->m_numBlitItems < BGFX_CONFIG_MAX_BLIT_ITEMS,
            "Exceed number of available blit items per frame. BGFX_CONFIG_MAX_BLIT_ITEMS is %d. Skipping blit.",
            BGFX_CONFIG_MAX_BLIT_ITEMS);
    if (m_frame->m_numBlitItems < BGFX_CONFIG_MAX_BLIT_ITEMS)
    {
        uint16_t item = m_frame->m_numBlitItems++;

        BlitItem &bi = m_frame->m_blitItem[item];
        bi.m_srcX = _srcX;
        bi.m_srcY = _srcY;
        bi.m_srcZ = _srcZ;
        bi.m_dstX = _dstX;
        bi.m_dstY = _dstY;
        bi.m_dstZ = _dstZ;
        bi.m_width = _width;
        bi.m_height = _height;
        bi.m_depth = _depth;
        bi.m_srcMip = _srcMip;
        bi.m_dstMip = _dstMip;
        bi.m_src = _src;
        bi.m_dst = _dst;

        BlitKey key;
        key.m_view = _id;
        key.m_item = item;
        m_frame->m_blitKeys[item] = key.encode();
    }
}

void Frame::sort()
{
    BGFX_PROFILER_SCOPE("bgfx/Sort", 0xff2040ff);

    ViewId viewRemap[BGFX_CONFIG_MAX_VIEWS];
    for (uint32_t ii = 0; ii < BGFX_CONFIG_MAX_VIEWS; ++ii)
    {
        viewRemap[m_viewRemap[ii]] = ViewId(ii);
    }

    for (uint32_t ii = 0, num = m_numRenderItems; ii < num; ++ii)
    {
        m_sortKeys[ii] = SortKey::remapView(m_sortKeys[ii], viewRemap);
    }
    bx::radixSort(m_sortKeys, s_ctx->m_tempKeys, m_sortValues, s_ctx->m_tempValues, m_numRenderItems);

    for (uint32_t ii = 0, num = m_numBlitItems; ii < num; ++ii)
    {
        m_blitKeys[ii] = BlitKey::remapView(m_blitKeys[ii], viewRemap);
    }
    bx::radixSort(m_blitKeys, (uint32_t *)&s_ctx->m_tempKeys, m_numBlitItems);
}

void UniformBuffer::writeUniform(UniformType::Enum _type, uint16_t _loc, const void *_value, uint16_t _num)
{
    uint32_t opcode = encodeOpcode(_type, _loc, _num, true);
    write(opcode);
    write(_value, g_uniformTypeSize[_type] * _num);
}

void UniformBuffer::writeUniformHandle(UniformType::Enum _type, uint16_t _loc, UniformHandle _handle, uint16_t _num)
{
    uint32_t opcode = encodeOpcode(_type, _loc, _num, false);
    write(opcode);
    write(&_handle, sizeof(UniformHandle));
}

void UniformBuffer::writeMarker(const char *_marker)
{
    uint16_t num = (uint16_t)bx::strLen(_marker) + 1;
    uint32_t opcode = encodeOpcode(bgfx::UniformType::Count, 0, num, true);
    write(opcode);
    write(_marker, num);
}

} // namespace bgfx