#pragma once
#include <iostream>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace makerfaire
{
    namespace fxat
    {
        using namespace std;
        using namespace boost::archive::iterators;
        using InputItr  = istreambuf_iterator<char>;
        using OutputItr = ostream_iterator<char>;
        using EncodeItr = base64_from_binary<transform_width<InputItr, 6, 8>>;
        using DecodeItr = transform_width<binary_from_base64<InputItr>, 8, 6, char>;
        
        template <typename InputStream, typename OutputStream>
        inline OutputStream& encode(InputStream& is, OutputStream& os) {
            copy(EncodeItr(InputItr(is)), EncodeItr(InputItr()), OutputItr(os));
            return os;
        }
        
        template <typename InputStream, typename OutputStream>
        inline OutputStream& decode(InputStream& is, OutputStream& os) {
            copy(DecodeItr(InputItr(is)), DecodeItr(InputItr()), OutputItr(os));
            return os;
        }
    }
}
