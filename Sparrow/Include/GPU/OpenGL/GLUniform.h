
#pragma once
#include "GPU/OpenGL/GLRequirement.h"
#include "Render/Uniform.h"

namespace Render
{
class GLUniform : Uniform
{
  public:
    GLuint location;

    public:

GLUniform();
~GLUniform();


void Set(int v1);
void Set(int v1,int v2);
void Set(int v1,int v2,int v3);
void Set(int v1,int v2,int v3,int v4);

void Set(float v1 );
void Set(float v1 ,float v2);
void Set(float v1 ,float v2 ,float v3);
void Set(float v1 ,float v2 ,float v3 ,float v4);



    /*

        Uniform(int location);
            ~Uniform(void);


    void
    set(int);
void set(int, int);
void set(int, int, int);
void set(int, int, int, int);

void set(float);
void set(float, float);
void set(float, float, float);
void set(float, float, float, float);

void set(const Vector2 &);
void set(const Vector3 &);
void set(const Vector4 &);

void set(const Matrix3 &);
void set(const Matrix3 &, bool transpose);

void set(const Matrix4 &);
void set(const Matrix4 &, bool transpose);

void set(const ColorRGBA32f &);

void set(const Texture &);

*/

    // /* Interger sets */

    // void Uniform::set(int v1)
    // {
    //     glUniform1i(_location, v1);
    // }

    // void Uniform::set(int v1, int v2)
    // {
    //     glUniform2i(_location, v1, v2);
    // }

    // void Uniform::set(int v1, int v2, int v3)
    // {
    //     glUniform3i(_location, v1, v2, v3);
    // }

    // void Uniform::set(int v1, int v2, int v3, int v4)
    // {
    //     glUniform4i(_location, v1, v2, v3, v4);
    // }

    // /* Float sets */

    // void Uniform::set(float v1)
    // {
    //     glUniform1f(_location, v1);
    // }

    // void Uniform::set(float v1, float v2)
    // {
    //     glUniform2f(_location, v1, v2);
    // }

    // void Uniform::set(float v1, float v2, float v3)
    // {
    //     glUniform3f(_location, v1, v2, v3);
    // }

    // void Uniform::set(float v1, float v2, float v3, float v4)
    // {
    //     glUniform4f(_location, v1, v2, v3, v4);
    // }

    // /* float value pointer */

    // void Uniform::set(const Vector2 &v)
    // {
    //     glUniform2f(_location, v.x, v.y);
    // }

    // void Uniform::set(const Vector3 &v)
    // {
    //     glUniform3f(_location, v.x, v.y, v.z);
    // }

    // void Uniform::set(const Vector4 &v)
    // {
    //     glUniform4f(_location, v.x, v.y, v.z, v.w);
    // }

    // /* Matrix 3x3 set */
    // void Uniform::set(const Matrix3 &v)
    // {
    //     set(v, false);
    // }

    // void Uniform::set(const Matrix3 &v, bool transpose)
    // {
    //     glUniformMatrix3fv(_location, 1, transpose, &v[0][0]);
    // }

    // /* Matrix 4x4 set */
    // void Uniform::set(const Matrix4 &v)
    // {
    //     set(v, false);
    // }

    // void Uniform::set(const Matrix4 &v, bool transpose)
    // {
    //     glUniformMatrix4fv(_location, 1, transpose, v.get());
    // }

    // /* Color */

    // void Uniform::set(const ColorRGBA32f &color)
    // {
    //     glUniform4f(_location, color.r, color.g, color.b, color.a);
    // }

    // /* Texture */
    // void Uniform::set(const Texture &texture)
    // {
    //     set(texture.getIndex());
    // }

    // /* Getters */

    // int Uniform::getLocation() const
    // {
    //     return _location;
    // }
};

} // namespace Render