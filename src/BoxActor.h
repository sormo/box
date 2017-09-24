#pragma once
#include "primitives\Polyline.h"
#include "Box2D\Box2D.h"
#include "common.h"
#include "BodyActor.h"
#include "Shape.h"

#define BOX_SIZE 15.0f

DECLARE_SMART(BoxActor, spBoxActor);
class BoxActor : public BodyActor
{
public:
	enum class BoxType
	{
		single,
		knight,
		plus,
		tee,
		zee,

		count
	};

	BoxActor(WorldActor * world, const oxygine::Vector2& pos, float rotation = 0.0f, int id = 0);
	BoxActor(WorldActor * world, BoxType type, const oxygine::Vector2& pos, float rotation = 0.0f, int id = 0);
	virtual ~BoxActor();

	void SetFillColor(const oxygine::Color & color);
	const oxygine::Color & GetFillColor();

	void SetGhost(bool set);

	BoxType GetBoxType();

	static oxygine::spActor GetBoxActor(BoxType & type);

protected:
	void AddShapesWithData(const std::vector<std::string> & data);

	virtual void doUpdate(const oxygine::UpdateState& us) override;
	virtual void BeginContact(BodyActor * other) override;

	bool m_isDestructed = false;
	const BoxType m_boxType;
};
