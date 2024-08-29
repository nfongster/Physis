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
    m_pWindow = glfwCreateWindow(1200, 1000, "Physis Engine", NULL, NULL);

    if (!m_pWindow)
        glfwTerminate();
	
    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    int num_particles = m_system->GetParticles().size();
    unsigned int indices[] = {
        0, 1, 2
    };
    
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_TRIANGLES * NUM_TRIANGLE_CORNERS * sizeof(int), indices, GL_STATIC_DRAW);

    // get initial position
    float l = 0.03;  // side length of equilateral triangle
    float c = (l / 2.0f) * (std::sqrt(3) / 3.0f);  // vertical distance to base
    
    for (auto p : m_system->GetParticles())
    {
        float pos[2 * 3];  // 2 coords * 3 vertices
        // todo: initalize separately
        // todo: create normalizer class
        float center_x = p->GetPosition().X;
        float center_y = p->GetPosition().Y;
        // vertex 1
        pos[0] = center_x;
        pos[1] = center_y + c;
        // vertex 2
        pos[2] = center_x - (l / 2);
        pos[3] = center_y - c;
        // vertex 3
        pos[4] = center_x + (l / 2);
        pos[5] = center_y - c;

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * POS_COORDS * sizeof(float), pos, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

        m_vao_map.emplace(vao, p);
    }

    auto builder = ShaderBuilder("C:\\Code\\Physis\\PhysisWindowApp\\resource\\shaders\\");
    m_shader_id = builder.Build();
    glUseProgram(m_shader_id);
    glGetUniformLocation(m_u_position_id, "u_Position");
}

bool OpenGLEngine::ContinueLoop()
{
    return !glfwWindowShouldClose(m_pWindow);//&& m_current_time < m_config.total_time;
}

void OpenGLEngine::OnCompletion()
{
    glDeleteProgram(m_shader_id);
    glfwTerminate();
}

void OpenGLEngine::Update(const double& dt)
{
	m_system->Step(dt);
}

void OpenGLEngine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& p : m_vao_map)
    {
        Vec2 pos = p.second->GetPosition();
        glUniform2f(m_u_position_id, pos.X, pos.Y);
        glBindVertexArray(p.first);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glDrawElements(GL_TRIANGLES, NUM_TRIANGLES * NUM_TRIANGLE_CORNERS, GL_UNSIGNED_INT, nullptr);
    }
    
    glfwSwapBuffers(m_pWindow);
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
