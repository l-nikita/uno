#pragma once

#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingtypes.h>

namespace shared::net
{
    using Connection = HSteamNetConnection;
    using Identity = SteamNetworkingIdentity;
    using Socket = HSteamListenSocket;
    using Interface = ISteamNetworkingSockets;
    using Address = SteamNetworkingIPAddr;
    using PollGroup = HSteamNetPollGroup;
    using Message = ISteamNetworkingMessage;
    using ErrorMsg = SteamNetworkingErrMsg;
    using Config = SteamNetworkingConfigValue_t;
    using ConnectionStatusCallback = SteamNetConnectionStatusChangedCallback_t;

    enum class ConnectState : int
    {
        NONE = k_ESteamNetworkingConnectionState_None,
        CONNECTING = k_ESteamNetworkingConnectionState_Connecting,
        FINDING_ROUTE = k_ESteamNetworkingConnectionState_FindingRoute,
        CONNECTED = k_ESteamNetworkingConnectionState_Connected,
        CLOSED_BY_PEER = k_ESteamNetworkingConnectionState_ClosedByPeer,
        PROBLEM_DETECTED_LOCALLY = k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
    };
}
