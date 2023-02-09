// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"

bool sx126x::isStandby() {
    theState = getStateFromHW();
    return ((theState == sx126xState::standbyRc) || (theState == sx126xState::standbyXosc));
}

void sx126x::executeCommand(const sx126xCommand opcode, const uint8_t* parameters, const uint8_t nmbrParameters) {
    // send all bytes via SPI
    // data received in SPI is stored in parameters[n] array
}

sx126xState sx126x::getStateFromHW() {
    constexpr uint8_t nmbrExtraBytes{1};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    executeCommand(sx126xCommand::getStatus, parametersIn, nmbrExtraBytes);

    switch (SUBGRF_GetOperatingMode()) {
        case MODE_TX:
            return RF_TX_RUNNING;
        case MODE_RX:
            return RF_RX_RUNNING;
        case MODE_CAD:
            return RF_CAD;
        default:
            return RF_IDLE;
    }

    return theState;
}

sx126xError sx126x::getLastError() {
    sx126xError result = lastError;
    lastError          = sx126xError::none;
    return result;
}

bool sx126x::isBusy() {
    // PWR_SR2 register, bit 1
}

void sx126x::setRegulatorMode() {
    // Check if in Standby Mode - you cannot change the regulator in other state
    // Also check OverCurrentProtection - default at 60 mA
    // EU 868 band also limits power to 16 dBm !! so maybe the DCDC is not needed in this case
    // Datasheet pg 76 : default the LDO is selected. This works but is not the most power efficient solution -> using the DCDC can be postponed for a next phase where we optimize power
}

void sx126x::reset() {
    // control the RESET line of the SX126X from the STM32WLE RCC registers
    // SX126x datasheet recommends to keep RESET low for 100 uS
    // After reset
    // * SX126x performs a calibration
    // * all context/config inside the SX126x is lost -> initialization is needed

    CLEAR_BIT(RCC->CSR, RCC_CSR_RFRST);        // Drive RESET LOW
                                               // wait 100uS
    SET_BIT(RCC->CSR, RCC_CSR_RFRST);          // Drive RESET HIGH
    READ_BIT(RCC->CSR, RCC_CSR_RFRSTF);        // Read RFRSTF flag and wait for SX126x to come out of reset
    // TODO : wait for flag

    // Also, after reset, the SX126x will be Busy for some time, so check this before doing anything
}

void sx126x::initialize() {
    // 1. Clocks
    // The SPI between STM32WLE and SX126x runs on the PCLK3 clock, divided by 2.
    // The SX126x-SPI interface has a maximum of 16 MhZ
    // As we are running the whole STM32WLE on 16 MHz (or lower), we can send the SYSCLK straight to the PCLK3 which means this SPI will run on 8 MHz

    // MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, Source);        // enable clocks to SPI1
    //      Source This parameter can be one of the following values:
    //    *         @arg @ref LL_RCC_SYS_CLKSOURCE_MSI
    //    *         @arg @ref LL_RCC_SYS_CLKSOURCE_HSI
    //    *         @arg @ref LL_RCC_SYS_CLKSOURCE_HSE
    //    *         @arg @ref LL_RCC_SYS_CLKSOURCE_PLL
    // MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, Prescaler);                      // set AHB prescaler
    // MODIFY_REG(RCC->EXTCFGR, RCC_EXTCFGR_SHDHPRE, Prescaler >> 4);        // set AHB 3 prescaler
    // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, Prescaler);                     // set APB1 prescaler
    // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, Prescaler);                     // set APB2 prescaler

    // MODIFY_REG(RCC->CFGR, RCC_CFGR_STOPWUCK, Clock);        // Set clock after WakeUp from Stop - TODO I think this needs to be a global thing for the STM32wle, not only for this SPI1

    // configure SPI1

    // set packet type
    uint8_t data[1];
    data[0] = 0x01;        // We only do LoRa
    executeCommand(sx126xCommand::setPACKETTYPE, data, 1);

    SUBGRF_SetPacketType(modulationParams->PacketType);

    Radio_SMPS_Set(SMPS_DRIVE_SETTING_DEFAULT);
    // uint8_t modReg;
    //     modReg= SUBGRF_ReadRegister(SUBGHZ_SMPSC2R);
    //     modReg&= (~SMPS_DRV_MASK);
    //     SUBGRF_WriteRegister(SUBGHZ_SMPSC2R, modReg | level);

    SUBGRF_WriteCommand(RADIO_SET_STANDBY, (uint8_t*)&standbyConfig, 1);
    // if( standbyConfig == STDBY_RC )
    // {
    //     OperatingMode = MODE_STDBY_RC;
    // }
    // else
    // {
    //     OperatingMode = MODE_STDBY_XOSC;
    // }
    CalibrateImage ? ? if (freq > 900000000) {
        calFreq[0] = 0xE1;
        calFreq[1] = 0xE9;
    }
    else if (freq > 850000000) {
        calFreq[0] = 0xD7;
        calFreq[1] = 0xDB;
    }
    else if (freq > 770000000) {
        calFreq[0] = 0xC1;
        calFreq[1] = 0xC5;
    }
    else if (freq > 460000000) {
        calFreq[0] = 0x75;
        calFreq[1] = 0x81;
    }
    else if (freq > 425000000) {
        calFreq[0] = 0x6B;
        calFreq[1] = 0x6F;
    }
    SUBGRF_WriteCommand(RADIO_CALIBRATEIMAGE, calFreq, 2);
}

void sx126x::goSleep() {
    sx126xState currentState = getStateFromHW();
    if (currentState == sx126xState::standby) {
        constexpr uint8_t nmbrExtraBytes{1};
        uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];

        executeCommand(sx126xCommand::setSleep, parametersIn, nmbrExtraBytes);
    } else {
        lastError = sx126xError::goSleepWhenNotInStandby;
    }

    // TODO : change the state

    /* switch the antenna OFF by SW */
    // RBI_ConfigRFSwitch(RBI_SWITCH_OFF);

    // Radio_SMPS_Set(SMPS_DRIVE_SETTING_DEFAULT);

    // uint8_t value = ( ( ( uint8_t )sleepConfig.Fields.WarmStart << 2 ) |
    //                   ( ( uint8_t )sleepConfig.Fields.Reset << 1 ) |
    //                   ( ( uint8_t )sleepConfig.Fields.WakeUpRTC ) );
    // SUBGRF_WriteCommand( RADIO_SET_SLEEP, &value, 1 );
    // OperatingMode = MODE_SLEEP;
}

void sx126x::goStandby(sx126xStandbyMode theStandbyMode = sx126xStandbyMode::rc) {
    // normally the SX126x goes to standby automatically after transmission / receive completed, or timed out

    constexpr uint8_t nmbrExtraBytes{1};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>(theStandbyMode);
    executeCommand(sx126xCommand::setStandby, parametersIn, nmbrExtraBytes);
}

