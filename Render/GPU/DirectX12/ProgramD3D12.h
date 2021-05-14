

	struct ProgramD3D12
	{
		ProgramD3D12()
			: m_vsh(NULL)
			, m_fsh(NULL)
		{
		}

		void create(const ShaderD3D12* _vsh, const ShaderD3D12* _fsh)
		{
			BX_ASSERT(NULL != _vsh->m_code, "Vertex shader doesn't exist.");
			m_vsh = _vsh;
			bx::memCopy(&m_predefined[0], _vsh->m_predefined, _vsh->m_numPredefined*sizeof(PredefinedUniform));
			m_numPredefined = _vsh->m_numPredefined;

			if (NULL != _fsh)
			{
				BX_ASSERT(NULL != _fsh->m_code, "Fragment shader doesn't exist.");
				m_fsh = _fsh;
				bx::memCopy(&m_predefined[m_numPredefined], _fsh->m_predefined, _fsh->m_numPredefined*sizeof(PredefinedUniform));
				m_numPredefined += _fsh->m_numPredefined;
			}
		}

		void destroy()
		{
			m_numPredefined = 0;
			m_vsh = NULL;
			m_fsh = NULL;
		}

		const ShaderD3D12* m_vsh;
		const ShaderD3D12* m_fsh;

		PredefinedUniform m_predefined[PredefinedUniform::Count * 2];
		uint8_t m_numPredefined;
	};