#include "OpenGLEngine.h"
#include "TriangleManager.h"

OpenGLEngine::OpenGLEngine(const TimeConfig& sc, EntityManager* entity_manager) 
    : m_entity_manager(entity_manager), EngineBase(sc)
{
}

OpenGLEngine::~OpenGLEngine()
{
    delete m_system_state;
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithTriangles(const TimeConfig& sc)
{
    return std::make_unique<OpenGLEngine>(sc, new TriangleManager());
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

void OpenGLEngine::Update(const double& dt)
{
    for (const auto& pair : m_system_state->GetCurrent()->GetParticles())
    {
        unsigned int index = pair.first;
        std::shared_ptr<Particle> particle = pair.second;
        InitialConditions ic = InitialConditions(particle->GetPosition(), particle->GetVelocity(), particle->GetAcceleration());
        m_system_state->GetPrevious()->Update(index, ic);
    }
    m_system_state->GetCurrent()->Step(dt);
}

void OpenGLEngine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_entity_manager->Render();
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}

void OpenGLEngine::Interpolate(const double& factor)
{
    for (auto pair : m_system_state->GetCurrent()->GetParticles())
    {
        unsigned int i = pair.first;
        std::shared_ptr<Particle> p = pair.second;
        p->Interpolate((*m_system_state->GetPrevious())[i], factor);
    }
}

void OpenGLEngine::AddParticle()
{
    m_system_state->GetCurrent()->Add(InitialConditions());
}

void OpenGLEngine::AddParticle(const InitialConditions& ic)
{
    m_system_state->GetCurrent()->Add(ic);
}
