#pragma once

#include <string>
#include "IDrawableObject.hpp"

class NumerableObject : public IDrawableObject {
public:
	NumerableObject();
	virtual ~NumerableObject() = default;

	virtual void setValue(int value);
	virtual int getValue();
	virtual std::string getValueString();
	virtual void increaseValue(int incr) = 0;

protected:
	int m_value;
};