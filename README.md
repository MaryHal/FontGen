FontGen [![Build Status](https://travis-ci.org/sanford1/FontGen.svg?branch=master)](https://travis-ci.org/sanford1/FontGen)
=======

C++11/Opengl - Simple font library

Build using:

    python bootstrap.py && ninja

Example:

    fgen::OpenGLFont font
    {
        "../bin/DroidSansFallback.ttf", // Filename
        {
            // Size, Unicode range pair (inclusive)
            { 20.0f, fgen::charset::ascii },
            { 20.0f, fgen::charset::jp::hiragana },
            { 20.0f, { 0x00A1, 0x00FF } }
        }
    };

    font.draw(16.0f, 16.0f, L"Hello World あおいえう");

Generated Bitmap:

![Build Status](https://raw.githubusercontent.com/sanford1/FontGen/master/font01.png)

TODO
====

- Allow creation of bitmap fonts by dumping font data.
- Mixed fonts maybe? Pulling characters from multiple TTF files.
- More built-in codepoint ranges / more integratable built-in ranges.
- Modernize OpenGL usage.
