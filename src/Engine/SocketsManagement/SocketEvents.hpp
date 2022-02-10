#pragma once

enum SocketEvents {
	Disconnected,
	Connected,
    NewPlayerConnected,
    PlayerDisconnected,
    SyncUpdate,

	Count
};