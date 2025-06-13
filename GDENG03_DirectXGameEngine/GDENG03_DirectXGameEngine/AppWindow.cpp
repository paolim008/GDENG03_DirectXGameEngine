#include "AppWindow.h"
#include <iostream>
#include <Windows.h>

#include "IndexBuffer.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Plane.h"
#include "RasterizerStateManager.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;

	float m_angle;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	double deltaTime = EngineTime::getDeltaTime();

	//ENGINE DELTA TIME
	m_angle += 50 * (deltaTime);
	constant cc;
	cc.m_angle = m_angle;

	m_delta_pos += deltaTime * 1.0f;
	if (m_delta_pos > 1.0f)
	{
		m_delta_pos = 0;
	}

	m_delta_scale += deltaTime / 0.55f;


	Matrix4x4 temp;

	// cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	// temp.setScale(Vector3D::lerp(Vector3D(.5,.5,0), Vector3D(1,1,0), (sin(m_delta_scale)+1.0f)/2.0f));
	// cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;

	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.3f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);


	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();


	cc.m_view = world_cam; 
	// cc.m_projection.setOrthoLH
	// (
	// 	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	// 	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	// 	-4.0f,
	// 	4.0f
	// );

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_projection.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.01f, 100.0f);


	//m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	EngineTime::initialize();
	EngineTime::setTimeScale(1.f);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	this->rasterizerStateManager = new RasterizerStateManager();
	rasterizerStateManager->InitializeStates();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	/*for (int i = 0; i < 5; i++)
	{
		Cube* cube = new Cube("Cube", shader_byte_code, size_shader);
		cube->setAnimSpeed(0.f);
		cube->setPosition(i*0.2, 0, 0);
		cube->setScale(Vector3D(.1, .1, .1));
		cube->setRotation(0, 0, 0);
		cube->setAnimSpeed(1.f);
		this->m_gameObjectList.push_back(cube);
		cout << "Cube Created: " << i << endl;

		Plane* plane = new Plane("Plane", shader_byte_code, size_shader);
		plane->setAnimSpeed(0.f);
		plane->setPosition(i*0.2, 0, 0);
		plane->setScale(Vector3D(.1, .1, .1));
		plane->setRotation(0, 0, 0);
		plane->setAnimSpeed(1.f);
		this->m_gameObjectList.push_back(plane);
		cout << "Plane Created: " << i << endl;
	}*/


	Cube* cube = new Cube("Cube", shader_byte_code, size_shader);
	cube->setAnimSpeed(0.f);
	cube->setPosition(0, 0, 0);
	cube->setScale(Vector3D(.1, .1, .1));
	cube->setRotation(45, 45, 0);
	cube->setAnimSpeed(1.f);
	this->m_gameObjectList.push_back(cube);

	Plane* plane = new Plane("Plane", shader_byte_code, size_shader);
	plane->setAnimSpeed(0.f);
	plane->setPosition(0, 0, 0);
	plane->setScale(Vector3D(.4, .4, .4));
	plane->setRotation(0, 90, 0);
	plane->setAnimSpeed(1.f);
	this->m_gameObjectList.push_back(plane);




	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	// constant cc;
	// cc.m_angle = 0;
	//
	// m_cb = GraphicsEngine::get()->createConstantBuffer();
	// m_cb->load(&cc, sizeof(constant));


}

void AppWindow::onUpdate()
{
	EngineTime::LogFrameStart();

	Window::onUpdate();

	InputSystem::get()->update();

#pragma region Setup Cube Rendering

	double deltaTime = EngineTime::getDeltaTime();
	
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);

	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(width, height);

	update();

#pragma endregion Setup Cube Rendering

	for (int i = 0; i < m_gameObjectList.size(); i++)
	{
		m_gameObjectList[i]->update(deltaTime);
		m_gameObjectList[i]->draw(width, height, this->m_vs, this->m_ps);
	}
	m_swap_chain->present(true);

	EngineTime::LogFrameEnd();
} 

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::InterpolateTimeScale()
{
	if (isIncreasing)
	{
		currentTimeScale += EngineTime::getUnscaledDeltaTime();
		if (currentTimeScale > 2)
		{
			isIncreasing = false;
		}
	}
	else
	{
		currentTimeScale -= EngineTime::getUnscaledDeltaTime();
		if (currentTimeScale < 0.1)
		{
			isIncreasing = true;
		}
	}
	//cout << currentTimeScale << endl;
	EngineTime::setTimeScale(currentTimeScale);
}

void AppWindow::onKeyDown(int key)
{
	float rotationSpeed = 10.0f;
	if (key == 'W')
	{
		//m_forward = 1.0f;
		//m_rot_x += rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'S')
	{
		//m_forward = -1.0f;
		//m_rot_x -= rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'A')
	{
		//m_rightward = -1.0f;

		//m_rot_y += rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'D')
	{
		//m_rightward = 1.0f;

		//m_rot_y -= rotationSpeed * EngineTime::getDeltaTime();
	}

}

void AppWindow::onKeyUp(int key)
{
	//m_forward = 0.0f;
	//m_rightward = 0.0f;

	if (key == 'R')
	{
			this->rasterizerStateManager->ChangeRasterState();
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	float engineDeltaTime = EngineTime::getDeltaTime();

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * engineDeltaTime;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f))* engineDeltaTime;

	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.2f;
	//this->rasterizerStateManager->UseWireframe(false);
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.f;
	//this->rasterizerStateManager->UseWireframe(true);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.f;

}
