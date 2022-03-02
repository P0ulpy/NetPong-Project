#pragma once

enum SocketEvents
{
	Disconnected,
	Connected,
    PlayerSendSettings,
    NewPlayerConnected,
    PlayerDisconnected,
    SceneUpdate,
    PongBallCollision,

	Count
};