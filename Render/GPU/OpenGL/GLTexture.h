


	struct TextureGL
	{
		TextureGL()
			: m_id(0)
			, m_rbo(0)
			, m_target(GL_TEXTURE_2D)
			, m_fmt(GL_ZERO)
			, m_type(GL_ZERO)
			, m_flags(0)
			, m_currentSamplerHash(UINT32_MAX)
			, m_numMips(0)
		{
		}

		bool init(GLenum _target, uint32_t _width, uint32_t _height, uint32_t _depth, uint8_t _numMips, uint64_t _flags);
		void create(const Memory* _mem, uint64_t _flags, uint8_t _skip);
		void destroy();
		void overrideInternal(uintptr_t _ptr);
		void update(uint8_t _side, uint8_t _mip, const Rect& _rect, uint16_t _z, uint16_t _depth, uint16_t _pitch, const Memory* _mem);
		void setSamplerState(uint32_t _flags, const float _rgba[4]);
		void commit(uint32_t _stage, uint32_t _flags, const float _palette[][4]);
		void resolve(uint8_t _resolve) const;

		bool isCubeMap() const
		{
			return 0
				|| GL_TEXTURE_CUBE_MAP       == m_target
				|| GL_TEXTURE_CUBE_MAP_ARRAY == m_target
				;
		}

		GLuint m_id;
		GLuint m_rbo;
		GLenum m_target;
		GLenum m_fmt;
		GLenum m_type;
		uint64_t m_flags;
		uint32_t m_currentSamplerHash;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint32_t m_numLayers;
		uint8_t m_numMips;
		uint8_t m_requestedFormat;
		uint8_t m_textureFormat;
	};
