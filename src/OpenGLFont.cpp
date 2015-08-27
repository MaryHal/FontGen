#include <fontgen/OpenGLFont.hpp>
#include <GL/glew.h>

#include <stb_truetype.h>

namespace fgen
{
    OpenGLFont::OpenGLFont(const std::string& filename,
                           const std::vector<PackRange>& charRanges,
                           unsigned int width, unsigned int height)
        : Font{filename, charRanges, width, height},
          textureID{0}
    {
        makeOpenGLTexture();
    }

    OpenGLFont::OpenGLFont(const uint8_t fontData[], unsigned int dataLength,
                           const std::vector<PackRange>& charRanges,
                           unsigned int width, unsigned int height)
        : Font{fontData, dataLength, charRanges, width, height},
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

    Text OpenGLFont:: makeText(float x, float y, const std::wstring& text)
    {
        Text t;

        for (int c : text)
        {
            // Find correct range for character
            const stbtt_pack_range* r = nullptr;
            for (const stbtt_pack_range& range : ranges)
            {
                if (c >= range.first_unicode_char_in_range &&
                    c <= range.first_unicode_char_in_range + range.num_chars_in_range)
                {
                    r = &range;

                    stbtt_aligned_quad q;
                    stbtt_GetPackedQuad(r->chardata_for_range,
                                        bitmapWidth, bitmapHeight,
                                        c - r->first_unicode_char_in_range,
                                        &x, &y, &q,
                                        1); //1=opengl & d3d10+,0=d3d9

                    t.data.push_back(q.s0);
                    t.data.push_back(q.t0);
                    t.data.push_back(q.x0);
                    t.data.push_back(q.y0);

                    t.data.push_back(q.s1);
                    t.data.push_back(q.t0);
                    t.data.push_back(q.x1);
                    t.data.push_back(q.y0);

                    t.data.push_back(q.s1);
                    t.data.push_back(q.t1);
                    t.data.push_back(q.x1);
                    t.data.push_back(q.y1);

                    t.data.push_back(q.s0);
                    t.data.push_back(q.t1);
                    t.data.push_back(q.x0);
                    t.data.push_back(q.y1);
                    break;
                }
            }
        }

        glGenBuffers(1, &t.vbo);

        glBindBuffer(GL_ARRAY_BUFFER, t.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * t.data.size(),
                     t.data.data(),
                     GL_STATIC_DRAW);

        return t;
    }

    void OpenGLFont::draw(float x, float y, const Text& t)
    {
        glPushMatrix();
        // glTranslatef(x, y, 0.0f);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindBuffer(GL_ARRAY_BUFFER, t.vbo);
        {
            glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 2, (const void*)0);
            glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, (const void*)(sizeof(float) * 2));

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);

            glDrawArrays(GL_QUADS, 0, t.data.size() / 4);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }

    void OpenGLFont::draw(float x, float y, const std::wstring& text) const
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Boo! Immediate mode!
        glBegin(GL_QUADS);
        for (int c : text)
        {
            // Find correct range for character
            const stbtt_pack_range* r = nullptr;
            for (const stbtt_pack_range& range : ranges)
            {
                if (c >= range.first_unicode_char_in_range &&
                    c <= range.first_unicode_char_in_range + range.num_chars_in_range)
                {
                    r = &range;

                    stbtt_aligned_quad q;
                    stbtt_GetPackedQuad(r->chardata_for_range,
                                        bitmapWidth, bitmapHeight,
                                        c - r->first_unicode_char_in_range,
                                        &x, &y, &q,
                                        1); //1=opengl & d3d10+,0=d3d9

                    glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
                    glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
                    glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
                    glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
                    break;
                }
            }
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void OpenGLFont::drawChar(float& x, float& y, const wchar_t c) const
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Boo! Immediate mode!
        glBegin(GL_QUADS);

        // Find correct range for character
        const stbtt_pack_range* r = nullptr;
        for (const stbtt_pack_range& range : ranges)
        {
            if (c >= range.first_unicode_char_in_range &&
                c <= range.first_unicode_char_in_range + range.num_chars_in_range)
            {
                r = &range;

                stbtt_aligned_quad q;
                stbtt_GetPackedQuad(r->chardata_for_range,
                                    bitmapWidth, bitmapHeight,
                                    c - r->first_unicode_char_in_range,
                                    &x, &y, &q,
                                    1); //1=opengl & d3d10+,0=d3d9

                glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
                glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
                glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
                glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
                break;
            }
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void OpenGLFont::makeOpenGLTexture()
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, bitmapWidth, bitmapHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &bitmap[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}
