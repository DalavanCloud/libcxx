//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits = char_traits<charT> >
//   class basic_istream;

// operator>>(long double& val);

#include <istream>
#include <cassert>

template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_string<CharT> string_type;
    typedef std::basic_streambuf<CharT> base;
private:
    string_type str_;
public:

    testbuf() {}
    testbuf(const string_type& str)
        : str_(str)
    {
        base::setg(const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()) + str_.size());
    }

    CharT* eback() const {return base::eback();}
    CharT* gptr() const {return base::gptr();}
    CharT* egptr() const {return base::egptr();}
};

int main(int, char**)
{
    {
        std::istream is((std::streambuf*)0);
        long double n = 0;
        is >> n;
        assert(is.fail());
    }
    {
        testbuf<char> sb("0");
        std::istream is(&sb);
        long double n = 10;
        is >> n;
        assert(n == 0);
        assert( is.eof());
        assert(!is.fail());
    }
    {
        testbuf<char> sb(" 123 ");
        std::istream is(&sb);
        long double n = 10;
        is >> n;
        assert(n == 123);
        assert(!is.eof());
        assert(!is.fail());
    }
    {
        testbuf<wchar_t> sb(L" -123.5 ");
        std::wistream is(&sb);
        long double n = 10;
        is >> n;
        assert(n == -123.5);
        assert(!is.eof());
        assert(!is.fail());
    }

  return 0;
}
