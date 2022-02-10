const Config = {
    port: 25565
}

const http = require('http');
const path = require('path');
const express = require('express');

const app = express();
const httpServer = http.createServer(app);

const { Server } = require("socket.io");

const ioConfig = {};
const IO = new Server(httpServer, ioConfig);

app.use(express.static(path.join(__dirname, './client')));

const sockets = {};

IO.on('connection', socket => 
{
    sockets[socket.id] = socket;    

    console.log('Connection :', socket.id);
    
    socket.emit('objectsState', {
        [socket.id] : {
            data: {
                pos: {
                   pos: { x: 10, y: 10 },
                   vel: { x: 1, y: -1 } 
                },
                rot: {
                    angle: 0,
                    vel: 0
                }
            }
        }
    });

    socket.on('disconnect', () => {
        console.log('Disconnection :', socket.id);
        if(sockets[socket.id]) delete sockets[socket.id]; 
    });
    
});

httpServer.listen(Config.port, () => console.log(`server listening on ${Config.port}`));