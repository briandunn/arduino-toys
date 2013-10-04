#include "button.h"
#include "Arduino.h"
class InterruptButton : public Button {
	public:
		InterruptButton(int);
		bool wasPressed();
	private:
		int reportedChanges;
		int interruptNumber;
		static volatile int changes[2];
		static void change0();
		static void change1();
};
