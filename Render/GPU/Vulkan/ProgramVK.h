

struct ProgramVK
{
    ProgramVK() : m_vsh(NULL), m_fsh(NULL), m_descriptorSetLayout(VK_NULL_HANDLE), m_pipelineLayout(VK_NULL_HANDLE)
    {
    }

    void create(const ShaderVK *_vsh, const ShaderVK *_fsh);
    void destroy();

    const ShaderVK *m_vsh;
    const ShaderVK *m_fsh;

    BindInfo m_bindInfo[BGFX_CONFIG_MAX_TEXTURE_SAMPLERS];

    TextureBindInfo m_textures[BGFX_CONFIG_MAX_TEXTURE_SAMPLERS];
    uint8_t m_numTextures;

    PredefinedUniform m_predefined[PredefinedUniform::Count * 2];
    uint8_t m_numPredefined;

    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
};