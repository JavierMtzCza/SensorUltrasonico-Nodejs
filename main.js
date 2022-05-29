const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')

const port = new SerialPort( 
    {
        path: 'COM4',
        baudRate: 9600
    }
)

port.on('error', function(err) {
    console.log('Error: ', err.message)
})

const parser = port.pipe(new ReadlineParser({ delimiter: '~' }))
parser.on('data',(data)=>{console.log(data)})

