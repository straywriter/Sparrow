#pragma once

class RenderCommand
{
  public:
    struct ResourceCommand
    {
        enum class ResourceType
        {
            VertexBuffer,
            IndexBuffer,
            Shader,
        };
        enum class ResourceCommandType
        {
            Create,
            Update,
            Destory,
        };
        ResourceCommandType resourceCommandType;
        ResourceType resourceType;
    };

    enum class RenderCommandType
    {
        Begin,
        Resource,
        Command,
        RenderCommand,
        None,
        End
    };
    union CommandType {

        ResourceCommand resourceCommand;
    };

    RenderCommandType resourceType;
    CommandType commandType;
    bool isSuccessed;

  public:
    RenderCommandType GetCommandType();
    inline bool IsSuccessed() const
    {
        return isSuccessed;
    }
};
