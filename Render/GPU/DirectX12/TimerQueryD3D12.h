

	struct TimerQueryD3D12
	{
		TimerQueryD3D12()
			: m_control(BX_COUNTOF(m_query) )
		{
		}

		void init();
		void shutdown();
		uint32_t begin(uint32_t _resultIdx);
		void end(uint32_t _idx);
		bool update();

		struct Query
		{
			uint32_t m_resultIdx;
			bool     m_ready;
			uint64_t m_fence;
		};

		struct Result
		{
			void reset()
			{
				m_begin     = 0;
				m_end       = 0;
				m_pending   = 0;
			}

			uint64_t m_begin;
			uint64_t m_end;
			uint32_t m_pending;
		};

		uint64_t m_frequency;

		Result m_result[BGFX_CONFIG_MAX_VIEWS+1];
		Query m_query[BGFX_CONFIG_MAX_VIEWS*4];

		ID3D12Resource*  m_readback;
		ID3D12QueryHeap* m_queryHeap;
		uint64_t* m_queryResult;
		bx::RingBufferControl m_control;
	};