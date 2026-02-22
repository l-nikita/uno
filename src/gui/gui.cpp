#include <ranges>
#include <algorithm>
#include "gui.hpp"
#include "panel.hpp"

Gui::~Gui()
{
	for (auto panel : m_panels)
		delete panel;

	m_panels.clear();
}

void Gui::Update()
{
	m_focusedPanel = nullptr;

	for (auto& panel : std::ranges::views::reverse(GetPanels()))
	{
		SDL_FPoint mousePoint = g_Input->GetMousePoint();

		if (!m_focusedPanel && SDL_PointInRectFloat(&mousePoint, &panel->GetRect()))
		{
			m_focusedPanel = panel;
			panel->SetHovered(true);
		}
		else
			panel->SetHovered(false);

		panel->Update();
	}

	std::erase_if(m_panels, [](Panel* panel) {
		if (panel->m_shouldRemove) 
		{
			delete panel;
			return true;
		}

		return false;
	});
}

void Gui::Render()
{
	for (auto& panel : GetPanels())
		panel->Render();
}

void Gui::AddPanel(Panel* panel)
{
	m_panels.push_back(panel);

	std::sort(m_panels.begin(), m_panels.end(), [](Panel* a, Panel* b) {
		return a->m_zIndex < b->m_zIndex;
	});
}

void Gui::OnWindowResize()
{
	int wW, wH;
	g_Game->GetWindowSize(&wW, &wH);

	m_scale = (float)wW / 1280.0f;

	for (auto& panel : GetPanels())
		panel->OnWindowResize();
}