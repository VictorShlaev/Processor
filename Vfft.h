#pragma once


//QT
#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QFileDialog>
//stl and my
#include <map>
#include <functional>

#include <complex>
#include <vector>
#include <algorithm>

#include <QFile>
#include <QTextStream>

#pragma pack(1)
using BYTE = unsigned char;
using WORD = unsigned short;
//head ADC File
//64 bytes
//old
//struct InputFileHead {
//  WORD year;        //дата год 0-1
//  BYTE month;       //дата месяц 2
//  BYTE day;         //дата день 3
//  BYTE h;           //время часы 4
//  BYTE min;         //время минуты 5
//  BYTE sec;         //время секунды 6
//  WORD ms;          //время миллисекунды 7-8
//  BYTE version;     //версия шапки 9
//  int freqSign;     //частота сигнала  10-13
//  float ampl;       //амплитуда 14-17
//  float timeCadre;  //длительность приема АЦП 18-21
//  double freqChan;   //Частота дискретизации канала 22-29
//  BYTE range;       //диапазон //0-10B....5-0.2B 30
//  int chanBufSize;      //количество отсчетов канала 31-34
//  BYTE chanCnt; //количество каналов 35
//  BYTE reserve[28]; //резерв на всякий случай 36-63
//};

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







using complexArray  = std::vector<std::complex<double>>;

struct myFFT{
  double processHilbert(complexArray &arr);
  double processSpectrum(complexArray& arr, double& freq, double Fd, size_t kNf, size_t chanNum, complexArray& resArr);
  std::complex<double> processPhase(complexArray arr, double freq, double Fd, size_t kNf,complexArray& resArr);
  void test();
private:
  //вычисляет ближайший стпень двойки от размера массива
  size_t getDesLen(size_t arrSize);
  //БПФ O(nlogn)
  void fft( complexArray& arr, bool inv);

  double Hilbert (complexArray& arr);
  //вычисление амплитуды по алгоритму
  double getAmpl(std::vector<double>& mods);
  void writeArr(const complexArray& arr, QString path);
  void writeVec(const std::vector<double>& arr, QString path);

};



