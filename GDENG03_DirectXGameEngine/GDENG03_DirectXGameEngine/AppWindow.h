#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();
	void updateQuadPosition();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	void InterpolateTimeScale();

private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

	float m_angle = 0;

private: 
	void InitRenderStates();
	bool useWireframe = false;

public:
	ID3D11RasterizerState* m_wireframe_RS;

	//Animation Oscillator
private:
	double currentTimeScale = 1.0f;
	bool isIncreasing = true;

private:

	float m_delta_pos;
	float m_delta_scale;
};