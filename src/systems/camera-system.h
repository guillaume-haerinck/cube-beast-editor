#pragma once

#include "i-system.h"

#include "data/graphics/camera.h"
#include "data/io/inputs.h"

class CameraSystem : public ISystem {
public:
    CameraSystem();
    virtual ~CameraSystem();

	void update(Camera& cam, const Inputs& inputs);
};
