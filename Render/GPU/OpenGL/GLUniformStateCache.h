

class UniformStateCache
{
  public:
    struct f4
    {
        float val[4];
        bool operator==(const f4 &rhs)
        {
            const uint64_t *a = (const uint64_t *)this;
            const uint64_t *b = (const uint64_t *)&rhs;
            return a[0] == b[0] && a[1] == b[1];
        }
    };
    struct f3x3
    {
        float val[9];
        bool operator==(const f3x3 &rhs)
        {
            const uint64_t *a = (const uint64_t *)this;
            const uint64_t *b = (const uint64_t *)&rhs;
            return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] &&
                   ((const uint32_t *)a)[8] == ((const uint32_t *)b)[8];
        }
    };
    struct f4x4
    {
        float val[16];
        bool operator==(const f4x4 &rhs)
        {
            const uint64_t *a = (const uint64_t *)this;
            const uint64_t *b = (const uint64_t *)&rhs;
            return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5] &&
                   a[6] == b[6] && a[7] == b[7];
        }
    };

    typedef stl::unordered_map<uint64_t, int> IMap;
    typedef stl::unordered_map<uint64_t, f4> F4Map;
    typedef stl::unordered_map<uint64_t, f3x3> F3x3Map;
    typedef stl::unordered_map<uint64_t, f4x4> F4x4Map;

    UniformStateCache() : m_currentProgram(0)
    {
    }

    // Inserts the new value into the uniform cache, and returns true
    // if the old value was different than the new one.
    template <typename T> bool updateUniformCache(uint32_t loc, const T &value)
    {
        if (BX_ENABLED(BGFX_GL_CONFIG_UNIFORM_CACHE))
        {
            // Uniform state cache for various types.
            stl::unordered_map<uint64_t, T> &uniformCacheMap = getUniformCache<T>();

            uint64_t key = (uint64_t(m_currentProgram) << 32) | loc;

            auto iter = uniformCacheMap.find(key);

            // Not found in the cache? Add it.
            if (iter == uniformCacheMap.end())
            {
                uniformCacheMap[key] = value;
                return true;
            }

            // Value in the cache was the same as new state? Skip reuploading this state.
            if (iter->second == value)
            {
                return false;
            }

            iter->second = value;
        }

        return true;
    }

    void saveCurrentProgram(GLuint program)
    {
        if (BX_ENABLED(BGFX_GL_CONFIG_UNIFORM_CACHE))
        {
            m_currentProgram = program;
        }
    }

  private:
    GLuint m_currentProgram;

    IMap m_uniformiCacheMap;
    F4Map m_uniformf4CacheMap;
    F3x3Map m_uniformf3x3CacheMap;
    F4x4Map m_uniformf4x4CacheMap;

    template <typename T> stl::unordered_map<uint64_t, T> &getUniformCache();
};

template <> inline UniformStateCache::IMap &UniformStateCache::getUniformCache()
{
    return m_uniformiCacheMap;
}

template <> inline UniformStateCache::F4Map &UniformStateCache::getUniformCache()
{
    return m_uniformf4CacheMap;
}

template <> inline UniformStateCache::F3x3Map &UniformStateCache::getUniformCache()
{
    return m_uniformf3x3CacheMap;
}

template <> inline UniformStateCache::F4x4Map &UniformStateCache::getUniformCache()
{
    return m_uniformf4x4CacheMap;
}