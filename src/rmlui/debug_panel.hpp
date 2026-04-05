#pragma once

#include <RmlUi/Core.h>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class DebugPanel final
{
public:
	DebugPanel(Rml::Context* context);

	void Update();

	void SetVisible(bool visible);
	bool IsVisible() const { return m_visible; }
	void Toggle() { SetVisible(!m_visible); }

	void Destroy();

private:
	void SetText(const Rml::String& id, const Rml::String& text);

	Rml::Context* m_context = nullptr;
	Rml::ElementDocument* m_document = nullptr;
	bool m_visible = false;
};