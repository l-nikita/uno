#include <print>
#include <format>
#include "debug_panel.hpp"
#include "../game.hpp"
#include "../clientmanager.hpp"
#include "../gamemanager.hpp"
#include "../net/net_manager.hpp"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DebugPanel::DebugPanel(Rml::Context* context)
	: m_context(context)
{
	m_document = m_context->LoadDocument("ui/rml/debug_panel.rml");
}

void DebugPanel::Destroy()
{
	if (m_document)
		m_document->Close(), m_document = nullptr;
}

//-----------------------------------------------------------------------------
void DebugPanel::SetVisible(bool visible)
{
	m_visible = visible;

	if (m_document)
	{
		if (m_visible)
			m_document->Show();
		else
			m_document->Hide();
	}
}

//-----------------------------------------------------------------------------
void DebugPanel::Update()
{
	if (!m_visible || !m_document)
		return;

	m_document->PullToFront();

	SetText("debug_header", "DEBUG: " + g_Game->m_GameSettings.Name);

	// fps, delta time
	double dt = g_Game->GetDeltaTime();
	int fps = (dt > 0.0) ? static_cast<int>(1.0 / dt) : 0;

	SetText("dbg_fps", "FPS: " + std::to_string(fps));
	SetText("dbg_dt", "DT: " + std::to_string((dt * 1000)));

	// network
	bool isHost = g_NetManager->IsHost();
	bool isConnected = g_NetManager->GetClient() && g_NetManager->GetClient()->IsConnected();

	SetText("dbg_role", "Role: <span style='font-size: 3vh; color: " + (isHost ? Rml::String("#34c3eb") : Rml::String("orange")) + "'>" + (isHost ? Rml::String("HOST") : Rml::String("CLIENT")) + "</span>");
	SetText("dbg_connected", isConnected ? "Conn: YES" : "Connection: NO");
	
	// game State
	auto& gameState = g_ClientManager->GetGameState();

	Rml::String stageStr;
	switch (gameState.Stage)
	{
		case GameStage::Lobby:			stageStr = "Lobby"; break;
		case GameStage::RoundInProgress:stageStr = "Round"; break;
		case GameStage::RoundEnd:		stageStr = "End"; break;
		default:						stageStr = "?"; break;
	}
	SetText("dbg_stage", "Stage: " + stageStr);
	SetText("dbg_current_player", "Turn: " + std::to_string(gameState.CurrentPlayer));

	// my turn
	auto& localPlayer = g_ClientManager->GetLocalPlayerInfo();
	bool myTurn = (gameState.CurrentPlayer == localPlayer.Index);
	SetText("dbg_my_turn", myTurn ? "My Turn: <span style='color: green'>YES</span>" : "My Turn: <span style='color: red'>NO</span>");

	// players list
	Rml::Element* playersEl = m_document->GetElementById("debug_players");
	if (playersEl)
	{
		Rml::String playersRml;
		for (const auto& player : gameState.Players)
		{
			Rml::String localClass = player.IsLocal ? " local" : "";
			Rml::String marker = player.IsLocal ? " " : "  ";

			playersRml += "<div class='debug-player-row'>"
				"<div class='debug-player-name" + localClass + "'>" + marker + player.Name + "</div>"
				"<div class='debug-player-cards'>" + Rml::ToString((int)player.Hand.size()) + " cards</div>"
				"</div>";
		}
		
		playersEl->SetInnerRML(playersRml);
	}
}

//-----------------------------------------------------------------------------
void DebugPanel::SetText(const Rml::String& id, const Rml::String& text)
{
	Rml::Element* el = m_document->GetElementById(id);
	if (el)
		el->SetInnerRML(text);
}