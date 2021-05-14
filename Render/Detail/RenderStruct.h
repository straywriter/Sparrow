

	/// Platform data.
	///
	/// @attention C99 equivalent is `bgfx_platform_data_t`.
	///
	struct PlatformData
	{
		PlatformData();

		void* ndt;          //!< Native display type (*nix specific).
		void* nwh;          //!< Native window handle. If `NULL` bgfx will create headless
		                    ///  context/device if renderer API supports it.
		void* context;      //!< GL context, or D3D device. If `NULL`, bgfx will create context/device.
		void* backBuffer;   //!< GL back-buffer, or D3D render target view. If `NULL` bgfx will
		                    ///  create back-buffer color surface.
		void* backBufferDS; //!< Backbuffer depth/stencil. If `NULL` bgfx will create back-buffer
		                    ///  depth/stencil surface.
	};

	/// Backbuffer resolution and reset parameters.
	///
	/// @attention C99 equivalent is `bgfx_resolution_t`.
	///
	struct Resolution
	{
		Resolution();

		TextureFormat::Enum format; //!< Backbuffer format.
		uint32_t width;             //!< Backbuffer width.
		uint32_t height;            //!< Backbuffer height.
		uint32_t reset;	            //!< Reset parameters.
		uint8_t  numBackBuffers;    //!< Number of back buffers.
		uint8_t  maxFrameLatency;   //!< Maximum frame latency.
	};

	/// Initialization parameters used by `bgfx::init`.
	///
	/// @attention C99 equivalent is `bgfx_init_t`.
	///
	struct Init
	{
		Init();

		/// Select rendering backend. When set to RendererType::Count
		/// a default rendering backend will be selected appropriate to the platform.
		/// See: `bgfx::RendererType`
		RendererType::Enum type;

		/// Vendor PCI id. If set to `BGFX_PCI_ID_NONE` it will select the first
		/// device.
		///   - `BGFX_PCI_ID_NONE` - Autoselect adapter.
		///   - `BGFX_PCI_ID_SOFTWARE_RASTERIZER` - Software rasterizer.
		///   - `BGFX_PCI_ID_AMD` - AMD adapter.
		///   - `BGFX_PCI_ID_INTEL` - Intel adapter.
		///   - `BGFX_PCI_ID_NVIDIA` - nVidia adapter.
		uint16_t vendorId;

		/// Device id. If set to 0 it will select first device, or device with
		/// matching id.
		uint16_t deviceId;

		uint64_t capabilities; //!< Capabilities initialization mask (default: UINT64_MAX).

		bool debug;   //!< Enable device for debuging.
		bool profile; //!< Enable device for profiling.

		/// Platform data.
		PlatformData platformData;

		/// Backbuffer resolution and reset parameters. See: `bgfx::Resolution`.
		Resolution resolution;

		/// Configurable runtime limits parameters.
		///
		/// @attention C99 equivalent is `bgfx_init_limits_t`.
		///
		struct Limits
		{
			Limits();

			uint16_t maxEncoders;       //!< Maximum number of encoder threads.
			uint32_t minResourceCbSize; //!< Minimum resource command buffer size.
			uint32_t transientVbSize;   //!< Maximum transient vertex buffer size.
			uint32_t transientIbSize;   //!< Maximum transient index buffer size.
		};

		Limits limits; // Configurable runtime limits.

		/// Provide application specific callback interface.
		/// See: `bgfx::CallbackI`
		CallbackI* callback;

		/// Custom allocator. When a custom allocator is not
		/// specified, bgfx uses the CRT allocator. Bgfx assumes
		/// custom allocator is thread safe.
		bx::AllocatorI* allocator;
	};

	/// Memory release callback.
	///
	/// param[in] _ptr Pointer to allocated data.
	/// param[in] _userData User defined data if needed.
	///
	/// @attention C99 equivalent is `bgfx_release_fn_t`.
	///
	typedef void (*ReleaseFn)(void* _ptr, void* _userData);

	/// Memory must be obtained by calling `bgfx::alloc`, `bgfx::copy`, or `bgfx::makeRef`.
	///
	/// @attention It is illegal to create this structure on stack and pass it to any bgfx API.
	///
	/// @attention C99 equivalent is `bgfx_memory_t`.
	///
	struct Memory
	{
		Memory() = delete;

		uint8_t* data; //!< Pointer to data.
		uint32_t size; //!< Data size.
	};

	/// Renderer capabilities.
	///
	/// @attention C99 equivalent is `bgfx_caps_t`.
	///
	struct Caps
	{
		/// Renderer backend type. See: `bgfx::RendererType`
		RendererType::Enum rendererType;

		/// Supported functionality.
		///
		/// @attention See BGFX_CAPS_* flags at https://bkaradzic.github.io/bgfx/bgfx.html#available-caps
		///
		uint64_t supported;

		uint16_t vendorId;         //!< Selected GPU vendor PCI id.
		uint16_t deviceId;         //!< Selected GPU device id.
		bool     homogeneousDepth; //!< True when NDC depth is in [-1, 1] range, otherwise its [0, 1].
		bool     originBottomLeft; //!< True when NDC origin is at bottom left.
		uint8_t  numGPUs;          //!< Number of enumerated GPUs.

		/// GPU info.
		///
		/// @attention C99 equivalent is `bgfx_caps_gpu_t`.
		///
		struct GPU
		{
			uint16_t vendorId; //!< Vendor PCI id. See `BGFX_PCI_ID_*`.
			uint16_t deviceId; //!< Device id.
		};

		GPU gpu[4]; //!< Enumerated GPUs.

		/// Renderer runtime limits.
		///
		/// @attention C99 equivalent is `bgfx_caps_limits_t`.
		///
		struct Limits
		{
			uint32_t maxDrawCalls;            //!< Maximum number of draw calls.
			uint32_t maxBlits;                //!< Maximum number of blit calls.
			uint32_t maxTextureSize;          //!< Maximum texture size.
			uint32_t maxTextureLayers;        //!< Maximum texture layers.
			uint32_t maxViews;                //!< Maximum number of views.
			uint32_t maxFrameBuffers;         //!< Maximum number of frame buffer handles.
			uint32_t maxFBAttachments;        //!< Maximum number of frame buffer attachments.
			uint32_t maxPrograms;             //!< Maximum number of program handles.
			uint32_t maxShaders;              //!< Maximum number of shader handles.
			uint32_t maxTextures;             //!< Maximum number of texture handles.
			uint32_t maxTextureSamplers;      //!< Maximum number of texture samplers.
			uint32_t maxComputeBindings;      //!< Maximum number of compute bindings.
			uint32_t maxVertexLayouts;        //!< Maximum number of vertex format layouts.
			uint32_t maxVertexStreams;        //!< Maximum number of vertex streams.
			uint32_t maxIndexBuffers;         //!< Maximum number of index buffer handles.
			uint32_t maxVertexBuffers;        //!< Maximum number of vertex buffer handles.
			uint32_t maxDynamicIndexBuffers;  //!< Maximum number of dynamic index buffer handles.
			uint32_t maxDynamicVertexBuffers; //!< Maximum number of dynamic vertex buffer handles.
			uint32_t maxUniforms;             //!< Maximum number of uniform handles.
			uint32_t maxOcclusionQueries;     //!< Maximum number of occlusion query handles.
			uint32_t maxEncoders;             //!< Maximum number of encoder threads.
			uint32_t minResourceCbSize;       //!< Minimum resource command buffer size.
			uint32_t transientVbSize;         //!< Maximum transient vertex buffer size.
			uint32_t transientIbSize;         //!< Maximum transient index buffer size.
		};

		Limits limits; //!< Renderer runtime limits.

		/// Supported texture format capabilities flags:
		///   - `BGFX_CAPS_FORMAT_TEXTURE_NONE` - Texture format is not supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_2D` - Texture format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB` - Texture as sRGB format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_2D_EMULATED` - Texture format is emulated.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_3D` - Texture format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB` - Texture as sRGB format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_3D_EMULATED` - Texture format is emulated.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_CUBE` - Texture format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB` - Texture as sRGB format is supported.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_CUBE_EMULATED` - Texture format is emulated.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_VERTEX` - Texture format can be used from vertex shader.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_IMAGE_READ` - Texture format can be used as image
		///     and read from.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_IMAGE_WRITE` - Texture format can be used as image
		///     and written to.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER` - Texture format can be used as frame
		///     buffer.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER_MSAA` - Texture format can be used as MSAA
		///     frame buffer.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_MSAA` - Texture can be sampled as MSAA.
		///   - `BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN` - Texture format supports auto-generated
		///     mips.
		uint16_t formats[TextureFormat::Count];
	};

	/// Transient index buffer.
	///
	/// @attention C99 equivalent is `bgfx_transient_index_buffer_t`.
	///
	struct TransientIndexBuffer
	{
		uint8_t* data;            //!< Pointer to data.
		uint32_t size;            //!< Data size.
		uint32_t startIndex;      //!< First index.
		IndexBufferHandle handle; //!< Index buffer handle.
		bool isIndex16;           //!< Index buffer format is 16-bits if true, otherwise it is 32-bit.
	};

	/// Transient vertex buffer.
	///
	/// @attention C99 equivalent is `bgfx_transient_vertex_buffer_t`.
	///
	struct TransientVertexBuffer
	{
		uint8_t* data;                      //!< Pointer to data.
		uint32_t size;                      //!< Data size.
		uint32_t startVertex;               //!< First vertex.
		uint16_t stride;                    //!< Vertex stride.
		VertexBufferHandle handle;          //!< Vertex buffer handle.
		VertexLayoutHandle layoutHandle;    //!< Vertex layout handle.
	};

	/// Instance data buffer info.
	///
	/// @attention C99 equivalent is `bgfx_instance_data_buffer_t`.
	///
	struct InstanceDataBuffer
	{
		uint8_t* data;             //!< Pointer to data.
		uint32_t size;             //!< Data size.
		uint32_t offset;           //!< Offset in vertex buffer.
		uint32_t num;              //!< Number of instances.
		uint16_t stride;           //!< Vertex buffer stride.
		VertexBufferHandle handle; //!< Vertex buffer object handle.
	};

	/// Texture info.
	///
	/// @attention C99 equivalent is `bgfx_texture_info_t`.
	///
	struct TextureInfo
	{
		TextureFormat::Enum format; //!< Texture format.
		uint32_t storageSize;       //!< Total amount of bytes required to store texture.
		uint16_t width;             //!< Texture width.
		uint16_t height;            //!< Texture height.
		uint16_t depth;             //!< Texture depth.
		uint16_t numLayers;         //!< Number of layers in texture array.
		uint8_t numMips;            //!< Number of MIP maps.
		uint8_t bitsPerPixel;       //!< Format bits per pixel.
		bool    cubeMap;            //!< Texture is cubemap.
	};

	/// Uniform info.
	///
	/// @attention C99 equivalent is `bgfx_uniform_info_t`.
	///
	struct UniformInfo
	{
		char name[256];         //!< Uniform name.
		UniformType::Enum type; //!< Uniform type.
		uint16_t num;           //!< Number of elements in array.
	};

	/// Frame buffer texture attachment info.
	///
	/// @attention C99 equivalent is `bgfx_attachment_t`.
	///
	struct Attachment
	{
		/// Init attachment.
		///
		/// @param[in] _handle Render target texture handle.
		/// @param[in] _access Access. See `Access::Enum`.
		/// @param[in] _layer Cubemap side or depth layer/slice to use.
		/// @param[in] _numLayers Number of texture layer/slice(s) in array to use.
		/// @param[in] _mip Mip level.
		/// @param[in] _resolve Resolve flags. See: `BGFX_RESOLVE_*`
		///
		void init(
			  TextureHandle _handle
			, Access::Enum _access = Access::Write
			, uint16_t _layer = 0
			, uint16_t _numLayers = 1
			, uint16_t _mip = 0
			, uint8_t _resolve = BGFX_RESOLVE_AUTO_GEN_MIPS
			);

		Access::Enum  access; //!< Attachment access. See `Access::Enum`.
		TextureHandle handle; //!< Render target texture handle.
		uint16_t mip;         //!< Mip level.
		uint16_t layer;       //!< Cubemap side or depth layer/slice to use.
		uint16_t numLayers;   //!< Number of texture layer/slice(s) in array to use.
		uint8_t  resolve;     //!< Resolve flags. See: `BGFX_RESOLVE_*`
	};

	/// Transform data.
	///
	/// @attention C99 equivalent is `bgfx_transform_t`.
	///
	struct Transform
	{
		float* data;  //!< Pointer to first 4x4 matrix.
		uint16_t num; //!< Number of matrices.
	};
