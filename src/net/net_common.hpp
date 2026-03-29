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
    NONE = k_ESteamNetworkingConnectionState_None,
    CONNECTING = k_ESteamNetworkingConnectionState_Connecting,
    FINDING_ROUTE = k_ESteamNetworkingConnectionState_FindingRoute,
    CONNECTED = k_ESteamNetworkingConnectionState_Connected,
    CLOSED_BY_PEER = k_ESteamNetworkingConnectionState_ClosedByPeer,
    PROBLEM_DETECTED_LOCALLY = k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
};