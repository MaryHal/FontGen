FontGen
=======

C++11/Opengl - Simple font library

Build using:

    python bootstrap.py && ninja

Example:

    fgen::OpenGLFont font
    {
        "../bin/DroidSansFallback.ttf", // Filename
        {
            // Size, Unicode range pair
            { 20.0f, fgen::set::ascii },
            { 20.0f, fgen::set::jp::hiragana },
            { 20.0f, { 0x00A1, 0x00FF } }
        }
    };

    font.draw(16.0f, 16.0f, L"Hello World あおいえう");

TODO
====

- Allow creation of bitmap fonts by dumping font data.
- Mixed fonts maybe? Pulling characters from multiple TTF files.
- More built-in codepoint ranges / more integratable built-in ranges.
- Modernize OpenGL usage.
