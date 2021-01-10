#ifndef _FONTS_H_
#define _FONTS_H_

#include "Module.h"
#include "SDL/include/SDL_pixels.h"
#include "List.h"

#define DEFAULT_FONT ""
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class Fonts : public Module
{
public:
	Fonts();

	// Destructor
	virtual ~Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL, SDL_Texture* tex = nullptr);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	List<_TTF_Font*> fonts;
	_TTF_Font* default;
	_TTF_Font* fontTitle;
};
#endif // !_FONTS_H_
