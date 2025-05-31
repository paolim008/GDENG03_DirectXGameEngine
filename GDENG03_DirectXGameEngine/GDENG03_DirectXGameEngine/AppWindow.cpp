#include "AppWindow.h"
#include <iostream>
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vertex
{
	Vector3D position;
	Vector3D position1;
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
	cout << m_delta_pos << endl;
	if (m_delta_pos > 1.0f)
	{
		m_delta_pos = 0;
	}

	m_delta_scale += EngineTime::getDeltaTime() * 30.0f;


	Matrix4x4 temp;

	cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));

	temp.setScale(Vector3D::lerp(Vector3D(.5,.5,0), Vector3D(1,1,0), (sin(m_delta_scale)+1.0f)/2.0f));



	cc.m_world *= temp;



	cc.m_view.setIdentity();
	cc.m_projection.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
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

	EngineTime::initialize();
	EngineTime::setTimeScale(1.f);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	InitRenderStates();

	vertex list[] =
	{
		//X - Y - Z
		{Vector3D(- 0.5f,-0.5f,0.0f),    Vector3D(-0.32f,-0.11f,0.0f),   Vector3D(0,0,0),  Vector3D(0,1,0)}, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.5f,0.4f,0.0f),    Vector3D(1,1,0),  Vector3D(0,1,1)}, // POS2
		{ Vector3D(0.5f,-0.5f,0.0f),     Vector3D(0.75f,-0.73f,0.0f),   Vector3D(0,0,1),  Vector3D(1,0,0)},// POS2
		{ Vector3D(0.5f,0.5f,0.0f),      Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(1,1,1),  Vector3D(0,0,1)}
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

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

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);

	if (useWireframe)
	{
		//Set Rasterizer State to wireframe
		GraphicsEngine::get()->getImmediateDeviceContext()->setRSState(m_wireframe_RS);
	}

	EngineTime::LogFrameEnd();
} 

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
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
	cout << currentTimeScale << endl;
	EngineTime::setTimeScale(currentTimeScale);
}


void AppWindow::InitRenderStates()
{
	D3D11_RASTERIZER_DESC wfd;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_BACK;
	wfd.DepthClipEnable = true;

	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_wireframe_RS);
}