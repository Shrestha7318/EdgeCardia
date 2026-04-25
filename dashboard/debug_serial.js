const { SerialPort } = require('serialport');

// Try 9600 first, then try 115200
const port = new SerialPort({
    path: 'COM3',
    baudRate: 115200
});

console.log('--- RAW BYTE MONITOR (COM5 @ 9600) ---');
console.log('Watching for ANY data (no line parser)...\n');

port.on('open', () => {
    console.log('Port opened successfully');
});

port.on('error', (err) => {
    console.error('Error:', err.message);
});

// Raw data handler - no parser, just dump bytes
port.on('data', (buffer) => {
    const hex = buffer.toString('hex').match(/.{1,2}/g).join(' ');
    const ascii = buffer.toString('ascii').replace(/[^\x20-\x7E]/g, '.');
    console.log(`[${buffer.length} bytes] HEX: ${hex}  ASCII: ${ascii}`);
});
