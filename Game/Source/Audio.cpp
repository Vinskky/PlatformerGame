#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

Audio::Audio() : Module()
{
	music = NULL;
	name.Create("audio");
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	if (config != NULL)
	{
		volume = config.child("volume").attribute("value").as_int();
		LoadFx(config.child("fx").child("jump").attribute("name").as_string());
		LoadFx(config.child("fx").child("checkpoint").attribute("name").as_string());
		LoadFx(config.child("fx").child("collectible").attribute("name").as_string());
		LoadFx(config.child("fx").child("healthRefill").attribute("name").as_string());
		LoadFx(config.child("fx").child("mouseHover").attribute("name").as_string());
		LoadFx(config.child("fx").child("mouseClick").attribute("name").as_string());
		LoadFx(config.child("fx").child("playerHurt").attribute("name").as_string());
		//Load checkpoint sound
		fxVolume = config.child("fx").child("volume").attribute("value").as_int();
	}

	return ret;
}

bool Audio::PreUpdate()
{
	return true;
}

//Update called each loop to assign volume value
bool Audio::Update(float dt)
{
	return true;
}

bool Audio::PostUpdate()
{
	bool ret = true;

	Mix_VolumeMusic(volume);
	ListItem< Mix_Chunk*>* item = fx.start;
	while (item != NULL)
	{
		Mix_VolumeChunk(item->data, fxVolume);
		item = item->next;
	}
	
	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	uint ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}
	return ret;
}

// Play WAV
bool Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.Count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

void Audio::SetVolume(uint value)
{
	if (value < 5)
	{
		volume = 0;
	}
	else
	{
		volume = value;
	}
}

void Audio::SetFXVolume(uint value)
{
	if (value < 5)
	{
		fxVolume = 0;
	}
	else
	{
		fxVolume = value;
	}
}

bool Audio::Load(pugi::xml_node &loadNode)
{
	volume = (float)loadNode.child("volume").attribute("value").as_int();
	return true;
}

bool Audio::Save(pugi::xml_node &saveNode) const
{
	bool ret = true;

	pugi::xml_node vol = saveNode.append_child("volume");
	if (vol == NULL)
	{
		LOG("Error on Save function of %s", name.GetString());
		ret = false;
	}
	else
	{
		vol.append_attribute("value").set_value(volume);
	}

	return ret;
}


