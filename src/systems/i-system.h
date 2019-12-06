#pragma once

class ISystem {
public:
	virtual ~ISystem() {};
	virtual void update() = 0;
};
