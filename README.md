# TxtLib
Native C++ wrapper for MS Speech SDK 11
---------------------------------------------
Just a plain brown wrapper that makes it easy to access the Speech SDK from native C++.
So of course you'll need to install the speech platform runtimes and a language or two.  There is a weird issue where if you try to switch between the "Pro" voices and the normal ones you get stuck with one or the other.  Not sure why, except that the standard voices run on a 32 bit service and the "Pro" voices run on a 64 bit service and somehow they crap on each other.  "Thanks Microsoft!

// Copyright © 2015 Roostertail Games

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

