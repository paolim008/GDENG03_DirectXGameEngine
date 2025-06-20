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
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();

	~SwapChain();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;

private:
	friend class DeviceContext;

};