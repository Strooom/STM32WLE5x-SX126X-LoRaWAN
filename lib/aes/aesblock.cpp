#include "aesblock.h"

void aesBlock::set(uint8_t bytes[16]) {
}
void aesBlock::set(uint32_t words[4]) {
}
void aesBlock::encrypt(aesKey &key) {
}
uint8_t *aesBlock::getAsBytes() {
}
uint8_t *aesBlock::getAsWords() {
}


/*

Initialize AES :
* clear EN bit in AES_CR
* configure EAS mode : encryption - mode = 0b00
* chaning mode : ECB - mode = 0b00
* datatype : 32 bit - datatype = 0b00
* key size : 128 bit - keysize = 0b00
* write key in AES_KEYRx registers


Encryption - polling mode
* write 4 words into AES_DINR register - 
* wait until CCF in AES_SR is set
* read 4 words from AES_DOUTR register


Disable AES peripheral by clearing EN bit in AES_CR













*/