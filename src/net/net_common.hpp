#pragma once

#include <steam/steamnetworkingtypes.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

using NetConnection = HSteamNetConnection;
using NetIdentity = SteamNetworkingIdentity;
using NetSocket = HSteamListenSocket;
using NetInterface = ISteamNetworkingSockets;
using NetAddress = SteamNetworkingIPAddr;
using NetPollGroup = HSteamNetPollGroup;
using NetMessage = ISteamNetworkingMessage;
using NetErrorMsg = SteamNetworkingErrMsg;
using NetConfig = SteamNetworkingConfigValue_t;
using NetConnectionStatusCallback = SteamNetConnectionStatusChangedCallback_t;

enum class NetConnectState : int {
    None = k_ESteamNetworkingConnectionState_None,
    Connecting = k_ESteamNetworkingConnectionState_Connecting,
    FindingRoute = k_ESteamNetworkingConnectionState_FindingRoute,
    Connected = k_ESteamNetworkingConnectionState_Connected,
    ClosedByPeer = k_ESteamNetworkingConnectionState_ClosedByPeer,
    ProblemDetectedLocally = k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
};