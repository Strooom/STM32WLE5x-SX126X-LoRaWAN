// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

class ePaperDisplay {
  public:
    bool initialize();

    static constexpr uint32_t displayWidth{200};         // [pixels]
    static constexpr uint32_t displayHeight{200};        // [pixels]

  private:
    static constexpr uint32_t displayBufferSize{displayWidth * displayHeight / 8};        // 1 bit per pixel
    uint8_t displayBuffer[displayBufferSize]{0};

    bool write();        // TODO add parameters
                         // bool read();         // TODO add parameters

  public:
    void testChipSelect();        // toggle chipSelect at 100 Hz - also test with NSS pin, putting the current pin in tristate and soldering a jumper cable
    void testBusy();              // read busy and display value on serial, control signal with resistor to GND or 3V3

  private:
    bool isBusy();
    void setDataOrCommand(bool isData);
    void setChipSelect(bool active);
};