#pragma once

#include <RmlUi/Core.h>
#include "scene.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Lobby final : public Scene
{
public:
	Lobby(Rml::Context* context);

	void ProcessEvent(Rml::Event& event) override;

	SceneId GetId() const override { return SceneId::LOBBY; }

	void Update() override;
	void Destroy() override;

private:
	Rml::ElementDocument* m_document = nullptr;

};