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

#ifndef SIGNALBLOCKS_COMPLEXTYPE_H
#define SIGNALBLOCKS_COMPLEXTYPE_H

namespace signalblocks {

    /** ComplexType Represent complex numbers.
     *
     * @note At present the only possible value of T is double
     *       while others are not implemented for simplicity and
     *       something for later.
     */
    template<class T>
    struct ComplexType {
        ComplexType();

        ComplexType(T real, T imag);

        void SetReal(T val);

        void SetImag(T val);

        T GetReal() const;

        T GetImag() const;

        /// @return an array of size 2, where the first
        ///         one is the real number and second is the
        ///         imaginary part.
        const T* GetInternalData() const;

        T GetArg() const;

        T GetAbs() const;

        /// @return square of absolute value
        T GetAbs2() const;

        /// @return natural logarithm of absolute value
        T GetLogAbs() const;

        ComplexType<T> GetSqrt() const;

        const ComplexType& operator+=(const ComplexType& rhs);

        bool operator==(const ComplexType<T>& rhs) const;

        bool operator!=(const ComplexType<T>& rhc) const;

        bool operator<(const ComplexType& rhs) const;

        bool operator>(const ComplexType& rhs) const;

    private:
        T value[2]; // [0]real, [1]complex
    };
}

#endif // SIGNALBLOCKS_COMPLEXTYPE_H

