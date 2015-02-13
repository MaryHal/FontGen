#ifndef _Font_hpp_
#define _Font_hpp_

#include <stb_truetype.h>

#include <string>
#include <vector>

#include <stdint.h>

class Font
{
    private:
        static constexpr unsigned int BITMAP_W = 512;
        static constexpr unsigned int BITMAP_H = 512;

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
        Font(const std::string& filename, std::vector<PackRange> charRanges);
        ~Font();

        // Copying
        Font(const Font& that);
        Font& operator=(Font that);

        // Moving
        Font(Font&& that);
        Font& operator=(Font&& that);

        stbtt_aligned_quad getQuad(int& charIndex, float& x, float& y);
        const std::vector<stbtt_packedchar>& getpdata() const;

        void writeBitmap(const std::string& filename);

    private:
        void loadFont(const std::string& filename, std::vector<PackRange> charRanges);

        // void draw(float x, float y, const std::string& text) const;
};

#endif /* _Font_hpp_ */
