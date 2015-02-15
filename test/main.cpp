#include <Font.hpp>

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
        fgen::Font font{fontFile, fgen::Font::jp};
        font.writeBitmap("font01.png");
        std::cout << "<<< JP bitmap created\n";
    }

    std::cout << '\n';

    {
        std::cout << ">>> Creating Mixed bitmap\n";
        fgen::Font font{fontFile,
            {
                { 20.0f, 32, 95 }, // ASCII
                { 54.0f, 0x3260, 0x327B - 0x3260 } // Circled Hangul???
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
                { 14.0f, 0x4E00, /*0x4E35 - 0x4E00*/ 512 }
            }
        };
        font.writeBitmap("font03.png");
        std::cout << "<<< CJK-unified bitmap created\n";
    }

    return 0;
}
