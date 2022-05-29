const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')

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
    console.log(data)
})

