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

#ifndef SIGBLOCKS_DATATYPES_H
#define  SIGBLOCKS_DATATYPES_H

namespace sigblocks {
    template<class T>
    struct ComplexType {
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

#endif // SIGBLOCKS_DATATYPES_H

