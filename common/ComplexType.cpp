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

using namespace signalblocks;


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
void
ComplexType<T>::SetReal(T val) {
    value[0] = val;
}

template<class T>
void
ComplexType<T>::SetImag(T val) {
    value[1] = val;
}

template<class T>
T
ComplexType<T>::GetReal() const {
    return value[0];
}

template<class T>
T
ComplexType<T>::GetImag() const {
    return value[1];
}

template<class T>
const T*
ComplexType<T>::GetInternalData() const {
    return &value[0];
}

template<class T>
T ComplexType<T>::GetArg() const {
    if ((value[0] == 0.0) && (value[1] == 0.0)) {
        return 0.0;
    }
    return atan2(value[1], value[0]);
}

template<class T>
T ComplexType<T>::GetAbs() const {
    T square_value = (value[0] * value[0]) + (value[1] * value[1]);
    T result = sqrt(square_value);
    return result;
}

template<class T>
T ComplexType<T>::GetAbs2() const {
    T square_value = (value[0] * value[0]) + (value[1] * value[1]);
    return square_value;
}

template<class T>
T ComplexType<T>::GetLogAbs() const {
    return log(this->GetAbs());
}

template<class T>
ComplexType<T> ComplexType<T>::GetSqrt() const {
    T r = this->GetAbs();
    double theta = this->GetArg();
    T sqrt_r = sqrt(r);
    return ComplexType<T>(sqrt_r * cos(theta / 2),
                          sqrt_r * sin(theta / 2));
}

template<class T>
const ComplexType<T>& ComplexType<T>::operator+=(const ComplexType& rhs) {
    value[0] += rhs.value[0];
    value[1] += rhs.value[1];
    return *this;
}

template<class T>
bool ComplexType<T>::operator==(const ComplexType<T>& rhs) const {
    return ((value[0] == rhs.value[0]) && (value[1] == rhs.value[1]));
}

template<class T>
bool ComplexType<T>::operator!=(const ComplexType<T>& rhs) const {
    return ((value[0] != rhs.value[0]) || (value[1] != rhs.value[1]));
}

template<class T>
bool
ComplexType<T>::operator<(const ComplexType& rhs) const {
    if (!operator==(rhs)) {
        if (value[0] < rhs.value[0]) {
            return true;
        } else {
            return (value[1] < rhs.value[1]);
        }
    }
    return false;
}

template<class T>
bool
ComplexType<T>::operator>(const ComplexType& rhs) const {
    if (!operator==(rhs)) {
        if (value[0] > rhs.value[0]) {
            return true;
        } else {
            return (value[1] > rhs.value[0]);
        }
    }
    return false;
}


// only allow double (for now)
template
class signalblocks::ComplexType<double>;