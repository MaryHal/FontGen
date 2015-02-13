#include <Font.hpp>

#include <stb_truetype.h>

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

    std::cout << ">>> Creating JP bitmap" << std::endl;
    {
        Font font1{fontFile, Font::jp};
        font1.writeBitmap("font01.png");
    }
    std::cout << "<<< JP bitmap created" << std::endl;

    std::cout << ">>> Creating Mixed bitmap" << std::endl;
    {
        Font font2{fontFile,
            {
                { 27.0f, 32, 95 }, // ASCII
                { 54.0f, 0x1100, 0x1112 - 0x1100 } // Circled Hangul???
            }
        };
        font2.writeBitmap("font02.png");
    }
    std::cout << "<<< Mixed bitmap created" << std::endl;

    std::cout << ">>> Creating CJK bitmap" << std::endl;
    {
        Font font3{fontFile,
            {
                { 20.0f, 0x4E00, /*0x4E35 - 0x4E00*/ 200 }
            }
        };
        font3.writeBitmap("font03.png");
    }
    std::cout << "<<< CJK bitmap created" << std::endl;

    return 0;
}
