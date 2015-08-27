#include <fontgen/OpenGLFont.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include <iostream>

class Application
{
    private:
        GLFWwindow* window;

    public:
        Application()
        {
            initGLFW();
            initOpenGL();
        }

        ~Application()
        {
            glfwTerminate();
        }

        void clear()
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void pollEvents()
        {
            glfwPollEvents();
        }

        void swapBuffers()
        {
            glfwSwapBuffers(window);
        }

        bool shouldWindowClose()
        {
            return glfwWindowShouldClose(window);
        }

    private:
        void initGLFW()
        {
            if (!glfwInit())
            {
                throw std::runtime_error{"Error initializing glfw."};;
            }

            glfwWindowHint(GLFW_RESIZABLE, false);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

            window = glfwCreateWindow(640, 480,
                                      "BlockTracker",
                                      nullptr, nullptr);

            glfwMakeContextCurrent(window);

            glewInit();

            if (GLEW_VERSION_1_3)
            {
                std::cout << "Supported!" << std::endl;
            }
        }

        void initOpenGL()
        {
            // OpenGL 2d perspective
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);

            // Initialize modelview matrix
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glDisable(GL_LIGHTING);
            glDisable(GL_DEPTH_TEST);

            glEnable(GL_LINE_SMOOTH);
            glLineWidth(2.0f);
        }
};

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    Application app;

    // int maxTexSize{};
    // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    // std::cout << maxTexSize << '\n';

    fgen::OpenGLFont font
    {
        "../bin/DroidSansFallback.ttf",
        {
            { 40.0f, fgen::charset::ascii }, // ASCII
            { 40.0f, fgen::charset::jp::hiragana }, // Hiragana
            { 40.0f, { 0x21E6, 0x21E9 } }, // Arrows
            { 40.0f, { 0x2190, 0x2193 } }  // Arrows 2
        },
        1024, 1024
    };

    font.writeBitmap("font.png");
    fgen::Text t = font.makeText(16.0f, 270.0f, L"Hello World!");

    for (int i = 0; i < 20; i++)
        std::cout << t.data.at(i) << std::endl;

    std::cout << "VBO index: \t" << t.vbo << std::endl <<
        "Element Count: \t" << t.data.size() << std::endl <<
        "Data Size: \t" << sizeof(float) * t.data.size() << std::endl;

    bool running = true;
    while (!app.shouldWindowClose() && running)
    {
        app.clear();

        font.draw(16.0f, 30.0f,  L"Aa Bb Cc あぁおぉいぃえぇうぅ");
        font.draw(16.0f, 70.0f,  L"abcdefghijklmnopqrstuvwxyz");
        font.draw(16.0f, 110.0f, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        font.draw(16.0f, 150.0f, L"1234567890!@#$%^&*()");
        font.draw(16.0f, 190.0f, L"⇦⇨⇧⇩");
        font.draw(16.0f, 230.0f, L"←↑→↓");
        font.draw(16.0f, 270.0f, t);

        app.swapBuffers();
        app.pollEvents();
    }

    return 0;
}
