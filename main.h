
#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__

#include "view_state.h"
#include "view_drawer.h"
#include "io_controller.h"

class Game {
private:
	ViewState vs;
	ViewDrawer vd = vd(5);
	// ViewDrawer vd;
	IOController ioc;

public:
	void mainloop();

	void exit_program(int);
};

#endif