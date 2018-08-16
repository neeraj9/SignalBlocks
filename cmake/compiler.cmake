# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# This file is part of SignalBlocks.
#
# SignalBlocks is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SignalBlocks is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
#


# ##################################################################################
# Compiler Settings
# ##################################################################################

# make reorder warning to failure
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=reorder -Werror=sign-compare")

if (USE_GNU_CXX17_BUILD)

    # ------------ option-2: use gnu g++ with libstdc++ ------
    # You can always revert to libstdc++ when using gnu c++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

else()

    if (USE_LLVM_WITH_LIBCXX_LIBCXXABI)
        # this will only work if the libc++ library is installed
        # as well

        # ------------ option-1: use llvm+clang with libc++ linked to libc++abi ------
        #set(LLVM_FOUND YES)
        #include(EnableLLVM.cmake)

        # uncomment the following to generate .bc files for every .cpp file
        string(REPLACE "<OBJECT>" "<OBJECT>.bc"
                generate_bc ${CMAKE_CXX_COMPILE_OBJECT})
        set(CMAKE_CXX_COMPILE_OBJECT
                "${CMAKE_CXX_COMPILE_OBJECT} && ${generate_bc} -emit-llvm")
        # the default compiler is c++, which is typically g++ (gnu c++ compiler)
        # lets use clang
        set(CMAKE_CXX_COMPILER           "clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
        # if we dont provide -lc++abi then the following error is displayed
        # libc++abi.so.1, needed by /..<llvm-path>../llvm/bin/../lib64/libc++.so
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
        # Since clang is used, so lets use libc++ instead of libstdc++ as well :)

        # ------------ option-4: use llvm+clang with libc++ linked to libc++abi ------
        # The compiler libc++ needs to be built for static instead of shared (default).
        # Since this is never the case so lets stick with shared option only.
        # -----------------------------------------------------------

    else()

        # ------------ option-3: use llvm+clang with libstdc++ ------
        set(CMAKE_CXX_COMPILER           "clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

    endif(USE_LLVM_WITH_LIBCXX_LIBCXXABI)

endif(USE_GNU_CXX17_BUILD)
