#include "aes.h"

void aes::encryptPayload() {}
void aes::generateMIC() {}
bool aes::validateMIC() { return false; }

void aes::encryptBlock() {
    uint8_t rowIndex, columnIndex, round = 0;

    //  Copy input to state arry
    for (columnIndex = 0; columnIndex < 4; columnIndex++) {
        for (rowIndex = 0; rowIndex < 4; rowIndex++) {
            state[rowIndex][columnIndex] = Data[rowIndex + (columnIndex << 2)];
        }
    }

    //  Copy key to round key
    memcpy(&roundKey[0], &Key[0], 16);

    addRoundKey();

    //  Perform 9 full rounds with mixed columns
    for (round = 1; round < 10; round++) {
        //  Perform Byte substitution with S table
        for (columnIndex = 0; columnIndex < 4; columnIndex++) {
            for (rowIndex = 0; rowIndex < 4; rowIndex++) {
                state[rowIndex][columnIndex] = subByte(state[rowIndex][columnIndex]);
            }
        }

        shiftRows();
        mixColumns();
        calculateRoundKey(round);
        addRoundKey();
    }

    //  Perform Byte substitution with S table whitout mix collums
    for (columnIndex = 0; columnIndex < 4; columnIndex++) {
        for (rowIndex = 0; rowIndex < 4; rowIndex++) {
            state[rowIndex][columnIndex] = subByte(state[rowIndex][columnIndex]);
        }
    }

    shiftRows();
    calculateRoundKey(round);
    addRoundKey();

    //  Copy the state into the data array
    for (columnIndex = 0; columnIndex < 4; columnIndex++) {
        for (rowIndex = 0; rowIndex < 4; rowIndex++) {
            Data[rowIndex + (columnIndex << 2)] = state[rowIndex][columnIndex];
        }
    }
}

void aes::addRoundKey() {
    uint8_t rowIndex, columnIndex;

    for (columnIndex = 0; columnIndex < 4; columnIndex++) {
        for (rowIndex = 0; rowIndex < 4; rowIndex++) {
            state[rowIndex][columnIndex] ^= roundKey[rowIndex + (columnIndex << 2)];
        }
    }
}

uint8_t aes::subByte(uint8_t aByte) {
    return sTable[((aByte >> 4) & 0x0F)][((aByte >> 0) & 0x0F)];
}

void aes::shiftRows() {
    uint8_t tmpStorage;

    tmpStorage  = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = tmpStorage;

    tmpStorage  = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = tmpStorage;

    tmpStorage  = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = tmpStorage;

    tmpStorage  = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = tmpStorage;
}

void aes::mixColumns() {
    uint8_t rowIndex, columnIndex;
    uint8_t a[4], b[4];

    for (columnIndex = 0; columnIndex < 4; columnIndex++) {
        for (rowIndex = 0; rowIndex < 4; rowIndex++) {
            a[rowIndex] = state[rowIndex][columnIndex];
            b[rowIndex] = (state[rowIndex][columnIndex] << 1);

            if ((state[rowIndex][columnIndex] & 0x80) == 0x80) {
                b[rowIndex] ^= 0x1B;
            }
        }

        state[0][columnIndex] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
        state[1][columnIndex] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
        state[2][columnIndex] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
        state[3][columnIndex] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
    }
}

void aes::calculateRoundKey(uint8_t Round) {
    uint8_t i, j, b, Rcon;
    uint8_t Temp[4];

    // Calculate Rcon
    Rcon = 0x01;
    while (Round != 1) {
        b    = Rcon & 0x80;
        Rcon = Rcon << 1;

        if (b == 0x80) {
            Rcon ^= 0x1b;
        }
        Round--;
    }

    //  Calculate first Temp
    //  Copy laste byte from previous key and substitute the byte, but shift the array contents around by 1.
    Temp[0] = subByte(roundKey[12 + 1]);
    Temp[1] = subByte(roundKey[12 + 2]);
    Temp[2] = subByte(roundKey[12 + 3]);
    Temp[3] = subByte(roundKey[12 + 0]);

    //  XOR with Rcon
    Temp[0] ^= Rcon;

    //  Calculate new key
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            roundKey[j + (i << 2)] ^= Temp[j];
            Temp[j] = roundKey[j + (i << 2)];
        }
    }
}
