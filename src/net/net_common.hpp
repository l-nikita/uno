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