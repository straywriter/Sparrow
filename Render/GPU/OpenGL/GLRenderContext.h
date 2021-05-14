
struct RendererContextGL : public RendererContextI
{
    RendererContextGL()
        : m_numWindows(1), m_rtMsaa(false), m_fbDiscard(BGFX_CLEAR_NONE), m_capture(NULL), m_captureSize(0),
          m_maxAnisotropy(0.0f), m_maxAnisotropyDefault(0.0f), m_maxMsaa(0), m_vao(0), m_blitSupported(false),
          m_readBackSupported(BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL)), m_vaoSupport(false),
          m_samplerObjectSupport(false), m_shadowSamplersSupport(false),
          m_srgbWriteControlSupport(BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL)),
          m_borderColorSupport(BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL)), m_programBinarySupport(false),
          m_textureSwizzleSupport(false), m_depthTextureSupport(false), m_timerQuerySupport(false),
          m_occlusionQuerySupport(false), m_atocSupport(false), m_conservativeRasterSupport(false), m_flip(false),
          m_hash((BX_PLATFORM_WINDOWS << 1) | BX_ARCH_64BIT), m_backBufferFbo(0), m_msaaBackBufferFbo(0),
          m_clearQuadColor(BGFX_INVALID_HANDLE), m_clearQuadDepth(BGFX_INVALID_HANDLE)
    {
        bx::memSet(m_msaaBackBufferRbos, 0, sizeof(m_msaaBackBufferRbos));
    }

    ~RendererContextGL()
    {
    }

    bool init(const Init &_init)
    {
        struct ErrorState
        {
            enum Enum
            {
                Default,
            };
        };

        ErrorState::Enum errorState = ErrorState::Default;

        if (_init.debug || _init.profile)
        {
            m_renderdocdll = loadRenderDoc();
        }

        m_fbh.idx = kInvalidHandle;
        bx::memSet(m_uniforms, 0, sizeof(m_uniforms));
        bx::memSet(&m_resolution, 0, sizeof(m_resolution));

        setRenderContextSize(_init.resolution.width, _init.resolution.height);

        m_vendor = getGLString(GL_VENDOR);
        m_renderer = getGLString(GL_RENDERER);
        m_version = getGLString(GL_VERSION);
        m_glslVersion = getGLString(GL_SHADING_LANGUAGE_VERSION);

        int glVersion;
        int majorGlVersion = 0;
        int minorGlVersion = 0;
        const char *version = m_version;

        while (*version && (*version < '0' || *version > '9'))
        {
            ++version;
        }

        majorGlVersion = atoi(version);
        minorGlVersion = atoi(version + 2);
        glVersion = majorGlVersion * 10 + minorGlVersion;

        for (uint32_t ii = 0; ii < BX_COUNTOF(s_vendorIds); ++ii)
        {
            const VendorId &vendorId = s_vendorIds[ii];
            if (0 == bx::strCmp(vendorId.name, m_vendor, bx::strLen(vendorId.name)))
            {
                g_caps.vendorId = vendorId.id;
                break;
            }
        }

        m_workaround.reset();

        GLint numCmpFormats = 0;
        GL_CHECK(glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCmpFormats));
        BX_TRACE("GL_NUM_COMPRESSED_TEXTURE_FORMATS %d", numCmpFormats);

        GLint *cmpFormat = NULL;

        if (0 < numCmpFormats)
        {
            numCmpFormats = numCmpFormats > 256 ? 256 : numCmpFormats;
            cmpFormat = (GLint *)alloca(sizeof(GLint) * numCmpFormats);
            GL_CHECK(glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, cmpFormat));

            for (GLint ii = 0; ii < numCmpFormats; ++ii)
            {
                GLint internalFmt = cmpFormat[ii];
                uint32_t fmt = uint32_t(TextureFormat::Unknown);
                for (uint32_t jj = 0; jj < fmt; ++jj)
                {
                    if (s_textureFormat[jj].m_internalFmt == (GLenum)internalFmt)
                    {
                        s_textureFormat[jj].m_supported = true;
                        fmt = jj;
                    }
                }

                BX_TRACE("  %3d: %8x %s", ii, internalFmt, getName((TextureFormat::Enum)fmt));
            }
        }

        if (BX_ENABLED(BGFX_CONFIG_DEBUG))
        {
#define GL_GET(_pname, _min) BX_TRACE("  " #_pname " %d (min: %d)", glGet(_pname), _min)
            BX_TRACE("Defaults:");
#if BGFX_CONFIG_RENDERER_OPENGL >= 41 || BGFX_CONFIG_RENDERER_OPENGLES
            GL_GET(GL_MAX_FRAGMENT_UNIFORM_VECTORS, 16);
            GL_GET(GL_MAX_VERTEX_UNIFORM_VECTORS, 128);
            GL_GET(GL_MAX_VARYING_VECTORS, 8);
#else
            GL_GET(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, 16 * 4);
            GL_GET(GL_MAX_VERTEX_UNIFORM_COMPONENTS, 128 * 4);
            GL_GET(GL_MAX_VARYING_FLOATS, 8 * 4);
#endif // BGFX_CONFIG_RENDERER_OPENGL >= 41 || BGFX_CONFIG_RENDERER_OPENGLES
            GL_GET(GL_MAX_VERTEX_ATTRIBS, 8);
            GL_GET(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, 8);
            GL_GET(GL_MAX_CUBE_MAP_TEXTURE_SIZE, 16);
            GL_GET(GL_MAX_TEXTURE_IMAGE_UNITS, 8);
            GL_GET(GL_MAX_TEXTURE_SIZE, 64);
            GL_GET(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, 0);
            GL_GET(GL_MAX_RENDERBUFFER_SIZE, 1);
            GL_GET(GL_MAX_COLOR_ATTACHMENTS, 1);
            GL_GET(GL_MAX_DRAW_BUFFERS, 1);

#undef GL_GET

            BX_TRACE("      Vendor: %s", m_vendor);
            BX_TRACE("    Renderer: %s", m_renderer);
            BX_TRACE("     Version: %s", m_version);
            BX_TRACE("GLSL version: %s", m_glslVersion);
        }

        // Initial binary shader hash depends on driver version.
        m_hash = ((BX_PLATFORM_WINDOWS << 1) | BX_ARCH_64BIT) ^ (uint64_t(getGLStringHash(GL_VENDOR)) << 32) ^
                 (uint64_t(getGLStringHash(GL_RENDERER)) << 0) ^ (uint64_t(getGLStringHash(GL_VERSION)) << 16);

        if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 31) && 0 == bx::strCmp(m_vendor, "Imagination Technologies") &&
            !bx::strFind(m_version, "(SDK 3.5@3510720)").isEmpty())
        {
            // Skip initializing extensions that are broken in emulator.
            s_extension[Extension::ARB_program_interface_query].m_initialize =
                s_extension[Extension::ARB_shader_storage_buffer_object].m_initialize = false;
        }

        if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES) && 0 == bx::strCmp(m_vendor, "Imagination Technologies") &&
            !bx::strFind(m_version, "1.8@905891").isEmpty())
        {
            m_workaround.m_detachShader = false;
        }

        if (BX_ENABLED(BGFX_CONFIG_RENDERER_USE_EXTENSIONS))
        {
            const char *extensions = (const char *)glGetString(GL_EXTENSIONS);
            getGlError(); // ignore error if glGetString returns NULL.
            if (NULL != extensions)
            {
                bx::StringView ext(extensions);
                uint32_t index = 0;
                while (!ext.isEmpty())
                {
                    const bx::StringView space = bx::strFind(ext, ' ');
                    const bx::StringView token = bx::StringView(ext.getPtr(), space.getPtr());
                    updateExtension(token);

                    ext.set(space.getPtr() + (space.isEmpty() ? 0 : 1), ext.getTerm());

                    ++index;
                }
            }
            else if (NULL != glGetStringi)
            {
                GLint numExtensions = 0;
                glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
                getGlError(); // ignore error if glGetIntegerv returns NULL.

                for (GLint index = 0; index < numExtensions; ++index)
                {
                    const char *name = (const char *)glGetStringi(GL_EXTENSIONS, index);
                    updateExtension(name);
                }
            }

            BX_TRACE("Supported extensions:");
            for (uint32_t ii = 0; ii < Extension::Count; ++ii)
            {
                if (s_extension[ii].m_supported)
                {
                    BX_TRACE("\t%2d: %s", ii, s_extension[ii].m_name);
                }
            }
        }

        if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) && !s_extension[Extension::ARB_framebuffer_object].m_supported)
        {
            BX_TRACE("Init error: ARB_framebuffer_object not supported.");
            goto error;
        }

        {
            // Allow all texture filters.
            bx::memSet(s_textureFilter, true, BX_COUNTOF(s_textureFilter));

            bool bc123Supported = 0 || s_extension[Extension::EXT_texture_compression_s3tc].m_supported ||
                                  s_extension[Extension::MOZ_WEBGL_compressed_texture_s3tc].m_supported ||
                                  s_extension[Extension::WEBGL_compressed_texture_s3tc].m_supported ||
                                  s_extension[Extension::WEBKIT_WEBGL_compressed_texture_s3tc].m_supported;
            s_textureFormat[TextureFormat::BC1].m_supported |=
                bc123Supported || s_extension[Extension::ANGLE_texture_compression_dxt1].m_supported ||
                s_extension[Extension::EXT_texture_compression_dxt1].m_supported;

            if (!s_textureFormat[TextureFormat::BC1].m_supported &&
                (s_textureFormat[TextureFormat::BC2].m_supported || s_textureFormat[TextureFormat::BC3].m_supported))
            {
                // If RGBA_S3TC_DXT1 is not supported, maybe RGB_S3TC_DXT1 is?
                for (GLint ii = 0; ii < numCmpFormats; ++ii)
                {
                    if (GL_COMPRESSED_RGB_S3TC_DXT1_EXT == cmpFormat[ii])
                    {
                        setTextureFormat(TextureFormat::BC1, GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
                                         GL_COMPRESSED_RGB_S3TC_DXT1_EXT);
                        s_textureFormat[TextureFormat::BC1].m_supported = true;
                        break;
                    }
                }
            }

            s_textureFormat[TextureFormat::BC2].m_supported |=
                bc123Supported || s_extension[Extension::ANGLE_texture_compression_dxt3].m_supported ||
                s_extension[Extension::CHROMIUM_texture_compression_dxt3].m_supported;

            s_textureFormat[TextureFormat::BC3].m_supported |=
                bc123Supported || s_extension[Extension::ANGLE_texture_compression_dxt5].m_supported ||
                s_extension[Extension::CHROMIUM_texture_compression_dxt5].m_supported;

            if (s_extension[Extension::EXT_texture_compression_latc].m_supported)
            {
                setTextureFormat(TextureFormat::BC4, GL_COMPRESSED_LUMINANCE_LATC1_EXT,
                                 GL_COMPRESSED_LUMINANCE_LATC1_EXT);
                setTextureFormat(TextureFormat::BC5, GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,
                                 GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT);
            }

            if (s_extension[Extension::ARB_texture_compression_rgtc].m_supported ||
                s_extension[Extension::EXT_texture_compression_rgtc].m_supported)
            {
                setTextureFormat(TextureFormat::BC4, GL_COMPRESSED_RED_RGTC1, GL_COMPRESSED_RED_RGTC1);
                setTextureFormat(TextureFormat::BC5, GL_COMPRESSED_RG_RGTC2, GL_COMPRESSED_RG_RGTC2);
            }

            bool etc1Supported = 0 || s_extension[Extension::OES_compressed_ETC1_RGB8_texture].m_supported ||
                                 s_extension[Extension::WEBGL_compressed_texture_etc1].m_supported;
            s_textureFormat[TextureFormat::ETC1].m_supported |= etc1Supported;

            bool etc2Supported =
                !!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) || s_extension[Extension::ARB_ES3_compatibility].m_supported;
            s_textureFormat[TextureFormat::ETC2].m_supported |= etc2Supported;
            s_textureFormat[TextureFormat::ETC2A].m_supported |= etc2Supported;
            s_textureFormat[TextureFormat::ETC2A1].m_supported |= etc2Supported;

            if (!s_textureFormat[TextureFormat::ETC1].m_supported && s_textureFormat[TextureFormat::ETC2].m_supported)
            {
                // When ETC2 is supported override ETC1 texture format settings.
                s_textureFormat[TextureFormat::ETC1].m_internalFmt = GL_COMPRESSED_RGB8_ETC2;
                s_textureFormat[TextureFormat::ETC1].m_fmt = GL_COMPRESSED_RGB8_ETC2;
                s_textureFormat[TextureFormat::ETC1].m_supported = true;
            }

            bool ptc1Supported = 0 || s_extension[Extension::IMG_texture_compression_pvrtc].m_supported ||
                                 s_extension[Extension::WEBGL_compressed_texture_pvrtc].m_supported;
            s_textureFormat[TextureFormat::PTC12].m_supported |= ptc1Supported;
            s_textureFormat[TextureFormat::PTC14].m_supported |= ptc1Supported;
            s_textureFormat[TextureFormat::PTC12A].m_supported |= ptc1Supported;
            s_textureFormat[TextureFormat::PTC14A].m_supported |= ptc1Supported;

            bool ptc2Supported = s_extension[Extension::IMG_texture_compression_pvrtc2].m_supported;
            s_textureFormat[TextureFormat::PTC22].m_supported |= ptc2Supported;
            s_textureFormat[TextureFormat::PTC24].m_supported |= ptc2Supported;

            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES))
            {
                if (glVersion < 30)
                {
                    setTextureFormat(TextureFormat::RGBA16F, GL_RGBA, GL_RGBA,
                                     GL_HALF_FLOAT); // Note: this is actually GL_HALF_FLOAT_OES and not GL_HALF_FLOAT
                                                     // if compiling for GLES target.
                    setTextureFormat(TextureFormat::RGBA32F, GL_RGBA, GL_RGBA, GL_FLOAT);
                    // internalFormat and format must match:
                    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
                    setTextureFormat(TextureFormat::RGBA8, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
                    setTextureFormat(TextureFormat::R5G6B5, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV);
                    setTextureFormat(TextureFormat::RGBA4, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4_REV);
                    setTextureFormat(TextureFormat::RGB5A1, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV);

                    if (s_extension[Extension::EXT_sRGB].m_supported)
                    {
                        setTextureFormatSrgb(TextureFormat::RGBA8, GL_SRGB_ALPHA_EXT, GL_SRGB_ALPHA_EXT);
                        setTextureFormatSrgb(TextureFormat::RGB8, GL_SRGB_EXT, GL_SRGB_EXT);
                    }

                    if (s_extension[Extension::OES_texture_half_float].m_supported ||
                        s_extension[Extension::OES_texture_float].m_supported)
                    {
                        // https://www.khronos.org/registry/gles/extensions/OES/OES_texture_float.txt
                        // When half/float is available via extensions texture will be marked as
                        // incomplete if it uses anything other than nearest filter.
                        const bool linear16F = s_extension[Extension::OES_texture_half_float_linear].m_supported;
                        const bool linear32F = s_extension[Extension::OES_texture_float_linear].m_supported;

                        s_textureFilter[TextureFormat::R16F] = linear16F;
                        s_textureFilter[TextureFormat::RG16F] = linear16F;
                        s_textureFilter[TextureFormat::RGBA16F] = linear16F;
                        s_textureFilter[TextureFormat::R32F] = linear32F;
                        s_textureFilter[TextureFormat::RG32F] = linear32F;
                        s_textureFilter[TextureFormat::RGBA32F] = linear32F;
                    }
                }
                else
                {
                    setTextureFormat(
                        TextureFormat::R16F, GL_R16F, GL_RED,
                        0x140B /* == GL_HALF_FLOAT, but bgfx overwrites it globally with GL_HALF_FLOAT_OES */);
                    setTextureFormat(
                        TextureFormat::RG16F, GL_RG16F, GL_RG,
                        0x140B /* == GL_HALF_FLOAT, but bgfx overwrites it globally with GL_HALF_FLOAT_OES */);
                    setTextureFormat(
                        TextureFormat::RGBA16F, GL_RGBA16F, GL_RGBA,
                        0x140B /* == GL_HALF_FLOAT, but bgfx overwrites it globally with GL_HALF_FLOAT_OES */);
                }

                if (BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) &&
                    (s_extension[Extension::WEBGL_depth_texture].m_supported ||
                     s_extension[Extension::MOZ_WEBGL_depth_texture].m_supported ||
                     s_extension[Extension::WEBKIT_WEBGL_depth_texture].m_supported))
                {
                    setTextureFormat(TextureFormat::D16, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
                    setTextureFormat(
                        TextureFormat::D24, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
                        GL_UNSIGNED_INT); // N.b. OpenGL ES does not guarantee that there are 24 bits available here,
                                          // could be 16. See
                                          // https://www.khronos.org/registry/webgl/extensions/WEBGL_depth_texture/
                    setTextureFormat(TextureFormat::D32, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
                                     GL_UNSIGNED_INT); // N.b. same as above.
                    setTextureFormat(TextureFormat::D24S8, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
                }

                // OpenGL ES 3.0 depth formats.
                if (glVersion >= 30)
                {
                    setTextureFormat(TextureFormat::D16, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
                    setTextureFormat(TextureFormat::D24, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
                    setTextureFormat(TextureFormat::D32, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
                    setTextureFormat(TextureFormat::D24S8, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
                    setTextureFormat(TextureFormat::D16F, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT,
                                     GL_FLOAT); // GLES 3.0 does not have D16F, overshoot to D32F
                    setTextureFormat(TextureFormat::D24F, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT,
                                     GL_FLOAT); // GLES 3.0 does not have D24F, overshoot to D32F
                    setTextureFormat(TextureFormat::D32F, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
                    setTextureFormat(TextureFormat::D0S8, GL_STENCIL_INDEX8, GL_STENCIL_INDEX,
                                     GL_UNSIGNED_BYTE); // Only works as renderbuffer, not as texture
                }
            }

            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30))
            {
                setTextureFormat(TextureFormat::R8I, GL_R8I, GL_RED_INTEGER, GL_BYTE);
                setTextureFormat(TextureFormat::R8U, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
                setTextureFormat(TextureFormat::R16I, GL_R16I, GL_RED_INTEGER, GL_SHORT);
                setTextureFormat(TextureFormat::R16U, GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
                //				setTextureFormat(TextureFormat::RG16,    GL_RG16UI,   GL_RG_INTEGER, GL_UNSIGNED_SHORT);
                //				setTextureFormat(TextureFormat::RGBA16,  GL_RGBA16UI, GL_RGBA_INTEGER,
                // GL_UNSIGNED_SHORT);
                setTextureFormat(TextureFormat::R32U, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
                setTextureFormat(TextureFormat::RG32U, GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT);
                setTextureFormat(TextureFormat::RGBA32U, GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT);
            }

            if (s_extension[Extension::EXT_texture_format_BGRA8888].m_supported ||
                s_extension[Extension::EXT_bgra].m_supported ||
                s_extension[Extension::IMG_texture_format_BGRA8888].m_supported ||
                s_extension[Extension::APPLE_texture_format_BGRA8888].m_supported)
            {
                if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL))
                {
                    m_readPixelsFmt = GL_BGRA;
                }

                // Mixing GLES and GL extensions here. OpenGL EXT_bgra and
                // APPLE_texture_format_BGRA8888 wants
                // format to be BGRA but internal format to stay RGBA, but
                // EXT_texture_format_BGRA8888 wants both format and internal
                // format to be BGRA.
                //
                // Reference(s):
                // -
                // https://web.archive.org/web/20181126035829/https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_texture_format_BGRA8888.txt
                // -
                // https://web.archive.org/web/20181126035841/https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_bgra.txt
                // -
                // https://web.archive.org/web/20181126035851/https://www.khronos.org/registry/OpenGL/extensions/APPLE/APPLE_texture_format_BGRA8888.txt
                //
                if (!s_extension[Extension::EXT_bgra].m_supported &&
                    !s_extension[Extension::APPLE_texture_format_BGRA8888].m_supported)
                {
                    s_textureFormat[TextureFormat::BGRA8].m_internalFmt = GL_BGRA;
                }

                if (!isTextureFormatValid(TextureFormat::BGRA8))
                {
                    // Revert back to RGBA if texture can't be created.
                    setTextureFormat(TextureFormat::BGRA8, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE);
                }
            }

            if (BX_ENABLED(BX_PLATFORM_EMSCRIPTEN))
            {
                // OpenGL ES does not have reversed BGRA4 and BGR5A1 support.
                setTextureFormat(TextureFormat::RGBA4, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
                setTextureFormat(TextureFormat::RGB5A1, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1);
                setTextureFormat(TextureFormat::R5G6B5, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);

                if (glVersion < 30)
                {
                    // OpenGL ES 2.0 uses unsized internal formats.
                    s_textureFormat[TextureFormat::RGB8].m_internalFmt = GL_RGB;

                    // OpenGL ES 2.0 does not have R8 texture format, only L8. Open GL ES 2.0 extension
                    // https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_texture_rg.txt adds support for R8 to
                    // GLES 2.0 core contexts. For those use L8 instead.
                    if (!s_extension[Extension::EXT_texture_rg].m_supported)
                    {
                        s_textureFormat[TextureFormat::R8].m_internalFmt = GL_LUMINANCE;
                        s_textureFormat[TextureFormat::R8].m_fmt = GL_LUMINANCE;
                    }
                }
            }

            for (uint32_t ii = BX_ENABLED(BX_PLATFORM_IOS) ? TextureFormat::Unknown : 0 // skip test on iOS!
                 ;
                 ii < TextureFormat::Count; ++ii)
            {
                if (TextureFormat::Unknown != ii && TextureFormat::UnknownDepth != ii)
                {
                    s_textureFormat[ii].m_supported = isTextureFormatValid(TextureFormat::Enum(ii));
                }
            }

            if (BX_ENABLED(0))
            {
                // Disable all compressed texture formats. For testing only.
                for (uint32_t ii = 0; ii < TextureFormat::Unknown; ++ii)
                {
                    s_textureFormat[ii].m_supported = false;
                }
            }

            const bool computeSupport = false || !!(BGFX_CONFIG_RENDERER_OPENGLES >= 31) ||
                                        s_extension[Extension::ARB_compute_shader].m_supported;

            for (uint32_t ii = 0; ii < TextureFormat::Count; ++ii)
            {
                const TextureFormat::Enum fmt = TextureFormat::Enum(ii);

                uint16_t supported = BGFX_CAPS_FORMAT_TEXTURE_NONE;
                supported |=
                    s_textureFormat[ii].m_supported
                        ? BGFX_CAPS_FORMAT_TEXTURE_2D | BGFX_CAPS_FORMAT_TEXTURE_3D | BGFX_CAPS_FORMAT_TEXTURE_CUBE
                        : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                supported |= isTextureFormatValid(fmt, true)
                                 ? BGFX_CAPS_FORMAT_TEXTURE_2D_SRGB | BGFX_CAPS_FORMAT_TEXTURE_3D_SRGB |
                                       BGFX_CAPS_FORMAT_TEXTURE_CUBE_SRGB
                                 : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                if (!bimg::isCompressed(bimg::TextureFormat::Enum(fmt)))
                {
                    supported |= isTextureFormatValid(fmt, false, true) ? BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN
                                                                        : BGFX_CAPS_FORMAT_TEXTURE_NONE;
                }

                supported |= computeSupport && isImageFormatValid(fmt)
                                 ? (BGFX_CAPS_FORMAT_TEXTURE_IMAGE_READ | BGFX_CAPS_FORMAT_TEXTURE_IMAGE_WRITE)
                                 : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                supported |= isFramebufferFormatValid(fmt) ? BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER
                                                           : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                supported |= isFramebufferFormatValid(fmt, false, true) ? BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER
                                                                        : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                if (NULL != glGetInternalformativ)
                {
                    GLint maxSamples;
                    glGetInternalformativ(GL_RENDERBUFFER, s_textureFormat[ii].m_internalFmt, GL_SAMPLES, 1,
                                          &maxSamples);
                    GLenum err = getGlError();
                    supported |= 0 == err && maxSamples > 0 ? BGFX_CAPS_FORMAT_TEXTURE_FRAMEBUFFER_MSAA
                                                            : BGFX_CAPS_FORMAT_TEXTURE_NONE;

                    glGetInternalformativ(GL_TEXTURE_2D_MULTISAMPLE, s_textureFormat[ii].m_internalFmt, GL_SAMPLES, 1,
                                          &maxSamples);
                    err = getGlError();
                    supported |=
                        0 == err && maxSamples > 0 ? BGFX_CAPS_FORMAT_TEXTURE_MSAA : BGFX_CAPS_FORMAT_TEXTURE_NONE;
                }

                g_caps.formats[ii] = supported;
            }

            g_caps.supported |= !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                        s_extension[Extension::OES_texture_3D].m_supported
                                    ? BGFX_CAPS_TEXTURE_3D
                                    : 0;
            g_caps.supported |= !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                        s_extension[Extension::EXT_shadow_samplers].m_supported
                                    ? BGFX_CAPS_TEXTURE_COMPARE_ALL
                                    : 0;
            g_caps.supported |= !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                        s_extension[Extension::OES_vertex_half_float].m_supported
                                    ? BGFX_CAPS_VERTEX_ATTRIB_HALF
                                    : 0;
            g_caps.supported |= false || s_extension[Extension::ARB_vertex_type_2_10_10_10_rev].m_supported ||
                                        s_extension[Extension::OES_vertex_type_10_10_10_2].m_supported
                                    ? BGFX_CAPS_VERTEX_ATTRIB_UINT10
                                    : 0;
            g_caps.supported |= !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                        s_extension[Extension::EXT_frag_depth].m_supported
                                    ? BGFX_CAPS_FRAGMENT_DEPTH
                                    : 0;
            g_caps.supported |=
                s_extension[Extension::ARB_draw_buffers_blend].m_supported ? BGFX_CAPS_BLEND_INDEPENDENT : 0;
            g_caps.supported |=
                s_extension[Extension::INTEL_fragment_shader_ordering].m_supported ? BGFX_CAPS_FRAGMENT_ORDERING : 0;
            g_caps.supported |= !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                        s_extension[Extension::OES_element_index_uint].m_supported
                                    ? BGFX_CAPS_INDEX32
                                    : 0;

            const bool drawIndirectSupported = false || s_extension[Extension::AMD_multi_draw_indirect].m_supported ||
                                               s_extension[Extension::ARB_draw_indirect].m_supported ||
                                               s_extension[Extension::ARB_multi_draw_indirect].m_supported ||
                                               s_extension[Extension::EXT_multi_draw_indirect].m_supported;

            if (drawIndirectSupported)
            {
                if (NULL == glMultiDrawArraysIndirect || NULL == glMultiDrawElementsIndirect)
                {
                    glMultiDrawArraysIndirect = stubMultiDrawArraysIndirect;
                    glMultiDrawElementsIndirect = stubMultiDrawElementsIndirect;
                }
            }

            g_caps.supported |= drawIndirectSupported ? BGFX_CAPS_DRAW_INDIRECT : 0;

            if (BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) || NULL == glPolygonMode)
            {
                glPolygonMode = stubPolygonMode;
            }

            if (s_extension[Extension::ARB_copy_image].m_supported ||
                s_extension[Extension::EXT_copy_image].m_supported ||
                s_extension[Extension::NV_copy_image].m_supported || s_extension[Extension::OES_copy_image].m_supported)
            {
                m_blitSupported = NULL != glCopyImageSubData;
            }

            g_caps.supported |=
                m_blitSupported || BX_ENABLED(BGFX_GL_CONFIG_BLIT_EMULATION) ? BGFX_CAPS_TEXTURE_BLIT : 0;

            g_caps.supported |= (m_readBackSupported || BX_ENABLED(BGFX_GL_CONFIG_TEXTURE_READ_BACK_EMULATION))
                                    ? BGFX_CAPS_TEXTURE_READ_BACK
                                    : 0;

            g_caps.supported |= false || s_extension[Extension::EXT_texture_array].m_supported ||
                                        s_extension[Extension::EXT_gpu_shader4].m_supported ||
                                        (!!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) && !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN))
                                    ? BGFX_CAPS_TEXTURE_2D_ARRAY
                                    : 0;

            g_caps.supported |= false || s_extension[Extension::EXT_gpu_shader4].m_supported ||
                                        (!!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) && !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN))
                                    ? BGFX_CAPS_VERTEX_ID
                                    : 0;

            g_caps.supported |= false || s_extension[Extension::ARB_texture_cube_map_array].m_supported ||
                                        s_extension[Extension::EXT_texture_cube_map_array].m_supported
                                    ? BGFX_CAPS_TEXTURE_CUBE_ARRAY
                                    : 0;

            g_caps.limits.maxTextureSize = uint16_t(glGet(GL_MAX_TEXTURE_SIZE));
            g_caps.limits.maxTextureLayers = BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL >= 31) ||
                                                     BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 31) ||
                                                     s_extension[Extension::EXT_texture_array].m_supported
                                                 ? uint16_t(bx::max(glGet(GL_MAX_ARRAY_TEXTURE_LAYERS), 1))
                                                 : 1;
            g_caps.limits.maxComputeBindings = computeSupport ? BGFX_MAX_COMPUTE_BINDINGS : 0;
            g_caps.limits.maxVertexStreams = BGFX_CONFIG_MAX_VERTEX_STREAMS;

            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                s_extension[Extension::EXT_draw_buffers].m_supported ||
                s_extension[Extension::WEBGL_draw_buffers].m_supported)
            {
                g_caps.limits.maxFBAttachments =
                    uint8_t(bx::uint32_clamp(glGet(GL_MAX_DRAW_BUFFERS), 1, BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS));
            }

            //				if (s_extension[Extension::ARB_clip_control].m_supported)
            //				{
            //					GL_CHECK(glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE) );
            //					g_caps.originBottomLeft = true;
            //				}
            //				else
            {
                g_caps.homogeneousDepth = true;
                g_caps.originBottomLeft = true;
            }

            m_vaoSupport =
                !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) && (!!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                                        s_extension[Extension::ARB_vertex_array_object].m_supported ||
                                                        s_extension[Extension::OES_vertex_array_object].m_supported);

            if (m_vaoSupport)
            {
                GL_CHECK(glGenVertexArrays(1, &m_vao));
            }

            m_samplerObjectSupport =
                !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) &&
                (!!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) || s_extension[Extension::ARB_sampler_objects].m_supported);

            m_shadowSamplersSupport = !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                      s_extension[Extension::EXT_shadow_samplers].m_supported;

            m_programBinarySupport =
                !BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) && (!!(BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                                        s_extension[Extension::ARB_get_program_binary].m_supported ||
                                                        s_extension[Extension::OES_get_program_binary].m_supported ||
                                                        s_extension[Extension::IMG_shader_binary].m_supported);

            m_textureSwizzleSupport = false || s_extension[Extension::ARB_texture_swizzle].m_supported ||
                                      s_extension[Extension::EXT_texture_swizzle].m_supported;

            m_depthTextureSupport = !!(BGFX_CONFIG_RENDERER_OPENGL || BGFX_CONFIG_RENDERER_OPENGLES >= 30) ||
                                    s_extension[Extension::ANGLE_depth_texture].m_supported ||
                                    s_extension[Extension::CHROMIUM_depth_texture].m_supported ||
                                    s_extension[Extension::GOOGLE_depth_texture].m_supported ||
                                    s_extension[Extension::OES_depth_texture].m_supported ||
                                    s_extension[Extension::MOZ_WEBGL_depth_texture].m_supported ||
                                    s_extension[Extension::WEBGL_depth_texture].m_supported ||
                                    s_extension[Extension::WEBKIT_WEBGL_depth_texture].m_supported;

            m_timerQuerySupport = false || s_extension[Extension::ANGLE_timer_query].m_supported ||
                                  s_extension[Extension::ARB_timer_query].m_supported ||
                                  s_extension[Extension::EXT_disjoint_timer_query].m_supported ||
                                  s_extension[Extension::EXT_timer_query].m_supported;

            m_timerQuerySupport &=
                true && NULL != glQueryCounter && NULL != glGetQueryObjectiv && NULL != glGetQueryObjectui64v;

            m_occlusionQuerySupport = false || s_extension[Extension::ARB_occlusion_query].m_supported ||
                                      s_extension[Extension::ARB_occlusion_query2].m_supported ||
                                      s_extension[Extension::EXT_occlusion_query_boolean].m_supported ||
                                      s_extension[Extension::NV_occlusion_query].m_supported;

            m_occlusionQuerySupport &=
                true && NULL != glGenQueries && NULL != glDeleteQueries && NULL != glBeginQuery && NULL != glEndQuery;

            m_atocSupport = s_extension[Extension::ARB_multisample].m_supported;
            m_conservativeRasterSupport = s_extension[Extension::NV_conservative_raster].m_supported;

            m_imageLoadStoreSupport = false || s_extension[Extension::ARB_shader_image_load_store].m_supported ||
                                      s_extension[Extension::EXT_shader_image_load_store].m_supported;

            g_caps.supported |= 0 | (m_atocSupport ? BGFX_CAPS_ALPHA_TO_COVERAGE : 0) |
                                (m_conservativeRasterSupport ? BGFX_CAPS_CONSERVATIVE_RASTER : 0) |
                                (m_occlusionQuerySupport ? BGFX_CAPS_OCCLUSION_QUERY : 0) |
                                (m_depthTextureSupport ? BGFX_CAPS_TEXTURE_COMPARE_LEQUAL : 0) |
                                (computeSupport ? BGFX_CAPS_COMPUTE : 0) |
                                (m_imageLoadStoreSupport ? BGFX_CAPS_IMAGE_RW : 0);

            g_caps.supported |= m_glctx.getCaps();

            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES))
            {
                m_srgbWriteControlSupport = s_extension[Extension::EXT_sRGB_write_control].m_supported;

                m_borderColorSupport = s_extension[Extension::NV_texture_border_clamp].m_supported;
                s_textureAddress[BGFX_SAMPLER_U_BORDER >> BGFX_SAMPLER_U_SHIFT] =
                    s_extension[Extension::NV_texture_border_clamp].m_supported ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
            }

            if (s_extension[Extension::EXT_texture_filter_anisotropic].m_supported)
            {
                GL_CHECK(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropyDefault));
            }

            if (s_extension[Extension::ARB_texture_multisample].m_supported ||
                s_extension[Extension::ANGLE_framebuffer_multisample].m_supported ||
                s_extension[Extension::EXT_multisampled_render_to_texture].m_supported)
            {
                GL_CHECK(glGetIntegerv(GL_MAX_SAMPLES, &m_maxMsaa));
            }

