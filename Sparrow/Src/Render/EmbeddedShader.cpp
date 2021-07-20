
#include "embedded_shader.h"
#include "RenderHandle.h"
#include "Render/RenderInternal.h"

namespace bgfx
{

#include  "EmbeddedShader/vs_debugfont.bin.h"
#include  "EmbeddedShader/fs_debugfont.bin.h"
#include  "EmbeddedShader/vs_clear.bin.h"
#include  "EmbeddedShader/fs_clear0.bin.h"
#include  "EmbeddedShader/fs_clear1.bin.h"
#include  "EmbeddedShader/fs_clear2.bin.h"
#include  "EmbeddedShader/fs_clear3.bin.h"
#include  "EmbeddedShader/fs_clear4.bin.h"
#include  "EmbeddedShader/fs_clear5.bin.h"
#include  "EmbeddedShader/fs_clear6.bin.h"
#include  "EmbeddedShader/fs_clear7.bin.h"

	static const EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_debugfont),
		BGFX_EMBEDDED_SHADER(fs_debugfont),
		BGFX_EMBEDDED_SHADER(vs_clear),
		BGFX_EMBEDDED_SHADER(fs_clear0),
		BGFX_EMBEDDED_SHADER(fs_clear1),
		BGFX_EMBEDDED_SHADER(fs_clear2),
		BGFX_EMBEDDED_SHADER(fs_clear3),
		BGFX_EMBEDDED_SHADER(fs_clear4),
		BGFX_EMBEDDED_SHADER(fs_clear5),
		BGFX_EMBEDDED_SHADER(fs_clear6),
		BGFX_EMBEDDED_SHADER(fs_clear7),

		BGFX_EMBEDDED_SHADER_END()
	};

	ShaderHandle createEmbeddedShader(const EmbeddedShader* _es, RendererType::Enum _type, const char* _name)
	{
		for (const EmbeddedShader* es = _es; NULL != es->name; ++es)
		{
			if (0 == bx::strCmp(_name, es->name) )
			{
				for (const EmbeddedShader::Data* esd = es->data; RendererType::Count != esd->type; ++esd)
				{
					if (_type == esd->type
					&&  1 < esd->size)
					{
						ShaderHandle handle = createShader(makeRef(esd->data, esd->size) );
						if (isValid(handle) )
						{
							setName(handle, _name);
						}

						return handle;
					}
				}
			}
		}

		ShaderHandle handle = BGFX_INVALID_HANDLE;
		return handle;
	}

}