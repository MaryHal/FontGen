FontGen
=======

C++11/Opengl - Simple font library

Build using:

    python bootstrap.py && ninja

Example:

    fgen::OpenGLFont font
    {
        "../bin/DroidSansFallback.ttf",
        {
            { 20.0f, 32, 95 }, // ASCII
            { 20.0f, 0x3041, 0x3096 - 0x3041 } // Hiragana
        }
    };
    font.draw(16.0f, 16.0f, L"Hello World あおいえう");

TODO
====

- Allow creation of bitmap fonts by dumping font data.
- More built-in codepoint ranges / more integratable built-in ranges.
- Modernize OpenGL usage.
