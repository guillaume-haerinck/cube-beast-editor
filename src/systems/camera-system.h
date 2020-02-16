#pragma once

#include "i-system.h"

class CameraSystem : public ISystem {
public:
    CameraSystem();
    virtual ~CameraSystem();

	void update();
};
