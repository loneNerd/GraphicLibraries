#include "opengl.h"

#include <iostream>
#include <sstream>

#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "utils.h"
#include "widgets/fps_counter.h"
#include "windows/sdl2.h"

using namespace GraphicLibraries::Engines;
using namespace GraphicLibraries::Widgets;
using namespace GraphicLibraries::Windows;

OpenGL::OpenGL()
    : m_window { nullptr },
      m_fpsCounter { nullptr },
      m_context { NULL },
      m_shaderProgram { 0u },
      m_vao { 0u },
      m_vbo { 0u }
{
    m_isInit = false;
}

OpenGL::~OpenGL()
{
    std::cout << "OpenGL Destructor" << std::endl;

    if (m_isInit)
        release();
}

void OpenGL::init()
{
    std::cout << "Initializing OpenGL" << std::endl;

    m_window = new SDL2Window;

    if (!m_window)
        throw std::exception("OPENGL: Can't create sdl2 window");

    m_window->initForOpenGL();
    m_window->setTitle("OpenGL");

    m_context = SDL_GL_CreateContext(m_window->getWindow());

    if (m_context == NULL)
        throw std::exception("OPENGL: Can't create SDL context");

    SDL_GL_MakeCurrent(m_window->getWindow(), m_context);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();

    if (glewStatus != 0)
        throw std::exception(std::string("OPENGL: ").append(reinterpret_cast<const char*>(glewGetErrorString(glewStatus))).c_str());

    compileShaders();

    // create a triangle using the Vertex struct
    Vertex triangle[] =
    {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position)));
    glEnableVertexAttribArray(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window->getWindow(), m_context);
    ImGui_ImplOpenGL3_Init(m_glslVersion);

    m_fpsCounter = new FpsCounter;

    if (!m_fpsCounter)
        throw std::exception("OPENGL: Can't initialize FPS Counter widget");

    m_isInit = true;
}

void OpenGL::release()
{
    std::cout << "Releasing OpenGL" << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_fpsCounter)
    {
        delete m_fpsCounter;
        m_fpsCounter = nullptr;
    }

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_shaderProgram);

    SDL_GL_DeleteContext(m_context);

    if (m_window)
    {
        m_window->release();
        delete m_window;
        m_window = nullptr;
    }

    m_isInit = false;
}

void OpenGL::newFrame()
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplSDL2_ProcessEvent(&m_window->getEvent());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    m_fpsCounter->draw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_window->getWindow());
}

void OpenGL::compileShaders()
{
    const char* tmp;

    // Retrive vertex source code
    std::string vertexCode;
    std::ifstream vertexFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexFile.open(getFileFullPath(L"shaders\\vertex.vert"));
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    vertexFile.close();
    vertexCode = vertexStream.str();

    // Compile vertex shader
    unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
    tmp = vertexCode.c_str();
    glShaderSource(vertex, 1, &tmp, nullptr);
    glCompileShader(vertex);
    checkError(vertex, "Can't compile vertex shader");

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertex);

    glDeleteShader(vertex);
    checkError(m_shaderProgram, "Can't execute shader program");

    // Retrive fragment source code
    std::string fragmentCode;
    std::ifstream fragmentFile;

    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.open(getFileFullPath(L"shaders\\fragment.frag"));
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    fragmentFile.close();
    fragmentCode = fragmentStream.str();

    // Compile fragment shader
    unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);
    tmp = fragmentCode.c_str();
    glShaderSource(fragment, 1, &tmp, nullptr);
    glCompileShader(fragment);
    checkError(fragment, "Can't compile fragment shader");

    glAttachShader(m_shaderProgram, fragment);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(fragment);

    checkError(m_shaderProgram, "Can't execute shader program");
}
