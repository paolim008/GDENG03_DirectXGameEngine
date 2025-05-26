#include "AppWindow.h"
#include "Vertex.h"
#include "Quad.h"

AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

#pragma region Instantiate and Add Quads to Gameobject List
	gameobjectList.push_back(new Quad(
		new Vertex(-0.25f, 0.75f, 0.0f, 1, 0, 0),
		new Vertex(-0.25f, 0.25f, 0.0f, 0, 1, 0),
		new Vertex(-0.75f, 0.75f, 0.0f, 0, 1, 0),
		new Vertex(-0.75f, 0.25f, 0.0f, 0, 0, 1)
	));

	gameobjectList.push_back(new Quad(
		new Vertex(-0.75f, -0.75f, 0.0f, 0, 0, 0),
		new Vertex(-0.75f, -0.25f, 0.0f, 1, 0, 0),
		new Vertex(-0.25f, -0.75f, 0.0f, 1, 0, 1),
		new Vertex(-0.25f, -0.25f, 0.0f, 0, 0, 0)
	));
	gameobjectList.push_back(new Quad(
		new Vertex(0.75f, 0.75f, 0.0f, 0, 0, 1),
		new Vertex(0.75f, 0.25f, 0.0f, 0, 1, 1),
		new Vertex(0.25f, 0.75f, 0.0f, 0, 0, 1),
		new Vertex(0.25f, 0.25f, 0.0f, 0, 0, 0)
	));
	gameobjectList.push_back(new Quad(
		new Vertex(0.25f, -0.75f, 0.0f, 0, 0, 1),
		new Vertex(0.25f, -0.25f, 0.0f, 1, 1, 0),
		new Vertex(0.75f, -0.75f, 0.0f, 0, 0, 0),
		new Vertex(0.75f, -0.25f, 0.0f, 1, 1, 0)
	));
#pragma endregion

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

#pragma region Run Update and Draw Methods of all Gameobjects
	if (!gameobjectList.empty())
	{
		for (int i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->update();
			gameobjectList[i]->draw();
		}
	}
#pragma endregion

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_swap_chain->release();

	GraphicsEngine::get()->release();
}