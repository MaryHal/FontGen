#ifndef _Font_hpp_
#define _Font_hpp_

#include <string>
#include <vector>

#include <stdint.h>
#include <utility>

// Get away with forward declaring the C struct.
struct _stbtt_pack_range;
typedef _stbtt_pack_range stbtt_pack_range;

struct _stbtt_packedchar;
typedef _stbtt_packedchar stbtt_packedchar;

namespace fgen
{
    namespace set
    {
        // Static character sets
        static const std::pair<int, int> ascii       {32    , 127};
        static const std::pair<int, int> jp_hiragana {0x3041, 0x3096};
        static const std::pair<int, int> jp_katakana {0x30A0, 0x30FF};
        static const std::pair<int, int> jp_punct    {0x3000, 0x303F};
    }

    class Font
    {
        protected:
            static constexpr unsigned int DEFAULT_BITMAP_W = 512;
            static constexpr unsigned int DEFAULT_BITMAP_H = 512;

            // Simpler version of stbtt_pack_range so a user can add codepoints more easily.
            struct PackRange
            {
                public:
                    float fontsize;
                    std::pair<int, int> unicodePair;
            };

            std::vector<stbtt_pack_range> ranges;
            std::vector<stbtt_packedchar> pdata;
            std::vector<uint8_t> bitmap;
            unsigned int bitmapWidth;
            unsigned int bitmapHeight;

        public:
            Font(const std::string& filename,
                 const std::vector<PackRange>& charRanges,
                 unsigned int width  = DEFAULT_BITMAP_W,
                 unsigned int height = DEFAULT_BITMAP_H);
            Font(const uint8_t fontData[], unsigned int dataLength,
                 const std::vector<PackRange>& charRanges,
                 unsigned int width  = DEFAULT_BITMAP_W,
                 unsigned int height = DEFAULT_BITMAP_H);
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
