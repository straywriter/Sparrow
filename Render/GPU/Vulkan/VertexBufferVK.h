

struct VertexBufferVK : public BufferVK
	{
		void create(VkCommandBuffer _commandBuffer, uint32_t _size, void* _data, VertexLayoutHandle _layoutHandle, uint16_t _flags);

		VertexLayoutHandle m_layoutHandle;
	};