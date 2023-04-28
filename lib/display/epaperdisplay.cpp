// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "epaperdisplay.h"

#ifndef environment_desktop

#include "main.h"
extern SPI_HandleTypeDef hspi2;

//#include "stm32wle5.h"

// PA0 :  displayReset
// PB5 :  displayChipSelect
// PB10 : displayBusy
// PB14 : displayDataCommand

//extern peripheralRegister PORTA_BSRR;
//extern peripheralRegister PORTB_BSRR;
//extern peripheralRegister PORTC_BSRR;
//extern peripheralRegister PORTB_IDR;

void ePaperDisplay::initialize() {
    reset();        // hardware reset

    uint8_t commandData[4]{0};

    commandData[0] = 0x7C;
    commandData[1] = 0x00;
    commandData[2] = 0x00;
    writeCommand(SSD1681Commands::DRIVER_OUTPUT_CONTROL, commandData, 3);

    commandData[0] = 0x01;
    writeCommand(SSD1681Commands::DATA_ENTRY_MODE_SETTING, commandData, 1);

    commandData[0] = 0x00;
    commandData[1] = 0x18;
    writeCommand(SSD1681Commands::SET_RAM_X_ADDRESS_START_END_POSITION, commandData, 2);

    commandData[0] = 0xC7;
    commandData[1] = 0x00;
    commandData[2] = 0x00;
    commandData[3] = 0x00;
    writeCommand(SSD1681Commands::SET_RAM_Y_ADDRESS_START_END_POSITION, commandData, 4);

    // TODO : some more initialization code required here.. need to study the datasheet first
}

void ePaperDisplay::set()
// EPD* epd,
// const unsigned char* image_buffer,
// int x,
// int y,
// int image_width,
// int image_height) {
// int x_end;
// int y_end;
{
    // if (
    //     image_buffer == NULL ||
    //     x < 0 || image_width < 0 ||
    //     y < 0 || image_height < 0) {
    //     return;
    // }
    // /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    // x &= 0xF8;
    // image_width &= 0xF8;
    // if (x + image_width >= epd->width) {
    //     x_end = epd->width - 1;
    // } else {
    //     x_end = x + image_width - 1;
    // }
    // if (y + image_height >= epd->height) {
    //     y_end = epd->height - 1;
    // } else {
    //     y_end = y + image_height - 1;
    // }
    // EPD_SetMemoryArea(epd, x, y, x_end, y_end);
    // EPD_SetMemoryPointer(epd, x, y);
    // EPD_SendCommand(epd, WRITE_RAM);
    // /* send the image data */
    // for (int j = 0; j < y_end - y + 1; j++) {
    //     for (int i = 0; i < (x_end - x + 1) / 8; i++) {
    //         EPD_SendData(epd, image_buffer[i + j * (image_width / 8)]);
    //     }
    // }
}

void ePaperDisplay::clear() {
    // EPD_SetMemoryArea(epd, 0, 0, epd->width - 1, epd->height - 1);
    // EPD_SetMemoryPointer(epd, 0, 0);
    // EPD_SendCommand(epd, WRITE_RAM);
    // /* send the color data */
    // for (int i = 0; i < epd->width / 8 * epd->height; i++) {
    //     EPD_SendData(epd, color);
    // }
}

void ePaperDisplay::show() {
    uint8_t commandData[1]{0xC4};
    writeCommand(SSD1681Commands::DISPLAY_UPDATE_CONTROL_2, commandData, 1);

    writeCommand(SSD1681Commands::MASTER_ACTIVATION, nullptr, 0);
    writeCommand(SSD1681Commands::TERMINATE_FRAME_READ_WRITE, nullptr, 0);
}

void ePaperDisplay::goSleep() {
    uint8_t commandData[1]{0x01};
    writeCommand(SSD1681Commands::DEEP_SLEEP_MODE, commandData, 1);
}

