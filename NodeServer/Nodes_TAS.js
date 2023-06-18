const express = require('express');
const app = express();
const socket = require('socket.io');
app.use(express.static('public'));
let server = app.listen(2001, console.log('Listening on Port 2001'));
let io = socket(server);

io.sockets.on('connect', socket=>{
    socket.on('sendtexttoserver',function(message){
        console.log(message);
    });
    console.log('Socket Connected..');
    socket.on('Ultra', function(data){
        console.log(data);
        io.sockets.emit('ultra',data);
    }); 
    socket.on('Pir', function(data){
        console.log(data);
        io.sockets.emit('pir',data);
    });
    socket.on('Relay1', function(data){
        console.log(data);
        io.sockets.emit('relay1',data);
    });
    socket.on('Relay2', function(data){
        console.log(data);
        io.sockets.emit('relay2',data);
    });
    socket.on('Relay3', function(data){
        console.log(data);
        io.sockets.emit('relay3',data);
    });
    // while(true){
    //     var Datas = Math.random()
    //     var pir = Math.random(1)
    //     var relay1 = Math.random(1)
    //     var relay2 = Math.random(1)
    //     var relay3 = Math.random(1)
    //     io.sockets.emit('ultra',Datas);
    //     // io.sockets.emit('pir',pir);
    //     // io.sockets.emit('relay1',relay1);
    //     // io.sockets.emit('relay2',relay2);
    //     // io.sockets.emit('relay3',relay3);
    //     console.log(Datas);
    //     // console.log(pir);
    //     // console.log(relay1);
    //     // console.log(relay2);
    //     // console.log(relay3);

    // }

});