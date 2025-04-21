#include "audio.h"

Result<Audio*, const char*> audio_create() {
	if(Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNEL, AUDIO_CHUNK_SIZE) == -1)
		return "SDL2_mixer could not be initialized!\n";
	return new Audio;
}

void audio_destroy(Audio* audio) {
	for (const auto& [id, chunk] : audio->chunks) {
		Mix_FreeChunk(chunk);
	}

	for (const auto& [id, music] : audio->musics) {
		Mix_FreeMusic(music);
	}

	delete audio;
}

b32 audio_register_chunk(Audio* audio, i32 id, const std::string& file_path) {
	Mix_Chunk* chunk = Mix_LoadWAV(file_path.c_str());
	if (chunk == NULL)
		return false;

	audio->chunks.insert({id, chunk});
	return true;
}

b32 audio_play_chunk(Audio* audio, i32 id, i32 loop) {
	if (audio->chunks.find(id) == audio->chunks.end()) {
		printf("Cannot find id: %d in chunks", id);
		return false;
	}

	return Mix_PlayChannel(-1, audio->chunks[id], loop) >= 0;
}

b32 audio_set_chunk_volume(Audio* audio, i32 id, i32 volume) {
	if (audio->chunks.find(id) == audio->chunks.end()) {
		printf("Cannot find id: %d in chunks", id);
		return false;
	}

	return Mix_VolumeChunk(audio->chunks[id], volume) != -1;
}

b32 audio_register_music(Audio* audio, i32 id, const std::string& file_path) {
	Mix_Music* music = Mix_LoadMUS(file_path.c_str());
	if (music == NULL)
		return false;

	audio->musics.insert({id, music});
	return true;
}

b32 audio_play_music(Audio* audio, i32 id, i32 loop) {
	if (audio->musics.find(id) == audio->musics.end()) {
		printf("Cannot find id: %d in musics", id);
		return false;
	}

	return Mix_PlayMusic(audio->musics[id], loop) >= 0;
}

b32 audio_set_music_volume(i32 volume) {
	return Mix_VolumeMusic(volume) != -1;
}

void audio_pause_music() {
	Mix_PauseMusic();
}

void audio_resume_music() {
	Mix_ResumeMusic();
}

b32 audio_is_music_playing() {
	return Mix_PlayingMusic() == 1;
}
