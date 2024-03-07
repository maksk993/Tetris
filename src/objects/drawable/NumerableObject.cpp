#include "NumerableObject.hpp"

NumerableObject::NumerableObject() : m_value(0) {
}

void NumerableObject::setValue(int value) {
	m_value = value;
}

int NumerableObject::getValue() {
	return m_value;
}

std::string NumerableObject::getValueString() {
	return std::to_string(m_value);
}