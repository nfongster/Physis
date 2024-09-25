#include "BenchmarkEngine.h"

/// <summary>
/// Simple engine for benchmarking.
/// </summary>
/// <param name="sc">System configuration.</param>
/// <param name="outdir">Directory to write results.</param>
/// <param name="render_time">Time to pause (ms) during the render phase.</param>
BenchmarkEngine::BenchmarkEngine(const TimeConfig& sc, const std::string& outdir, 
	const std::chrono::duration<double, std::milli>& render_time)
	: EngineBase(sc), m_outdir(outdir), m_render_time(render_time)
{
}

BenchmarkEngine::~BenchmarkEngine()
{
}

void BenchmarkEngine::OnCompletion()
{
	this->m_durations.push_back(m_duration);

	// Stability
	{
		std::stringstream ss;
		ss << m_outdir << "stability.txt";

		std::ofstream file;
		file.open(ss.str());
		std::string buffer;
		int reservedBytes = 1 * 1024 * 1024;
		buffer.reserve(reservedBytes);

		for (auto duration : this->m_durations)
		{
			if (buffer.size() >= reservedBytes)
			{
				file.write(buffer.c_str(), buffer.size());
				buffer.clear();
			}
			buffer.append(std::to_string(duration.count()));
			buffer.append("\n");
		}
		if (!buffer.empty())
			file.write(buffer.c_str(), buffer.size());

		file.close();
	}

	// Trajectory
	{
		std::stringstream ss;
		ss << m_outdir << "trajectory.txt";

		std::ofstream file;
		file.open(ss.str());
		std::string buffer;
		int reservedBytes = 1 * 1024 * 1024;
		buffer.reserve(reservedBytes);

		for (const auto& pair : this->m_history)  // for each particle
		{
			const unsigned int id = pair.first;
			std::vector<InitialConditions> history = pair.second;
			if (buffer.size() >= reservedBytes)
			{
				file.write(buffer.c_str(), buffer.size());
				buffer.clear();
			}
			for (const auto& params : history) // for each timestamp
			{
				std::stringstream sub_ss;
				sub_ss << params.r << '\t' << params.v << '\t' << params.a << '\n';
				buffer.append(sub_ss.str());
			}
		}
		if (!buffer.empty())
			file.write(buffer.c_str(), buffer.size());

		file.close();
	}
}

void BenchmarkEngine::Render()
{
	// TODO: Multiply render time by num particles?
	std::this_thread::sleep_for(m_render_time);
	this->m_durations.push_back(m_duration);

	// Store particle positions
	auto pmap = this->m_system_state->GetCurrent()->GetParticles();
	for (const auto& pair : pmap)
	{
		const unsigned int id = pair.first;
		std::shared_ptr<Particle> p = pair.second;
		this->m_history[id].push_back(InitialConditions(p->GetPosition(), p->GetVelocity(), p->GetAcceleration()));
	}
}

void BenchmarkEngine::Interpolate(const double& factor)
{
	// TODO: Include or override base class interpolate?
}

void BenchmarkEngine::AddParticle()
{
	m_system_state->AddParticle(InitialConditions());
}

void BenchmarkEngine::AddParticle(const InitialConditions& ic)
{
	m_system_state->AddParticle(ic);
}
