#include <stdio.h>
#include "rmlui_file_interface.hpp"

FileInterface::FileInterface(const Rml::String& root) : root(root) 
{
}

FileInterface::~FileInterface() 
{
}

Rml::FileHandle FileInterface::Open(const Rml::String& path)
{
	// Attempt to open the file relative to the application's root.
	FILE* fp = fopen((root + path).c_str(), "rb");
	if (fp != nullptr)
		return (Rml::FileHandle)fp;

	// Attempt to open the file relative to the current working directory.
	fp = fopen(path.c_str(), "rb");
	return (Rml::FileHandle)fp;
}

void FileInterface::Close(Rml::FileHandle file)
{
	fclose((FILE*)file);
}

size_t FileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*)file);
}

bool FileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	return fseek((FILE*)file, offset, origin) == 0;
}

size_t FileInterface::Tell(Rml::FileHandle file)
{
	return ftell((FILE*)file);
}
