#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"

class AppWindow : public Window
{
public:
	AppWindow();
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	~AppWindow();

private:
	SwapChain* m_swap_chain;

};

