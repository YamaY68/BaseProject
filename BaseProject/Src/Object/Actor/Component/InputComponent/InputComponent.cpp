#include "InputComponent.h"

InputComponent::InputComponent(int front, int back, int left, int right, int up, int down)
	:
	frontKey_(front),
	backKey_(back),
	leftKey_(left),
	rightKey_(right),
	upKey_(up),
	downKey_(down)
{
}
