#include <Font.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdexcept>

const std::vector<Font::PackRange> Font::jp
{
    { 20.0f, 32, 95 },
    { 20.0f, 0x3041, 0x3096 - 0x3041 }, // Hiragana
    { 20.0f, 0x30A0, 0x30FF - 0x30A0 }, // Katakana
    { 20.0f, 0x3000, 0x303F - 0x3000 }  // Punctuation
};

Font::Font(const std::string& filename, const std::vector<PackRange>& charRanges)
    : pdata{}, bitmap{}
    {
        loadFont(filename, charRanges);
    }

Font::~Font()
{
}

Font::Font(const Font& that)
    : pdata{that.pdata},
      bitmap{that.bitmap}
    {
    }

Font& Font::operator=(Font that)
{
    std::swap(pdata, that.pdata);
    std::swap(bitmap, that.bitmap);
    return *this;
}

Font::Font(Font&& that)
    : pdata{that.pdata},
      bitmap{that.bitmap}
    {
    }

Font& Font::operator=(Font&& that)
{
    pdata = that.pdata;
    bitmap = that.bitmap;

    return *this;
}

stbtt_aligned_quad Font::getQuad(int& charIndex, float& x, float& y)
{
    stbtt_aligned_quad q;
    stbtt_GetPackedQuad(&pdata[0], BITMAP_W, BITMAP_H,
                        charIndex - 32,
                        &x, &y, &q,
                        1); //1=opengl & d3d10+,0=d3d9

    return q;
}

const std::vector<stbtt_packedchar>& Font::getpdata() const
{
    return pdata;
}

void Font::writeBitmap(const std::string& filename)
{
    stbi_write_png(filename.c_str(), BITMAP_W, BITMAP_H, 1, &bitmap[0], 0);
}

void Font::loadFont(const std::string& filename, const std::vector<PackRange>& charRanges)
{
    const int DATASIZE = 1 << 25;
    std::vector<uint8_t> ttf_buffer;
    ttf_buffer.reserve(DATASIZE);

    // uint8_t temp_bitmap[BITMAP_H][BITMAP_W];
    bitmap.reserve(BITMAP_W * BITMAP_H);

    std::FILE* ttf_file = std::fopen(filename.c_str(), "rb");
    if (!ttf_file)
    {
        throw std::runtime_error{"Could not open ttf file: " + filename};
    }
    std::fread(&ttf_buffer[0], sizeof(uint8_t), DATASIZE, ttf_file);
    std::fclose(ttf_file);

    stbtt_pack_context pc;
    if (!stbtt_PackBegin(&pc, &bitmap[0], BITMAP_W, BITMAP_H, 0, 1, nullptr))
    {
        throw std::runtime_error{"stbtt_PackBegin error"};
    }

    std::vector<stbtt_pack_range> pranges;

// Figure out how many characters to save data for.
    int totalChars{};
    for (auto& range : charRanges)
    {
        totalChars += range.num_chars;
    }
    pdata.reserve(totalChars);

    int runningTotal{};
    for (auto& range : charRanges)
    {
        pranges.push_back({range.fontsize, range.first_unicode_char, range.num_chars, &pdata[runningTotal]});
        runningTotal += range.num_chars;
    }

    stbtt_PackSetOversampling(&pc, 2, 2);
    if (!stbtt_PackFontRanges(&pc, &ttf_buffer[0], 0, &pranges[0], pranges.size()))
    {
        throw std::runtime_error{"stbtt_PackFontRanges error. Chars cannot fit on bitmap?"};
    }

// And we're done!
    stbtt_PackEnd(&pc);
}

// void Font::draw(float x, float y, const std::string& text) const
// {
//     glEnable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, texID);

//     glBegin(GL_QUADS);
//     for (int c : text)
//     {
//         stbtt_aligned_quad q;
//         stbtt_GetPackedQuad(const_cast<stbtt_packedchar*>(pdata),
//                             BITMAP_W, BITMAP_H,
//                             c - 32,
//                             &x, &y, &q,
//                             1); //1=opengl & d3d10+,0=d3d9

//         glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
//         glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
//         glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
//         glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
//     }
//     glEnd();

//     glDisable(GL_TEXTURE_2D);
// }
