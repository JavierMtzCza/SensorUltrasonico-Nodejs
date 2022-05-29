const {SerialPort, ReadlineParser} = require("serialport");

const port= new SerialPort({
    path:'COM4',
    baudRate: 9600
})

port.on('data',function (data) {
    console.log('Data:',data)
})

const lineStream = port.pipe(new ReadlineParser())