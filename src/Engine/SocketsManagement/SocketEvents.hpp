#pragma once

enum SocketEvents {
	Disconnected,
	Connected,
    PlayerSendSettings,
    NewPlayerConnected,
    PlayerDisconnected,
    SyncUpdate,

	Count
};