#include <OpenGLFont.hpp>
#include <GL/glew.h>

#include <stb_truetype.h>

namespace fgen
{
    OpenGLFont::OpenGLFont(const std::string& filename,
                           const std::vector<PackRange>& charRanges)
        : Font{filename, charRanges},
          textureID{0}
    {
        makeOpenGLTexture();
    }
    
    OpenGLFont::~OpenGLFont()
    {
        glDeleteTextures(1, &textureID);
    }

    // Copying
    OpenGLFont::OpenGLFont(const OpenGLFont& that)
        : Font(that)
    {
        makeOpenGLTexture();
    }

    OpenGLFont& OpenGLFont::operator=(OpenGLFont that)
    {
        Font::operator=(that);
        makeOpenGLTexture();

        return *this;
    }

    // Moving
    OpenGLFont::OpenGLFont(OpenGLFont&& that)
        : Font{that}
    {
        makeOpenGLTexture();
    }
    OpenGLFont& OpenGLFont::operator=(OpenGLFont&& that)
    {
        Font::operator=(that);
        makeOpenGLTexture();

        return *this;
    }

    void OpenGLFont::draw(float x, float y, const std::string& text)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Boo! Immediate mode!
        glBegin(GL_QUADS);
        for (int c : text)
        {
            stbtt_aligned_quad q;
            stbtt_GetPackedQuad(&pdata[0],
                                BITMAP_W, BITMAP_H,
                                c - 32,
                                &x, &y, &q,
                                1); //1=opengl & d3d10+,0=d3d9

            glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
            glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
            glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
            glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void OpenGLFont::makeOpenGLTexture()
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                     BITMAP_W, BITMAP_H, 0, GL_ALPHA,
                     GL_UNSIGNED_BYTE, &bitmap[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}
