var express = require('express');
var bodyParser = require('body-parser');
var request=require('request');

var fs = require('fs');
const { callbackify } = require('util');
var app = express();

//Untuk membuat file json

fs.exists('Fils.json',function(exists){
    if (exists){
        console.log("File Ditemukan !");

    }
    else{
        console.log("Membuat File...");
        fs.writeFile('Fils.json',json,'utf-8')
        console.log("Done !");
    }
});


var urlencodedParser = bodyParser.urlencoded({ extended: false });

app.get('/',function(req,res){
    console.log("SENDING RESPONSE !");
    res.send("Online Brooooo!!!")
});

app.post('/Terimadata', urlencodedParser, function(req,res){
    console.log(req.body);

    var SensorUltra = req.body.sensorultra;
    var SensorPir = req.body.sensorpir;
    var Relay1 = req.body.Relay1;
    var Relay2 = req.body.Relay2;
    var Relay3 = req.body.Relay3;

    let data = {
        'Nodemcu':{
            'ultra' : SensorUltra,
            'pir' : SensorPir,
            'relay1' : Relay1,
            'relay2' : Relay2,
            'relay3' : Relay3
        }
    };
    res.json(data);
    let dats = JSON.stringify(data,null,2);
    fs.writeFile('Fils.json', dats, (err) => {
        if (err) throw err;
        console.log('Data Write !');
    });
});


app.get('/Kirimdata',function(req,res){
    let rawdata = fs.readFileSync('Fils.json');
    let student = JSON.parse(rawdata);
    console.log(student);
    console.log("ÖNLINESSS");
    res.send(student);
});


//untuk request data cctv
function Requestdata(url){
    request(url, function(error,res,body){
    if(!error && res.statusCode== 200){
        console.log(body);
    }
    else{
        console.log("Error "+res.statusCode);
    }
});
}



app.listen(2020);