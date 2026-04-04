#pragma once

#include <RmlUi/Core.h>
#include "scene.hpp"
#include "clientmanager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Lobby final : public Scene, IStateListener
{
public:
	Lobby(Rml::Context* context);

	void ProcessEvent(Rml::Event& event) override;

	SceneId GetId() const override { return SceneId::LOBBY; }

	void Update() override;
	void Destroy() override;

	void OnStateUpdate(const StateUpdate& update) override;

private:
	void RefreshPlayers();

	Rml::ElementDocument* m_document = nullptr;

};