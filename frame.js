const BufferShift = require('buffershift');
const schedule = [
    [12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 , 12, 12, 12, 12, 12, 12, 12, 12],
    [12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12],
    [ 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8],
    [ 8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0],
    [ 0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8],
    [12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0],
    [ 0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12],
    [12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0],
    [12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12],
    [ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0],
    [12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12],
    [ 0,  0,  0,  0, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0],
    [ 0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0],
    [ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0],
    [ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12],
];

const frames = [
    0x01, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26
];

const names = ["Pm2_5", "Pm10", "Noise", "Solar", "Battery", "O3", "S02", "NO2", "CO", "Ammonia", "Temperature", "Humidity", "Barometric_Pressure", "VOX_CO2", "CO2"];

parse = (data) => {
    const buffer = Buffer.from(data, 'hex');
    const frameId = buffer[0];
    BufferShift.shl(buffer, 8);

    const frameIndex = frames.indexOf(frameId);
    let temp = {};
    for(let measurementIndex = 0; measurementIndex < schedule.length; measurementIndex++) {
        const bits = schedule[measurementIndex][frameIndex];
        const name = names[measurementIndex];
        if(bits == 8) {
            temp[name] = buffer[0];
        } else if(bits == 12) {
            temp[name] = 16 * buffer[0] + (0xf0 & buffer[1]) / 16;
        }
        BufferShift.shl(buffer, bits);
    }
    return temp;
}

//const result = parse("2647f480999b09d09f0a10a5");
//console.log(JSON.stringify(result));

module.exports = { parse, names };