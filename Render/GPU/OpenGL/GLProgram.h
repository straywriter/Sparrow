

	struct ProgramGL
	{
		ProgramGL()
			: m_id(0)
			, m_constantBuffer(NULL)
			, m_numPredefined(0)
		{
			m_instanceData[0] = -1;
		}

		void create(const ShaderGL& _vsh, const ShaderGL& _fsh);
		void destroy();
		void init();
		void bindInstanceData(uint32_t _stride, uint32_t _baseVertex = 0) const;
		void unbindInstanceData() const;

		void bindAttributesBegin()
		{
			bx::memCopy(m_unboundUsedAttrib, m_used, sizeof(m_unboundUsedAttrib) );
		}

		void bindAttributes(const VertexLayout& _layout, uint32_t _baseVertex = 0);

		void bindAttributesEnd()
		{
			for (uint32_t ii = 0, iiEnd = m_usedCount; ii < iiEnd; ++ii)
			{
				if (Attrib::Count != m_unboundUsedAttrib[ii])
				{
					Attrib::Enum attr = Attrib::Enum(m_unboundUsedAttrib[ii]);
					GLint loc = m_attributes[attr];
					GL_CHECK(lazyDisableVertexAttribArray(loc) );
				}
			}
		}

		void unbindAttributes();

		GLuint m_id;

		uint8_t m_unboundUsedAttrib[Attrib::Count]; // For tracking unbound used attributes between begin()/end().
		uint8_t m_usedCount;
		uint8_t m_used[Attrib::Count]; // Dense.
		GLint m_attributes[Attrib::Count]; // Sparse.
		GLint m_instanceData[BGFX_CONFIG_MAX_INSTANCE_DATA_COUNT+1];

		GLint m_sampler[BGFX_CONFIG_MAX_TEXTURE_SAMPLERS];
		uint8_t m_numSamplers;

		UniformBuffer* m_constantBuffer;
		PredefinedUniform m_predefined[PredefinedUniform::Count];
		uint8_t m_numPredefined;
	};