// static void EPD_SetMemoryArea(EPD* epd, int x_start, int y_start, int x_end, int y_end) {
//     EPD_SendCommand(epd, SET_RAM_X_ADDRESS_START_END_POSITION);
//     /* x point must be the multiple of 8 or the last 3 bits will be ignored */
//     EPD_SendData(epd, (x_start >> 3) & 0xFF);
//     EPD_SendData(epd, (x_end >> 3) & 0xFF);
//     EPD_SendCommand(epd, SET_RAM_Y_ADDRESS_START_END_POSITION);
//     EPD_SendData(epd, y_start & 0xFF);
//     EPD_SendData(epd, (y_start >> 8) & 0xFF);
//     EPD_SendData(epd, y_end & 0xFF);
//     EPD_SendData(epd, (y_end >> 8) & 0xFF);
// }

// /**
//  *  @brief: private function to specify the start point for data R/W
//  */
// static void EPD_SetMemoryPointer(EPD* epd, int x, int y) {
//     EPD_SendCommand(epd, SET_RAM_X_ADDRESS_COUNTER);
//     /* x point must be the multiple of 8 or the last 3 bits will be ignored */
//     EPD_SendData(epd, (x >> 3) & 0xFF);
//     EPD_SendCommand(epd, SET_RAM_Y_ADDRESS_COUNTER);
//     EPD_SendData(epd, y & 0xFF);
//     EPD_SendData(epd, (y >> 8) & 0xFF);
//     EPD_WaitUntilIdle(epd);
// }

void ePaperDisplay::reset() {
	HAL_GPIO_WritePin(GPIOA, displayReset_Pin, GPIO_PIN_RESET);
    // PORTA_BSRR.write(1 << 0);               // reset = LOW
    HAL_Delay(10U);                         // datasheet, section 4.2
	HAL_GPIO_WritePin(GPIOA, displayReset_Pin, GPIO_PIN_SET);
    // PORTA_BSRR.write(1 << (0 + 16));        // reset = HIGH
    HAL_Delay(10U);                         //
    writeCommand(SSD1681Commands::SW_RESET, nullptr, 0);
}

void ePaperDisplay::waitWhileBusy() const {
    while (isBusy()) {
        asm("NOP");
    }
}

void ePaperDisplay::writeCommand(SSD1681Commands theCommand, uint8_t* theData, uint32_t dataLength) {
    if (!isBusy()) {
        setChipSelect(active);
        setDataOrCommand(command);
        uint8_t commandAsByte = static_cast<uint8_t>(theCommand);
        HAL_SPI_Transmit(&hspi2, &commandAsByte, 1U, 1U);
        if (dataLength > 0) {
            setDataOrCommand(data);
            HAL_SPI_Transmit(&hspi2, theData, dataLength, 1U);
        }
        setChipSelect(inActive);
    }
}

bool ePaperDisplay::isBusy() const {
	return (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, displayBusy_Pin));
    //return (PORTB_IDR.readBit(10));
}

void ePaperDisplay::setDataOrCommand(bool isData) const {
    if (isData) {
    	HAL_GPIO_WritePin(GPIOB, displayDataCommand_Pin, GPIO_PIN_SET);
        // PORTB_BSRR.write(1 << 14);               // data = HIGH
    } else {                                     //
    	HAL_GPIO_WritePin(GPIOB, displayDataCommand_Pin, GPIO_PIN_RESET);
    	// PORTB_BSRR.write(1 << (14 + 16));        // command = LOW
    }
}

void ePaperDisplay::setChipSelect(bool active) const {
    if (active) {
    	HAL_GPIO_WritePin(GPIOB, displayChipSelect_Pin, GPIO_PIN_RESET);
        // PORTB_BSRR.write(1 << (5 + 16));        // active = LOW
    } else {                                    //
    	HAL_GPIO_WritePin(GPIOB, displayChipSelect_Pin, GPIO_PIN_SET);
        // PORTB_BSRR.write(1 << 5);               // release = HIGH
    }
}

#else

// TODO : a desktop console version goes here

#endif