#include "CircleManager.h"

CircleManager::CircleManager(const unsigned int num_segments) : m_num_segments(num_segments)
{
}

CircleManager::~CircleManager()
{
	glDeleteProgram(m_shader_id);
}

void CircleManager::Initialize(std::shared_ptr<SystemState>& system_state)
{
    std::vector<int> indices;
    for (int i = 0; i <= m_num_segments + 1; i++)
        indices.push_back(i);

    // TODO: Hack!
    unsigned int indices_arr[10] = {
        indices[0], indices[1], indices[2], indices[3], indices[4], indices[5], indices[6], indices[7], indices[8], indices[9]
    };
	
    glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_num_segments + 2) * sizeof(int), &indices_arr, GL_STATIC_DRAW);

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
        for (int i = 0; i <= m_num_segments; i++)
        {
            float radians = 2.0f * M_PI * (i * 1.0f / m_num_segments);
            float x_coord = r * std::cos(radians);
            float y_coord = r * std::sin(radians);
            pos.push_back(center_x + x_coord);
            pos.push_back(center_y + y_coord);
        }

        // TODO: Hack!
        float pos_arr[(9+1)*2] = {
            pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], pos[6], pos[7], pos[8],
            pos[9], pos[10], pos[11], pos[12], pos[13], pos[14], pos[15], pos[16], pos[17], pos[18], pos[19]
        };

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (m_num_segments + 2) * POS_COORDS * sizeof(float), pos_arr, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

        m_vao_map.emplace(vao, p);
    }

    auto builder = ShaderBuilder("C:\\Code\\Physis\\PhysisWindowApp\\resource\\shaders\\");
    m_shader_id = builder.Build();
    glUseProgram(m_shader_id);
    glGetUniformLocation(m_u_position_id, "u_Position");
}

void CircleManager::Render()
{
    for (const auto& p : m_vao_map)
    {
        Vec2 pos = p.second->GetPosition();
        glUniform2f(m_u_position_id, pos.X, pos.Y);
        glBindVertexArray(p.first);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glDrawElements(GL_TRIANGLE_FAN, m_num_segments + 2, GL_UNSIGNED_INT, nullptr);
    }
}
