#pragma once

class IWindow
{

virtual bool Create();

virtual bool Init();

virtual bool Shutdown();

virtual bool Destory();

virtual Window Open();

};