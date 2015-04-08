#include <fontgen/Font.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdexcept>

namespace fgen
{
    FontData::FontData(const std::string& filename)
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

        // Reserve the correct amount of space for our buffer and load it all in.
        buffer.reserve(filesize);

        std::fread(buffer.data(), sizeof(uint8_t), filesize, ttf_file);
        std::fclose(ttf_file);
    }

    FontData::FontData(const uint8_t data[], unsigned int dataLength)
        : buffer{data, data + dataLength}
    {
    }

    const std::vector<uint8_t>& FontData::get() const
    {
        return buffer;
    }

    Font::Font(const std::string& filename,
               const std::vector<PackRange>& charRanges,
               unsigned int width, unsigned int height)
        : Font{FontData{filename}, charRanges, width, height}
    {
    }

    Font::Font(const uint8_t data[], unsigned int dataLength,
               const std::vector<PackRange>& charRanges,
               unsigned int width, unsigned int height)
        : Font{FontData{data, dataLength}, charRanges, width, height}
    {
    }

    Font::Font(const FontData& fontData,
               const std::vector<PackRange>& charRanges,
               unsigned int width, unsigned int height)
        : ranges{}, pdata{}, bitmap{},
          bitmapWidth{width},
          bitmapHeight{height}
    {
        bitmap.reserve(bitmapWidth * bitmapHeight);
        packFont(fontData, charRanges);
    }

    Font::~Font()
    {
    }

    Font::Font(const Font& that)
        : ranges{that.ranges},
          pdata{that.pdata},
          bitmap{that.bitmap},
          bitmapWidth{that.bitmapWidth},
          bitmapHeight{that.bitmapHeight}
    {
        rebuildRangePointers();
    }

    Font& Font::operator=(Font that)
    {
        std::swap(ranges, that.ranges);
        std::swap(pdata, that.pdata);
        std::swap(bitmap, that.bitmap);

        bitmapWidth  = that.bitmapWidth;
        bitmapHeight = that.bitmapHeight;

        rebuildRangePointers();

        return *this;
    }

    Font::Font(Font&& that)
        : ranges{that.ranges},
          pdata{that.pdata},
          bitmap{that.bitmap},
          bitmapWidth{that.bitmapWidth},
          bitmapHeight{that.bitmapHeight}
    {
        rebuildRangePointers();
    }

    Font& Font::operator=(Font&& that)
    {
        ranges = that.ranges;
        pdata = that.pdata;
        bitmap = that.bitmap;

        bitmapWidth  = that.bitmapWidth;
        bitmapHeight = that.bitmapHeight;

        rebuildRangePointers();

        return *this;
    }

    void Font::writeBitmap(const std::string& filename)
    {
        stbi_write_png(filename.c_str(), bitmapWidth, bitmapHeight, 1, bitmap.data(), 0);
    }

    void Font::packFont(const FontData& fontData,
                        const std::vector<PackRange>& charRanges)
    {
        stbtt_pack_context pc;
        if (!stbtt_PackBegin(&pc, bitmap.data(), bitmapWidth, bitmapHeight, 0, 1, nullptr))
        {
            throw std::runtime_error{"stbtt_PackBegin error"};
        }

        // Figure out how many characters to save data for.
        int totalChars{};
        for (const auto& range : charRanges)
        {
            totalChars += 1 + range.unicodePair.second - range.unicodePair.first;
        }
        pdata.reserve(totalChars);

        int runningTotal{};
        for (const auto& range : charRanges)
        {
            // Convert our simple PackRange into stbtt_pack_range and calculate packed char data
            // indices.
            ranges.push_back({range.fontsize, range.unicodePair.first, 1 + range.unicodePair.second - range.unicodePair.first, &pdata[runningTotal]});
            runningTotal += 1 + range.unicodePair.second - range.unicodePair.first;
        }

        stbtt_PackSetOversampling(&pc, 2, 2);
        if (!stbtt_PackFontRanges(&pc, const_cast<uint8_t*>(fontData.get().data()), 0, ranges.data(), ranges.size()))
        {
            throw std::runtime_error{"stbtt_PackFontRanges error. Chars cannot fit on bitmap?"};
        }

        // And we're done!
        stbtt_PackEnd(&pc);
    }

    void Font::rebuildRangePointers()
    {
        int runningTotal{};
        for (auto& range : ranges)
        {
            range.chardata_for_range = &pdata.data()[runningTotal];
            runningTotal += range.num_chars_in_range;
        }
    }
} /* namespace fgen */
