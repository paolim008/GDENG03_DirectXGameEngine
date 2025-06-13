#pragma once
#include <vector>

#include "Window.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Cube.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "RasterizerStateManager.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	void update();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	//Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
	
private:
	void InterpolateTimeScale();

private:
	SwapChain* m_swap_chain;
	//VertexBuffer* m_vb;
	//IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;
	//ConstantBuffer* m_cb;

	float m_angle = 0;

public:
	ID3D11RasterizerState* m_wireframe_RS;
	RasterizerStateManager* rasterizerStateManager;
	//Animation Oscillator
private:
	double currentTimeScale = 1.0f;
	bool isIncreasing = true;
	ConstantBuffer* m_cb;

private:
	float m_delta_pos;
	float m_delta_scale;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world_cam;

private:
	vector<AGameObject*> m_gameObjectList;

};


