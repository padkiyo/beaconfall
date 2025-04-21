#pragma once
#include "common.h"

#define AUDIO_FREQUENCY 22050
#define AUDIO_CHANNEL 2
#define AUDIO_CHUNK_SIZE 4096

struct Audio {
	std::unordered_map<i32, Mix_Chunk*> chunks;
	std::unordered_map<i32, Mix_Music*> musics;
};

Result<Audio*, const char*> audio_create();
void audio_destroy(Audio* audio);

/*
 * The `id` here is a unique identifier for a audio file.
 * Such that we can just play music using that id
 *
 * eg:
 * JUMP_WAV = 0
 * audio_register_chunk(audio, JUMP_WAV, "jump.wav");
 *
 * now we can just use
 * audio_play_chunk(audio, JUMP_WAV, 0);
 */

//-------------
//   Chunks
//-------------

b32 audio_register_chunk(Audio* audio, i32 id, const std::string& file_path);

/*
 * loop:
 *   0  = No loop
 *   -1 = Infinite loop ~65000
 *   n  = Number of loops
 */
b32 audio_play_chunk(Audio* audio, i32 id, i32 loop = 0);

/*
 * Volume 0 - 128 (Idk why 128 ask SDL guys)
 */
b32 audio_set_chunk_volume(Audio* audio, i32 id, i32 volume);


//-------------
//   Musics
//-------------

b32 audio_register_music(Audio* audio, i32 id, const std::string& file_path);

/*
 * loop:
 *   0  = No loop
 *   -1 = Infinite loop ~65000
 *   n  = Number of loops
 */
b32 audio_play_music(Audio* audio, i32 id, i32 loop = 0);

/*
 * Volume 0 - 128 (Idk why 128 ask SDL guys)
 */
b32 audio_set_music_volume(i32 volume);

void audio_pause_music();
b32 audio_paused_music();
void audio_resume_music();
b32 audio_is_music_playing();
