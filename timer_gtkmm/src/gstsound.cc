/*
 * gstsound.cc
 *
 *  Created on: 25 févr. 2013
 *      Author: yann
 */

#include "gstsound.h"
#include <iomanip>


Sound::Sound(Glib::ustring filename)
{
	// for frequency play
	m_pipeline = Gst::Pipeline::create("player");
	m_source = Gst::ElementFactory::create_element("audiotestsrc", "source");
	m_sink = Gst::ElementFactory::create_element("autoaudiosink", "output");
	m_pipeline->add(m_source);
	m_pipeline->add(m_sink);
	m_source->link(m_sink);
	//for file play
	m_bin = Gst::PlayBin::create("myplayer");
	m_bus = m_bin->get_bus();
	m_filetoplay = "file://" + filename;
	m_bin->set_property("uri", m_filetoplay);
}

void Sound::start_playing_frequency (double frequency)
{
	m_source->set_property("freq", frequency);
	m_pipeline->set_state(Gst::STATE_PLAYING);

	/* stop it after 1000ms */
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Sound::stop_playing),
	                               1000);
}

bool Sound::stop_playing()
{
	m_pipeline->set_state(Gst::STATE_NULL);
	return false;
}

bool Sound::on_bus_message(const Glib::RefPtr<Gst::Bus> &bus, const Glib::RefPtr<Gst::Message> &message)
{
	switch(message->get_message_type())
	{
	case Gst::MESSAGE_EOS:
		m_bin->set_state(Gst::STATE_PAUSED); //pause
		m_bin->seek(Gst::FORMAT_TIME, Gst::SEEK_FLAG_FLUSH, 0); //rewind
		m_bin->set_state(Gst::STATE_PLAYING); //play
		break;
	default:
		break;
	}

	return true;

}

bool Sound::on_timeout()
{
	play_file();
	//As this is a timeout function, return true so that it
	//continues to get called
	return true;
}

void Sound::play_file_each(int mseconds)
{
	//Add a timer callback to update the values
	//http://developer.gnome.org/gtkmm-tutorial/3.3/sec-timeouts.html.en
	m_connection_id_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Sound::on_timeout), mseconds );

}

void Sound::stop_file_each()
{
	m_connection_id_timeout.disconnect();
}

void Sound::play_file_looped()
{
	m_bin->set_state(Gst::STATE_PLAYING);
	// Add a bus watch to receive messages from the pipeline's bus:
	m_bus->add_watch(sigc::mem_fun(*this,&Sound::on_bus_message));
}

void Sound::play_file()
{
	//m_bin->set_state(Gst::STATE_NULL); //need to replay the file if same
	m_bin->set_state(Gst::STATE_PAUSED); //pause
	m_bin->seek(Gst::FORMAT_TIME, Gst::SEEK_FLAG_FLUSH, 0); //rewind
	m_bin->set_state(Gst::STATE_PLAYING); //play
}

void Sound::stop_file()
{
	m_bin->set_state(Gst::STATE_NULL);
}
