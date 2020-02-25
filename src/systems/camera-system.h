#pragma once

#include "i-system.h"

#include "scomponents/graphics/camera.h"
#include "scomponents/io/inputs.h"

class CameraSystem : public ISystem {
public:
    CameraSystem();
    virtual ~CameraSystem();

	void update(Camera& cam, const Inputs& inputs);
};