#if BGFX_CONFIG_RENDERER_OPENGLES && (BGFX_CONFIG_RENDERER_OPENGLES < 30)
            if (!m_maxMsaa && s_extension[Extension::IMG_multisampled_render_to_texture].m_supported)
            {
                GL_CHECK(glGetIntegerv(GL_MAX_SAMPLES_IMG, &m_maxMsaa));
            }
#endif // BGFX_CONFIG_RENDERER_OPENGLES < 30

            if (s_extension[Extension::OES_read_format].m_supported &&
                (s_extension[Extension::IMG_read_format].m_supported ||
                 s_extension[Extension::EXT_read_format_bgra].m_supported))
            {
                m_readPixelsFmt = GL_BGRA;
            }
            else
            {
                m_readPixelsFmt = GL_RGBA;
            }

            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30))
            {
                g_caps.supported |= BGFX_CAPS_INSTANCING;
            }
            else
            {
                if (s_extension[Extension::ANGLE_instanced_arrays].m_supported ||
                    s_extension[Extension::ARB_instanced_arrays].m_supported ||
                    s_extension[Extension::EXT_instanced_arrays].m_supported)
                {
                    if (NULL != glVertexAttribDivisor && NULL != glDrawArraysInstanced &&
                        NULL != glDrawElementsInstanced)
                    {
                        g_caps.supported |= BGFX_CAPS_INSTANCING;
                    }
                }

                if (0 == (g_caps.supported & BGFX_CAPS_INSTANCING))
                {
                    glVertexAttribDivisor = stubVertexAttribDivisor;
                    glDrawArraysInstanced = stubDrawArraysInstanced;
                    glDrawElementsInstanced = stubDrawElementsInstanced;
                }
            }

            g_caps.supported |= s_extension[Extension::ARB_shader_viewport_layer_array].m_supported
                                    ? BGFX_CAPS_VIEWPORT_LAYER_ARRAY
                                    : 0;

            if (s_extension[Extension::ARB_debug_output].m_supported || s_extension[Extension::KHR_debug].m_supported)
            {
                if (NULL != glDebugMessageControl && NULL != glDebugMessageInsert && NULL != glDebugMessageCallback &&
                    NULL != glGetDebugMessageLog)
                {
                    GL_CHECK(glDebugMessageCallback(debugProcCb, NULL));
                    GL_CHECK(
                        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_TRUE));
                }
            }

            if (NULL == glPushDebugGroup || NULL == glPopDebugGroup)
            {
                glPushDebugGroup = stubPushDebugGroup;
                glPopDebugGroup = stubPopDebugGroup;
            }

            if (s_extension[Extension::ARB_seamless_cube_map].m_supported)
            {
                GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
            }

            if (NULL != glProvokingVertex && s_extension[Extension::ARB_provoking_vertex].m_supported)
            {
                GL_CHECK(glProvokingVertex(GL_FIRST_VERTEX_CONVENTION));
            }

            if (NULL == glInsertEventMarker || !s_extension[Extension::EXT_debug_marker].m_supported)
            {
                glInsertEventMarker = stubInsertEventMarker;
            }

            m_maxLabelLen = BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 32) ||
                                    BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL >= 43) ||
                                    s_extension[Extension::KHR_debug].m_supported
                                ? uint16_t(glGet(GL_MAX_LABEL_LENGTH))
                                : 0;

            setGraphicsDebuggerPresent(s_extension[Extension::EXT_debug_tool].m_supported);

            if (NULL == glObjectLabel)
            {
                glObjectLabel = stubObjectLabel;
            }

            if (NULL == glInvalidateFramebuffer)
            {
                glInvalidateFramebuffer = stubInvalidateFramebuffer;
            }

            if (NULL == glFramebufferTexture)
            {
                glFramebufferTexture = stubFramebufferTexture;
            }

            if (m_timerQuerySupport)
            {
                m_gpuTimer.create();
            }

            if (m_occlusionQuerySupport)
            {
                m_occlusionQuery.create();
            }

            // Init reserved part of view name.
            for (uint32_t ii = 0; ii < BGFX_CONFIG_MAX_VIEWS; ++ii)
            {
                bx::snprintf(s_viewName[ii], BGFX_CONFIG_MAX_VIEW_NAME_RESERVED + 1, "%3d   ", ii);
            }

            m_needPresent = false;
        }

        return true;

    error:
        switch (errorState)
        {
        case ErrorState::Default:
            break;
        }

        m_glctx.destroy();

        unloadRenderDoc(m_renderdocdll);
        return false;
    }

    void shutdown()
    {
        if (m_vaoSupport)
        {
            GL_CHECK(glBindVertexArray(0));
            GL_CHECK(glDeleteVertexArrays(1, &m_vao));
            m_vao = 0;
        }

        captureFinish();

        invalidateCache();

        if (m_timerQuerySupport)
        {
            m_gpuTimer.destroy();
        }

        if (m_occlusionQuerySupport)
        {
            m_occlusionQuery.destroy();
        }

        destroyMsaaFbo();
        m_glctx.destroy();

        m_flip = false;

        unloadRenderDoc(m_renderdocdll);
    }

    RendererType::Enum getRendererType() const override
    {
        if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL))
        {
            return RendererType::OpenGL;
        }

        return RendererType::OpenGLES;
    }

    const char *getRendererName() const override
    {
        return BGFX_RENDERER_OPENGL_NAME;
    }

    bool isDeviceRemoved() override
    {
        return false;
    }

    void flip() override
    {
        if (m_flip)
        {
            for (uint32_t ii = 1, num = m_numWindows; ii < num; ++ii)
            {
                FrameBufferGL &frameBuffer = m_frameBuffers[m_windows[ii].idx];
                if (frameBuffer.m_needPresent)
                {
                    m_glctx.swap(frameBuffer.m_swapChain);
                    frameBuffer.m_needPresent = false;
                }
            }

            if (m_needPresent)
            {
                // Ensure the back buffer is bound as the source of the flip
                GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_backBufferFbo));

                m_glctx.swap();
                m_needPresent = false;
            }
        }
    }

    void createIndexBuffer(IndexBufferHandle _handle, const Memory *_mem, uint16_t _flags) override
    {
        m_indexBuffers[_handle.idx].create(_mem->size, _mem->data, _flags);
    }

    void destroyIndexBuffer(IndexBufferHandle _handle) override
    {
        m_indexBuffers[_handle.idx].destroy();
    }

    void createVertexLayout(VertexLayoutHandle _handle, const VertexLayout &_layout) override
    {
        VertexLayout &layout = m_vertexLayouts[_handle.idx];
        bx::memCopy(&layout, &_layout, sizeof(VertexLayout));
        dump(layout);
    }

    void destroyVertexLayout(VertexLayoutHandle /*_handle*/) override
    {
    }

    void createVertexBuffer(VertexBufferHandle _handle, const Memory *_mem, VertexLayoutHandle _layoutHandle,
                            uint16_t _flags) override
    {
        m_vertexBuffers[_handle.idx].create(_mem->size, _mem->data, _layoutHandle, _flags);
    }

    void destroyVertexBuffer(VertexBufferHandle _handle) override
    {
        m_vertexBuffers[_handle.idx].destroy();
    }

    void createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, uint16_t _flags) override
    {
        m_indexBuffers[_handle.idx].create(_size, NULL, _flags);
    }

    void updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _offset, uint32_t _size,
                                  const Memory *_mem) override
    {
        m_indexBuffers[_handle.idx].update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
    }

    void destroyDynamicIndexBuffer(IndexBufferHandle _handle) override
    {
        m_indexBuffers[_handle.idx].destroy();
    }

    void createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, uint16_t _flags) override
    {
        VertexLayoutHandle layoutHandle = BGFX_INVALID_HANDLE;
        m_vertexBuffers[_handle.idx].create(_size, NULL, layoutHandle, _flags);
    }

    void updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _offset, uint32_t _size,
                                   const Memory *_mem) override
    {
        m_vertexBuffers[_handle.idx].update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
    }

    void destroyDynamicVertexBuffer(VertexBufferHandle _handle) override
    {
        m_vertexBuffers[_handle.idx].destroy();
    }

    void createShader(ShaderHandle _handle, const Memory *_mem) override
    {
        m_shaders[_handle.idx].create(_mem);
    }

    void destroyShader(ShaderHandle _handle) override
    {
        m_shaders[_handle.idx].destroy();
    }

    void createProgram(ProgramHandle _handle, ShaderHandle _vsh, ShaderHandle _fsh) override
    {
        ShaderGL dummyFragmentShader;
        m_program[_handle.idx].create(m_shaders[_vsh.idx], isValid(_fsh) ? m_shaders[_fsh.idx] : dummyFragmentShader);
    }

    void destroyProgram(ProgramHandle _handle) override
    {
        m_program[_handle.idx].destroy();
    }

    void *createTexture(TextureHandle _handle, const Memory *_mem, uint64_t _flags, uint8_t _skip) override
    {
        m_textures[_handle.idx].create(_mem, _flags, _skip);
        return NULL;
    }

    void updateTextureBegin(TextureHandle /*_handle*/, uint8_t /*_side*/, uint8_t /*_mip*/) override
    {
    }

    void updateTexture(TextureHandle _handle, uint8_t _side, uint8_t _mip, const Rect &_rect, uint16_t _z,
                       uint16_t _depth, uint16_t _pitch, const Memory *_mem) override
    {
        m_textures[_handle.idx].update(_side, _mip, _rect, _z, _depth, _pitch, _mem);
    }

    void updateTextureEnd() override
    {
    }

    void readTexture(TextureHandle _handle, void *_data, uint8_t _mip) override
    {
        if (m_readBackSupported)
        {
            const TextureGL &texture = m_textures[_handle.idx];
            const bool compressed = bimg::isCompressed(bimg::TextureFormat::Enum(texture.m_textureFormat));

            GL_CHECK(glBindTexture(texture.m_target, texture.m_id));

            if (compressed)
            {
                GL_CHECK(glGetCompressedTexImage(texture.m_target, _mip, _data));
            }
            else
            {
                GL_CHECK(glGetTexImage(texture.m_target, _mip, texture.m_fmt, texture.m_type, _data));
            }

            GL_CHECK(glBindTexture(texture.m_target, 0));
        }
        else if (BX_ENABLED(BGFX_GL_CONFIG_TEXTURE_READ_BACK_EMULATION))
        {
            const TextureGL &texture = m_textures[_handle.idx];
            const bool compressed = bimg::isCompressed(bimg::TextureFormat::Enum(texture.m_textureFormat));

            if (!compressed)
            {
                Attachment at[1];
                at[0].init(_handle);

                FrameBufferGL frameBuffer;
                frameBuffer.create(BX_COUNTOF(at), at);
                GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.m_fbo[0]));
                GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_id,
                                                at[0].mip));

                if (!BX_ENABLED(BX_PLATFORM_EMSCRIPTEN) && !BX_ENABLED(BX_PLATFORM_IOS))
                {
                    GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0));
                }

                if (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER))
                {
                    GL_CHECK(glReadPixels(0, 0, texture.m_width, texture.m_height, m_readPixelsFmt, GL_UNSIGNED_BYTE,
                                          _data));
                }

                frameBuffer.destroy();
            }
        }
    }

    void resizeTexture(TextureHandle _handle, uint16_t _width, uint16_t _height, uint8_t _numMips,
                       uint16_t _numLayers) override
    {
        TextureGL &texture = m_textures[_handle.idx];

        uint32_t size = sizeof(uint32_t) + sizeof(TextureCreate);
        const Memory *mem = alloc(size);

        bx::StaticMemoryBlockWriter writer(mem->data, mem->size);
        uint32_t magic = BGFX_CHUNK_MAGIC_TEX;
        bx::write(&writer, magic);

        TextureCreate tc;
        tc.m_width = _width;
        tc.m_height = _height;
        tc.m_depth = 0;
        tc.m_numLayers = _numLayers;
        tc.m_numMips = _numMips;
        tc.m_format = TextureFormat::Enum(texture.m_requestedFormat);
        tc.m_cubeMap = false;
        tc.m_mem = NULL;
        bx::write(&writer, tc);

        texture.destroy();
        texture.create(mem, texture.m_flags, 0);

        release(mem);
    }

    void overrideInternal(TextureHandle _handle, uintptr_t _ptr) override
    {
        m_textures[_handle.idx].overrideInternal(_ptr);
    }

    uintptr_t getInternal(TextureHandle _handle) override
    {
        return uintptr_t(m_textures[_handle.idx].m_id);
    }

    void destroyTexture(TextureHandle _handle) override
    {
        m_textures[_handle.idx].destroy();
    }

    void createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment *_attachment) override
    {
        m_frameBuffers[_handle.idx].create(_num, _attachment);
    }

    void createFrameBuffer(FrameBufferHandle _handle, void *_nwh, uint32_t _width, uint32_t _height,
                           TextureFormat::Enum _format, TextureFormat::Enum _depthFormat) override
    {
        uint16_t denseIdx = m_numWindows++;
        m_windows[denseIdx] = _handle;
        m_frameBuffers[_handle.idx].create(denseIdx, _nwh, _width, _height, _format, _depthFormat);
    }

    void destroyFrameBuffer(FrameBufferHandle _handle) override
    {
        uint16_t denseIdx = m_frameBuffers[_handle.idx].destroy();
        if (UINT16_MAX != denseIdx)
        {
            --m_numWindows;
            if (m_numWindows > 1)
            {
                FrameBufferHandle handle = m_windows[m_numWindows];
                m_windows[m_numWindows] = {kInvalidHandle};
                if (m_numWindows != denseIdx)
                {
                    m_windows[denseIdx] = handle;
                    m_frameBuffers[handle.idx].m_denseIdx = denseIdx;
                }
            }
        }
    }

    void createUniform(UniformHandle _handle, UniformType::Enum _type, uint16_t _num, const char *_name) override
    {
        if (NULL != m_uniforms[_handle.idx])
        {
            BX_FREE(g_allocator, m_uniforms[_handle.idx]);
        }

        uint32_t size = g_uniformTypeSize[_type] * _num;
        void *data = BX_ALLOC(g_allocator, size);
        bx::memSet(data, 0, size);
        m_uniforms[_handle.idx] = data;
        m_uniformReg.add(_handle, _name);
    }

    void destroyUniform(UniformHandle _handle) override
    {
        BX_FREE(g_allocator, m_uniforms[_handle.idx]);
        m_uniforms[_handle.idx] = NULL;
        m_uniformReg.remove(_handle);
    }

    void requestScreenShot(FrameBufferHandle _handle, const char *_filePath) override
    {
        SwapChainGL *swapChain = NULL;
        uint32_t width = m_resolution.width;
        uint32_t height = m_resolution.height;

        if (isValid(_handle))
        {
            const FrameBufferGL &frameBuffer = m_frameBuffers[_handle.idx];
            swapChain = frameBuffer.m_swapChain;
            width = frameBuffer.m_width;
            height = frameBuffer.m_height;
        }

        m_glctx.makeCurrent(swapChain);

        uint32_t length = width * height * 4;
        uint8_t *data = (uint8_t *)BX_ALLOC(g_allocator, length);

        GL_CHECK(glReadPixels(0, 0, width, height, m_readPixelsFmt, GL_UNSIGNED_BYTE, data));

        if (GL_RGBA == m_readPixelsFmt)
        {
            bimg::imageSwizzleBgra8(data, width * 4, width, height, data, width * 4);
        }

        g_callback->screenShot(_filePath, width, height, width * 4, data, length, true);
        BX_FREE(g_allocator, data);
    }

    void updateViewName(ViewId _id, const char *_name) override
    {
        bx::strCopy(&s_viewName[_id][BGFX_CONFIG_MAX_VIEW_NAME_RESERVED],
                    BX_COUNTOF(s_viewName[0]) - BGFX_CONFIG_MAX_VIEW_NAME_RESERVED, _name);
    }

    void updateUniform(uint16_t _loc, const void *_data, uint32_t _size) override
    {
        bx::memCopy(m_uniforms[_loc], _data, _size);
    }

    void invalidateOcclusionQuery(OcclusionQueryHandle _handle) override
    {
        m_occlusionQuery.invalidate(_handle);
    }

    void setMarker(const char *_marker, uint16_t _len) override
    {
        GL_CHECK(glInsertEventMarker(_len, _marker));
    }

    virtual void setName(Handle _handle, const char *_name, uint16_t _len) override
    {
        uint16_t len = bx::min(_len, m_maxLabelLen);

        switch (_handle.type)
        {
        case Handle::IndexBuffer:
            GL_CHECK(glObjectLabel(GL_BUFFER, m_indexBuffers[_handle.idx].m_id, len, _name));
            break;

        case Handle::Shader:
            GL_CHECK(glObjectLabel(GL_SHADER, m_shaders[_handle.idx].m_id, len, _name));
            break;

        case Handle::Texture: {
            GLint id = m_textures[_handle.idx].m_id;
            if (0 != id)
            {
                GL_CHECK(glObjectLabel(GL_TEXTURE, id, len, _name));
            }
            else
            {
                GL_CHECK(glObjectLabel(GL_RENDERBUFFER, m_textures[_handle.idx].m_rbo, len, _name));
            }
        }
        break;

        case Handle::VertexBuffer:
            GL_CHECK(glObjectLabel(GL_BUFFER, m_vertexBuffers[_handle.idx].m_id, len, _name));
            break;

        default:
            BX_ASSERT(false, "Invalid handle type?! %d", _handle.type);
            break;
        }
    }

    void submitBlit(BlitState &_bs, uint16_t _view);

    void submit(Frame *_render, ClearQuad &_clearQuad, TextVideoMemBlitter &_textVideoMemBlitter) override;

    void blitSetup(TextVideoMemBlitter &_blitter) override
    {
        if (0 != m_vao)
        {
            GL_CHECK(glBindVertexArray(m_vao));
        }

        uint32_t width = m_resolution.width;
        uint32_t height = m_resolution.height;

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_backBufferFbo));
        GL_CHECK(glViewport(0, 0, width, height));

        GL_CHECK(glDisable(GL_SCISSOR_TEST));
        GL_CHECK(glDisable(GL_STENCIL_TEST));
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDepthFunc(GL_ALWAYS));
        GL_CHECK(glDisable(GL_CULL_FACE));
        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

        ProgramGL &program = m_program[_blitter.m_program.idx];
        setProgram(program.m_id);
        setUniform1i(program.m_sampler[0], 0);

        float proj[16];
        bx::mtxOrtho(proj, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 1000.0f, 0.0f, g_caps.homogeneousDepth);

        setUniformMatrix4fv(program.m_predefined[0].m_loc, 1, GL_FALSE, proj);

        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_textures[_blitter.m_texture.idx].m_id));

        if (!BX_ENABLED(BX_PLATFORM_OSX))
        {
            if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30))
            {
                if (m_samplerObjectSupport)
                {
                    GL_CHECK(glBindSampler(0, 0));
                }
            }
        }
    }

    void blitRender(TextVideoMemBlitter &_blitter, uint32_t _numIndices) override
    {
        const uint32_t numVertices = _numIndices * 4 / 6;
        if (0 < numVertices)
        {
            m_indexBuffers[_blitter.m_ib->handle.idx].update(0, _numIndices * 2, _blitter.m_ib->data);
            m_vertexBuffers[_blitter.m_vb->handle.idx].update(0, numVertices * _blitter.m_layout.m_stride,
                                                              _blitter.m_vb->data);

            VertexBufferGL &vb = m_vertexBuffers[_blitter.m_vb->handle.idx];
            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vb.m_id));

            IndexBufferGL &ib = m_indexBuffers[_blitter.m_ib->handle.idx];
            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.m_id));

            ProgramGL &program = m_program[_blitter.m_program.idx];
            program.bindAttributesBegin();
            program.bindAttributes(_blitter.m_layout, 0);
            program.bindAttributesEnd();

            GL_CHECK(glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, (void *)0));
        }
    }

    void updateResolution(const Resolution &_resolution)
    {
        m_maxAnisotropy = !!(_resolution.reset & BGFX_RESET_MAXANISOTROPY) ? m_maxAnisotropyDefault : 0.0f;

        if (s_extension[Extension::ARB_depth_clamp].m_supported)
        {
            if (!!(_resolution.reset & BGFX_RESET_DEPTH_CLAMP))
            {
                GL_CHECK(glEnable(GL_DEPTH_CLAMP));
            }
            else
            {
                GL_CHECK(glDisable(GL_DEPTH_CLAMP));
            }
        }

        const uint32_t maskFlags = ~(0 | BGFX_RESET_MAXANISOTROPY | BGFX_RESET_DEPTH_CLAMP | BGFX_RESET_SUSPEND);

        if (m_resolution.width != _resolution.width || m_resolution.height != _resolution.height ||
            (m_resolution.reset & maskFlags) != (_resolution.reset & maskFlags))
        {
            uint32_t flags = _resolution.reset & (~BGFX_RESET_INTERNAL_FORCE);

            m_resolution = _resolution;
            m_resolution.reset = flags;

            m_textVideoMem.resize(false, _resolution.width, _resolution.height);
            m_textVideoMem.clear();

            setRenderContextSize(m_resolution.width, m_resolution.height, flags);
            updateCapture();

            for (uint32_t ii = 0; ii < BX_COUNTOF(m_frameBuffers); ++ii)
            {
                m_frameBuffers[ii].postReset();
            }

            m_currentFbo = 0;

            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_currentFbo));
        }
    }

    void setShaderUniform4f(uint8_t /*_flags*/, uint32_t _regIndex, const void *_val, uint32_t _numRegs)
    {
        setUniform4fv(_regIndex, _numRegs, (const GLfloat *)_val);
    }

    void setShaderUniform4x4f(uint8_t /*_flags*/, uint32_t _regIndex, const void *_val, uint32_t _numRegs)
    {
        setUniformMatrix4fv(_regIndex, _numRegs, GL_FALSE, (const GLfloat *)_val);
    }

    uint32_t setFrameBuffer(FrameBufferHandle _fbh, uint32_t _height, uint16_t _discard = BGFX_CLEAR_NONE,
                            bool _msaa = true)
    {
        if (isValid(m_fbh) && m_fbh.idx != _fbh.idx)
        {
            FrameBufferGL &frameBuffer = m_frameBuffers[m_fbh.idx];
            frameBuffer.resolve();

            if (BGFX_CLEAR_NONE != m_fbDiscard)
            {
                frameBuffer.discard(m_fbDiscard);
                m_fbDiscard = BGFX_CLEAR_NONE;
            }
        }

        m_glctx.makeCurrent(NULL);

        if (!isValid(_fbh))
        {
            m_needPresent |= true;

            m_currentFbo = m_msaaBackBufferFbo;

            if (m_srgbWriteControlSupport)
            {
                if (0 != (m_resolution.reset & BGFX_RESET_SRGB_BACKBUFFER))
                {
                    GL_CHECK(glEnable(GL_FRAMEBUFFER_SRGB));
                }
                else
                {
                    GL_CHECK(glDisable(GL_FRAMEBUFFER_SRGB));
                }
            }
        }
        else
        {
            FrameBufferGL &frameBuffer = m_frameBuffers[_fbh.idx];
            _height = frameBuffer.m_height;
            if (UINT16_MAX != frameBuffer.m_denseIdx)
            {
                m_glctx.makeCurrent(frameBuffer.m_swapChain);
                GL_CHECK(glFrontFace(GL_CW));

                frameBuffer.m_needPresent = true;
                m_currentFbo = 0;
            }
            else
            {
                m_glctx.makeCurrent(NULL);
                m_currentFbo = frameBuffer.m_fbo[0];
            }
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_currentFbo));

        m_fbh = _fbh;
        m_fbDiscard = _discard;
        m_rtMsaa = _msaa;

        return _height;
    }

    uint32_t getNumRt() const
    {
        if (isValid(m_fbh))
        {
            const FrameBufferGL &frameBuffer = m_frameBuffers[m_fbh.idx];
            return frameBuffer.m_num;
        }

        return 1;
    }

    void createMsaaFbo(uint32_t _width, uint32_t _height, uint32_t _msaa)
    {
        if (0 == m_msaaBackBufferFbo // iOS
            && 1 < _msaa)
        {
            GLenum storageFormat = (m_resolution.reset & BGFX_RESET_SRGB_BACKBUFFER) ? GL_SRGB8_ALPHA8 : GL_RGBA8;

            GL_CHECK(glGenFramebuffers(1, &m_msaaBackBufferFbo));
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaBackBufferFbo));
            GL_CHECK(glGenRenderbuffers(BX_COUNTOF(m_msaaBackBufferRbos), m_msaaBackBufferRbos));
            GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_msaaBackBufferRbos[0]));
            GL_CHECK(glRenderbufferStorageMultisample(GL_RENDERBUFFER, _msaa, storageFormat, _width, _height));
            GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_msaaBackBufferRbos[1]));
            GL_CHECK(glRenderbufferStorageMultisample(GL_RENDERBUFFER, _msaa, GL_DEPTH24_STENCIL8, _width, _height));
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                                               m_msaaBackBufferRbos[0]));

            GLenum attachment =
                BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30)
                    ? GL_DEPTH_STENCIL_ATTACHMENT
                    : GL_DEPTH_ATTACHMENT;
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_msaaBackBufferRbos[1]));

            BX_ASSERT(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER),
                      "glCheckFramebufferStatus failed 0x%08x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_msaaBackBufferFbo));
        }
    }

    void destroyMsaaFbo()
    {
        if (m_backBufferFbo != m_msaaBackBufferFbo // iOS
            && 0 != m_msaaBackBufferFbo)
        {
            GL_CHECK(glDeleteFramebuffers(1, &m_msaaBackBufferFbo));
            m_msaaBackBufferFbo = 0;

            if (0 != m_msaaBackBufferRbos[0])
            {
                GL_CHECK(glDeleteRenderbuffers(BX_COUNTOF(m_msaaBackBufferRbos), m_msaaBackBufferRbos));
                m_msaaBackBufferRbos[0] = 0;
                m_msaaBackBufferRbos[1] = 0;
            }
        }
    }

    void blitMsaaFbo()
    {
        if (m_backBufferFbo != m_msaaBackBufferFbo // iOS
            && 0 != m_msaaBackBufferFbo)
        {
            GL_CHECK(glDisable(GL_SCISSOR_TEST));
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_backBufferFbo));
            GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaaBackBufferFbo));
            GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
            uint32_t width = m_resolution.width;
            uint32_t height = m_resolution.height;
            GLenum filter = BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES < 30)
                                ? GL_NEAREST
                                : GL_LINEAR;
            GL_CHECK(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, filter));
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_backBufferFbo));
        }
    }

    void setRenderContextSize(uint32_t _width, uint32_t _height, uint32_t _flags = 0)
    {
        if (_width != 0 || _height != 0)
        {
            if (!m_glctx.isValid())
            {
                m_glctx.create(_width, _height);

#if BX_PLATFORM_IOS
                // iOS: need to figure out how to deal with FBO created by context.
                m_backBufferFbo = m_msaaBackBufferFbo = m_glctx.getFbo();
#endif // BX_PLATFORM_IOS
            }
            else
            {
                destroyMsaaFbo();

                m_glctx.resize(_width, _height, _flags);

                uint32_t msaa = (_flags & BGFX_RESET_MSAA_MASK) >> BGFX_RESET_MSAA_SHIFT;
                msaa = bx::uint32_min(m_maxMsaa, msaa == 0 ? 0 : 1 << msaa);

                createMsaaFbo(_width, _height, msaa);
            }
        }

        m_flip = true;
    }

    void invalidateCache()
    {
        if ((BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30)) &&
            m_samplerObjectSupport)
        {
            m_samplerStateCache.invalidate();
        }
    }

    void setSamplerState(uint32_t _stage, uint32_t _numMips, uint32_t _flags, const float _rgba[4])
    {
        if ((BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL) || BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30)) &&
            m_samplerObjectSupport)
        {
            if (0 == (BGFX_SAMPLER_INTERNAL_DEFAULT & _flags))
            {
                const uint32_t index = (_flags & BGFX_SAMPLER_BORDER_COLOR_MASK) >> BGFX_SAMPLER_BORDER_COLOR_SHIFT;

                _flags &= ~BGFX_SAMPLER_RESERVED_MASK;
                _flags &= BGFX_SAMPLER_BITS_MASK;
                _flags |= _numMips << BGFX_SAMPLER_RESERVED_SHIFT;

                GLuint sampler;

                bool hasBorderColor = false;
                bx::HashMurmur2A murmur;
                uint32_t hash;

                murmur.begin();
                murmur.add(_flags);
                if (!needBorderColor(_flags))
                {
                    murmur.add(-1);
                    hash = murmur.end();

                    sampler = m_samplerStateCache.find(hash);
                }
                else
                {
                    murmur.add(index);
                    hash = murmur.end();

                    if (NULL != _rgba)
                    {
                        hasBorderColor = true;
                        sampler = UINT32_MAX;
                    }
                    else
                    {
                        sampler = m_samplerStateCache.find(hash);
                    }
                }

                if (UINT32_MAX == sampler)
                {
                    sampler = m_samplerStateCache.add(hash);

                    GL_CHECK(
                        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S,
                                            s_textureAddress[(_flags & BGFX_SAMPLER_U_MASK) >> BGFX_SAMPLER_U_SHIFT]));
                    GL_CHECK(
                        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T,
                                            s_textureAddress[(_flags & BGFX_SAMPLER_V_MASK) >> BGFX_SAMPLER_V_SHIFT]));
                    GL_CHECK(
                        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R,
                                            s_textureAddress[(_flags & BGFX_SAMPLER_W_MASK) >> BGFX_SAMPLER_W_SHIFT]));

                    GLenum minFilter;
                    GLenum magFilter;
                    getFilters(_flags, 1 < _numMips, magFilter, minFilter);
                    GL_CHECK(glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter));
                    GL_CHECK(glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter));

                    if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGL))
                    {
                        GL_CHECK(glSamplerParameterf(sampler, GL_TEXTURE_LOD_BIAS, float(BGFX_CONFIG_MIP_LOD_BIAS)));
                    }

                    if (m_borderColorSupport && hasBorderColor)
                    {
                        GL_CHECK(glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, _rgba));
                    }

                    if (0 != (_flags & (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC)) &&
                        0.0f < m_maxAnisotropy)
                    {
                        GL_CHECK(glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy));
                    }

                    if (BX_ENABLED(BGFX_CONFIG_RENDERER_OPENGLES >= 30) || m_shadowSamplersSupport)
                    {
                        const uint32_t cmpFunc = (_flags & BGFX_SAMPLER_COMPARE_MASK) >> BGFX_SAMPLER_COMPARE_SHIFT;
                        if (0 == cmpFunc)
                        {
                            GL_CHECK(glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE));
                        }
                        else
                        {
                            GL_CHECK(glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
                            GL_CHECK(glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, s_cmpFunc[cmpFunc]));
                        }
                    }
                }

                GL_CHECK(glBindSampler(_stage, sampler));
            }
            else
            {
                GL_CHECK(glBindSampler(_stage, 0));
            }
        }
    }

    bool isVisible(Frame *_render, OcclusionQueryHandle _handle, bool _visible)
    {
        m_occlusionQuery.resolve(_render);
        return _visible == (0 != _render->m_occlusion[_handle.idx]);
    }

    void updateCapture()
    {
        if (m_resolution.reset & BGFX_RESET_CAPTURE)
        {
            m_captureSize = m_resolution.width * m_resolution.height * 4;
            m_capture = BX_REALLOC(g_allocator, m_capture, m_captureSize);
            g_callback->captureBegin(m_resolution.width, m_resolution.height, m_resolution.width * 4,
                                     TextureFormat::BGRA8, true);
        }
        else
        {
            captureFinish();
        }
    }

    void capture()
    {
        if (NULL != m_capture)
        {
            GL_CHECK(glReadPixels(0, 0, m_resolution.width, m_resolution.height, m_readPixelsFmt, GL_UNSIGNED_BYTE,
                                  m_capture));

            if (GL_RGBA == m_readPixelsFmt)
            {
                bimg::imageSwizzleBgra8(m_capture, m_resolution.width * 4, m_resolution.width, m_resolution.height,
                                        m_capture, m_resolution.width * 4);
            }

            g_callback->captureFrame(m_capture, m_captureSize);
        }
    }

    void captureFinish()
    {
        if (NULL != m_capture)
        {
            g_callback->captureEnd();
            BX_FREE(g_allocator, m_capture);
            m_capture = NULL;
            m_captureSize = 0;
        }
    }

    bool programFetchFromCache(GLuint programId, uint64_t _id)
    {
        _id ^= m_hash;

        bool cached = false;

        if (m_programBinarySupport)
        {
            uint32_t length = g_callback->cacheReadSize(_id);
            cached = length > 0;

            if (cached)
            {
                void *data = BX_ALLOC(g_allocator, length);
                if (g_callback->cacheRead(_id, data, length))
                {
                    bx::MemoryReader reader(data, length);

                    GLenum format;
                    bx::read(&reader, format);

                    GL_CHECK(glProgramBinary(programId, format, reader.getDataPtr(), (GLsizei)reader.remaining()));
                }

                BX_FREE(g_allocator, data);
            }

#if BGFX_CONFIG_RENDERER_OPENGL
            GL_CHECK(glProgramParameteri(programId, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE));
#endif // BGFX_CONFIG_RENDERER_OPENGL
        }

        return cached;
    }

    void programCache(GLuint programId, uint64_t _id)
    {
        _id ^= m_hash;

        if (m_programBinarySupport)
        {
            GLint programLength;
            GLenum format;
            GL_CHECK(glGetProgramiv(programId, GL_PROGRAM_BINARY_LENGTH, &programLength));

            if (0 < programLength)
            {
                uint32_t length = programLength + 4;
                uint8_t *data = (uint8_t *)BX_ALLOC(g_allocator, length);
                GL_CHECK(glGetProgramBinary(programId, programLength, NULL, &format, &data[4]));
                *(uint32_t *)data = format;

                g_callback->cacheWrite(_id, data, length);

                BX_FREE(g_allocator, data);
            }
        }
    }

    void commit(UniformBuffer &_uniformBuffer)
    {
        _uniformBuffer.reset();

        for (;;)
        {
            uint32_t opcode = _uniformBuffer.read();

            if (UniformType::End == opcode)
            {
                break;
            }

            UniformType::Enum type;
            uint16_t ignore;
            uint16_t num;
            uint16_t copy;
            UniformBuffer::decodeOpcode(opcode, type, ignore, num, copy);

            const char *data;
            if (copy)
            {
                data = _uniformBuffer.read(g_uniformTypeSize[type] * num);
            }
            else
            {
                UniformHandle handle;
                bx::memCopy(&handle, _uniformBuffer.read(sizeof(UniformHandle)), sizeof(UniformHandle));
                data = (const char *)m_uniforms[handle.idx];
            }

            uint32_t loc = _uniformBuffer.read();

#define CASE_IMPLEMENT_UNIFORM(_uniform, _glsuffix, _dxsuffix, _type)                                                  \
    case UniformType::_uniform: {                                                                                      \
        _type *value = (_type *)data;                                                                                  \
        setUniform##_glsuffix(loc, num, value);                                                                        \
    }                                                                                                                  \
    break;

#define CASE_IMPLEMENT_UNIFORM_T(_uniform, _glsuffix, _dxsuffix, _type)                                                \
    case UniformType::_uniform: {                                                                                      \
        _type *value = (_type *)data;                                                                                  \
        setUniform##_glsuffix(loc, num, GL_FALSE, value);                                                              \
    }                                                                                                                  \
    break;

            switch (type)
            {
#if BX_PLATFORM_EMSCRIPTEN
            // For WebAssembly the array forms glUniform1iv/glUniform4fv are much slower compared to
            // glUniform1i/glUniform4f since they need to marshal an array over from Wasm to JS, so optimize the case
            // when there is exactly one uniform to upload.
            case UniformType::Sampler:
                if (num > 1)
                    setUniform1iv(loc, num, (int *)data);
                else
                    setUniform1i(loc, *(int *)data);
                break;
            case UniformType::Vec4:
                if (num > 1)
                    setUniform4fv(loc, num, (float *)data);
                else
                    setUniform4f(loc, ((float *)data)[0], ((float *)data)[1], ((float *)data)[2], ((float *)data)[3]);
                break;
#else
                CASE_IMPLEMENT_UNIFORM(Sampler, 1iv, I, int);
                CASE_IMPLEMENT_UNIFORM(Vec4, 4fv, F, float);
#endif
                CASE_IMPLEMENT_UNIFORM_T(Mat3, Matrix3fv, F, float);
                CASE_IMPLEMENT_UNIFORM_T(Mat4, Matrix4fv, F, float);

            case UniformType::End:
                break;

            default:
                BX_TRACE("%4d: INVALID 0x%08x, t %d, l %d, n %d, c %d", _uniformBuffer.getPos(), opcode, type, loc, num,
                         copy);
                break;
            }

#undef CASE_IMPLEMENT_UNIFORM
#undef CASE_IMPLEMENT_UNIFORM_T
        }
    }

    void clearQuad(ClearQuad &_clearQuad, const Rect &_rect, const Clear &_clear, uint32_t _height,
                   const float _palette[][4])
    {
        uint32_t numMrt = 1;
        FrameBufferHandle fbh = m_fbh;
        if (isValid(fbh))
        {
            const FrameBufferGL &fb = m_frameBuffers[fbh.idx];
            numMrt = bx::uint32_max(1, fb.m_num);
        }

        if (1 == numMrt)
        {
            GLuint flags = 0;
            if (BGFX_CLEAR_COLOR & _clear.m_flags)
            {
                if (BGFX_CLEAR_COLOR_USE_PALETTE & _clear.m_flags)
                {
                    uint8_t index = (uint8_t)bx::uint32_min(BGFX_CONFIG_MAX_COLOR_PALETTE - 1, _clear.m_index[0]);
                    const float *rgba = _palette[index];
                    const float rr = rgba[0];
                    const float gg = rgba[1];
                    const float bb = rgba[2];
                    const float aa = rgba[3];
                    GL_CHECK(glClearColor(rr, gg, bb, aa));
                }
                else
                {
                    float rr = _clear.m_index[0] * 1.0f / 255.0f;
                    float gg = _clear.m_index[1] * 1.0f / 255.0f;
                    float bb = _clear.m_index[2] * 1.0f / 255.0f;
                    float aa = _clear.m_index[3] * 1.0f / 255.0f;
                    GL_CHECK(glClearColor(rr, gg, bb, aa));
                }

                flags |= GL_COLOR_BUFFER_BIT;
                GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
            }

            if (BGFX_CLEAR_DEPTH & _clear.m_flags)
            {
                flags |= GL_DEPTH_BUFFER_BIT;
                GL_CHECK(glClearDepth(_clear.m_depth));
                GL_CHECK(glDepthMask(GL_TRUE));
            }

            if (BGFX_CLEAR_STENCIL & _clear.m_flags)
            {
                flags |= GL_STENCIL_BUFFER_BIT;
                GL_CHECK(glClearStencil(_clear.m_stencil));
            }

            if (0 != flags)
            {
                GL_CHECK(glEnable(GL_SCISSOR_TEST));
                GL_CHECK(glScissor(_rect.m_x, _height - _rect.m_height - _rect.m_y, _rect.m_width, _rect.m_height));
                GL_CHECK(glClear(flags));
                GL_CHECK(glDisable(GL_SCISSOR_TEST));
            }
        }
        else
        {
            const GLuint defaultVao = m_vao;
            if (0 != defaultVao)
            {
                GL_CHECK(glBindVertexArray(defaultVao));
            }

            GL_CHECK(glDisable(GL_SCISSOR_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));
            GL_CHECK(glDisable(GL_BLEND));

            GLboolean colorMask = !!(BGFX_CLEAR_COLOR & _clear.m_flags);
            GL_CHECK(glColorMask(colorMask, colorMask, colorMask, colorMask));

            if (BGFX_CLEAR_DEPTH & _clear.m_flags)
            {
                GL_CHECK(glEnable(GL_DEPTH_TEST));
                GL_CHECK(glDepthFunc(GL_ALWAYS));
                GL_CHECK(glDepthMask(GL_TRUE));
            }
            else
            {
                GL_CHECK(glDisable(GL_DEPTH_TEST));
            }

            if (BGFX_CLEAR_STENCIL & _clear.m_flags)
            {
                GL_CHECK(glEnable(GL_STENCIL_TEST));
                GL_CHECK(glStencilFuncSeparate(GL_FRONT_AND_BACK, GL_ALWAYS, _clear.m_stencil, 0xff));
                GL_CHECK(glStencilOpSeparate(GL_FRONT_AND_BACK, GL_REPLACE, GL_REPLACE, GL_REPLACE));
            }
            else
            {
                GL_CHECK(glDisable(GL_STENCIL_TEST));
            }

            VertexBufferGL &vb = m_vertexBuffers[_clearQuad.m_vb.idx];
            VertexLayout &layout = _clearQuad.m_layout;

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vb.m_id));

            ProgramGL &program = m_program[_clearQuad.m_program[numMrt - 1].idx];
            setProgram(program.m_id);
            program.bindAttributesBegin();
            program.bindAttributes(layout, 0);
            program.bindAttributesEnd();

            if (m_clearQuadColor.idx == kInvalidHandle)
            {
                const UniformRegInfo *infoClearColor = m_uniformReg.find("bgfx_clear_color");
                if (NULL != infoClearColor)
                    m_clearQuadColor = infoClearColor->m_handle;
            }

            if (m_clearQuadDepth.idx == kInvalidHandle)
            {
                const UniformRegInfo *infoClearDepth = m_uniformReg.find("bgfx_clear_depth");
                if (NULL != infoClearDepth)
                    m_clearQuadDepth = infoClearDepth->m_handle;
            }

            float mrtClearDepth[4] = {g_caps.homogeneousDepth ? (_clear.m_depth * 2.0f - 1.0f) : _clear.m_depth};
            updateUniform(m_clearQuadDepth.idx, mrtClearDepth, sizeof(float) * 4);

            float mrtClearColor[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS][4];

            if (BGFX_CLEAR_COLOR_USE_PALETTE & _clear.m_flags)
            {
                for (uint32_t ii = 0; ii < numMrt; ++ii)
                {
                    uint8_t index = (uint8_t)bx::uint32_min(BGFX_CONFIG_MAX_COLOR_PALETTE - 1, _clear.m_index[ii]);
                    bx::memCopy(mrtClearColor[ii], _palette[index], 16);
                }
            }
            else
            {
                float rgba[4] = {
                    _clear.m_index[0] * 1.0f / 255.0f,
                    _clear.m_index[1] * 1.0f / 255.0f,
                    _clear.m_index[2] * 1.0f / 255.0f,
                    _clear.m_index[3] * 1.0f / 255.0f,
                };

                for (uint32_t ii = 0; ii < numMrt; ++ii)
                {
                    bx::memCopy(mrtClearColor[ii], rgba, 16);
                }
            }

            updateUniform(m_clearQuadColor.idx, mrtClearColor[0], numMrt * sizeof(float) * 4);

            commit(*program.m_constantBuffer);

            GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        }
    }

    void setProgram(GLuint program)
    {
        m_uniformStateCache.saveCurrentProgram(program);
        GL_CHECK(glUseProgram(program));
    }

    // Cache uniform uploads to avoid redundant uploading of state that is
    // already set to a shader program
    void setUniform1i(uint32_t loc, int value)
    {
        if (m_uniformStateCache.updateUniformCache(loc, value))
        {
            GL_CHECK(glUniform1i(loc, value));
        }
    }

    void setUniform1iv(uint32_t loc, int num, const int *data)
    {
        bool changed = false;
        for (int i = 0; i < num; ++i)
        {
            if (m_uniformStateCache.updateUniformCache(loc + i, data[i]))
            {
                changed = true;
            }
        }
        if (changed)
        {
            GL_CHECK(glUniform1iv(loc, num, data));
        }
    }

    void setUniform4f(uint32_t loc, float x, float y, float z, float w)
    {
        UniformStateCache::f4 f;
        f.val[0] = x;
        f.val[1] = y;
        f.val[2] = z;
        f.val[3] = w;
        if (m_uniformStateCache.updateUniformCache(loc, f))
        {
            GL_CHECK(glUniform4f(loc, x, y, z, w));
        }
    }

    void setUniform4fv(uint32_t loc, int num, const float *data)
    {
        bool changed = false;
        for (int i = 0; i < num; ++i)
        {
            if (m_uniformStateCache.updateUniformCache(loc + i, *(const UniformStateCache::f4 *)&data[4 * i]))
            {
                changed = true;
            }
        }
        if (changed)
        {
            GL_CHECK(glUniform4fv(loc, num, data));
        }
    }

    void setUniformMatrix3fv(uint32_t loc, int num, GLboolean transpose, const float *data)
    {
        bool changed = false;
        for (int i = 0; i < num; ++i)
        {
            if (m_uniformStateCache.updateUniformCache(loc + i, *(const UniformStateCache::f3x3 *)&data[9 * i]))
            {
                changed = true;
            }
        }
        if (changed)
        {
            GL_CHECK(glUniformMatrix3fv(loc, num, transpose, data));
        }
    }

    void setUniformMatrix4fv(uint32_t loc, int num, GLboolean transpose, const float *data)
    {
        bool changed = false;
        for (int i = 0; i < num; ++i)
        {
            if (m_uniformStateCache.updateUniformCache(loc + i, *(const UniformStateCache::f4x4 *)&data[16 * i]))
            {
                changed = true;
            }
        }
        if (changed)
        {
            GL_CHECK(glUniformMatrix4fv(loc, num, transpose, data));
        }
    }

    void *m_renderdocdll;

    uint16_t m_numWindows;
    FrameBufferHandle m_windows[BGFX_CONFIG_MAX_FRAME_BUFFERS];

    IndexBufferGL m_indexBuffers[BGFX_CONFIG_MAX_INDEX_BUFFERS];
    VertexBufferGL m_vertexBuffers[BGFX_CONFIG_MAX_VERTEX_BUFFERS];
    ShaderGL m_shaders[BGFX_CONFIG_MAX_SHADERS];
    ProgramGL m_program[BGFX_CONFIG_MAX_PROGRAMS];
    TextureGL m_textures[BGFX_CONFIG_MAX_TEXTURES];
    VertexLayout m_vertexLayouts[BGFX_CONFIG_MAX_VERTEX_LAYOUTS];
    FrameBufferGL m_frameBuffers[BGFX_CONFIG_MAX_FRAME_BUFFERS];
    UniformRegistry m_uniformReg;
    void *m_uniforms[BGFX_CONFIG_MAX_UNIFORMS];

    TimerQueryGL m_gpuTimer;
    OcclusionQueryGL m_occlusionQuery;

    SamplerStateCache m_samplerStateCache;
    UniformStateCache m_uniformStateCache;

    TextVideoMem m_textVideoMem;
    bool m_rtMsaa;

    FrameBufferHandle m_fbh;
    uint16_t m_fbDiscard;

    Resolution m_resolution;
    void *m_capture;
    uint32_t m_captureSize;
    float m_maxAnisotropy;
    float m_maxAnisotropyDefault;
    int32_t m_maxMsaa;
    GLuint m_vao;
    uint16_t m_maxLabelLen;
    bool m_blitSupported;
    bool m_readBackSupported;
    bool m_vaoSupport;
    bool m_samplerObjectSupport;
    bool m_shadowSamplersSupport;
    bool m_srgbWriteControlSupport;
    bool m_borderColorSupport;
    bool m_programBinarySupport;
    bool m_textureSwizzleSupport;
    bool m_depthTextureSupport;
    bool m_timerQuerySupport;
    bool m_occlusionQuerySupport;
    bool m_atocSupport;
    bool m_conservativeRasterSupport;
    bool m_imageLoadStoreSupport;
    bool m_flip;

    uint64_t m_hash;

    GLenum m_readPixelsFmt;
    GLuint m_backBufferFbo;
    GLuint m_msaaBackBufferFbo;
    GLuint m_msaaBackBufferRbos[2];
    GlContext m_glctx;
    bool m_needPresent;

    UniformHandle m_clearQuadColor;
    UniformHandle m_clearQuadDepth;

    const char *m_vendor;
    const char *m_renderer;
    const char *m_version;
    const char *m_glslVersion;

    Workaround m_workaround;

    GLuint m_currentFbo;
};