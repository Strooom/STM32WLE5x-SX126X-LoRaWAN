function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.
    var decoded = {};
  
    decoded.version = bytes[0];
    if(decoded.version == 1){
      decoded.battery = bytes[1];
      decoded.humidity = bytes[2];
      decoded.humidity += (bytes[3]/100);
      decoded.temperature = bytes[4];
      decoded.temperature += (bytes[5]/100);
      decoded.pressure = (bytes[6])<<8;
      decoded.pressure += (bytes[7]);
      decoded.pressure =  (decoded.pressure+100000)/100;
      decoded.illumination = (bytes[8]<<8);
      decoded.illumination += bytes[9];
    }else if(decoded.version == 2){ //New decoder function for the air pressure
      decoded.battery = bytes[1];
      decoded.humidity = bytes[2];
      decoded.humidity += (bytes[3]/100);
      decoded.temperature = bytes[4];
      decoded.temperature += (bytes[5]/100);
      decoded.pressure = (bytes[6])<<8;
      decoded.pressure += (bytes[7]);
      decoded.pressure =  (decoded.pressure-30000+101325)/100;
      decoded.illumination = (bytes[8]<<8);
      decoded.illumination += bytes[9];
    }else if(decoded.version == 3){ //New decoder function for the air pressure
      decoded.battery = bytes[1];
      decoded.humidity = bytes[2];
      decoded.humidity += (bytes[3]/100);
      decoded.temperature = bytes[4];
      decoded.temperature += (bytes[5]/100);
      decoded.pressure = (bytes[6])<<8;
      decoded.pressure += (bytes[7]);
      decoded.pressure =  (decoded.pressure-30000+101325)/100;
      decoded.illumination = (bytes[8]<<8);
      decoded.illumination += bytes[9];
    }
    return decoded;
  }