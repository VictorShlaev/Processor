#include <Vfft.h>
#include<QDebug>


double myFFT::processHilbert(complexArray &arr)
{
  return Hilbert (arr);

}

double myFFT::processSpectrum(complexArray &arr, double& freq, double Fd, size_t kNf, size_t chanNum, complexArray& resArr)
{
  size_t nt= arr.size ();
  size_t nf = getDesLen (arr.size ())*kNf;
  arr.resize (nf);
  fft(arr, false);
  std::vector<double> absF (nf/2);
  std::transform(arr.begin (), arr.begin ()+nf/2, absF.begin(), [=](std::complex<double> val){
    return 2*abs(val)/nt;
  });
  int ng =  static_cast<int>(round (nf* freq/Fd));
  int delN =  static_cast<int>(round (nf* 500./Fd));
  size_t newSize = static_cast<size_t>(std::distance(arr.begin ()+ng-delN, arr.begin ()+ng+delN+1));
  resArr.resize (newSize);
  std::copy(arr.begin ()+ng-delN, arr.begin ()+ng+delN+1, resArr.begin ());
//если нужно знать позицию максимального
  size_t nMax =static_cast<size_t>(std::distance(absF.begin (),
                            std::max_element(absF.begin ()+ng-delN,  absF.begin ()+ng+delN+1)));
  if(chanNum ==0)
    freq = nMax*Fd/nf;


  return *std::max_element(absF.begin ()+ng-delN,  absF.begin ()+ng+delN+1);

}

std::complex<double> myFFT::processPhase(complexArray arr, double freq, double Fd, size_t kNf, complexArray &resArr)
{
  size_t nt= arr.size ();
  size_t nf = getDesLen (arr.size ())*kNf;
  arr.resize (nf);
  fft(arr, false);
  std::vector<double> absF (nf/2);
  std::transform(arr.begin (), arr.begin ()+nf/2, absF.begin(), [=](std::complex<double> val){
    return 2*abs(val)/nt;
  });
  int ng =  static_cast<int>(round (nf* freq/Fd));
  int  delN=  static_cast<int>(round (nf* 500/Fd));
  size_t newSize = static_cast<size_t>(std::distance(arr.begin ()+ng-delN, arr.begin ()+ng+delN+1));
  resArr.resize (newSize);
  std::copy(arr.begin ()+ng-delN, arr.begin ()+ng+delN+1, resArr.begin ());
  return arr[static_cast<size_t>(ng)];
}

void myFFT::test()
{
  const double Pi =  3.141592653589793238462643383279;
  size_t nf = 1024;
  int l =2;
  complexArray st (nf);
  for (size_t i =0; i < nf; ++i){
      st[i].real (cos (2*Pi*l*i/nf));
      st[i].imag (sin (2*Pi*l*i/nf));
    }
  fft(st, false);
  writeArr (st, "d://first.csv");

}

void myFFT::fft(complexArray &arr, bool inv)
{
  const double Pi =  3.141592653589793238462643383279;
  size_t n = arr.size ();
  if (n==1) return;
  complexArray  a0(n/2), a1(n/2);
  //прореживание на четные - нечетные
  for(size_t i = 0, j = 0; i < n; i+=2, ++j){
      a0[j]=arr[i];
      a1[j]=arr[i+1];
    }
  //рекурсивно делим обе части до элементов единичного размера
  fft(a0, inv);
  fft(a1, inv);
  //Взятие комплексных корней
  //если указано обратное то число берется сопряженное
  double ang = 2*Pi/n*(inv ? 1 : -1); //радиотехник ты или акустик тут решается
  std::complex<double> w(1), wn(cos(ang), sin(ang));
  //трансформация бабочки
  for (size_t i = 0; i < n/2; ++i){
      arr[i] = a0[i] + w* a1[i];
      arr[i+n/2] = a0[i] - w*a1[i];
      //если указано обратное БПФ то каждый элемент делится на 2, в итоге на каждом уровне рекурсии произойдет деление на n
      if(inv){
          arr[i] /=2;
          arr[i+n/2] /=2;
        }
      w*=wn;
    }
}

double myFFT::Hilbert(complexArray &arr)
{
  size_t nt = arr.size ();
  size_t nf = getDesLen (arr.size());
  arr.resize(nf);
  fft(arr, false);
  for(size_t i = 1; i < nf/2; ++i) arr[i] *= 2;
  for(size_t i = nf/2+1; i < nf; ++i) arr[i] = {0 ,0};
 fft(arr, true);
// emit sendArr (arr);
  std::vector<double> mods (nt);
  for(size_t i = 0; i < nt; ++i){
      mods[i]  = abs(arr[i]);
    }
  return getAmpl (mods);
}



double myFFT::getAmpl(std::vector<double> &mods)
{
  double sum = std::accumulate(mods.begin (), mods.end (),0.0);
  return sum/mods.size ();

}

void myFFT::writeArr(const complexArray &arr, QString path)
{
  QFile fOut (path);
  if(fOut.open (QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream ts(&fOut);
        for(const auto& val: arr){
            ts<<QString("%L1").arg(val.real ())<<";" <<QString("%L1").arg(val.imag ())<<"\n";
          }
      fOut.close();

    }
}

void myFFT::writeVec(const std::vector<double> &arr, QString path)
{
  QFile fOut (path);
  if(fOut.open (QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream ts(&fOut);
        for(const auto& val: arr){
            ts<<QString("%L1").arg(val)<<"\n";
          }
      fOut.close();
    }
}

size_t myFFT::getDesLen(size_t arrSize)
{
  size_t n = 1;
  while(n<arrSize) n <<=1;
  return n;
}

