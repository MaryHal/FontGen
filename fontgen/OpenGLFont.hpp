#ifndef _OpenGLFont_hpp_
#define _OpenGLFont_hpp_

#include <Font.hpp>

namespace fgen
{
    class OpenGLFont : public Font
    {
        private:
            unsigned int textureID;

        public:
            OpenGLFont(const std::string& filename,
                       const std::vector<PackRange>& charRanges);
            OpenGLFont(const uint8_t fontData[], unsigned int dataLength,
                       const std::vector<PackRange>& charRanges);
            ~OpenGLFont();

            // Copying
            OpenGLFont(const OpenGLFont& that);
            OpenGLFont& operator=(OpenGLFont that);

            // Moving
            OpenGLFont(OpenGLFont&& that);
            OpenGLFont& operator=(OpenGLFont&& that);

            void draw(float x, float y, const std::string& text);

        private:
            void makeOpenGLTexture();
    };
}

#endif /* _OpenGLFont_hpp_ */
