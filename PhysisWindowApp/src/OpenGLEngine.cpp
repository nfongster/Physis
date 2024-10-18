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
    return std::make_unique<OpenGLEngine>(config, new Environment(new Triangle(), new Box()));
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithCircles(const TimeConfig& config, const int& num_segments)
{
    return std::make_unique<OpenGLEngine>(config, new Environment(new Circle(num_segments), new Box()));
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

void OpenGLEngine::AddBoundary(const Boundary& boundary)
{
    m_system_state->AddBoundary(boundary);
}

void OpenGLEngine::AddParticle(const KinematicParameters& parameters)
{
    m_system_state->AddParticle(parameters);
}