void sx126x::setRfFrequency(uint32_t frequencyInHz) {
    uint64_t tmpFrequencyRegisterValue = ((static_cast<uint64_t>(frequencyInHz) << 25) / static_cast<uint64_t>(crystalFrequency));        // 64 bit math needed to avoid overflow, result is only 32 bits wide
    uint32_t frequencyRegisterValue    = (static_cast<uint32_t>(tmpFrequencyRegisterValue && 0x00000000FFFFFFFF));                        // take only lower 32-bits, upper bits should be zero

    constexpr uint8_t nmbrExtraBytes{4};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>((frequencyRegisterValue >> 24) & 0xFF);        //
    parametersIn[1] = static_cast<uint8_t>((frequencyRegisterValue >> 16) & 0xFF);        //
    parametersIn[2] = static_cast<uint8_t>((frequencyRegisterValue >> 8) & 0xFF);         //
    parametersIn[3] = static_cast<uint8_t>((frequencyRegisterValue)&0xFF);                //

    executeCommand(sx126xCommand::setRfFRequency, parametersIn, nmbrExtraBytes);
}

void setTxParameters(int8_t transmitPowerdBm) {        // caution - signed int8, negative dBm values are in two's complement
    constexpr uint8_t nmbrExtraBytes{2};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];

    if (transmitPowerdBm < 0) {
        parametersIn[0] = 255 - static_cast<uint8_t>(-(transmitPowerdBm + 1));        // magic to turn negative signed value into a raw byte in two's complement
    } else {
        parametersIn[0] = static_cast<uint8_t>(transmitPowerdBm);        // if value is not negative, casting it is safe
    }
    parametersIn[1] = 0x04;        // rampTime 200 uS - no info why this value, but this was in the demo application from ST / Semtech
                                   // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setTXPARAMS, parametersIn, nmbrExtraBytes);
}

void sx126x::setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate) {
    constexpr uint8_t nmbrExtraBytes{8};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = static_cast<uint8_t>(theSpreadingFactor);        //
    parametersIn[1] = static_cast<uint8_t>(theBandwidth);              //
    parametersIn[2] = static_cast<uint8_t>(theCodingRate);             //
    parametersIn[3] = 0x01;                                            // TODO ?? LowDatarateOptimize // why would you NOT want to do this ??
                                                                       // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPACKETPARAMS, parametersIn, nmbrExtraBytes);
}

void sx126x::setPacketParameters(uint8_t payloadLength) {
    constexpr uint8_t nmbrExtraBytes{9};
    uint8_t parametersIn[nmbrExtraBytes]{0}, dataOut[nmbrExtraBytes];
    parametersIn[0] = 0x00;                 // MSB for PreambleLength
    parametersIn[1] = 0x08;                 // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    parametersIn[2] = 0x00;                 // Variable length packet (explicit header)
    parametersIn[3] = payloadLength;        // Payload Length
    parametersIn[4] = 0x01;                 // CRC On ??
    parametersIn[5] = 0x00;                 // Standard IQ : for uplink. Downlink requires inverted IQ
                                            // the remaining 3 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, parametersIn, nmbrExtraBytes);
}

void sx126x::setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut) {
    // Guessing from the STM32WLE datasheet, there is an SX1262 inside, ie. the high power version, with up to 22 dBm transmit power.
    // However, in Europe, ETSI limits the power to 16 dBm anyway, so it seems like useing the extra power of the SX1262 vs the SX1261 is not allowed...

    constexpr uint8_t nmbrExtraBytes{4};
    uint8_t parametersIn[nmbrExtraBytes], dataOut[nmbrExtraBytes];
    parametersIn[0] = 0x06;        // paDutyCycle - +15 dBm
    parametersIn[1] = 0x00;        // hpMax - no effect in case of SX1261 mode, only for SX1262
    parametersIn[2] = 0x01;        // 0x01 = SX1261, 0x00 = SX1262 mode
    parametersIn[3] = 0x01;        // reserved and always 0x01

    executeCommand(sx126xCommand::setPowerAmplifierConfig, parametersIn, nmbrExtraBytes);
}

void sx126x::writeBuffer(uint8_t* data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
    // Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
    executeCommand(sx126xCommand::writeBuffer, parametersIn, nmbrExtraBytes);
}

uint32_t sx126x::readBuffer(uint8_t* data, uint8_t dataLength, uint8_t startAddressOffset = 0) {
    // Problem : we need to insert the offset byte into the stream of data... maybe we need to make a local copy, but this buffer can be 256 bytes long..
    executeCommand(sx126xCommand::writeBuffer, parametersIn, nmbrExtraBytes);
}

// /* Includes ------------------------------------------------------------------*/
// #include "radio_conf.h"
// #include "radio_driver.h"
// #include "mw_log_conf.h"

// /* External variables ---------------------------------------------------------*/
// /*!
//  * \brief Sughz handler
//  */
// extern SUBGHZ_HandleTypeDef hsubghz;

// /* Private typedef -----------------------------------------------------------*/
// /*!
//  * FSK bandwidth definition
//  */
// typedef struct FskBandwidth_s
// {
//     uint32_t bandwidth;
//     uint8_t  RegValue;
// } FskBandwidth_t;
// /* Private define ------------------------------------------------------------*/
// /**
//   * @brief drive value used anytime radio is NOT in TX low power mode
//   * @note SMPS_DRIVE_SETTING_DEFAULT can be redefined in radio_conf.h
//   */
// #ifndef SMPS_DRIVE_SETTING_DEFAULT
// #define SMPS_DRIVE_SETTING_DEFAULT  SMPS_DRV_40
// #endif /* SMPS_DRIVE_SETTING_DEFAULT */

// /**
//   * @brief drive value used anytime radio is in TX low power mode
//   *        TX low power mode is the worst case because the PA sinks from SMPS
//   *        while in high power mode, current is sunk directly from the battery
//   * @note SMPS_DRIVE_SETTING_MAX can be redefined in radio_conf.h
//   */
// #ifndef SMPS_DRIVE_SETTING_MAX
// #define SMPS_DRIVE_SETTING_MAX      SMPS_DRV_60
// #endif /* SMPS_DRIVE_SETTING_MAX */

// /**
//   * @brief Provides the frequency of the chip running on the radio and the frequency step
//   * @remark These defines are used for computing the frequency divider to set the RF frequency
//   * @note XTAL_FREQ can be redefined in radio_conf.h
//   */
// #ifndef XTAL_FREQ
// #define XTAL_FREQ                   ( 32000000UL )
// #endif /* XTAL_FREQ */

// /**
//   * @brief in XO mode, set internal capacitor (from 0x00 to 0x2F starting 11.2pF with 0.47pF steps)
//   * @note XTAL_DEFAULT_CAP_VALUE can be redefined in radio_conf.h
//   */
// #ifndef XTAL_DEFAULT_CAP_VALUE
// #define XTAL_DEFAULT_CAP_VALUE      ( 0x20UL )
// #endif /* XTAL_DEFAULT_CAP_VALUE */

// /**
//   * @brief voltage of vdd tcxo.
//   * @note TCXO_CTRL_VOLTAGE can be redefined in radio_conf.h
//   */
// #ifndef TCXO_CTRL_VOLTAGE
// #define TCXO_CTRL_VOLTAGE           TCXO_CTRL_1_7V
// #endif /* TCXO_CTRL_VOLTAGE */

