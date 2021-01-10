#include "App.h"
#include "Textures.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_ttf/include/SDL_ttf.h"

#pragma comment( lib, "../Game/Source/External/SDL_ttf/libx86/SDL2_ttf.lib" )

Fonts::Fonts() : Module()
{
	name.Create("fonts");
}

// Destructor
Fonts::~Fonts()
{
}

// Called before render is available
bool Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string();
		int size = conf.child("default_font").attribute("size").as_int();
		default = Load(path, size);
		fontTitle = Load(path, 25);
	}

	return ret;
}

// Called before quitting
bool Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	ListItem<_TTF_Font*>* item;

	for (item = fonts.start; item != NULL; item = item->next)
	{
		TTF_CloseFont(item->data);
	}

	fonts.Clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
_TTF_Font* const Fonts::Load(const char* path, int size)
{
	_TTF_Font* font = TTF_OpenFont(path, size);


	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.Add(font);
	}

	return font;
}

// Print text using font
SDL_Texture* Fonts::Print(const char* text, SDL_Color color, _TTF_Font* font, SDL_Texture* tex)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = app->tex->LoadSurface(surface, tex);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, & width, & height) != 0)
	{
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = true;
	}

	return ret;
}