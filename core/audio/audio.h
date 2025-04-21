#pragma once

#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <stdio.h>

#include "core/base/types.h"
#include "core/base/result.h"
#include "core/base/log.h"

#define AUDIO_FREQUENCY 22050
#define AUDIO_CHANNEL 2
#define AUDIO_CHUNK_SIZE 4096

struct Audio {
	std::unordered_map<i32, Mix_Chunk*> chunks;
	std::unordered_map<i32, Mix_Music*> musics;
};

Result<Audio*, const char*> audio_create();
void audio_destroy(Audio* audio);

// Chunked audios
b32 audio_register_chunk(Audio* audio, i32 id, const std::string& file_path);
b32 audio_play_chunk(Audio* audio, i32 id, i32 loop);
b32 audio_set_chunk_volume(Audio* audio, i32 id, i32 volume);

// Musics
b32 audio_register_music(Audio* audio, i32 id, const std::string& file_path);
b32 audio_play_music(Audio* audio, i32 id, i32 loop);
b32 audio_set_music_volume(i32 volume);
void audio_pause_music();
void audio_resume_music();
b32 audio_is_music_playing();