// /**
//   * @brief Radio maximum wakeup time (in ms)
//   * @note RF_WAKEUP_TIME can be redefined in radio_conf.h
//   */
// #ifndef RF_WAKEUP_TIME
// #define RF_WAKEUP_TIME              ( 10UL )
// #endif /* RF_WAKEUP_TIME */

// /**
//   * @brief DCDC is present and enabled
//   * @remark this define is only used if the DCDC is present on the board
//   * @note DCDC_ENABLE can be redefined in radio_conf.h
//   */
// #ifndef DCDC_ENABLE
// #define DCDC_ENABLE                 ( 1UL )
// #endif /* DCDC_ENABLE */

// /* Private macro -------------------------------------------------------------*/

// #define SX_FREQ_TO_CHANNEL( channel, freq )                                  \
// do                                                                           \
// {                                                                            \
//   channel = (uint32_t) ((((uint64_t) freq)<<25)/(XTAL_FREQ) );               \
// }while( 0 )

// /* Private variables ---------------------------------------------------------*/
// /*!
//  * \brief Holds the internal operating mode of the radio
//  */
// static RadioOperatingModes_t OperatingMode;

// /*!
//  * \brief Stores the current packet type set in the radio
//  */
// static RadioPacketTypes_t PacketType;

// /*!
//  * \brief Stores the current packet header type set in the radio
//  */
// static volatile RadioLoRaPacketLengthsMode_t LoRaHeaderType;

// /*!
//  * \brief Stores the last frequency error measured on LoRa received packet
//  */
// volatile uint32_t FrequencyError = 0;

// /*!
//  * \brief Hold the status of the Image calibration
//  */
// static bool ImageCalibrated = false;

// /*!
//  * Precomputed FSK bandwidth registers values
//  */
// static const FskBandwidth_t FskBandwidths[] =
// {
//     { 4800  , 0x1F },
//     { 5800  , 0x17 },
//     { 7300  , 0x0F },
//     { 9700  , 0x1E },
//     { 11700 , 0x16 },
//     { 14600 , 0x0E },
//     { 19500 , 0x1D },
//     { 23400 , 0x15 },
//     { 29300 , 0x0D },
//     { 39000 , 0x1C },
//     { 46900 , 0x14 },
//     { 58600 , 0x0C },
//     { 78200 , 0x1B },
//     { 93800 , 0x13 },
//     { 117300, 0x0B },
//     { 156200, 0x1A },
//     { 187200, 0x12 },
//     { 234300, 0x0A },
//     { 312000, 0x19 },
//     { 373600, 0x11 },
//     { 467000, 0x09 },
//     { 500000, 0x00 }, // Invalid Bandwidth
// };

// /* Private function prototypes -----------------------------------------------*/

// /*!
//  * \brief This set SMPS drive capability wrt. RF mode
//  */
// static void Radio_SMPS_Set( uint8_t level );

// /*!
//  * \brief IRQ Callback radio function
//  */
// static DioIrqHandler RadioOnDioIrqCb;

// /*!
//  * \brief Write command to the radio
//  *
//  * \param [in]  SetCommand    The Write Command
//  * \param [out] buffer        A pointer command buffer
//  * \param [in]  size          Size in byte of the command buffer
//  */
// static void SUBGRF_WriteCommand( SUBGHZ_RadioSetCmd_t Command, uint8_t *pBuffer,
//                                         uint16_t Size );
// /*!
//  * \brief Read command to the radio
//  *
//  * \param [in]  GetCommand    The Read Command
//  * \param [out] buffer        A pointer command buffer
//  * \param [in]  size          Size in byte of the command buffer
//  */
// static void SUBGRF_ReadCommand( SUBGHZ_RadioGetCmd_t Command, uint8_t *pBuffer,
//                                         uint16_t Size );

// /* Exported functions ---------------------------------------------------------*/
// void SUBGRF_Init( DioIrqHandler dioIrq )
// {
//     if ( dioIrq != NULL)
//     {
//         RadioOnDioIrqCb = dioIrq;
//     }

//     RADIO_INIT();

//     /* set default SMPS current drive to default*/
//     Radio_SMPS_Set(SMPS_DRIVE_SETTING_DEFAULT);

//     ImageCalibrated = false;

//     SUBGRF_SetStandby( STDBY_RC );

//     // Initialize TCXO control
//     if (1U == RBI_IsTCXO() )
//     {
//         SUBGRF_SetTcxoMode( TCXO_CTRL_VOLTAGE, RF_WAKEUP_TIME << 6 );// 100 ms
//         SUBGRF_WriteRegister( REG_XTA_TRIM, 0x00 );

//         /*enable calibration for cut1.1 and later*/
//         CalibrationParams_t calibParam;
//         calibParam.Value = 0x7F;
//         SUBGRF_Calibrate( calibParam );
//     }
//     else
//     {
//         SUBGRF_WriteRegister( REG_XTA_TRIM, XTAL_DEFAULT_CAP_VALUE );
//         SUBGRF_WriteRegister( REG_XTB_TRIM, XTAL_DEFAULT_CAP_VALUE );
//     }
//     /* Init RF Switch */
//     RBI_Init();

//     OperatingMode = MODE_STDBY_RC;
// }

// RadioOperatingModes_t SUBGRF_GetOperatingMode( void )
// {
//     return OperatingMode;
// }

// void SUBGRF_SetPayload( uint8_t *payload, uint8_t size )
// {
//     SUBGRF_WriteBuffer( 0x00, payload, size );
// }

// uint8_t SUBGRF_GetPayload( uint8_t *buffer, uint8_t *size,  uint8_t maxSize )
// {
//     uint8_t offset = 0;

//     SUBGRF_GetRxBufferStatus( size, &offset );
//     if( *size > maxSize )
//     {
//         return 1;
//     }
//     SUBGRF_ReadBuffer( offset, buffer, *size );

//     return 0;
// }

// void SUBGRF_SendPayload( uint8_t *payload, uint8_t size, uint32_t timeout)
// {
//     SUBGRF_SetPayload( payload, size );
//     SUBGRF_SetTx( timeout );
// }

// uint8_t SUBGRF_SetSyncWord( uint8_t *syncWord )
// {
//     SUBGRF_WriteRegisters( REG_LR_SYNCWORDBASEADDRESS, syncWord, 8 );
//     return 0;
// }

// void SUBGRF_SetCrcSeed( uint16_t seed )
// {
//     uint8_t buf[2];

//     buf[0] = ( uint8_t )( ( seed >> 8 ) & 0xFF );
//     buf[1] = ( uint8_t )( seed & 0xFF );

//     switch( SUBGRF_GetPacketType( ) )
//     {
//         case PACKET_TYPE_GFSK:
//             SUBGRF_WriteRegisters( REG_LR_CRCSEEDBASEADDR, buf, 2 );
//             break;

//         default:
//             break;
//     }
// }

