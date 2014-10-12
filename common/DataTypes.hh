// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_DataTypes_hh
#define  iblocks_common_DataTypes_hh

namespace iblocks
{
  template <class T>
  struct ComplexType
  {
    ComplexType();
    ComplexType(T real, T imag);

    void SetReal(T val);
    void SetImag(T val);
    T GetReal() const;
    T GetImag() const;

    T GetArg() const;
    T GetAbs() const;
    T GetAbs2() const;
    T GetLogAbs() const;
    ComplexType<T> GetSqrt() const;
    ComplexType<T> GetInverse() const;
    ComplexType<T> GetConjugate() const;
    ComplexType<T> GetExp() const;
    ComplexType<T> GetPower(const ComplexType<T>& power) const;
    ComplexType<T> GetPowerReal(T power) const;
    ComplexType<T> GetLog() const;
    ComplexType<T> GetLog10() const;
    ComplexType<T> GetLogN(const ComplexType<T>& n) const;
    ComplexType<T> GetSin() const;
    ComplexType<T> GetCos() const;
    ComplexType<T> GetTan() const;
    ComplexType<T> GetSec() const;
    ComplexType<T> GetCsc() const;
    ComplexType<T> GetCot() const;

    T value[2]; // [0]real, [1]complex
  };
}

#endif // iblocks_common_DataTypes_hh

