#pragma once
#include "Render/VertexLayout.h"

namespace Render
{

struct GLVertexAttribute
{

    GLuint index;          ///< Index of the vertex attribute
    GLint size;            ///< Number of elements in this attribute
    GLenum type;           ///< Datatype of the elements
    GLboolean normalized;  ///< Normalize fixed-point data
    GLsizei stride;        ///< Byte offset between these attributes
    const GLvoid *pointer; ///< Offset of this attribute into the data

    void Set();

    void Enable();
};

class GLVertexLayout : public VertexLayout
{

  public:
    // add a list a? for vertex attribute list

  public:
    // GLVertexLayout();
    // ~GLVertexLayout();

    void Bind();

    void Enable();
};

} // namespace Render