// void SUBGRF_SetCrcPolynomial( uint16_t polynomial )
// {
//     uint8_t buf[2];

//     buf[0] = ( uint8_t )( ( polynomial >> 8 ) & 0xFF );
//     buf[1] = ( uint8_t )( polynomial & 0xFF );

//     switch( SUBGRF_GetPacketType( ) )
//     {
//         case PACKET_TYPE_GFSK:
//             SUBGRF_WriteRegisters( REG_LR_CRCPOLYBASEADDR, buf, 2 );
//             break;

//         default:
//             break;
//     }
// }

// void SUBGRF_SetWhiteningSeed( uint16_t seed )
// {
//     uint8_t regValue = 0;

//     switch( SUBGRF_GetPacketType( ) )
//     {
//         case PACKET_TYPE_GFSK:
//             regValue = SUBGRF_ReadRegister( REG_LR_WHITSEEDBASEADDR_MSB ) & 0xFE;
//             regValue = ( ( seed >> 8 ) & 0x01 ) | regValue;
//             SUBGRF_WriteRegister( REG_LR_WHITSEEDBASEADDR_MSB, regValue ); // only 1 bit.
//             SUBGRF_WriteRegister( REG_LR_WHITSEEDBASEADDR_LSB, (uint8_t)seed );
//             break;

//         default:
//             break;
//     }
// }

// uint32_t SUBGRF_GetRandom( void )
// {
//     uint32_t number = 0;
//     uint8_t regAnaLna = 0;
//     uint8_t regAnaMixer = 0;

//     regAnaLna = SUBGRF_ReadRegister( REG_ANA_LNA );
//     SUBGRF_WriteRegister( REG_ANA_LNA, regAnaLna & ~( 1 << 0 ) );

//     regAnaMixer = SUBGRF_ReadRegister( REG_ANA_MIXER );
//     SUBGRF_WriteRegister( REG_ANA_MIXER, regAnaMixer & ~( 1 << 7 ) );

//     // Set radio in continuous reception
//     SUBGRF_SetRx( 0xFFFFFF ); // Rx Continuous

//     SUBGRF_ReadRegisters( RANDOM_NUMBER_GENERATORBASEADDR, ( uint8_t* )&number, 4 );

//     SUBGRF_SetStandby( STDBY_RC );

//     SUBGRF_WriteRegister( REG_ANA_LNA, regAnaLna );
//     SUBGRF_WriteRegister( REG_ANA_MIXER, regAnaMixer );

//     return number;
// }

// void SUBGRF_SetSleep( SleepParams_t sleepConfig )
// {
//     /* switch the antenna OFF by SW */
//     RBI_ConfigRFSwitch(RBI_SWITCH_OFF);

//     Radio_SMPS_Set(SMPS_DRIVE_SETTING_DEFAULT);

//     uint8_t value = ( ( ( uint8_t )sleepConfig.Fields.WarmStart << 2 ) |
//                       ( ( uint8_t )sleepConfig.Fields.Reset << 1 ) |
//                       ( ( uint8_t )sleepConfig.Fields.WakeUpRTC ) );
//     SUBGRF_WriteCommand( RADIO_SET_SLEEP, &value, 1 );
//     OperatingMode = MODE_SLEEP;
// }

// void SUBGRF_SetStandby( RadioStandbyModes_t standbyConfig )
// {
//     SUBGRF_WriteCommand( RADIO_SET_STANDBY, ( uint8_t* )&standbyConfig, 1 );
//     if( standbyConfig == STDBY_RC )
//     {
//         OperatingMode = MODE_STDBY_RC;
//     }
//     else
//     {
//         OperatingMode = MODE_STDBY_XOSC;
//     }
// }

// void SUBGRF_SetFs( void )
// {
//     SUBGRF_WriteCommand( RADIO_SET_FS, 0, 0 );
//     OperatingMode = MODE_FS;
// }

// void SUBGRF_SetTx( uint32_t timeout )
// {
//     uint8_t buf[3];

//     OperatingMode = MODE_TX;

//     buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
//     buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
//     buf[2] = ( uint8_t )( timeout & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_TX, buf, 3 );
// }

// void SUBGRF_SetRx( uint32_t timeout )
// {
//     uint8_t buf[3];

//     OperatingMode = MODE_RX;

//     buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
//     buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
//     buf[2] = ( uint8_t )( timeout & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_RX, buf, 3 );
// }

// void SUBGRF_SetRxBoosted( uint32_t timeout )
// {
//     uint8_t buf[3];

//     OperatingMode = MODE_RX;

//     /* ST_WORKAROUND_BEGIN: Sigfox patch > 0x96 replaced by 0x97 */
//     SUBGRF_WriteRegister( REG_RX_GAIN, 0x97 ); // max LNA gain, increase current by ~2mA for around ~3dB in sensitivity
//     /* ST_WORKAROUND_END */

//     buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
//     buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
//     buf[2] = ( uint8_t )( timeout & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_RX, buf, 3 );
// }

// void SUBGRF_SetRxDutyCycle( uint32_t rxTime, uint32_t sleepTime )
// {
//     uint8_t buf[6];

//     buf[0] = ( uint8_t )( ( rxTime >> 16 ) & 0xFF );
//     buf[1] = ( uint8_t )( ( rxTime >> 8 ) & 0xFF );
//     buf[2] = ( uint8_t )( rxTime & 0xFF );
//     buf[3] = ( uint8_t )( ( sleepTime >> 16 ) & 0xFF );
//     buf[4] = ( uint8_t )( ( sleepTime >> 8 ) & 0xFF );
//     buf[5] = ( uint8_t )( sleepTime & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_RXDUTYCYCLE, buf, 6 );
//     OperatingMode = MODE_RX_DC;
// }

// void SUBGRF_SetCad( void )
// {
//     SUBGRF_WriteCommand( RADIO_SET_CAD, 0, 0 );
//     OperatingMode = MODE_CAD;
// }

// void SUBGRF_SetTxContinuousWave( void )
// {
//     SUBGRF_WriteCommand( RADIO_SET_TXCONTINUOUSWAVE, 0, 0 );
// }

// void SUBGRF_SetTxInfinitePreamble( void )
// {
//     SUBGRF_WriteCommand( RADIO_SET_TXCONTINUOUSPREAMBLE, 0, 0 );
// }

// void SUBGRF_SetStopRxTimerOnPreambleDetect( bool enable )
// {
//     SUBGRF_WriteCommand( RADIO_SET_STOPRXTIMERONPREAMBLE, ( uint8_t* )&enable, 1 );
// }

// void SUBGRF_SetLoRaSymbNumTimeout( uint8_t symbNum )
// {
//     SUBGRF_WriteCommand( RADIO_SET_LORASYMBTIMEOUT, &symbNum, 1 );

//     if( symbNum >= 64 )
//     {
//         uint8_t mant = symbNum >> 1;
//         uint8_t exp  = 0;
//         uint8_t reg  = 0;

//         while( mant > 31 )
//         {
//             mant >>= 2;
//             exp++;
//         }

