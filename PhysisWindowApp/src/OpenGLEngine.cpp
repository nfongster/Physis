#include "OpenGLEngine.h"
#include "TriangleManager.h"

OpenGLEngine::OpenGLEngine(const SystemConfig& sc, EntityManager* entity_manager) 
    : m_entity_manager(entity_manager), EngineBase(sc)
{
}

OpenGLEngine::~OpenGLEngine()
{
	delete m_system;
    delete m_system_prev_state;
}

std::unique_ptr<OpenGLEngine> OpenGLEngine::WithTriangles(const SystemConfig& sc)
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

    m_entity_manager->Initialize(m_system);

    // TODO: Create a deep copy constructor
    m_system_prev_state = new ParticleSystem();
    for (const auto& pair : m_system->GetParticles())
    {
        m_system_prev_state->Add(InitialConditions(
            pair.second->GetPosition(),
            pair.second->GetVelocity(),
            pair.second->GetAcceleration()));
    }
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
    for (const auto& pair : m_system->GetParticles())
    {
        unsigned int index = pair.first;
        std::shared_ptr<Particle> particle = pair.second;
        InitialConditions ic = InitialConditions(particle->GetPosition(), particle->GetVelocity(), particle->GetAcceleration());
        m_system_prev_state->Update(index, ic);
    }
    m_system->Step(dt);
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
    for (auto pair : m_system->GetParticles())
    {
        unsigned int i = pair.first;
        std::shared_ptr<Particle> p = pair.second;
        p->Interpolate((*m_system_prev_state)[i], factor);
    }
}

void OpenGLEngine::AddParticle()
{
	m_system->Add(InitialConditions());
}

void OpenGLEngine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}
