#include <Font.hpp>

#include <stb_truetype.h>

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    
    Font font1{"DroidSansFallback.ttf", Font::jp};
    
    Font font2{"DroidSansFallback.ttf",
        {
            { 16.0f, 32, 95 }, // ASCII
            { 60.0f, 0x3260, 0x327B - 0x3260 } // Circled Hangul???
        }
    };

    font1.writeBitmap("DroidSans01.png");
    font2.writeBitmap("DroidSans02.png");

    // const auto& pdata = font.getpdata();
    // for (int i = 0; i < 256; ++i)
    // {
    //     std::cout << i << '\t' << pdata[i].x0 << std::endl;
    // }

    return 0;
}