//         reg = exp + ( mant << 3 );
//         SUBGRF_WriteRegister( REG_LR_SYNCH_TIMEOUT, reg );
//     }
// }

// void SUBGRF_SetRegulatorMode( void )
// {
//     /* ST_WORKAROUND_BEGIN: Get RegulatorMode value from RBI */
//     RadioRegulatorMode_t mode;

//     if ( ( 1UL == RBI_IsDCDC() ) && ( 1UL == DCDC_ENABLE ) )
//     {
//         mode = USE_DCDC ;
//     }
//     else
//     {
//         mode = USE_LDO ;
//     }
//     /* ST_WORKAROUND_END */
//     SUBGRF_WriteCommand( RADIO_SET_REGULATORMODE, ( uint8_t* )&mode, 1 );
// }

// void SUBGRF_Calibrate( CalibrationParams_t calibParam )
// {
//     uint8_t value = ( ( ( uint8_t )calibParam.Fields.ImgEnable << 6 ) |
//                       ( ( uint8_t )calibParam.Fields.ADCBulkPEnable << 5 ) |
//                       ( ( uint8_t )calibParam.Fields.ADCBulkNEnable << 4 ) |
//                       ( ( uint8_t )calibParam.Fields.ADCPulseEnable << 3 ) |
//                       ( ( uint8_t )calibParam.Fields.PLLEnable << 2 ) |
//                       ( ( uint8_t )calibParam.Fields.RC13MEnable << 1 ) |
//                       ( ( uint8_t )calibParam.Fields.RC64KEnable ) );

//     SUBGRF_WriteCommand( RADIO_CALIBRATE, &value, 1 );
// }

// void SUBGRF_CalibrateImage( uint32_t freq )
// {
//     uint8_t calFreq[2];

//     if( freq > 900000000 )
//     {
//         calFreq[0] = 0xE1;
//         calFreq[1] = 0xE9;
//     }
//     else if( freq > 850000000 )
//     {
//         calFreq[0] = 0xD7;
//         calFreq[1] = 0xDB;
//     }
//     else if( freq > 770000000 )
//     {
//         calFreq[0] = 0xC1;
//         calFreq[1] = 0xC5;
//     }
//     else if( freq > 460000000 )
//     {
//         calFreq[0] = 0x75;
//         calFreq[1] = 0x81;
//     }
//     else if( freq > 425000000 )
//     {
//         calFreq[0] = 0x6B;
//         calFreq[1] = 0x6F;
//     }
//     SUBGRF_WriteCommand( RADIO_CALIBRATEIMAGE, calFreq, 2 );
// }

// void SUBGRF_SetPaConfig( uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut )
// {
//     uint8_t buf[4];

//     buf[0] = paDutyCycle;
//     buf[1] = hpMax;
//     buf[2] = deviceSel;
//     buf[3] = paLut;
//     SUBGRF_WriteCommand( RADIO_SET_PACONFIG, buf, 4 );
// }

// void SUBGRF_SetRxTxFallbackMode( uint8_t fallbackMode )
// {
//     SUBGRF_WriteCommand( RADIO_SET_TXFALLBACKMODE, &fallbackMode, 1 );
// }

// void SUBGRF_SetDioIrqParams( uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask )
// {
//     uint8_t buf[8];

//     buf[0] = ( uint8_t )( ( irqMask >> 8 ) & 0x00FF );
//     buf[1] = ( uint8_t )( irqMask & 0x00FF );
//     buf[2] = ( uint8_t )( ( dio1Mask >> 8 ) & 0x00FF );
//     buf[3] = ( uint8_t )( dio1Mask & 0x00FF );
//     buf[4] = ( uint8_t )( ( dio2Mask >> 8 ) & 0x00FF );
//     buf[5] = ( uint8_t )( dio2Mask & 0x00FF );
//     buf[6] = ( uint8_t )( ( dio3Mask >> 8 ) & 0x00FF );
//     buf[7] = ( uint8_t )( dio3Mask & 0x00FF );
//     SUBGRF_WriteCommand( RADIO_CFG_DIOIRQ, buf, 8 );
// }

// uint16_t SUBGRF_GetIrqStatus( void )
// {
//     uint8_t irqStatus[2];

//     SUBGRF_ReadCommand( RADIO_GET_IRQSTATUS, irqStatus, 2 );
//     return ( irqStatus[0] << 8 ) | irqStatus[1];
// }

// void SUBGRF_SetTcxoMode (RadioTcxoCtrlVoltage_t tcxoVoltage, uint32_t timeout )
// {
//     uint8_t buf[4];

//     buf[0] = tcxoVoltage & 0x07;
//     buf[1] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
//     buf[2] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
//     buf[3] = ( uint8_t )( timeout & 0xFF );

//     SUBGRF_WriteCommand( RADIO_SET_TCXOMODE, buf, 4 );
// }

// void SUBGRF_SetRfFrequency( uint32_t frequency )
// {
//     uint8_t buf[4];
//     uint32_t chan = 0;

//     if( ImageCalibrated == false )
//     {
//         SUBGRF_CalibrateImage( frequency );
//         ImageCalibrated = true;
//     }
//     /* ST_WORKAROUND_BEGIN: Simplified frequency calculation */
//     SX_FREQ_TO_CHANNEL(chan, frequency);
//     /* ST_WORKAROUND_END */
//     buf[0] = ( uint8_t )( ( chan >> 24 ) & 0xFF );
//     buf[1] = ( uint8_t )( ( chan >> 16 ) & 0xFF );
//     buf[2] = ( uint8_t )( ( chan >> 8 ) & 0xFF );
//     buf[3] = ( uint8_t )( chan & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_RFFREQUENCY, buf, 4 );
// }

// void SUBGRF_SetPacketType( RadioPacketTypes_t packetType )
// {
//     // Save packet type internally to avoid questioning the radio
//     PacketType = packetType;

//     if( packetType == PACKET_TYPE_GFSK )
//     {
//         SUBGRF_WriteRegister( REG_BIT_SYNC, 0x00 );
//     }
//     SUBGRF_WriteCommand( RADIO_SET_PACKETTYPE, ( uint8_t* )&packetType, 1 );
// }

// RadioPacketTypes_t SUBGRF_GetPacketType( void )
// {
//     return PacketType;
// }

// void SUBGRF_SetTxParams( uint8_t paSelect, int8_t power, RadioRampTimes_t rampTime )
// {
//     uint8_t buf[2];

//     if( paSelect == RFO_LP )
//     {
//         if( power == 15 )
//         {
//             SUBGRF_SetPaConfig( 0x06, 0x00, 0x01, 0x01 );
//         }
//         else
//         {
//             SUBGRF_SetPaConfig( 0x04, 0x00, 0x01, 0x01 );
//         }
//         if( power >= 14 )
//         {
//             power = 14;
//         }
//         else if( power < -17 )
//         {
//             power = -17;
//         }
//         SUBGRF_WriteRegister( REG_OCP, 0x18 ); // current max is 80 mA for the whole device
//     }
//     else // rfo_hp
//     {
//         // WORKAROUND - Better Resistance of the SX1262 Tx to Antenna Mismatch, see DS_SX1261-2_V1.2 datasheet chapter 15.2
//         // RegTxClampConfig = @address 0x08D8
//         SUBGRF_WriteRegister( REG_TX_CLAMP, SUBGRF_ReadRegister( REG_TX_CLAMP ) | ( 0x0F << 1 ) );
//         // WORKAROUND END

