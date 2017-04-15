#pragma once

#include "AVFrameQ.h"
#include "avplayer.h"
#include "sys/thread.h"
#include "sys/sync.hpp"

class AVPlayerCore
{
public:
	AVPlayerCore(avplayer_onrender avrender, void* param);
	~AVPlayerCore();

public:
	void Play();
	void Stop();
	void Pause();

	uint64_t GetAudioDuration(uint64_t clock) const;

public:
	void Input(const void* pcm, uint64_t pts, uint64_t duration, int serial);
	void Input(const void* yuv, uint64_t pts, int serial);

private:
	static int STDCALL OnThread(void* param);
	int OnThread();

	int OnPlay(uint64_t clock);
	int OnVideo(uint64_t clock);
	int OnAudio(uint64_t clock);
	int AVSync(uint64_t clock);

private:
	AVFrame m_audio; // next play audio frame
	AVFrame m_video; // next play video frame
	AVFrameQ m_audioQ;
	AVFrameQ m_videoQ;

	bool m_running;
	pthread_t m_thread;
	ThreadEvent m_event;

	avplayer_onrender m_avrender;
	void* m_param;

	int m_status; // avplayer_status_xxx

	struct avclock_t
	{
		uint64_t pts; // (MS), last played audio/video frame PTS
		uint64_t clock; // time(MS), last played audio/video frame time
		uint64_t frame_time; // frame play time(predict)
		uint64_t duration; // audio only
	};

	struct avclock_t m_aclock;
	struct avclock_t m_vclock;
	struct avclock_t m_system;
};
