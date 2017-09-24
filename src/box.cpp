#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "MainActor.h"
#include "nanovgInc.h"
#include "common.h"
#include "BoxWorldActor.h"
#include "BodyActor.h"
#include <set>

NVGcontext * g_nanovgContext = nullptr;

using namespace oxygine;

Resources g_gameResources;

void InitializeNanovg()
{
	g_nanovgContext = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG); // or nvgCreateGL2

	static oxygine::file::buffer fontBuffer;
	if (oxygine::file::read("fonts\\calibri.ttf", fontBuffer))
	{
		nvgCreateFontMem(g_nanovgContext, "main-font", fontBuffer.data.data(), fontBuffer.data.size(), 0);
	}
}

void example_preinit()
{
}

void example_init()
{
	oxygine::log::messageln("example_init");

	InitializeNanovg();

	DebugActor::setCorner(1);

	g_gameResources.loadXML("res.xml");

	oxygine::getStage()->setName("Stage");

	oxygine::Vector2 stageSize = oxygine::getStage()->getSize();

	g_mainActor = new MainActor;
	g_mainActor->setSize(stageSize * 2);

	g_worldActor = new BoxWorldActor;

	if (!g_worldActor->LoadBoxes())
	{
		spBodyActor ground = new BodyActor(g_worldActor.get(),
			{ oxygine::getStage()->getWidth() / 2.0f, oxygine::getStage()->getHeight() - 40.0f }, 0.0f, true);
		ground->AddShape(new RectangleShape(oxygine::getStage()->getWidth() / 2.0f - 80.0f, 10.0f))->SetFillColor(oxygine::Color::AntiqueWhite);
		ground->attachTo(g_worldActor);
	}

	oxygine::getStage()->addEventListener(oxygine::Stage::DEACTIVATE, [](oxygine::Event *)
	{
		g_worldActor->SaveBoxes();
	});

	//oxygine::getStage()->addChild(g_mainActor);
	//g_mainActor->addChild(world);
	oxygine::getStage()->addChild(g_worldActor);
}

void example_destroy()
{
	nvgDeleteGLES2(g_nanovgContext);
	//nvgDeleteGL2(g_nanovgContext);

	g_gameResources.free();

#ifdef WIN32
	g_worldActor->SaveBoxes();
#endif

	// free global actors
	g_mainActor = nullptr;
	g_worldActor = nullptr;

}

void example_update()
{

}
