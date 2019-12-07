#pragma once

#include "i-system.h"
#include "scomponents/singleton-components.h"

class CameraSystem : public ISystem {
public:
    CameraSystem(SingletonComponents& scomps);
    virtual ~CameraSystem();

	void update() override;

private:
    SingletonComponents& m_scomps;
};
