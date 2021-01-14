#pragma pack(1)
using BYTE = unsigned char;
using WORD = unsigned short;
struct InputFileHead {
  WORD year;        //дата год 0-1
  BYTE month;       //дата месяц 2
  BYTE day;         //дата день 3
  BYTE h;           //время часы 4
  BYTE min;         //время минуты 5
  BYTE sec;         //время секунды 6
  WORD ms;          //время миллисекунды 7-8
  BYTE version;     //версия шапки 9
  int freqSign;     //частота сигнала  10-13
  float ampl;       //амплитуда 14-17
  int  impSize;  //длительность приема АЦП 18-21
  double freqChan;   //Частота дискретизации канала 22-29
  BYTE range;       //диапазон //0-10B....5-0.2B 30
  int chanBufSize;      //количество отсчетов канала 31-34
  int chanNum; //номер канала 35-38
  BYTE reserve[25]; //резерв на всякий случай 39-63
};
#pragma pack()
