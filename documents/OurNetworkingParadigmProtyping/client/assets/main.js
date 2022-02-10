import { Airplane } from "./airplane.js";
import { SocketManager } from "./socket.js";

export const env = {
    instances: {},
    register(name = "", inst)
    {
        if(this.instances[name]) console.warn("name, allready in env instances");
        this.instances[name] = inst;        
    },
    get: name => instances[name]
};

export const Time = 
{
    now: 0,
    get deltaTime() { return deltaTime },
    get deltaSecond() { return this.deltaTime / 1000; }
};

export function preload()
{
    new SocketManager({
        remoteAddr: "localhost",
        remotePort: 25565
    });
}

export let airplane;

export function setup()
{
    createCanvas(800, 800);
    airplane = new Airplane();
}

export function draw()
{
    Time.now += Time.deltaSecond;
    background(0);

    airplane.update();
    airplane.draw();
}