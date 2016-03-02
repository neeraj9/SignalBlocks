// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "DataTypes.hh"

#include <cmath>

using namespace sigblocks;

template <class T>
ComplexType<T>::ComplexType()
{
  value[0] = 0.0;
  value[1] = 0.0;
}

template <class T>
ComplexType<T>::ComplexType(T real, T imag)
{
  value[0] = real;
  value[1] = imag;
}

template <class T>
inline void
ComplexType<T>::SetReal(T val)
{
  value[0] = val;
}

template <class T>
inline void
ComplexType<T>::SetImag(T val)
{
  value[1] = val;
}

template <class T>
inline T
ComplexType<T>::GetReal() const
{
  return value[0];
}

template <class T>
inline T
ComplexType<T>::GetImag() const
{
  return value[1];
}

template <class T>
inline T ComplexType<T>::GetArg() const
{
  if ((value[0] == 0.0) && (value[1] == 0.0))
  {
    return 0.0;
  }
  return atan2(value[1], value[0]);
}

template class sigblocks::ComplexType<double>;
template class sigblocks::ComplexType<float>;
template class sigblocks::ComplexType<long long int>;
template class sigblocks::ComplexType<long int>;
template class sigblocks::ComplexType<int>;
template class sigblocks::ComplexType<short int>;
template class sigblocks::ComplexType<char>;
