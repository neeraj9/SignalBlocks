// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#include "ComplexType.h"

#include <cmath>

using namespace sigblocks;


template<class T>
ComplexType<T>::ComplexType()
        : value() {
}

template<class T>
ComplexType<T>::ComplexType(T real, T imag)
        : value() {
    value[0] = real;
    value[1] = imag;
}

template<class T>
inline void
ComplexType<T>::SetReal(T val) {
    value[0] = val;
}

template<class T>
inline void
ComplexType<T>::SetImag(T val) {
    value[1] = val;
}

template<class T>
inline T
ComplexType<T>::GetReal() const {
    return value[0];
}

template<class T>
inline T
ComplexType<T>::GetImag() const {
    return value[1];
}

template<class T>
inline T ComplexType<T>::GetArg() const {
    if ((value[0] == 0.0) && (value[1] == 0.0)) {
        return 0.0;
    }
    return atan2(value[1], value[0]);
}

template<class T>
inline bool ComplexType<T>::operator==(const ComplexType<T>& rhs) {
    return ((value[0] == rhs.value[0]) && (value[1] == rhs.value[1]));
}

template<class T>
inline bool ComplexType<T>::operator!=(const ComplexType<T>& rhs) {
    return ((value[0] != rhs.value[0]) || (value[1] != rhs.value[1]));
}

template
class sigblocks::ComplexType<double>;

template
class sigblocks::ComplexType<float>;

template
class sigblocks::ComplexType<long long int>;

template
class sigblocks::ComplexType<long int>;

template
class sigblocks::ComplexType<int>;

template
class sigblocks::ComplexType<short int>;

template
class sigblocks::ComplexType<char>;
