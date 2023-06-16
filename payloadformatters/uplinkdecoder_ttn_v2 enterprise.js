function decodeUplink(input) {
  var decoded = {};
  var payloadEncodingVersion = input.bytes[0];
  decoded.data.version = payloadEncodingVersion;
  switch (payloadEncodingVersion) {
    case 0x02: // mumo_v2_0
      for (var i = 1; i < input.bytes.length;) {
        var measurementChannel = input.bytes[i];
        var measurementTimestamp = input.bytes[i + 4] << 24 | input.bytes[i + 3] << 16 | input.bytes[i + 2] << 8 | input.bytes[i + 1];
        var floatValue = bytesToFloat(input.bytes[i + 5], input.bytes[i + 6], input.bytes[i + 7], input.bytes[i + 8])

        switch (measurementChannel) {
          case 0x01:
            decoded.data.battery = floatValue;
            break;

          case 0x10:
            decoded.data.temperature = floatValue;
            break;

          case 0x11:
            decoded.data.humidity = floatValue;
            break;

          case 0x12:
            decoded.data.pressure = floatValue;
            break;

          default:
        }
        i += 9;
      }
      break;

    default:
  }
  return decoded;
}

function bytesToFloat(byte1, byte2, byte3, byte4) {
  var bits = byte4 << 24 | byte3 << 16 | byte2 << 8 | byte1;
  var sign = (bits >>> 31 === 0) ? 1.0 : -1.0;
  var e = bits >>> 23 & 0xff;
  var m = (e === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
  var f = sign * m * Math.pow(2, e - 150);
  return f;
}


