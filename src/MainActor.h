#pragma once

#include "oxygine-framework.h"
#include <map>
#include <bitset>
#include <mutex>

#define MAXIMUM_SCALE 300000.0f

class MainActor : public oxygine::Actor
{
public:
	MainActor();

	void followChild(oxygine::spActor child);
	void enableMove(bool enable);
	void enableZoom(bool enable);

	void setPositionFromLocal(const oxygine::Vector2 & localPosition);

	oxygine::Vector2 localFromChild(oxygine::spActor & child);
	oxygine::Vector2 localFromStage(const oxygine::Vector2 & position);

	void setScaleCamera(float scale);
	float getScaleCamera();
	void setMaximumScale(float scale);
	void registerOnScaleChange(void * key, std::function<void(float)> cbk);
	void unregisterOnScaleChange(void * key);

private:
	float const m_zoomFactor = 1.1f;

	void OnWheelUp(oxygine::Event* event);
	void OnWheelDown(oxygine::Event* event);
	void OnTouchDown(oxygine::Event* event);
	void OnTouchUp(oxygine::Event* event);
	void OnMove(oxygine::Event* event);
	
	void Zoom(oxygine::Event* event, float factor);
	
	oxygine::Vector2 m_position;
	float m_maximumScale = MAXIMUM_SCALE;
	
	enum class EnableBit { move = 0, zoom };
	std::bitset<2> m_enable;

	struct Touch
	{
		oxygine::Vector2 current;
		oxygine::Vector2 previous;
	};
	std::map<oxygine::pointer_index, Touch> m_touches;

	void ZoomMulti(const Touch & p1, const Touch & p2);

	virtual void update(const oxygine::UpdateState & us) override;

	void UpdateFollowPosition();
	oxygine::spActor m_follow;
	std::mutex m_followLock;

	void NotifyOnScaleChange();
	std::map<void*, std::function<void(float)>> m_onScaleChangeRegistrations;
};

typedef oxygine::intrusive_ptr<MainActor> spMainActor;

// global instance of main actor
extern spMainActor g_mainActor;
