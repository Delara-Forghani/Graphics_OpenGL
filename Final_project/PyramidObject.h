#pragma once

#include "GraphicDevice.h"
#include "GameObject.h"


class GraphicDevice;

class PyramidObject : public GameObject
{
public:
	PyramidObject(GraphicDevice* device);

	void				Update(GLFWwindow* window) override;
	void				Setup() override;

};

