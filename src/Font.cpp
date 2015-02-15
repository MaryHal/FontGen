#include <Font.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdexcept>

namespace fgen
{
    const std::vector<Font::PackRange> Font::jp
    {
        { 20.0f, 32, 95 },
        { 20.0f, 0x3041, 0x3096 - 0x3041 }, // Hiragana
        { 20.0f, 0x30A0, 0x30FF - 0x30A0 }, // Katakana
        { 20.0f, 0x3000, 0x303F - 0x3000 }  // Punctuation
    };

    Font::Font(const std::string& filename,
               const std::vector<PackRange>& charRanges)
        : ranges{}, pdata{}, bitmap{}
        {
            loadFont(filename, charRanges);
        }

    Font::Font(const uint8_t fontData[], unsigned int dataLength,
               const std::vector<PackRange>& charRanges)
        : ranges{}, pdata{}, bitmap{}
            {
                loadFont(fontData, dataLength, charRanges);
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

    void Font::writeBitmap(const std::string& filename)
    {
        stbi_write_png(filename.c_str(), BITMAP_W, BITMAP_H, 1, &bitmap[0], 0);
    }

    std::vector<uint8_t> Font::loadDataFromFile(const std::string& filename) const
    {
        std::FILE* ttf_file = std::fopen(filename.c_str(), "rb");
        if (!ttf_file)
        {
            throw std::runtime_error{"Could not open ttf file: " + filename};
        }

        // Read Filesize
        std::fseek(ttf_file, 0L, SEEK_END);
        long filesize{ftell(ttf_file)};

        std::rewind(ttf_file);

        std::vector<uint8_t> ttf_buffer;
        ttf_buffer.reserve(filesize);

        std::fread(&ttf_buffer[0], sizeof(uint8_t), filesize, ttf_file);
        std::fclose(ttf_file);

        return ttf_buffer;
    }

    std::vector<uint8_t> Font::loadData(const uint8_t fontData[], unsigned int dataLength) const
    {
        return std::vector<uint8_t>(fontData, fontData + dataLength);
    }

    void Font::loadFont(const std::string& filename,
                        const std::vector<PackRange>& charRanges)
    {
        bitmap.reserve(BITMAP_W * BITMAP_H);

        packFont(loadDataFromFile(filename), charRanges);
    }

    void Font::loadFont(const uint8_t fontData[], unsigned int dataLength,
                        const std::vector<PackRange>& charRanges)
    {
        bitmap.reserve(BITMAP_W * BITMAP_H);

        packFont(loadData(fontData, dataLength), charRanges);
    }

    void Font::packFont(const std::vector<uint8_t>& ttf_data,
                        const std::vector<PackRange>& charRanges)
    {
        stbtt_pack_context pc;
        if (!stbtt_PackBegin(&pc, &bitmap[0], BITMAP_W, BITMAP_H, 0, 1, nullptr))
        {
            throw std::runtime_error{"stbtt_PackBegin error"};
        }

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
            // Convert our simple PackRange into stbtt_pack_range and calculate packed char data
            // indices.
            ranges.push_back({range.fontsize, range.first_unicode_char, range.num_chars, &pdata[runningTotal]});
            runningTotal += range.num_chars;
        }

        stbtt_PackSetOversampling(&pc, 2, 2);
        if (!stbtt_PackFontRanges(&pc, const_cast<uint8_t*>(&ttf_data[0]), 0, &ranges[0], ranges.size()))
        {
            throw std::runtime_error{"stbtt_PackFontRanges error. Chars cannot fit on bitmap?"};
        }

        // And we're done!
        stbtt_PackEnd(&pc);
    }
} /* namespace fgen */
