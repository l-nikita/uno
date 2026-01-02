#pragma once

#include <stdexcept>
#include <unordered_map>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class TextEngine
{
public:
	TextEngine(SDL_Renderer* renderer)
	{
		if (!TTF_Init()) 
		{
			SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
			throw std::runtime_error("Couldn't initialize SDL_ttf");
		}

		m_engine = TTF_CreateRendererTextEngine(renderer);
		if (!m_engine)
		{
			SDL_Log("Couldn't create text engine: %s\n", SDL_GetError());
			throw std::runtime_error("Couldn't create text engine");
		}
	}

	~TextEngine()
	{
		for (auto& [text, ttfText] : m_texts)
			TTF_DestroyText(ttfText);

		m_texts.clear();

		for (auto& [name, font] : m_fonts)
			TTF_CloseFont(font);

		m_fonts.clear();

		if (m_engine)
			TTF_DestroyRendererTextEngine(m_engine);

		TTF_Quit();
	}

	void CreateFont(std::string name, std::string filePath, float size)
	{
		auto font = TTF_OpenFont(filePath.c_str(), size);
		if (!font) 
		{
			SDL_Log("Couldn't open font: %s\n", SDL_GetError());
			throw std::runtime_error("Couldn't open font");
		}

		m_fonts[name] = font;
	}

	void DrawText(std::string text, std::string fontName, float x, float y, SDL_Color color)
	{
		auto search = m_texts.find(text);
		if (search == m_texts.end())
		{
			CreateText(text, fontName);
			return;
		}

		auto txt = search->second;

		if (txt)
			TTF_DrawRendererText(txt, x, y), TTF_SetTextColor(txt, color.r, color.g, color.b, color.a);
	}
	
private:
	void CreateText(std::string text, std::string fontName)
	{
		auto search = m_fonts.find(fontName);
		if (search == m_fonts.end())
		{
			SDL_Log("Font [%s] not found\n", fontName.c_str());
			throw std::runtime_error("Font not found");
		}

		auto txt = TTF_CreateText(m_engine, search->second, text.c_str(), 0);
		if (!txt)
		{
			SDL_Log("Couldn't create text: %s\n", SDL_GetError());
			throw std::runtime_error("Couldn't open font");
		}

		m_texts[text] = txt;
	}

private:
	TTF_TextEngine* m_engine = nullptr;
	std::unordered_map<std::string, TTF_Font*> m_fonts;
	std::unordered_map<std::string, TTF_Text*> m_texts;
};