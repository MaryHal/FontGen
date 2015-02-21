#include <fontgen/OpenGLFont.hpp>

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
            window = glfwCreateWindow(640, 480,
                                      "BlockTracker",
                                      nullptr, nullptr);

            glfwMakeContextCurrent(window);
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

    int maxTexSize{};
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << maxTexSize << '\n';

    fgen::OpenGLFont font
    {
        "../bin/DroidSansFallback.ttf",
        {
            { 20.0f, fgen::charset::ascii }, // ASCII
            { 20.0f, fgen::charset::jp::hiragana }, // Hiragana
            { 20.0f, { 0x00A1, 0x00FF } }
        }
    };

    font.writeBitmap("font.png");

    bool running = true;
    while (!app.shouldWindowClose() && running)
    {
        app.clear();

        font.draw(16.0f, 16.0f, L"Hello Worldあおいえう");

        app.swapBuffers();
        app.pollEvents();
    }

    return 0;
}
