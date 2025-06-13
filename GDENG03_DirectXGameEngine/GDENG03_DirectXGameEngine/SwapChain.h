#pragma once
#include <d3d11.h>
class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	//Initialize Swap Chain for a window
	bool init(HWND hwnd, UINT width, UINT height);
	//Release the Swap Chain
	bool release();

	bool present(bool vsync);
	~SwapChain();

public:
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_depthView;

private:
	friend class DeviceContext;

};