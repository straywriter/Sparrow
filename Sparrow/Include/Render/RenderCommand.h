#pragma once









class RenderCommand
{
  public:
    enum ECommandType
    {
        Begin,

        End
    };

    ECommandType type;
    bool isSuccessed;
};
