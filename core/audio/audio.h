#pragma once
#include "common.h"

#define AUDIO_FREQUENCY 22050
#define AUDIO_CHANNEL 2
#define AUDIO_CHUNK_SIZE 4096

/*
 * The `id` here is a unique identifier for a audio file.
 * Such that we can just play music using that id
 *
 * eg:
 * JUMP_WAV = 0
 * audio_mgr.register_chunk(audio, JUMP_WAV, "jump.wav");
 *
 * now we can just use
 * audio_mgr.play_chunk(audio, JUMP_WAV, 0);
 */

class AudioManager {
public:
	AudioManager(i32 frequency = 22050, i32 channel = 2, i32 chunk_size = 4096);
	~AudioManager();

	//-------------
	//   Chunks
	//-------------
	void register_chunk(i32 id, const std::string& file_path);
	void play_chunk(i32 id, i32 loop = 0);      // 0 = No loop, -1 = Infinitie, n = number of loops
	void set_chunk_volume(i32 id, i32 volume);  // volume (0-128)

	//-------------
	//   Musics
	//-------------
	void register_music(i32 id, const std::string& file_path);
	void play_music(i32 id, i32 loop = 0);  // 0 = No loop, -1 = Infinitie, n = number of loops
	void set_music_volume(i32 volume);      // volume (0-128)
	void pause_music();
	void resume_music();
	b32 is_music_playing();

private:
	i32 m_freq, m_channel, m_chunk_size;

	std::unordered_map<i32, Mix_Chunk*> m_chunks;
	std::unordered_map<i32, Mix_Music*> m_musics;
};
