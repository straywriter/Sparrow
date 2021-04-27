#pragma once

class RenderCommand
{
  public:
    enum ECommandType
    {
        Begin,
        VertexBufferCommand,

        End
    };

    ECommandType type;
    bool isSuccessed;

  public:
    ECommandType GetCommandType();
    bool IsSuccessed();

    
};
