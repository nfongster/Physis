#include "OpenGLEngine.h"
#include "TriangleManager.h"

OpenGLEngine::OpenGLEngine(const TimeConfig& config, EntityManager* entity_manager) 
    : m_entity_manager(entity_manager), EngineBase(config)
{
}

OpenGLEngine::~OpenGLEngine()
{
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithTriangles(const TimeConfig& config)
{
    return std::make_unique<OpenGLEngine>(config, new TriangleManager());
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

    m_entity_manager->Initialize(m_system_state);
}

bool OpenGLEngine::ContinueLoop()
{
    return !glfwWindowShouldClose(m_pWindow);//&& m_current_time < m_config.total_time;
}

void OpenGLEngine::OnCompletion()
{
    delete m_entity_manager;
    glfwTerminate();
}

void OpenGLEngine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_entity_manager->Render();
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
