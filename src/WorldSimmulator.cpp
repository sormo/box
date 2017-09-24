#include "WorldSimmulator.h"
#include "WorldActor.h"
#include "BodyActor.h"

WorldSimmulator::WorldSimmulator(const WorldActor & reference, std::function<void(WorldActor&, std::atomic<bool>&)> init)
	: m_worldReference(reference), m_initCopy(init), m_worldCopy(new WorldActor),
		m_simmulationRunning(true), m_worldSimmulatorRunning(true)
{
	m_simmulation = std::thread([this]() { SimmulationCallback(); });

	Reset();
}

WorldSimmulator::~WorldSimmulator()
{
	m_worldSimmulatorRunning = false;
	{
		std::lock_guard<std::mutex> lock(m_simmulationLock);
		m_simmulationReady = true;
	}
	m_simmulationCondition.notify_one();
	
	if (m_simmulation.joinable())
		m_simmulation.join();
}

void WorldSimmulator::SimmulationCallback()
{
	oxygine::UpdateState state;
	oxygine::Clock clock;
	oxygine::timeMS dt = 17; // constants milliseconds per frame
	
	while (m_worldSimmulatorRunning)
	{
		std::unique_lock<std::mutex> lock(m_simmulationLock);
		m_simmulationCondition.wait(lock, [this] { return m_simmulationReady; });
		m_simmulationReady = false;

		while (m_worldSimmulatorRunning && m_simmulationRunning)
		{
			state.dt = dt;
			state.time = clock.getTime();

			m_worldCopy->update(state);

			//dt = clock.doTick();
			state.iteration += 1;
		}
	}
}

void WorldSimmulator::MakePlainCopyOfWorldReference()
{
	m_worldCopy->Clear();
	m_worldCopy->LoadWorld(m_worldReference.SaveWorld());
}

void WorldSimmulator::Reset()
{
	m_simmulationRunning = false;
	std::lock_guard<std::mutex> lock(m_simmulationLock);

	MakePlainCopyOfWorldReference();

	auto reference = DumpDebugData(m_worldReference);
	auto copy = DumpDebugData(*m_worldCopy);

	if (m_initCopy)
	{
		m_initCopy(*m_worldCopy, m_simmulationRunning);

		m_simmulationReady = true;
		m_simmulationRunning = true;
		m_simmulationCondition.notify_one();
	}
}


void WorldSimmulator::Reset(std::function<void(WorldActor&, std::atomic<bool>&)> init)
{
	m_initCopy = init;
	Reset();
}

// ---

WorldSimmulatorHelper::WorldSimmulatorHelper(const WorldActor & reference)
	: WorldSimmulator(reference, nullptr)
{

}

void WorldSimmulatorHelper::ListenForFirstCollision(const BodyActor & actor, std::function<void(BodyActor&)> result)
{
	Reset([&actor, &result](WorldActor & world, std::atomic<bool> & running) 
	{
		// find actor in copied world (it is not ghost)
		BodyActor * copyActor = world.GetActor(actor.GetId());
		// register for collision of that actor, callback call result and set running
		world.GetContactManager().Register(copyActor, [copyActor, &result, &running](BodyActor*) {
			result(*copyActor);
			running = false;
		}, nullptr);
	});
}
