#pragma once

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Profiling.h>
#include <SDL3_image/SDL_image.h>
#include "rmlui_platform_sdl.hpp"
#include "rmlui_renderer_gl3.hpp"

/*
	Custom render interface example for the SDL/GL3 backend.
	Overloads the OpenGL3 render interface to load textures through SDL_image's built-in texture loading functionality.
*/
class RenderInterface_GL3_SDL : public RenderInterface_GL3 
{
public:
	RenderInterface_GL3_SDL() { }

	Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override
	{
		Rml::FileInterface* file_interface = Rml::GetFileInterface();
		Rml::FileHandle file_handle = file_interface->Open(source);

		if (!file_handle)
			return {};

		file_interface->Seek(file_handle, 0, SEEK_END);
		const size_t buffer_size = file_interface->Tell(file_handle);
		file_interface->Seek(file_handle, 0, SEEK_SET);

		using Rml::byte;
		Rml::UniquePtr<byte[]> buffer(new byte[buffer_size]);
		file_interface->Read(buffer.get(), buffer_size, file_handle);
		file_interface->Close(file_handle);

		const size_t i_ext = source.rfind('.');
		Rml::String extension = (i_ext == Rml::String::npos ? Rml::String() : source.substr(i_ext + 1));

		auto CreateSurface = [&]() { return IMG_LoadTyped_IO(SDL_IOFromMem(buffer.get(), int(buffer_size)), 1, extension.c_str()); };
		auto GetSurfaceFormat = [](SDL_Surface* surface) { return surface->format; };
		auto ConvertSurface = [](SDL_Surface* surface, SDL_PixelFormat format) { return SDL_ConvertSurface(surface, format); };
		auto DestroySurface = [](SDL_Surface* surface) { SDL_DestroySurface(surface); };

		SDL_Surface* surface = CreateSurface();
		if (!surface)
			return {};

		texture_dimensions = { surface->w, surface->h };

		if (GetSurfaceFormat(surface) != SDL_PIXELFORMAT_RGBA32)
		{
			// Ensure correct format for premultiplied alpha conversion and GenerateTexture below.
			SDL_Surface* converted_surface = ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
			DestroySurface(surface);
			if (!converted_surface)
				return {};

			surface = converted_surface;
		}

		// Convert colors to premultiplied alpha, which is necessary for correct alpha compositing.
		const size_t pixels_byte_size = surface->w * surface->h * 4;
		byte* pixels = static_cast<byte*>(surface->pixels);
		for (size_t i = 0; i < pixels_byte_size; i += 4)
		{
			const byte alpha = pixels[i + 3];
			for (size_t j = 0; j < 3; ++j)
				pixels[i + j] = byte(int(pixels[i + j]) * int(alpha) / 255);
		}

		Rml::TextureHandle texture_handle = RenderInterface_GL3::GenerateTexture({ pixels, pixels_byte_size }, texture_dimensions);

		DestroySurface(surface);

		return texture_handle;
	}
};