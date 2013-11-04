#include "button.h"
#include "pin.h"

class PollButton : public Button {
public:
	PollButton(Pin*);
	bool wasPressed();
private:
	Pin* pin;
	int val;
	unsigned long lastAt;
};
