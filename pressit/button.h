#include "pin.h"
class Button {
public:
	Button(Pin*);
	bool wasPressed();
	bool isPressed();
private:
	Pin* pin;
	bool wasUp;
};
