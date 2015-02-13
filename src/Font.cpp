#include <Font.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>

const std::vector<Font::PackRange> Font::jp
{
    { 20.0f, 32, 95 },
    { 20.0f, 0x3040, 95 },
    { 20.0f, 0x30A0, 95 }
};

Font::Font(const std::string& filename, std::vector<PackRange> charRanges)
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

void Font::loadFont(const std::string& filename, std::vector<PackRange> charRanges)
{
    uint8_t ttf_buffer[1<<20];

    // uint8_t temp_bitmap[BITMAP_H][BITMAP_W];
    bitmap.reserve(BITMAP_W * BITMAP_H);

    fread(ttf_buffer, 1, 1 << 20, fopen(filename.c_str(), "rb"));

    stbtt_pack_context pc;
    stbtt_PackBegin(&pc, &bitmap[0], BITMAP_W, BITMAP_H, 0, 1, nullptr);

    std::vector<stbtt_pack_range> pranges;

    int totalChars{};
    for (auto& range : charRanges)
    {
        totalChars += range.num_chars;
    }

    pdata.reserve(totalChars);

    totalChars = 0;
    for (auto& range : charRanges)
    {
        stbtt_pack_range pr{range.fontsize, range.first_unicode_char, range.num_chars, &pdata[totalChars]};
        pranges.push_back(pr);

        totalChars += range.num_chars;
    }
    std::cout << pranges.size() << std::endl << pdata.capacity() << std::endl;

    stbtt_PackSetOversampling(&pc, 2, 2);
    stbtt_PackFontRanges(&pc, ttf_buffer, 0, &pranges[0], pranges.size());
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