//         SUBGRF_SetPaConfig( 0x04, 0x07, 0x00, 0x01 );
//         if( power > 22 )
//         {
//             power = 22;
//         }
//         else if( power < -9 )
//         {
//             power = -9;
//         }
//         SUBGRF_WriteRegister( REG_OCP, 0x38 ); // current max 160mA for the whole device
//     }
//     buf[0] = power;
//     buf[1] = ( uint8_t )rampTime;
//     SUBGRF_WriteCommand( RADIO_SET_TXPARAMS, buf, 2 );
// }

// void SUBGRF_SetModulationParams( ModulationParams_t *modulationParams )
// {
//     uint8_t n;
//     uint32_t tempVal = 0;
//     uint8_t buf[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//     // Check if required configuration corresponds to the stored packet type
//     // If not, silently update radio packet type
//     if( PacketType != modulationParams->PacketType )
//     {
//         SUBGRF_SetPacketType( modulationParams->PacketType );
//     }

//     switch( modulationParams->PacketType )
//     {
//     case PACKET_TYPE_GFSK:
//         n = 8;
//         tempVal = ( uint32_t )(( 32 * XTAL_FREQ ) / modulationParams->Params.Gfsk.BitRate );
//         buf[0] = ( tempVal >> 16 ) & 0xFF;
//         buf[1] = ( tempVal >> 8 ) & 0xFF;
//         buf[2] = tempVal & 0xFF;
//         buf[3] = modulationParams->Params.Gfsk.ModulationShaping;
//         buf[4] = modulationParams->Params.Gfsk.Bandwidth;
//         /* ST_WORKAROUND_BEGIN: Simplified frequency calculation */
//         SX_FREQ_TO_CHANNEL(tempVal, modulationParams->Params.Gfsk.Fdev);
//         /* ST_WORKAROUND_END */
//         buf[5] = ( tempVal >> 16 ) & 0xFF;
//         buf[6] = ( tempVal >> 8 ) & 0xFF;
//         buf[7] = ( tempVal& 0xFF );
//         SUBGRF_WriteCommand( RADIO_SET_MODULATIONPARAMS, buf, n );
//         break;
//     case PACKET_TYPE_BPSK:
//         n = 4;
//         tempVal = ( uint32_t ) (( 32 * XTAL_FREQ) / modulationParams->Params.Bpsk.BitRate );
//         buf[0] = ( tempVal >> 16 ) & 0xFF;
//         buf[1] = ( tempVal >> 8 ) & 0xFF;
//         buf[2] = tempVal & 0xFF;
//         buf[3] = modulationParams->Params.Bpsk.ModulationShaping;
//         SUBGRF_WriteCommand( RADIO_SET_MODULATIONPARAMS, buf, n );
//         break;
//     case PACKET_TYPE_LORA:
//         n = 4;
//         buf[0] = modulationParams->Params.LoRa.SpreadingFactor;
//         buf[1] = modulationParams->Params.LoRa.Bandwidth;
//         buf[2] = modulationParams->Params.LoRa.CodingRate;
//         buf[3] = modulationParams->Params.LoRa.LowDatarateOptimize;

//         SUBGRF_WriteCommand( RADIO_SET_MODULATIONPARAMS, buf, n );

//         break;
//     case PACKET_TYPE_GMSK:
//         n = 5;
//         tempVal = ( uint32_t )(( 32 *XTAL_FREQ) / modulationParams->Params.Gfsk.BitRate );
//         buf[0] = ( tempVal >> 16 ) & 0xFF;
//         buf[1] = ( tempVal >> 8 ) & 0xFF;
//         buf[2] = tempVal & 0xFF;
//         buf[3] = modulationParams->Params.Gfsk.ModulationShaping;
//         buf[4] = modulationParams->Params.Gfsk.Bandwidth;
//         SUBGRF_WriteCommand( RADIO_SET_MODULATIONPARAMS, buf, n );
//         break;
//     default:
//     case PACKET_TYPE_NONE:
//       break;
//     }
// }

// void SUBGRF_SetPacketParams( PacketParams_t *packetParams )
// {
//     uint8_t n;
//     uint8_t crcVal = 0;
//     uint8_t buf[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//     // Check if required configuration corresponds to the stored packet type
//     // If not, silently update radio packet type
//     if( PacketType != packetParams->PacketType )
//     {
//         SUBGRF_SetPacketType( packetParams->PacketType );
//     }

//     switch( packetParams->PacketType )
//     {
//     case PACKET_TYPE_GMSK:
//     case PACKET_TYPE_GFSK:
//         if( packetParams->Params.Gfsk.CrcLength == RADIO_CRC_2_BYTES_IBM )
//         {
//             SUBGRF_SetCrcSeed( CRC_IBM_SEED );
//             SUBGRF_SetCrcPolynomial( CRC_POLYNOMIAL_IBM );
//             crcVal = RADIO_CRC_2_BYTES;
//         }
//         else if( packetParams->Params.Gfsk.CrcLength == RADIO_CRC_2_BYTES_CCIT )
//         {
//             SUBGRF_SetCrcSeed( CRC_CCITT_SEED );
//             SUBGRF_SetCrcPolynomial( CRC_POLYNOMIAL_CCITT );
//             crcVal = RADIO_CRC_2_BYTES_INV;
//         }
//         else
//         {
//             crcVal = packetParams->Params.Gfsk.CrcLength;
//         }
//         n = 9;
//         buf[0] = ( packetParams->Params.Gfsk.PreambleLength >> 8 ) & 0xFF;
//         buf[1] = packetParams->Params.Gfsk.PreambleLength;
//         buf[2] = packetParams->Params.Gfsk.PreambleMinDetect;
//         buf[3] = ( packetParams->Params.Gfsk.SyncWordLength /*<< 3*/ ); // convert from byte to bit
//         buf[4] = packetParams->Params.Gfsk.AddrComp;
//         buf[5] = packetParams->Params.Gfsk.HeaderType;
//         buf[6] = packetParams->Params.Gfsk.PayloadLength;
//         buf[7] = crcVal;
//         buf[8] = packetParams->Params.Gfsk.DcFree;
//         break;
//     case PACKET_TYPE_BPSK:
//         n = 1;
//         buf[0] = packetParams->Params.Bpsk.PayloadLength;
//         break;
//     case PACKET_TYPE_LORA:
//         n = 6;
//         buf[0] = ( packetParams->Params.LoRa.PreambleLength >> 8 ) & 0xFF;
//         buf[1] = packetParams->Params.LoRa.PreambleLength;
//         buf[2] = LoRaHeaderType = packetParams->Params.LoRa.HeaderType;
//         buf[3] = packetParams->Params.LoRa.PayloadLength;
//         buf[4] = packetParams->Params.LoRa.CrcMode;
//         buf[5] = packetParams->Params.LoRa.InvertIQ;
//         break;
//     default:
//     case PACKET_TYPE_NONE:
//         return;
//     }
//     SUBGRF_WriteCommand( RADIO_SET_PACKETPARAMS, buf, n );
// }

