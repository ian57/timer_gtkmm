/*
 ============================================================================
 Name        : Timer.cc
 Author      : Yann
 Version     :
 Copyright   : Your copyright notice
 Description : Simpler timer in gttkmm class file
 ============================================================================
 */
#include "Timer.h"
#include <iomanip>

//#include <glibmm.h>

using namespace std;

Timer::Timer() :
m_spinbutton_hours_adj( Gtk::Adjustment::create(0.0, 0.0, 23.0, 1.0, 5.0, 0.0)),
m_spinbutton_minutes_adj( Gtk::Adjustment::create(0.0, 0.0, 59.0, 1.0, 5.0, 0.0)),
m_spinbutton_seconds_adj( Gtk::Adjustment::create(0.0, 0.0, 59.0, 1.0, 5.0, 0.0))
{

	  //Load the GtkBuilder file and instantiate its widgets:
	  m_refBuilder = Gtk::Builder::create();
	  try
	  {
	    m_refBuilder->add_from_file(Glib::ustring("../src/timer.glade"));
	  }
	  catch(const Glib::FileError& ex)
	  {
	    std::cerr << "FileError: " << ex.what() << std::endl;
	  }
	  catch(const Glib::MarkupError& ex)
	  {
	    std::cerr << "MarkupError: " << ex.what() << std::endl;
	  }
	  catch(const Gtk::BuilderError& ex)
	  {
	    std::cerr << "BuilderError: " << ex.what() << std::endl;
	  }

	  //Get the GtkBuilder-instantiated Dialog:
	  m_refBuilder->get_widget("window_timer", m_window_timer);
	  if(m_window_timer)
	  {
		  m_window_timer->signal_delete_event().connect(sigc::mem_fun(*this,&Timer::on_window_timer_destroy_cb));
	  }

	  m_refBuilder->get_widget("button_start", m_Button_Timer_Start);
	  if(m_Button_Timer_Start)
	  {
		  m_Button_Timer_Start->signal_released().connect(sigc::mem_fun(*this,&Timer::on_Button_Timer_Start_release_cb));

	  }

	  m_refBuilder->get_widget("button_stop", m_Button_Timer_Stop);
	  if(m_Button_Timer_Stop)
	  {
		  m_Button_Timer_Stop->signal_released().connect(sigc::mem_fun(*this,&Timer::on_Button_Timer_Stop_release_cb));

	  }

	  m_refBuilder->get_widget("spinbutton_hours", m_spinbutton_hours);
		if(m_spinbutton_hours)
		{
			m_spinbutton_hours->configure(m_spinbutton_hours_adj,1.0,0);
		}
	  m_refBuilder->get_widget("spinbutton_minutes", m_spinbutton_minutes);
		if(m_spinbutton_minutes)
		{
			m_spinbutton_minutes->configure(m_spinbutton_minutes_adj,1.0,0);
		}
	  m_refBuilder->get_widget("spinbutton_seconds", m_spinbutton_seconds);
		if(m_spinbutton_seconds)
		{
			m_spinbutton_seconds->configure(m_spinbutton_seconds_adj,1.0,0);
		}
	  m_refBuilder->get_widget("label_remain_hours_value", m_label_hours);
	  m_refBuilder->get_widget("label_remain_minutes_value", m_label_minutes);
	  m_refBuilder->get_widget("label_remain_seconds_value", m_label_seconds);
	  m_refBuilder->get_widget("eventbox_labels", m_eventbox_labels);

	  m_tick = new Sound("/home/yann/git/timer_gtkmm/timer_gtkmm/data/clock_tick.mp3");
	  m_alarm = new Sound("/home/yann/git/timer_gtkmm/timer_gtkmm/data/cuckoo_clock.mp3");


}

bool Timer::on_window_timer_destroy_cb(GdkEventAny * event)
{
	m_window_timer->hide();
	std::cout << "Hello on_window_timer_destroy_cb" << std::endl;
	return true;
}

void Timer::on_Button_Timer_Start_release_cb()
{

	std::cout << "Hello on_Button_Timer_Start_release_cb" << std::endl;
	m_hours = m_spinbutton_hours->get_value();
	m_minutes = m_spinbutton_minutes->get_value();
	m_seconds = m_spinbutton_seconds->get_value();

	std::cout << m_hours << ":" << m_minutes << ":" << m_seconds << std::endl;
	m_label_seconds->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_seconds));
	m_label_minutes->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_minutes));
	m_label_hours->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_hours));

//	// need iomanip cf. https://github.com/vickenty/tomator/blob/master/states.cc
//	m_label_hours->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_hours));
//	m_label_minutes->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_minutes));
//	m_label_seconds->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_seconds));
	//http://developer.gnome.org/gtk3/3.0/gtk-question-index.html
	m_eventbox_labels->override_background_color(Gdk::RGBA("green"));

	//Add a timer callback to update the values
	//http://developer.gnome.org/gtkmm-tutorial/3.3/sec-timeouts.html.en
	m_connection_id_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Timer::on_timeout), 1000 );
	m_seconds--; //because on_timout starts in 1 second.
}

void Timer::on_Button_Timer_Stop_release_cb()
{
	std::cout << "Hello on_Button_Timer_Stop_release_cb" << std::endl;
	//http://developer.gnome.org/gtkmm-tutorial/3.3/sec-timeouts.html.en
	m_connection_id_timeout.disconnect();
}

bool Timer::on_timeout()
{
	m_label_seconds->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_seconds));
	m_label_minutes->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_minutes));
	m_label_hours->set_text(Glib::ustring::format(setfill(L'0'), std::setw(2), m_hours));
	//play sound
	m_tick->play_file();
	// time management
	m_seconds --;
	if (m_seconds<0)
	{
		m_seconds=59;
		m_minutes --;
		if (m_minutes < 0)
		{
			m_minutes = 59;
			m_hours --;
			if (m_hours < 0)
			{
				m_connection_id_timeout.disconnect();
				std::cout << "Hello m_connection_id_timeout.disconnect" << std::endl;
				//http://developer.gnome.org/gtk3/3.0/gtk-question-index.html
				m_eventbox_labels->override_background_color(Gdk::RGBA("red"));

				m_alarm->play_file_looped();

				Gtk::MessageDialog dialog(*this, "Click OK to stop Alarm");
				  dialog.set_secondary_text(
				          "Don't click if you like Cuckoo :-).");
				  int result = dialog.run();

				  //Handle the response:
				  switch(result)
				  {
				    default: //don't mind the result!!!
				    {
				      m_alarm->stop_file();
				      break;
				    }
				  }

			}
		}
	}

  //As this is a timeout function, return true so that it
  //continues to get called
  return true;
}

Gtk::Window* Timer::GetMainWindow()
{
	return this->m_window_timer;
}
