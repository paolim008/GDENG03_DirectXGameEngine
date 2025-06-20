#pragma once
#include "AGameObject.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "InputListener.h"
#include "VertexBuffer.h"

using namespace std;
class Cube : public AGameObject, public InputListener
{
public:
	Cube(string name, void* shaderByteCode, size_t sizeShader);
	~Cube();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) override;
	void setAnimSpeed(float speed);

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& delta_mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;
	//void onMiddleMouseDown(const Point& mouse_pos) override;
	// void onMiddleMouseUp(const Point& mouse_pos) override;

	float deltaPos = 0.0f;


private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;
	CBData cbData;

	float ticks = 0.0f;
	float deltaTime = 0.0f;
	float deltaScale = 1.0f;
	float speed = 100.0f;

private:
	bool isKeyDown = false;
	bool isScalingUp = false;

};