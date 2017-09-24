#pragma once
#include "oxygine-framework.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

DECLARE_SMART(WorldActor, spWorldActor);

class WorldSimmulator
{
public:
	WorldSimmulator(const WorldActor & reference, std::function<void(WorldActor&, std::atomic<bool>&)> init);
	~WorldSimmulator();
	void Reset();
	void Reset(std::function<void(WorldActor&, std::atomic<bool>&)> init);

	WorldSimmulator(const WorldSimmulator & other) = delete;
	WorldSimmulator & operator=(const WorldSimmulator & other) = delete;

private:
	const WorldActor & m_worldReference;
	spWorldActor m_worldCopy;
	std::function<void(WorldActor&, std::atomic<bool>&)> m_initCopy;

	void MakePlainCopyOfWorldReference();
	void SimmulationCallback();

	std::thread m_simmulation;
	std::atomic<bool> m_simmulationRunning;
	std::atomic<bool> m_worldSimmulatorRunning;

	std::mutex m_simmulationLock;
	std::condition_variable m_simmulationCondition;
	bool m_simmulationReady = false;
};

class BodyActor;

class WorldSimmulatorHelper : public WorldSimmulator
{
public:
	WorldSimmulatorHelper(const WorldActor & reference);
	void ListenForFirstCollision(const BodyActor & actor, std::function<void(BodyActor&)> result);

};

