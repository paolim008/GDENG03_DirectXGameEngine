#include "RasterizerStateManager.h"

#include <d3d11.h>

#include "DeviceContext.h"
#include "GraphicsEngine.h"

RasterizerStateManager::RasterizerStateManager()
{

}

RasterizerStateManager::~RasterizerStateManager()
{

}

void RasterizerStateManager::InitializeStates()
{
	D3D11_RASTERIZER_DESC wfd;

	//INITIALIZE NO WIREFRAME
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_SOLID;
	wfd.CullMode = D3D11_CULL_BACK;
	wfd.DepthClipEnable = true;
	//CREATE RASTERIZER STATE
	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_solid_back_cull_RS);

	//INITIALIZE NO CULL WIREFRAME
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;
	//CREATE RASTERIZER STATE
	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_wireframe_no_cull_RS);

	//INITIALIZE FRONT CULL WIREFRAME
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_FRONT;
	wfd.DepthClipEnable = true;
	//CREATE RASTERIZER STATE
	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_wireframe_front_cull_RS);

	//INITIALIZE BACK CULL WIREFRAME
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_BACK;
	wfd.DepthClipEnable = true;
	//CREATE RASTERIZER STATE
	GraphicsEngine::get()->get_device()->CreateRasterizerState(&wfd, &m_wireframe_back_cull_RS);
}

void RasterizerStateManager::update()
{
	if (!this->useWireframe) return;

	switch (this->rasterState)
	{
	case NOCULLWF: GraphicsEngine::get()->getImmediateDeviceContext()->setRSState(m_wireframe_no_cull_RS);
		break;
	case FRONTCULLWF: GraphicsEngine::get()->getImmediateDeviceContext()->setRSState(m_wireframe_front_cull_RS);
		break;
	case BACKCULLWF: GraphicsEngine::get()->getImmediateDeviceContext()->setRSState(m_wireframe_back_cull_RS);
		break;
	}

}

void RasterizerStateManager::ChangeRasterState(RASTERSTATE rasterState)
{
	this->rasterState = rasterState;
}

void RasterizerStateManager::UseWireframe(bool status)
{
	this->useWireframe = status;
	if (!status)
	{
		this->rasterState = BACKCULLS;
		GraphicsEngine::get()->getImmediateDeviceContext()->setRSState(m_solid_back_cull_RS);
	}
	else
	{
		this->rasterState = NOCULLWF;
	}
	
}

