//-----------------------------------------
//------------------------------------------------
#ifndef NRFDS_H
#define NRFDS_H


extern uint8_t Ds18b20ReadByte(uint8_t DataIO);
extern void Ds18b20TempConvertCmd(uint8_t DataIO);
extern void Ds18b20TempReadCmd(uint8_t DataIO);
extern uint8_t* Ds18b20CalculateTemp(uint8_t DataIO);



#endif
