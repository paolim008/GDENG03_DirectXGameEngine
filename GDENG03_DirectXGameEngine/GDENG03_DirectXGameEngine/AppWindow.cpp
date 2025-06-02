#include "AppWindow.h"
#include <iostream>
#include <Windows.h>

#include "IndexBuffer.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
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

void AppWindow::updateQuadPosition()
{
	//ENGINE DELTA TIME
	m_angle += 50 * (EngineTime::getDeltaTime());
	constant cc;
	cc.m_angle = m_angle;

	m_delta_pos += EngineTime::getDeltaTime() * 1.0f;
	if (m_delta_pos > 1.0f)
	{
		m_delta_pos = 0;
	}

	m_delta_scale += EngineTime::getDeltaTime() / 0.55f;


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


	cc.m_view.setIdentity();
	cc.m_projection.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);



	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);

	EngineTime::initialize();
	EngineTime::setTimeScale(1.f);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	this->rasterizerStateManager = new RasterizerStateManager();
	rasterizerStateManager->InitializeStates();

	InitRenderStates();

	vertex vertex_list[] =
	{
		//X - Y - Z

		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),  Vector3D(1,0,1),  Vector3D(0,1,0)}, // POS1
		{Vector3D(-0.5f,0.5f,-0.5f), Vector3D(1,0,0),  Vector3D(0,1,1)}, // POS2
		{ Vector3D(0.5f,0.5f,-0.5f),Vector3D(0,1,1),  Vector3D(1,0,0)},// POS2
		{ Vector3D(0.5f,-0.5f,-0.5f), Vector3D(1,1,0),  Vector3D(0,0,1)}, //POS4

		//BACK FACE
		{Vector3D(0.5f,-0.5f,0.5f),  Vector3D(1,0,1),  Vector3D(0,1,0)}, // POS1
		{Vector3D(0.5f,0.5f,0.5f), Vector3D(1,0,0),  Vector3D(0,1,1)}, // POS2
		{ Vector3D(-0.5f,0.5f,0.5f),Vector3D(0,1,1),  Vector3D(1,0,0)},// POS2
		{ Vector3D(-0.5f,-0.5f,0.5f), Vector3D(1,1,0),  Vector3D(0,0,1)} //POS4
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,
		2,3,0,
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7,
	};

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list); 




	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_angle = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	EngineTime::LogFrameStart();

	InterpolateTimeScale();

	Window::onUpdate();

	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	m_swap_chain->present(true);

	rasterizerStateManager->update();

	
	EngineTime::LogFrameEnd();
} 

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
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


void AppWindow::InitRenderStates()
{
	D3D11_RASTERIZER_DESC wfd;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;

	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_wireframe_RS);
}

void AppWindow::onKeyDown(int key)
{
	float rotationSpeed = 10.0f;
	if (key == 'W')
	{
		m_rot_x += rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'S')
	{
		m_rot_x -= rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'A')
	{
		m_rot_y += rotationSpeed * EngineTime::getDeltaTime();
	}
	else if (key == 'D')
	{
		m_rot_y -= rotationSpeed * EngineTime::getDeltaTime();
	}
}

void AppWindow::onKeyUp(int key)
{
}

void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
	float engineDeltaTime = EngineTime::getDeltaTime();
	m_rot_x -= delta_mouse_pos.m_y * engineDeltaTime;
	m_rot_y -= delta_mouse_pos.m_x * engineDeltaTime;
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 0.5f;
	this->rasterizerStateManager->UseWireframe(false);
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 2.f;
	this->rasterizerStateManager->UseWireframe(true);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.f;

}
