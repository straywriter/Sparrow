#pragma once
#include "GPU/OpenGL/GLRequirement.h"
#include "GPU/OpenGL/GLVertexLayout.h"
#include "Render/VertexBuffer.h"

namespace Render
{
    
    // //! Valid Target
	// 	enum class Target
	// 	{
	// 		ARRAY              = GL_ARRAY_BUFFER,
	// 		ATOMIC_COUNTER     = GL_ATOMIC_COUNTER_BUFFER,
	// 		COPY_READ          = GL_COPY_READ_BUFFER,
	// 		COPY_WRITE         = GL_COPY_WRITE_BUFFER,
	// 		DRAW_INDIRECT      = GL_DRAW_INDIRECT_BUFFER,
	// 		DISPATCH_INDIRECT  = GL_DISPATCH_INDIRECT_BUFFER,
	// 		ELEMENT            = GL_ELEMENT_ARRAY_BUFFER,
	// 		PIXEL_PACK         = GL_PIXEL_PACK_BUFFER,
	// 		PIXEL_UNPACK       = GL_PIXEL_UNPACK_BUFFER,
	// 		QUERY              = GL_QUERY_BUFFER,
	// 		SHADER_STORAGE     = GL_SHADER_STORAGE_BUFFER,
	// 		TEXTURE            = GL_TEXTURE_BUFFER,
	// 		TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
	// 		UNIFORM            = GL_UNIFORM_BUFFER
	// 	};

	// 	//! Valid Usages
	// 	enum class Usage
	// 	{
	// 		STREAM_DRAW  = GL_STREAM_DRAW,
	// 		STREAM_READ  = GL_STREAM_READ,
	// 		STREAM_COPY  = GL_STREAM_COPY,
	// 		STATIC_DRAW  = GL_STATIC_DRAW,
	// 		STATIC_READ  = GL_STATIC_READ,
	// 		STATIC_COPY  = GL_STATIC_COPY,
	// 		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
	// 		DYNAMIC_READ = GL_DYNAMIC_READ,
	// 		DYANMIC_COPY = GL_DYNAMIC_COPY
	// 	};

class GLVertexBuffer : public VertexBuffer
{

    // gl id
  public:
    // GLuint id;
    // GLenum target;
    GLuint id=0;     ///<
    // unsigned int id;
    GLenum target; ///< to glBindData fist prama  ,,default is GL_ARRAY_BUFFER
    GLenum usage;
    uint32_t size; ///<
    GLVertexLayout layout;

    GLVertexBuffer();

    GLVertexBuffer(GLenum target, GLVertexLayout layout);

    bool Create(void *data, uint32_t size, GLenum target = GL_ARRAY_BUFFER);

    void Update(uint32_t offset, uint32_t size, void *data);

    /** delete VertexBuffer object */
    void Delete();

    /** glBindBuffer function use */
    void Bind();
    /** glBindBuffer unbind ?? */
    void UnBind();

void SetData();

void SubData();

void SetTarget();

void SetUsage();

GLuint GetId();



    // layout op

    friend bool operator==(const GLVertexBuffer &left, const GLVertexBuffer &right)
    {
        if (left.id == right.id)
            return true;
        else
            return false;
    }
};

} // namespace Render