const express = require('express');
const http = require('http');
const { Server } = require("socket.io");
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

// Configuration
const app = express();
const server = http.createServer(app);
const io = new Server(server);
const PORT = 3000;
const SERIAL_PORT_PATH = 'COM5';  // Make sure this matches your board
const BAUD_RATE = 9600;

// Serve static files
app.use(express.static('public'));

// Setup Serial Port
const port = new SerialPort({
    path: SERIAL_PORT_PATH,
    baudRate: BAUD_RATE
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

console.log(`Connecting to ${SERIAL_PORT_PATH}...`);

// Handle serial data
parser.on('data', (line) => {
    line = line.trim(); // Clean up whitespace
    if (!line) return;

    // console.log('Raw received:', line); 

    // Parse: "BPM:150 R:1 S:0 C:1 A:0"
    if (line.startsWith('BPM:')) {
        try {
            const data = {};
            const parts = line.split(' ');

            parts.forEach(part => {
                if (!part.includes(':')) return;
                const [key, val] = part.split(':');
                const numVal = parseInt(val, 10); // Use integer parsing

                switch (key) {
                    case 'BPM': data.bpm = numVal; break;
                    case 'R': data.rhythm = numVal; break;
                    case 'S': data.stress = numVal; break;
                    case 'C': data.copd = numVal; break;
                    case 'A': data.age = numVal; break;
                }
            });

            console.log('✅ Parsed Metrics:', data); // DEBUG LOG
            io.emit('metrics', data);

        } catch (e) {
            console.error('Error parsing line:', line, e);
        }
    }
    // Parse: "E:0.123" (Real-time ECG sample)
    else if (line.startsWith('E:')) {
        const val = parseFloat(line.substring(2));
        if (!isNaN(val)) {
            io.emit('ecg_sample', val);
        }
    }
    // Parse: "Mode:1"
    else if (line.startsWith('Mode:')) {
        const mode = parseInt(line.split(':')[1], 10);
        console.log('🔄 Mode Switch:', mode);
        io.emit('mode', mode);
    }
    // Parse: "LEADS ON" / "LEADS OFF"
    else if (line === 'LEADS ON' || line === 'LEADS OFF') {
        const connected = line === 'LEADS ON';
        console.log(connected ? '🟢 Leads Connected' : '🔴 Leads Disconnected');
        io.emit('leads', connected);
    }
});

io.on('connection', (socket) => {
    console.log('Client connected');
});

// Start server
server.listen(PORT, () => {
    console.log(`Dashboard running at http://localhost:${PORT}`);
});
