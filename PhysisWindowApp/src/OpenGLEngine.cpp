#include "OpenGLEngine.h"

OpenGLEngine::OpenGLEngine(const TimeConfig& config, Environment* environment) 
    : m_environment(environment), EngineBase(config)
{
}

OpenGLEngine::~OpenGLEngine()
{
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithTriangles(const TimeConfig& config)
{
    float height = 1.99f;
    float width = 1.99f;
    return std::make_unique<OpenGLEngine>(config, new Environment(new Triangle(), new Box(height, width)));
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithCircles(const TimeConfig& config, const int& num_segments)
{
    float height = 1.99f;
    float width = 1.99f;
    return std::make_unique<OpenGLEngine>(config, new Environment(new Circle(num_segments), new Box(height, width)));
}

void OpenGLEngine::OnStartup()
{    
    glfwInit();
    m_pWindow = glfwCreateWindow(1200, 1000, "Physis Engine", NULL, NULL);

    if (!m_pWindow)
        glfwTerminate();
	
    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    m_environment->Initialize(m_system_state);
}

bool OpenGLEngine::ContinueLoop()
{
    return EngineBase::ContinueLoop() && !glfwWindowShouldClose(m_pWindow);
}

void OpenGLEngine::OnCompletion()
{
    delete m_environment;
    glfwTerminate();
}

void OpenGLEngine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_environment->Render();
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}

void OpenGLEngine::AddParticle()
{
    m_system_state->AddParticle(KinematicParameters());
}

void OpenGLEngine::AddParticle(const KinematicParameters& parameters)
{
    m_system_state->AddParticle(parameters);
}
