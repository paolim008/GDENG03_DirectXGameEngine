#pragma once
#include <d3d11.h>

class RasterizerStateManager
{
public:
	RasterizerStateManager();
	~RasterizerStateManager();
	void InitializeStates();
	void update();

	enum RASTERSTATE
	{
		BACKCULLS,
		NOCULLWF,
		FRONTCULLWF,
		BACKCULLWF,
	};

	void ChangeRasterState(RASTERSTATE rasterState);
	void UseWireframe(bool status);

private:
	ID3D11RasterizerState* m_solid_back_cull_RS;
	ID3D11RasterizerState* m_wireframe_no_cull_RS;
	ID3D11RasterizerState* m_wireframe_front_cull_RS;
	ID3D11RasterizerState* m_wireframe_back_cull_RS;

	RASTERSTATE rasterState = NOCULLWF;
	bool useWireframe = false;
};



