#pragma once
#include "AGameObject.h"
#include "InputListener.h"

class Camera: public AGameObject, public InputListener
{
public:
	Camera(std::string name);
	~Camera() override;

	void update(float deltaTime) override;
	Matrix4x4 getViewMatrix();

	

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;

private:
	void updateViewMatrix();

public:
	void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) override;

private:
	float deltaTime;
	bool mouseDown = false;
};

