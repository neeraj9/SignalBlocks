// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_BLOCKTYPES_H
#define SIGNALBLOCKS_BLOCKTYPES_H

#include <string>

namespace sigblocks {
    enum BlockType {
        BLOCK_CHAR = 0,
        BLOCK_UCHAR,
        BLOCK_SHORT,
        BLOCK_USHORT,
        BLOCK_INT,
        BLOCK_UINT,
        BLOCK_LONG,
        BLOCK_ULONG,
        BLOCK_FLOAT,
        BLOCK_DOUBLE,
        BLOCK_LONG_DOUBLE,
        BLOCK_STRING,

        BLOCK_TYPE_INVALID
    };


    template <typename T>
    class TypeToBt {
    public:
        static BlockType ToType() {
            return BLOCK_TYPE_INVALID;
        }
    };

#define DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(T, BT) \
    template <> \
    class TypeToBt<T> { \
    public: \
        static BlockType ToType() { \
            return BT; \
        } \
    };

    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(char, BLOCK_CHAR)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(unsigned char, BLOCK_UCHAR)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(short, BLOCK_SHORT)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(unsigned short, BLOCK_USHORT)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(int, BLOCK_INT)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(unsigned int, BLOCK_UINT)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(long, BLOCK_LONG)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(unsigned long, BLOCK_ULONG)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(float, BLOCK_FLOAT)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(double, BLOCK_DOUBLE)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(long double, BLOCK_LONG_DOUBLE)
    DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT(std::string, BLOCK_STRING)

#undef DEFINE_TEMPLATE_SPECIALIZATION_TYPE_TO_BT

}

#endif //SIGNALBLOCKS_BLOCKTYPES_H
