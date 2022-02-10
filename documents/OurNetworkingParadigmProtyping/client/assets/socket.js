import { env } from './main.js'

export class SocketManager 
{
    constructor(config = {})
    {
        if(env.socketManager) throw 'SocketManager is a singleton';

        this.remoteAddr = config.remoteAddr || "127.0.0.1";
        this.remotePort = config.remotePort || 25565;

        this.socket = io(`${this.remoteAddr}:${this.remotePort}`);

        this.setup();
        env.register("SocketManager", this);
    }

    setup()
    {
        this.socket.connect();

        this.socket.on('connect', () => this.onConnect());
        this.socket.on('disconnect', () => this.onDisconnect());
        this.socket.on('objectsState', (states) => this.onReveiveObjectsState(states))

    }

    onConnect()
    {
        console.log(`socket connected with id "${this.socket.id}"`);
    }

    onReveiveObjectsState(states = {})
    {
        console.log("objectsState", states);
    }

    onDisconnect()
    {
        console.info("socket disconnected");
    }
}