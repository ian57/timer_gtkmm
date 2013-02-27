/*
 * gstsound.h
 *
 *  Created on: 25 févr. 2013
 *      Author: yann
 */

#ifndef GSTSOUND_H_
#define GSTSOUND_H_

#include <gstreamermm.h>

class Sound
{
	public:
		Sound(std::string filename);

		void start_playing_frequency(double frequency);
		bool stop_playing();
		void play_file();
		void play_file_looped();
		void play_file_each(int mseconds);
		void stop_file_each();
		bool on_timeout();
		void stop_file();
		bool on_bus_message(const Glib::RefPtr<Gst::Bus> &bus, const Glib::RefPtr<Gst::Message> &message);

	private:
		Glib::RefPtr<Gst::Pipeline> m_pipeline;
		Glib::RefPtr<Gst::Element> m_source;
		Glib::RefPtr<Gst::Element> m_sink;
		Glib::RefPtr<Gst::PlayBin> m_bin;
		Glib::RefPtr<Gst::Bus> m_bus;
		std::string m_filetoplay;
		sigc::connection m_connection_id_timeout;
};

#endif /* GSTSOUND_H_ */
