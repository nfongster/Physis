#include "OpenGLEngine.h"

OpenGLEngine::OpenGLEngine(const SystemConfig& sc) : EngineBase(sc)
{
}

OpenGLEngine::~OpenGLEngine()
{
	delete m_system;
}

void OpenGLEngine::OnStartup()
{    
    glfwInit();
    m_window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!m_window)
        glfwTerminate();
	
    glfwMakeContextCurrent(m_window);
}

bool OpenGLEngine::ContinueLoop()
{
    return !glfwWindowShouldClose(m_window);//&& m_current_time < m_config.total_time;
}

void OpenGLEngine::OnCompletion()
{
    glfwTerminate();
}

void OpenGLEngine::Update(const double& dt)
{
	m_system->Step(dt);
}

void OpenGLEngine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGLEngine::Interpolate(const double& factor)
{
	// Interpolate remaining accumulator time
}

void OpenGLEngine::AddParticle()
{
	m_system->Add(InitialConditions());
}

void OpenGLEngine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}
