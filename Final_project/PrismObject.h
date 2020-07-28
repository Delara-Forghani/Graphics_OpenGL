#pragma once

#include "GraphicDevice.h"
#include "GameObject.h"


class GraphicDevice;

class PrismObject : public GameObject
{
public:
	PrismObject(GraphicDevice* device);

	void				Update(GLFWwindow* window) override;
	void				Setup() override;

};
