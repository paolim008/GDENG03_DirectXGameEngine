#pragma once
#include "Window.h"

class AppWindow : public Window
{
public:
	AppWindow();
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	~AppWindow();

};

