#ifndef MODBUS_H_
#define MODBUS_H_


extern const unsigned char  auchCRCHi[];
extern const unsigned char auchCRCLo[];
extern unsigned int crc16(unsigned char *puchMsg, unsigned int usDataLen);


#endif /* MODBUS_H_ */
