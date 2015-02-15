#ifndef _Font_hpp_
#define _Font_hpp_

#include <string>
#include <vector>

#include <stdint.h>

// Get away with forward declaring the C struct.
struct _stbtt_pack_range;
typedef _stbtt_pack_range stbtt_pack_range;

struct _stbtt_packedchar;
typedef _stbtt_packedchar stbtt_packedchar;

namespace fgen
{
    class Font
    {
        protected:
            static constexpr unsigned int BITMAP_W = 512;
            static constexpr unsigned int BITMAP_H = 512;

            // Simpler version of stbtt_pack_range so a user can add codepoints more easily.
            struct PackRange
            {
                public:
                    float fontsize;
                    int first_unicode_char;
                    int num_chars;
            };

            std::vector<stbtt_pack_range> ranges;
            std::vector<stbtt_packedchar> pdata;
            std::vector<uint8_t> bitmap;

        public:
            // Static character sets
            static const std::vector<PackRange> jp;

        public:
            Font(const std::string& filename,
                 const std::vector<PackRange>& charRanges);
            Font(const uint8_t fontData[], unsigned int dataLength,
                 const std::vector<PackRange>& charRanges);
            ~Font();

            // Copying
            Font(const Font& that);
            Font& operator=(Font that);

            // Moving
            Font(Font&& that);
            Font& operator=(Font&& that);

            // Write our font bitmap as a png file.
            void writeBitmap(const std::string& filename);

        private:
            // Load font data into a buffer
            std::vector<uint8_t> loadDataFromFile(const std::string& filename) const;
            std::vector<uint8_t> loadData(const uint8_t fontData[], unsigned int dataLength) const;

            // Load and pack fonts
            void loadFont(const std::string& filename,
                          const std::vector<PackRange>& charRanges);
            void loadFont(const uint8_t fontData[], unsigned int dataLength,
                          const std::vector<PackRange>& charRanges);

            // Create a font bitmap and setup font data.
            void packFont(const std::vector<uint8_t>& ttf_data,
                          const std::vector<PackRange>& charRanges);
    };
} /* namespace fgen */

#endif /* _Font_hpp_ */
