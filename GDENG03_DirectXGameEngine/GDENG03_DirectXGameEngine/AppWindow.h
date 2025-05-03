#pragma once
#include "Window.h"
#include "GraphicsEngine.h"

class AppWindow : public Window
{
public:
	AppWindow();
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	~AppWindow();

};

