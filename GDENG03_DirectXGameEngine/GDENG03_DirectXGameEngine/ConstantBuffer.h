#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	bool load(void* buffer, UINT size_buffer);
	void update(DeviceContext* context, void* buffer);
	bool release();
	~ConstantBuffer();
public:
	ID3D11Buffer* m_buffer;
private:
	friend class DeviceContext;
};