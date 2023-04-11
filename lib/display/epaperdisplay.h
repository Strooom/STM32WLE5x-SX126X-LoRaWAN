// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

class ePaperDisplay {
  public:
    void initialize();        // wake up from deep sleep and initialize
    void set();               // write displayBuffer to display
    void clear();             // write blank data to display
    void show();              // refresh display
    void goSleep();           //

    static constexpr uint32_t displayWidth{200};         // [pixels]
    static constexpr uint32_t displayHeight{200};        // [pixels]

  private:
    static constexpr uint32_t displayBufferSize{displayWidth * displayHeight / 8};        // 1 bit per pixel
    uint8_t displayBuffer[displayBufferSize]{0};
    enum class SSD1681Commands : uint8_t {
        DRIVER_OUTPUT_CONTROL                = 0x01,
        BOOSTER_SOFT_START_CONTROL           = 0x0C,
        GATE_SCAN_START_POSITION             = 0x0F,
        DEEP_SLEEP_MODE                      = 0x10,
        DATA_ENTRY_MODE_SETTING              = 0x11,
        SW_RESET                             = 0x12,
        TEMPERATURE_SENSOR_CONTROL           = 0x1A,
        MASTER_ACTIVATION                    = 0x20,
        DISPLAY_UPDATE_CONTROL_1             = 0x21,
        DISPLAY_UPDATE_CONTROL_2             = 0x22,
        WRITE_RAM                            = 0x24,
        WRITE_VCOM_REGISTER                  = 0x2C,
        WRITE_LUT_REGISTER                   = 0x32,
        SET_DUMMY_LINE_PERIOD                = 0x3A,
        SET_GATE_TIME                        = 0x3B,
        BORDER_WAVEFORM_CONTROL              = 0x3C,
        SET_RAM_X_ADDRESS_START_END_POSITION = 0x44,
        SET_RAM_Y_ADDRESS_START_END_POSITION = 0x45,
        SET_RAM_X_ADDRESS_COUNTER            = 0x4E,
        SET_RAM_Y_ADDRESS_COUNTER            = 0x4F,
        TERMINATE_FRAME_READ_WRITE           = 0xFF,
    };

    void writeCommand(SSD1681Commands command, uint8_t* data, uint32_t dataLength);
    void reset();
    void waitWhileBusy() const;
    bool isBusy() const;
    void setDataOrCommand(bool isData) const;
    void setChipSelect(bool active) const;

    bool active{false};         // chipSelect active = LOW
    bool inActive{true};        // chipSelect inactive = HIGH
    bool data{true};            // data = HIGH
    bool command{false};        // command = LOW
};