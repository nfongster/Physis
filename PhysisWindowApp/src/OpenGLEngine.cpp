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
    m_window = glfwCreateWindow(640, 480, "Physis Engine", NULL, NULL);

    if (!m_window)
        glfwTerminate();
	
    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    float positions[] = {
        0, 0.5,
        0.5, 0.25,
        0.5, -0.25,
        0, -0.5,
        -0.5, -0.25,
        -0.5, 0.25
    };

    unsigned int indices[] = {
        0, 1, 5,
        5, 1, 2,
        2, 3, 4,
        2, 4, 5
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * POS_COORDS * sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_TRIANGLES * NUM_TRIANGLE_CORNERS * sizeof(int), indices, GL_STATIC_DRAW);
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
    glDrawElements(GL_TRIANGLES, NUM_TRIANGLES * NUM_TRIANGLE_CORNERS, GL_UNSIGNED_INT, nullptr);
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
