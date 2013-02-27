/*
 ============================================================================
 Name        : Timer.h
 Author      : Yann
 Version     :
 Copyright   : Your copyright notice
 Description : Simpler timer in gttkmm Header class file
 ============================================================================
 */
#ifndef GTKMM_TIMER_H
#define GTKMM_TIMER_H

#include <gtkmm.h>
#include <iostream>
#include <map>
#include "gstsound.h"

class Timer : public Gtk::Window
{
public:
  Timer();
  Gtk::Window * GetMainWindow();

protected:
  // signal handlers
  bool on_window_timer_destroy_cb(GdkEventAny * event);
  void on_Button_Timer_Start_release_cb();
  void on_Button_Timer_Stop_release_cb();
  bool on_timeout();

  // Member data:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Gtk::Window *m_window_timer;
  Gtk::Button *m_Button_Timer_Start, *m_Button_Timer_Stop;
  Gtk::SpinButton * m_spinbutton_hours, * m_spinbutton_minutes, * m_spinbutton_seconds;
  Gtk::Label * m_label_hours, * m_label_minutes, *m_label_seconds;
  Gtk::EventBox * m_eventbox_labels;
  Glib::RefPtr<Gtk::Adjustment> m_spinbutton_hours_adj, m_spinbutton_minutes_adj,m_spinbutton_seconds_adj;
  // Keep track of the timer added
  sigc::connection m_connection_id_timeout;
  //current values
  gint m_hours, m_minutes, m_seconds;
  Sound *m_tick, *m_alarm;
};

#endif // GTKMM_TIMER_H

