#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"

#include <future>
#include <iostream>
#include <functional>

#include "Audio.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "PerformanceTimer.h"

#define BYTES_TO_MEGABYTES(bytes) (bytes / (1024.0f * 1024.0f))

void ody::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}

    //Preloading
}

ody::ResourceManager::~ResourceManager()
{
    //Release the memory of the cached audio files
	for (const auto& sound : m_CachedAudios)
		sound.second->Release();

	for (const auto& sound : m_PreLoadedAudios)
		sound.second->Release();
}


std::shared_ptr<ody::Texture2D> ody::ResourceManager::LoadTexture(const std::string& file) const
{
    const auto fullPath = m_DataPath + file;

    auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

    if (texture == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
    }

    return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<ody::Font> ody::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_DataPath + file, size);
}

std::shared_ptr<ody::Music> ody::ResourceManager::LoadMusic(const std::string& file) const
{
    return std::make_shared<Music>(m_DataPath + file);
}

std::shared_ptr<ody::Sound> ody::ResourceManager::LoadSoundEffect(const std::string& file)
{
    // Check if the sound effect is already loaded
    auto it = m_CachedAudios.find(file);
    if (it != m_CachedAudios.end())
        return std::dynamic_pointer_cast<Sound>(it->second);

    // Check if the sound effect is already preloaded
    it = m_PreLoadedAudios.find(file);
    if (it != m_PreLoadedAudios.end())
        return std::dynamic_pointer_cast<Sound>(it->second);

    auto sound = std::make_shared<Sound>(m_DataPath + file);

	m_CachedAudios.insert(std::make_pair(file, sound));

    //Todo: Test this deletion part
    if (BYTES_TO_MEGABYTES(GetLoadedAudiosSize()) >= m_MaxCachedAudioSize)
    {
        while (BYTES_TO_MEGABYTES(GetLoadedAudiosSize()) > m_MaxCachedAudioSize - m_AudioClearSize)
        {
            //Clear audios from the map, until the size is less than the max size - the clear size
            for (auto mapIt = m_CachedAudios.begin(); mapIt != m_CachedAudios.end(); ++mapIt)
            {
                mapIt->second->Release();
                mapIt->second.reset();
                m_CachedAudios.erase(mapIt);
            }
        }
	   
    }

    return sound;
}
std::mutex ody::ResourceManager::m_MapMutex{};

void ody::ResourceManager::AsyncLoadSound(const std::string file, 
    std::map<std::string, std::shared_ptr<Sound>>* map)
{
    std::cout << "Loading: " << file << std::endl;

    auto sound = std::make_shared<Sound>(file);

    std::lock_guard<std::mutex> lock(m_MapMutex);
    map->insert(std::make_pair(file, sound));
}


void ody::ResourceManager::PreLoad(const std::vector<std::string>& paths)
{
    for (const auto& file : paths)
    {
        m_Futures.emplace_back(std::async(std::launch::async, AsyncLoadSound, std::string{m_DataPath + file}, &m_PreLoadedAudios));
    }
}

unsigned int ody::ResourceManager::GetLoadedAudiosSize() const
{
    unsigned int totalSize{0}; // In Bytes

    for (auto it = m_CachedAudios.begin(); it != m_CachedAudios.end(); ++it)
	    totalSize += it->second->GetSize();

    return totalSize;
}
