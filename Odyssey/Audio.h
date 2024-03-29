#pragma once
#include <string>
struct _Mix_Music;
struct Mix_Chunk;

//Simple RAII wrappers for SDL_mixer sfx & music structs
namespace ody
{
	class Sound final
	{
	public:
		Mix_Chunk* GetSoundEffect() const;
		explicit Sound(const std::string& fullPath);

		Sound(const Sound&) = delete;
		Sound(Sound&&) = delete;
		Sound& operator= (const Sound&) = delete;
		Sound& operator= (const Sound&&) = delete;

		void Release() const;

		unsigned int GetSize() const { return m_Size; }
	private:
		Mix_Chunk* m_pMixChunk;
		unsigned int m_Size{0};
	};

	class Music final
	{
	public:
		_Mix_Music* GetMusic() const;
		explicit Music(const std::string& fullPath);
		~Music();

		Music(const Music&) = delete;
		Music(Music&&) = delete;
		Music& operator= (const Music&) = delete;
		Music& operator= (const Music&&) = delete;
	private:
		_Mix_Music* m_pMusic;
	};
}

