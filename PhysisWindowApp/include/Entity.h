#pragma once
class Entity
{
protected:
	unsigned int POS_COORDS = 2;

public:
	virtual void Render() = 0;
};

