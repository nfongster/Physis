#include "Circle.h"

Circle::Circle(const unsigned int num_segments) : m_num_vertices(num_segments)
{
}

Circle::~Circle()
{
	glDeleteProgram(m_shader);
}

void Circle::Initialize(std::shared_ptr<SystemState>& system_state)
{
    // The first index is the center; the remaining indices form the perimeter points.
    std::vector<int> indices;
    for (int i = 0; i <= m_num_vertices + 1; i++)
        indices.push_back(i);
	
    // Vertices + 2, because we need to include (1) the center, and (2) the start/end perimeter point.
    glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_num_vertices + 2) * sizeof(int), &indices[0], GL_STATIC_DRAW);

    float r = 0.015;  // radius

    for (const auto& pair : system_state->GetCurrent()->GetParticles())
    {
        std::shared_ptr<Particle> p = pair.second;
        std::vector<float> pos;
        float center_x = p->GetPosition().X;
        float center_y = p->GetPosition().Y;

        // First position is the center of the fan
        pos.push_back(center_x);
        pos.push_back(center_y);

        // The fan always begins on the +x axis at 0 degrees.
        for (int i = 0; i <= m_num_vertices; i++)
        {
            float radians = 2.0f * M_PI * (i * 1.0f / m_num_vertices);
            float x_coord = r * std::cos(radians);
            float y_coord = r * std::sin(radians);
            pos.push_back(center_x + x_coord);
            pos.push_back(center_y + y_coord);
        }

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (m_num_vertices + 2) * POS_COORDS * sizeof(float), &pos[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

        m_vao_map.emplace(vao, p);
    }

    auto builder = ShaderBuilder("C:\\Code\\Physis\\PhysisWindowApp\\resource\\shaders\\particle\\");
    m_shader = builder.Build();
    glGetUniformLocation(m_u_position_id, "u_Position");
}

void Circle::Render()
{
    glUseProgram(m_shader);
    for (const auto& p : m_vao_map)
    {
        Vec2 pos = p.second->GetPosition();
        glUniform2f(m_u_position_id, pos.X, pos.Y);
        glBindVertexArray(p.first);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glDrawElements(GL_TRIANGLE_FAN, m_num_vertices + 2, GL_UNSIGNED_INT, nullptr);
    }
}