// void SUBGRF_SetCadParams( RadioLoRaCadSymbols_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, RadioCadExitModes_t cadExitMode, uint32_t cadTimeout )
// {
//     uint8_t buf[7];

//     buf[0] = ( uint8_t )cadSymbolNum;
//     buf[1] = cadDetPeak;
//     buf[2] = cadDetMin;
//     buf[3] = ( uint8_t )cadExitMode;
//     buf[4] = ( uint8_t )( ( cadTimeout >> 16 ) & 0xFF );
//     buf[5] = ( uint8_t )( ( cadTimeout >> 8 ) & 0xFF );
//     buf[6] = ( uint8_t )( cadTimeout & 0xFF );
//     SUBGRF_WriteCommand( RADIO_SET_CADPARAMS, buf, 7 );
//     OperatingMode = MODE_CAD;
// }

// void SUBGRF_SetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress )
// {
//     uint8_t buf[2];

//     buf[0] = txBaseAddress;
//     buf[1] = rxBaseAddress;
//     SUBGRF_WriteCommand( RADIO_SET_BUFFERBASEADDRESS, buf, 2 );
// }

// RadioStatus_t SUBGRF_GetStatus( void )
// {
//     uint8_t stat = 0;
//     RadioStatus_t status = { .Value = 0 };

//     /* ST_WORKAROUND_BEGIN: Read the Device Status by the GET_STATUS command (HAL limitations) */
//     SUBGRF_ReadCommand( RADIO_GET_STATUS, &stat, 1 );
//     /* ST_WORKAROUND_END */
//     status.Fields.CmdStatus = ( stat & ( 0x07 << 1 ) ) >> 1;
//     status.Fields.ChipMode = ( stat & ( 0x07 << 4 ) ) >> 4;
//     return status;
// }

// int8_t SUBGRF_GetRssiInst( void )
// {
//     uint8_t buf[1];
//     int8_t rssi = 0;

//     SUBGRF_ReadCommand( RADIO_GET_RSSIINST, buf, 1 );
//     rssi = -buf[0] >> 1;
//     return rssi;
// }

// void SUBGRF_GetRxBufferStatus( uint8_t *payloadLength, uint8_t *rxStartBufferPointer )
// {
//     uint8_t status[2];

//     SUBGRF_ReadCommand( RADIO_GET_RXBUFFERSTATUS, status, 2 );

//     // In case of LORA fixed header, the payloadLength is obtained by reading
//     // the register REG_LR_PAYLOADLENGTH
//     if( ( SUBGRF_GetPacketType( ) == PACKET_TYPE_LORA ) && ( LoRaHeaderType == LORA_PACKET_FIXED_LENGTH ) )
//     {
//         *payloadLength = SUBGRF_ReadRegister( REG_LR_PAYLOADLENGTH );
//     }
//     else
//     {
//         *payloadLength = status[0];
//     }
//     *rxStartBufferPointer = status[1];
// }

// void SUBGRF_GetPacketStatus( PacketStatus_t *pktStatus )
// {
//     uint8_t status[3];

//     SUBGRF_ReadCommand( RADIO_GET_PACKETSTATUS, status, 3 );

//     pktStatus->packetType = SUBGRF_GetPacketType( );
//     switch( pktStatus->packetType )
//     {
//         case PACKET_TYPE_GFSK:
//             pktStatus->Params.Gfsk.RxStatus = status[0];
//             pktStatus->Params.Gfsk.RssiSync = -status[1] >> 1;
//             pktStatus->Params.Gfsk.RssiAvg = -status[2] >> 1;
//             pktStatus->Params.Gfsk.FreqError = 0;
//             break;

//         case PACKET_TYPE_LORA:
//             pktStatus->Params.LoRa.RssiPkt = -status[0] >> 1;
//             // Returns SNR value [dB] rounded to the nearest integer value
//             pktStatus->Params.LoRa.SnrPkt = ( ( ( int8_t )status[1] ) + 2 ) >> 2;
//             pktStatus->Params.LoRa.SignalRssiPkt = -status[2] >> 1;
//             pktStatus->Params.LoRa.FreqError = FrequencyError;
//             break;

//         default:
//         case PACKET_TYPE_NONE:
//             // In that specific case, we set everything in the pktStatus to zeros
//             // and reset the packet type accordingly
//             RADIO_MEMSET8( pktStatus, 0, sizeof( PacketStatus_t ) );
//             pktStatus->packetType = PACKET_TYPE_NONE;
//             break;
//     }
// }

// RadioError_t SUBGRF_GetDeviceErrors( void )
// {
//     uint8_t err[] = { 0, 0 };
//     RadioError_t error = { .Value = 0 };

//     SUBGRF_ReadCommand( RADIO_GET_ERROR, ( uint8_t * )err, 2 );
//     error.Fields.PaRamp     = ( err[0] & ( 1 << 0 ) ) >> 0;
//     error.Fields.PllLock    = ( err[1] & ( 1 << 6 ) ) >> 6;
//     error.Fields.XoscStart  = ( err[1] & ( 1 << 5 ) ) >> 5;
//     error.Fields.ImgCalib   = ( err[1] & ( 1 << 4 ) ) >> 4;
//     error.Fields.AdcCalib   = ( err[1] & ( 1 << 3 ) ) >> 3;
//     error.Fields.PllCalib   = ( err[1] & ( 1 << 2 ) ) >> 2;
//     error.Fields.Rc13mCalib = ( err[1] & ( 1 << 1 ) ) >> 1;
//     error.Fields.Rc64kCalib = ( err[1] & ( 1 << 0 ) ) >> 0;
//     return error;
// }

// void SUBGRF_ClearDeviceErrors( void )
// {
//     uint8_t buf[2] = { 0x00, 0x00 };
//     SUBGRF_WriteCommand( RADIO_CLR_ERROR, buf, 2 );
// }

// void SUBGRF_ClearIrqStatus( uint16_t irq )
// {
//     uint8_t buf[2];

//     buf[0] = ( uint8_t )( ( ( uint16_t )irq >> 8 ) & 0x00FF );
//     buf[1] = ( uint8_t )( ( uint16_t )irq & 0x00FF );
//     SUBGRF_WriteCommand( RADIO_CLR_IRQSTATUS, buf, 2 );
// }

// void SUBGRF_WriteRegister( uint16_t addr, uint8_t data )
// {
//     HAL_SUBGHZ_WriteRegisters( &hsubghz, addr, (uint8_t*)&data, 1 );
// }

// uint8_t SUBGRF_ReadRegister( uint16_t addr )
// {
//     uint8_t data;
//     HAL_SUBGHZ_ReadRegisters( &hsubghz, addr, &data, 1 );
//     return data;
// }

