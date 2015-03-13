#ifndef _OpenGLFont_hpp_
#define _OpenGLFont_hpp_

#include <fontgen/Font.hpp>

namespace fgen
{
    class OpenGLFont : public Font
    {
        private:
            unsigned int textureID;

        public:
            OpenGLFont(const std::string& filename,
                       const std::vector<PackRange>& charRanges,
                       unsigned int width  = DEFAULT_BITMAP_W,
                       unsigned int height = DEFAULT_BITMAP_H);
            OpenGLFont(const uint8_t fontData[], unsigned int dataLength,
                       const std::vector<PackRange>& charRanges,
                       unsigned int width  = DEFAULT_BITMAP_W,
                       unsigned int height = DEFAULT_BITMAP_H);
            ~OpenGLFont();

            // Copying
            OpenGLFont(const OpenGLFont& that);
            OpenGLFont& operator=(OpenGLFont that);

            // Moving
            OpenGLFont(OpenGLFont&& that);
            OpenGLFont& operator=(OpenGLFont&& that);

            void draw(float x, float y, const std::wstring& text) const;
            void drawChar(float& x, float& y, const wchar_t c) const;

        private:
            void makeOpenGLTexture();
    };
}

#endif /* _OpenGLFont_hpp_ */
