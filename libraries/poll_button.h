#include "button.h"
#include "pin.h"

class PollButton : public Button {
public:
	PollButton(Pin*);
	bool wasPressed();
	bool isPressed();
private:
	Pin* pin;
	bool wasUp;
};
