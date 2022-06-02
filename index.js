const express = require('express');
const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')

const app = express();

let data =  [
    {
        "ID":1,
        "Estado":0,
    },
    {
        "ID":2,
        "Estado":0,
    },
    {
        "ID":3,
        "Estado":0,
    },
    {
        "ID":4,
        "Estado":0,
    },
    {
        "ID":5,
        "Estado":0,
    },
]
    app.get('/', (request, response) => {
        response.send('hola');
    })

    app.get('/api/data',(request,response) => {
        response.json(data);
    })


const PORT=3001;
app.listen(PORT)
console.log(`Server is running in port ${PORT}`)

//===========================  LEER EL PUERTO SERIAL ================================

// iniciamos conexion con nuestro arduino en el COM que le especificamos y a 9600 bauds
const port = new SerialPort( 
    {
        path: 'COM4',
        baudRate: 9600
    }
)

// Si hay un error esta funcion nos muestra que salio mal
port.on('error', function(err) {
    console.log('Error: ', err.message)
})

// Este parser  nos lo transforma a string cada que encuentre un ~
const parser = port.pipe(new ReadlineParser({ delimiter: '~' }))
//cada que se detecte una recepcion, lo imprimimos en la consola
parser.on('data',(data)=>{
    const distancia = parseInt(data,10);
    if(distancia<15 && distancia>10){
        console.log('menor que 15 pero mayor de 10')
        console.log(data[1])
        data[0].ID=0;
    }else if(distancia<10){
        console.log('menor que 10')
        data[0].ID=1;
    }   
})
