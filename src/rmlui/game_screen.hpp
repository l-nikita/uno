#pragma once

#include <RmlUi/Core.h>
#include "scene.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class GameScreen final : public Scene
{
public:
	GameScreen(Rml::Context* context);

	void ProcessEvent(Rml::Event& event) override;

	SceneID GetID() const override { return SceneID::GAME_SCREEN; }

	void Update() override;
	void Destroy() override;

private:
	Rml::ElementDocument* m_document = nullptr;

	struct PendingDeletion 
	{
		Rml::Element* Element;
		double Time;
	};

	std::vector<PendingDeletion> m_deletionQueue;
};