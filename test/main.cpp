#include <fontgen/Font.hpp>

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Give me a ttf file!\nUsage: ./FontGen <fontfile>\n";
        return -1;
    }
    std::string fontFile = argv[1];

    {
        std::cout << ">>> Creating JP bitmap\n";
        fgen::Font font{fontFile,
            {
                { 20.0f, fgen::charset::ascii },
                { 20.0f, fgen::charset::jp::hiragana },
                { 20.0f, fgen::charset::jp::katakana },
                { 20.0f, fgen::charset::jp::punct }
            }
        };

        font.writeBitmap("font01.png");
        std::cout << "<<< JP bitmap created\n";
    }

    std::cout << '\n';

    {
        std::cout << ">>> Creating Mixed bitmap\n";
        fgen::Font font{fontFile,
            {
                { 20.0f, fgen::charset::ascii }, // ASCII
                { 54.0f, { 0x3260, 0x327B } } // Circled Hangul???
            }
        };
        font.writeBitmap("font02.png");
        std::cout << "<<< Mixed bitmap created\n";
    }

    std::cout << '\n';

    {
        std::cout << ">>> Creating CJK-unified bitmap\n";
        fgen::Font font{fontFile,
            {
                /* { 14.0f, { 0x4E00, 0x4E00 + 20000 } } */
                { 14.0f, { 0x4E00, 0x4E00 + 1000 } }
            },
            /* 4096, 4096 */
            1024, 1024
        };
        font.writeBitmap("font03.png");
        std::cout << "<<< CJK-unified bitmap created\n";
    }

    std::cout << '\n';

    {
        std::cout << ">>> Creating miscellaneous bitmap\n";
        fgen::Font font{fontFile,
            {
                { 30.0f, { 0x2700, 0x27BF } }
            }
        };
        font.writeBitmap("font04.png");
        std::cout << "<<< Miscellaneous bitmap created\n";
    }

    return 0;
}