// void SUBGRF_WriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_WriteRegisters( &hsubghz, address, buffer, size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_ReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_ReadRegisters( &hsubghz, address, buffer, size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_WriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_WriteBuffer( &hsubghz, offset, buffer, size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_ReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_ReadBuffer( &hsubghz, offset, buffer, size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_WriteCommand( SUBGHZ_RadioSetCmd_t Command, uint8_t *pBuffer,
//                                         uint16_t Size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_ExecSetCmd( &hsubghz, Command, pBuffer, Size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_ReadCommand( SUBGHZ_RadioGetCmd_t Command, uint8_t *pBuffer,
//                                         uint16_t Size )
// {
//     CRITICAL_SECTION_BEGIN();
//     HAL_SUBGHZ_ExecGetCmd( &hsubghz, Command, pBuffer, Size );
//     CRITICAL_SECTION_END();
// }

// void SUBGRF_SetSwitch( uint8_t paSelect, RFState_t rxtx )
// {
//     RBI_Switch_TypeDef state = RBI_SWITCH_RX;

//     if (rxtx == RFSWITCH_TX)
//     {
//         if (paSelect == RFO_LP)
//         {
//             state = RBI_SWITCH_RFO_LP;
//             Radio_SMPS_Set(SMPS_DRIVE_SETTING_MAX);
//         }
//         if (paSelect == RFO_HP)
//         {
//             state = RBI_SWITCH_RFO_HP;
//         }
//     }
//     else
//     {
//         if (rxtx == RFSWITCH_RX)
//         {
//             state = RBI_SWITCH_RX;
//         }
//     }
//     RBI_ConfigRFSwitch(state);
// }

// uint8_t SUBGRF_SetRfTxPower( int8_t power )
// {
//     uint8_t paSelect= RFO_LP;

//     int32_t TxConfig = RBI_GetTxConfig();

//     switch (TxConfig)
//     {
//         case RBI_CONF_RFO_LP_HP:
//         {
//             if (power > 15)
//             {
//                 paSelect = RFO_HP;
//             }
//             else
//             {
//                 paSelect = RFO_LP;
//             }
//             break;
//         }
//         case RBI_CONF_RFO_LP:
//         {
//             paSelect = RFO_LP;
//             break;
//         }
//         case RBI_CONF_RFO_HP:
//         {
//             paSelect = RFO_HP;
//             break;
//         }
//         default:
//             break;
//     }

//     SUBGRF_SetTxParams( paSelect, power, RADIO_RAMP_40_US );

//     return paSelect;
// }

// uint32_t SUBGRF_GetRadioWakeUpTime( void )
// {
//     return RF_WAKEUP_TIME;
// }

// /* HAL_SUBGHz Callbacks definitions */
// void HAL_SUBGHZ_TxCpltCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_TX_DONE );
// }

// void HAL_SUBGHZ_RxCpltCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_RX_DONE );
// }

// void HAL_SUBGHZ_CRCErrorCallback (SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_CRC_ERROR);
// }

// void HAL_SUBGHZ_CADStatusCallback(SUBGHZ_HandleTypeDef *hsubghz, HAL_SUBGHZ_CadStatusTypeDef cadstatus)
// {
//     switch (cadstatus)
//     {
//         case HAL_SUBGHZ_CAD_CLEAR:
//             RadioOnDioIrqCb( IRQ_CAD_CLEAR);
//             break;
//         case HAL_SUBGHZ_CAD_DETECTED:
//             RadioOnDioIrqCb( IRQ_CAD_DETECTED);
//             break;
//         default:
//             break;
//     }
// }

// void HAL_SUBGHZ_RxTxTimeoutCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_RX_TX_TIMEOUT );
// }

// void HAL_SUBGHZ_HeaderErrorCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_HEADER_ERROR );
// }

// void HAL_SUBGHZ_PreambleDetectedCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_PREAMBLE_DETECTED );
// }

// void HAL_SUBGHZ_SyncWordValidCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_SYNCWORD_VALID );
// }

// void HAL_SUBGHZ_HeaderValidCallback(SUBGHZ_HandleTypeDef *hsubghz)
// {
//     RadioOnDioIrqCb( IRQ_HEADER_VALID );
// }

// static void Radio_SMPS_Set(uint8_t level)
// {
//   if ( 1U == RBI_IsDCDC() )
//   {
//     uint8_t modReg;
//     modReg= SUBGRF_ReadRegister(SUBGHZ_SMPSC2R);
//     modReg&= (~SMPS_DRV_MASK);
//     SUBGRF_WriteRegister(SUBGHZ_SMPSC2R, modReg | level);
//   }
// }

// uint8_t SUBGRF_GetFskBandwidthRegValue( uint32_t bandwidth )
// {
//     uint8_t i;

//     if( bandwidth == 0 )
//     {
//         return( 0x1F );
//     }

//     /* ST_WORKAROUND_BEGIN: Simplified loop */
//     for( i = 0; i < ( sizeof( FskBandwidths ) / sizeof( FskBandwidth_t ) ); i++ )
//     {
//         if ( bandwidth < FskBandwidths[i].bandwidth )
//         {
//             return FskBandwidths[i].RegValue;
//         }
//     }
//     /* ST_WORKAROUND_END */
//     // ERROR: Value not found
//     while( 1 );
// }
// void SUBGRF_GetCFO( uint32_t bitRate, int32_t *cfo)
// {
//   uint8_t BwMant[] = {4, 8, 10, 12};
//   /* read demod bandwidth: mant bit4:3, exp bits 2:0 */
//   uint8_t reg = (SUBGRF_ReadRegister( SUBGHZ_BWSEL ));
//   uint8_t bandwidth_mant = BwMant[( reg >> 3 ) & 0x3];
//   uint8_t bandwidth_exp = reg & 0x7;
//   uint32_t cf_fs = XTAL_FREQ / ( bandwidth_mant * ( 1 << ( bandwidth_exp - 1 )));
//   uint32_t cf_osr = cf_fs / bitRate;
//   uint8_t interp = 1;
//   /* calculate demod interpolation factor */
//   if (cf_osr * interp < 8)
//   {
//     interp = 2;
//   }
//   if (cf_osr * interp < 4)
//   {
//     interp = 4;
//   }
//   /* calculate demod sampling frequency */
//   uint32_t fs = cf_fs* interp;
//   /* get the cfo registers */
//   int32_t cfo_bin = ( SUBGRF_ReadRegister( SUBGHZ_CFO_H ) & 0xF ) << 8;
//   cfo_bin |= SUBGRF_ReadRegister( SUBGHZ_CFO_L );
//   /* negate if 12 bits sign bit is 1 */
//   if (( cfo_bin & 0x800 ) == 0x800 )
//   {
//     cfo_bin |= 0xFFFFF000;
//   }
//   /* calculate cfo in Hz */
//   /* shift by 5 first to not saturate, cfo_bin on 12bits */
//   *cfo = ((int32_t)( cfo_bin * ( fs >> 5 ))) >> ( 12 - 5 );
// }
// /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
