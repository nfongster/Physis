#include "OpenGLEngine.h"

OpenGLEngine::OpenGLEngine() : EngineBase(SystemConfig())
{
}

OpenGLEngine::~OpenGLEngine()
{
	delete m_system;
}

void OpenGLEngine::OnStartup()
{
    // TODO: window ptr should be a class member field
    GLFWwindow* window;
    glfwInit();
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
		glfwTerminate();
    glfwMakeContextCurrent(window);
    // TODO: glfwWindowShouldClose should go into LoopCondition overridable.  e.g.,
    // while (!glfwWindowShouldClose(window) && t < total_time)
    while (!glfwWindowShouldClose(window))
    {
        // TODO: Below goes into Update method, along w/ m_system->Step(dt);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // TODO: This should go into OnCompletion
    glfwTerminate();
}

void OpenGLEngine::OnCompletion()
{
}

void OpenGLEngine::Update(const double& dt)
{
	m_system->Step(dt);
}

void OpenGLEngine::Render()
{
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
