/*
 ============================================================================
 Name        : timer_gtkmm.cc
 Author      : Yann
 Version     :
 Copyright   : Your copyright notice
 Description : Simpler timer in gttkmm
 ============================================================================
 */

#include "Timer.h"
#include <gstreamermm.h>
#include <gtkmm.h>

int main (int argc, char *argv[])
{
	  Gtk::Main kit(argc, argv);
	  Gst::init (argc, argv);
	  Timer timer;
	  Gtk::Window * win = timer.GetMainWindow();
	  //Shows the window and returns when it is closed.

	  kit.run((* win));
	  return 0;
}


