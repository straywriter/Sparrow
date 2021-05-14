
struct BatchD3D12
{
    enum Enum
    {
        Draw,
        DrawIndexed,

        Count
    };

    BatchD3D12() : m_currIndirect(0), m_maxDrawPerBatch(0), m_minIndirect(0), m_flushPerBatch(0)
    {
        bx::memSet(m_num, 0, sizeof(m_num));
    }

    ~BatchD3D12()
    {
    }

    void create(uint32_t _maxDrawPerBatch);
    void destroy();

    template <typename Ty> Ty &getCmd(Enum _type);

    uint32_t draw(ID3D12GraphicsCommandList *_commandList, D3D12_GPU_VIRTUAL_ADDRESS _cbv, const RenderDraw &_draw);

    void flush(ID3D12GraphicsCommandList *_commandList, Enum _type);
    void flush(ID3D12GraphicsCommandList *_commandList, bool _clean = false);

    void begin();
    void end(ID3D12GraphicsCommandList *_commandList);

    void setSeqMode(bool _enabled)
    {
        m_flushPerBatch = _enabled ? 1 : m_maxDrawPerBatch;
    }

    void setIndirectMode(bool _enabled)
    {
        m_minIndirect = _enabled ? 64 : UINT32_MAX;
    }

    ID3D12CommandSignature *m_commandSignature[Count];
    uint32_t m_num[Count];
    void *m_cmds[Count];

    struct DrawIndirectCommand
    {
        D3D12_VERTEX_BUFFER_VIEW vbv[BGFX_CONFIG_MAX_VERTEX_STREAMS + 1 /* instanced buffer */];
        D3D12_GPU_VIRTUAL_ADDRESS cbv;
        D3D12_DRAW_ARGUMENTS args;
    };

    struct DrawIndexedIndirectCommand
    {
        D3D12_VERTEX_BUFFER_VIEW vbv[BGFX_CONFIG_MAX_VERTEX_STREAMS + 1 /* instanced buffer */];
        D3D12_INDEX_BUFFER_VIEW ibv;
        D3D12_GPU_VIRTUAL_ADDRESS cbv;
        D3D12_DRAW_INDEXED_ARGUMENTS args;
    };

    struct Stats
    {
        uint32_t m_numImmediate[Count];
        uint32_t m_numIndirect[Count];
    };

    BufferD3D12 m_indirect[32];
    uint32_t m_currIndirect;
    DrawIndexedIndirectCommand m_current;

    Stats m_stats;
    uint32_t m_maxDrawPerBatch;
    uint32_t m_minIndirect;
    uint32_t m_flushPerBatch;
};