#pragma once

#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Types.h>

namespace client::ui
{
    class FileInterface : public Rml::FileInterface
    {
    public:
        explicit FileInterface( const Rml::String& root );
        ~FileInterface() override;

        /// Opens a file.
        Rml::FileHandle Open( const Rml::String& path ) override;

        /// Closes a previously opened file.
        void Close( Rml::FileHandle file ) override;

        /// Reads data from a previously opened file.
        std::size_t Read( void* buffer, size_t size, Rml::FileHandle file ) override;

        /// Seeks to a point in a previously opened file.
        bool Seek( Rml::FileHandle file, long offset, int origin ) override;

        /// Returns the current position of the file pointer.
        std::size_t Tell( Rml::FileHandle file ) override;

    private:
        Rml::String root;
    };
}
