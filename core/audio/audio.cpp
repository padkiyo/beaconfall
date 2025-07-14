#include "audio.h"

AudioManager::AudioManager(i32 frequency, i32 channel, i32 chunk_size)
	: m_freq(frequency), m_channel(channel), m_chunk_size(chunk_size) {
	panic(
		Mix_OpenAudio(m_freq, MIX_DEFAULT_FORMAT, m_channel, m_chunk_size) == 0,
		"Failed to initialize SDL2 Mixer"
	);
}

AudioManager::~AudioManager() {
	for (const auto& [id, chunk] : m_chunks) {
		Mix_FreeChunk(chunk);
	}

	for (const auto& [id, music] : m_musics) {
		Mix_FreeMusic(music);
	}
}

void AudioManager::register_chunk(i32 id, const std::string& file_path) {
	Mix_Chunk* chunk = Mix_LoadWAV(file_path.c_str());
	panic(chunk, "Failed to load WAV file: %s", file_path.c_str());
	m_chunks.insert({id, chunk});
}

void AudioManager::play_chunk(i32 id, i32 loop) {
	panic(
		m_chunks.find(id) != m_chunks.end(),
		"Cannot find Chunk ID: %d", id
	);

	panic(
		Mix_PlayChannel(-1, m_chunks[id], loop) != -1,
		"Failed to play Chunk with ID: %d", id
	);
}

void AudioManager::set_chunk_volume(i32 id, i32 volume) {
	panic(
		m_chunks.find(id) != m_chunks.end(),
		"Cannot find Chunk ID: %d", id
	);

	panic(
		Mix_VolumeChunk(m_chunks[id], volume) != -1,
		"Failed to change volume of Chunk with ID: %d", id
	);
}

void AudioManager::register_music(i32 id, const std::string& file_path) {
	Mix_Music* music = Mix_LoadMUS(file_path.c_str());
	panic(music, "Failed to load Music: %s", file_path.c_str());
	m_musics.insert({id, music});
}

void AudioManager::play_music(i32 id, i32 loop) {
	panic(
		m_musics.find(id) != m_musics.end(),
		"Cannot find Music with ID: %d", id
	);

	panic(
		Mix_PlayMusic(m_musics[id], loop) != -1,
		"Failed to play Music with ID: %d", id
	);
}

void AudioManager::set_music_volume(i32 volume) {
	panic(
		Mix_VolumeMusic(volume) != -1,
		"Failed to change volume of Music"
	);
}

void AudioManager::pause_music() {
	Mix_PauseMusic();
}

void AudioManager::resume_music() {
	Mix_ResumeMusic();
}

b32 AudioManager::is_music_playing() {
	return Mix_PlayingMusic() == 1;
}
