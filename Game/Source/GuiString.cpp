#include "GuiString.h"

GuiString::GuiString(uint32 id, SDL_Rect bounds, const char* text, _TTF_Font* textFont) : GuiControl(GuiControlType::TEXT, id)
{
	this->bounds = bounds;
	this->text = text;
	this->textFont = textFont;
	textTexture = app->font->Print(this->text.GetString(), { 255,255,255,255 }, textFont);
}

GuiString::GuiString(): GuiControl(GuiControlType::TEXT,id), textFont(nullptr)
{
}


GuiString::~GuiString()
{
}

void GuiString::Draw()
{
	app->render->DrawTexture(textTexture, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y);
}

void GuiString::SetString(char* newText)
{
	text = newText;
	textTexture = app->font->Print(text.GetString(), { 255,255,255,255 }, textFont, textTexture);
}

void GuiString::SetTextFont(_TTF_Font* textFont)
{
	this->textFont = textFont;
}
