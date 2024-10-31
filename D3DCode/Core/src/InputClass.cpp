#include "../headers/InputClass.h"

InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass& i)
{

}

InputClass::~InputClass()
{

}

bool InputClass::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return true;
}

bool InputClass::Shutdown()
{

	return true;
}

void InputClass::KeyDown(unsigned int keycode)
{
	m_keys[keycode] = true;
}

void InputClass::KeyUp(unsigned int keycode)
{
	m_keys[keycode] = false;
}

bool InputClass::isKeyPressed(unsigned int keycode)
{
	return m_keys[keycode];
}

