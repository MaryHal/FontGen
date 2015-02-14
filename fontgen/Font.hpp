#ifndef _Font_hpp_
#define _Font_hpp_

#include <string>
#include <vector>

#include <stdint.h>

// Get away with forward declaring the C struct.
struct _stbtt_packedchar;
typedef _stbtt_packedchar stbtt_packedchar;

namespace fgen
{
    class Font
    {
        private:
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

            std::vector<stbtt_packedchar> pdata;
            std::vector<uint8_t> bitmap;

        public:
            // Static character sets
            static const std::vector<PackRange> jp;

        public:
            Font(const std::string& filename, const std::vector<PackRange>& charRanges);
            ~Font();

            // Copying
            Font(const Font& that);
            Font& operator=(Font that);

            // Moving
            Font(Font&& that);
            Font& operator=(Font&& that);

            void writeBitmap(const std::string& filename);

        private:
            void loadFont(const std::string& filename, const std::vector<PackRange>& charRanges);

            // void draw(float x, float y, const std::string& text) const;
    };
} /* namespace fgen */

#endif /* _Font_hpp_ */
