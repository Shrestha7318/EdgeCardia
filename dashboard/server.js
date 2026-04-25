const express = require('express');
const http = require('http');
const { Server } = require("socket.io");
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = 3000;
const SERIAL_PORT_PATH = 'COM3';
const BAUD_RATE = 115200;

app.use(express.static('public'));
app.use(express.json());

let lastSource = 'none';

const port = new SerialPort({
    path: SERIAL_PORT_PATH,
    baudRate: BAUD_RATE,
    autoOpen: true
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

console.log(`Connecting to ${SERIAL_PORT_PATH} @ ${BAUD_RATE}...`);

port.on('open', () => {
    console.log(`✅ Serial port opened: ${SERIAL_PORT_PATH}`);
});

port.on('error', (err) => {
    console.error('❌ Serial error:', err.message);
});

function emitMetrics(data, source) {
    lastSource = source;
    io.emit('source', source);
    io.emit('metrics', data);
}

parser.on('data', (line) => {
    line = line.trim();
    if (!line) return;

    lastSource = 'wired';
    io.emit('source', 'wired');

    if (line.startsWith('BPM:')) {
        try {
            const data = {};
            const parts = line.split(' ');

            parts.forEach(part => {
                if (!part.includes(':')) return;

                const [key, val] = part.split(':');
                const numVal = parseInt(val, 10);

                switch (key) {
                    case 'BPM':
                        data.bpm = numVal;
                        break;
                    case 'R':
                        data.rhythm = numVal;
                        break;
                    case 'S':
                        data.stress = numVal;
                        break;
                    case 'C':
                        data.copd = numVal;
                        break;
                    case 'A':
                        data.age = numVal;
                        break;
                }
            });

            console.log('🔌 Wired Metrics:', data);
            emitMetrics(data, 'wired');

        } catch (e) {
            console.error('Error parsing metrics line:', line, e);
        }
    }

    else if (line.startsWith('E:')) {
        const val = parseFloat(line.substring(2));

        if (!isNaN(val)) {
            io.emit('source', 'wired');
            io.emit('ecg_sample', val);
        }
    }

    else if (line.startsWith('Mode:')) {
        const mode = parseInt(line.split(':')[1], 10);
        console.log('🔄 Mode Switch:', mode);
        io.emit('mode', mode);
    }

    else if (line === 'LEADS ON' || line === 'LEADS OFF') {
        const connected = line === 'LEADS ON';

        console.log(connected ? '🟢 Leads Connected' : '🔴 Leads Disconnected');
        io.emit('leads', connected);
    }

    else {
        console.log('Serial:', line);
    }
});

app.post('/api/metrics', (req, res) => {
    const data = req.body;

    const metrics = {
        bpm: Number(data.bpm) || 0,
        rhythm: Number(data.rhythm) || 0,
        stress: Number(data.stress) || 0,
        copd: Number(data.copd) || 0,
        age: Number(data.age) || 0
    };

    console.log('📡 WiFi Metrics:', metrics);

    emitMetrics(metrics, 'wifi');

    if (typeof data.leads !== 'undefined') {
        io.emit('leads', Number(data.leads) === 1);
    }

    res.status(200).send('OK');
});

app.get('/api/status', (req, res) => {
    res.json({
        server: 'running',
        lastSource,
        serialPort: SERIAL_PORT_PATH,
        baudRate: BAUD_RATE
    });
});

io.on('connection', (socket) => {
    console.log('Client connected');
    socket.emit('source', lastSource);
});

server.listen(PORT, '0.0.0.0', () => {
    console.log(`Dashboard running at http://localhost:${PORT}`);
    console.log(`WiFi endpoint ready at http://YOUR_PC_IP:${PORT}/api/metrics`);
});