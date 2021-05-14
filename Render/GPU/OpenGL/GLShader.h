
	struct ShaderGL
	{
		ShaderGL()
			: m_id(0)
			, m_type(0)
			, m_hash(0)
		{
		}

		void create(const Memory* _mem);
		void destroy();

		GLuint m_id;
		GLenum m_type;
		uint32_t m_hash;
	};

	struct FrameBufferGL
	{
		FrameBufferGL()
			: m_swapChain(NULL)
			, m_denseIdx(UINT16_MAX)
			, m_num(0)
			, m_needPresent(false)
		{
			bx::memSet(m_fbo, 0, sizeof(m_fbo) );
		}

		void create(uint8_t _num, const Attachment* _attachment);
		void create(uint16_t _denseIdx, void* _nwh, uint32_t _width, uint32_t _height, TextureFormat::Enum _format, TextureFormat::Enum _depthFormat);
		void postReset();
		uint16_t destroy();
		void resolve();
		void discard(uint16_t _flags);

		SwapChainGL* m_swapChain;
		GLuint m_fbo[2];
		uint32_t m_width;
		uint32_t m_height;
		uint16_t m_denseIdx;
		uint8_t  m_num;
		uint8_t  m_numTh;
		bool     m_needPresent;
		Attachment m_attachment[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
	};