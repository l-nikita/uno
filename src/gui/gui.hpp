#pragma once

#include <vector>

class Panel;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class Gui
{
public:
	Gui() { OnWindowResize(); }
	~Gui();

	void Update();
	void Render();

	void AddPanel(Panel* panel);
	const std::vector<Panel*>& GetPanels() const { return m_panels; }

	float GetScale() const { return m_scale; }

	void OnWindowResize();

private:
	std::vector<Panel*> m_panels;
	Panel* m_focusedPanel = nullptr;

	float m_scale = 1.0f;
};

extern Gui* g_Gui;