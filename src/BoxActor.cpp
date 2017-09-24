#include "BoxActor.h"
#include "WorldActor.h"

#include <vector>
#include <string>

#define ALPHA_DECREASE 5

struct BoxType
{
	BoxActor::BoxType type;
	std::vector<std::string> data;
};

static const BoxType BOX_TYPES[] = 
{
	{
		BoxActor::BoxType::single,
		{
			"1"
		}
	},
	{
		BoxActor::BoxType::knight,
		{
			"001",
			"001",
			"111",
		}
	},
	{
		BoxActor::BoxType::plus,
		{
			"010",
			"111",
			"010"
		}
	},
	{
		BoxActor::BoxType::tee,
		{
			"111",
			"010",
			"010"
		}
	},
	{
		BoxActor::BoxType::zee,
		{
			"011",
			"010",
			"110"
		}
	}
};

const std::vector<std::string> * GetDataForBoxType(BoxActor::BoxType type)
{
	for (size_t i = 0; i < COUNTOF(BOX_TYPES); ++i)
	{
		if (BOX_TYPES[i].type == type)
			return &BOX_TYPES[i].data;
	}
	return nullptr;
}

BoxActor::BoxActor(WorldActor * world, const oxygine::Vector2& pos, float rotation, int id)
	: BodyActor(world, pos, rotation, false, Type::box, id), m_boxType(BoxType::single)
{
	spRectangleShape shape = new RectangleShape(BOX_SIZE, BOX_SIZE);
	AddShape(shape.get());
	m_shapes.push_back(shape.get());

	SetPosition(getPosition());
	SetRotation(rotation);

	m_world->GetContactManager().Register(this);
}

BoxActor::BoxActor(WorldActor * world, BoxType type, const oxygine::Vector2& pos, float rotation, int id)
	: BodyActor(world, pos, rotation, false, Type::box, id), m_boxType(type)
{
	auto data = GetDataForBoxType(type);
	assert(data);

	AddShapesWithData(*data);
	SetPosition(getPosition());
	SetRotation(rotation);

	m_world->GetContactManager().Register(this);
}

BoxActor::~BoxActor()
{

}

void BoxActor::AddShapesWithData(const std::vector<std::string> & data)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		for (size_t j = 0; j < data[i].size(); ++j)
		{
			if (data[i][j] == '1')
			{
				spRectangleShape shape = new RectangleShape({ 2.0f*i*BOX_SIZE, 2.0f*j*BOX_SIZE }, BOX_SIZE, BOX_SIZE);
				AddShape(shape.get());
			}
		}
	}
}

void BoxActor::BeginContact(BodyActor * other)
{
	BoxActor * box = dynamic_cast<BoxActor*>(other);
	if (!box)
		return;

	if (box->GetFillColor().r == GetFillColor().r &&
		box->GetFillColor().g == GetFillColor().g &&
		box->GetFillColor().b == GetFillColor().b)
		m_isDestructed = true;
}

void BoxActor::doUpdate(const oxygine::UpdateState & us)
{
	if (m_isDestructed)
	{
		if (GetFillColor().a <= ALPHA_DECREASE)
		{
			detach();
			return;
		}

		SetFillColor(oxygine::Color{ GetFillColor().r, GetFillColor().g,
			GetFillColor().b, (unsigned char)(GetFillColor().a - ALPHA_DECREASE) });
	}
}

void BoxActor::SetFillColor(const oxygine::Color & color)
{
	std::for_each(std::begin(m_shapes), std::end(m_shapes), [&color](spShape & s) { s->SetFillColor(color); });
}

const oxygine::Color & BoxActor::GetFillColor()
{
	assert(!m_shapes.empty());
	return m_shapes[0]->GetFillColor();
}

void BoxActor::SetGhost(bool set)
{
	m_body->SetActive(!set);

	oxygine::Color fillColor = GetFillColor();
	if (set)
		fillColor.a = 60;
	else
		fillColor.a = 255;
	SetFillColor(fillColor);
}

BoxActor::BoxType BoxActor::GetBoxType()
{
	return m_boxType;
}

oxygine::spActor BoxActor::GetBoxActor(BoxType & type)
{
	auto data = GetDataForBoxType(type);
	assert(data);

	oxygine::spActor ret = new oxygine::Actor;

	for (size_t i = 0; i < data->size(); ++i)
	{
		for (size_t j = 0; j < data->at(i).size(); ++j)
		{
			if (data->at(i)[j] == '1')
			{
				primitive::spPolyline box = new primitive::Polyline({ { -BOX_SIZE, -BOX_SIZE },{ BOX_SIZE, -BOX_SIZE },
					{ BOX_SIZE, BOX_SIZE },{ -BOX_SIZE, BOX_SIZE },{ -BOX_SIZE, -BOX_SIZE } });
				box->setPosition({ 2.0f*i*BOX_SIZE, 2.0f*j*BOX_SIZE });
				box->setTouchChildrenEnabled(false);
				box->attachTo(ret);
			}
		}
	}

	return ret;
}
