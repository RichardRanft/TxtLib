# TxtLib
Native C++ wrapper for MS Speech SDK 11
---------------------------------------------
Just a plain brown wrapper that makes it easy to access the Speech SDK from native C++.
So of course you'll need to install the speech platform runtimes and a language or two.  There is a weird issue where if you try to switch between the "Pro" voices and the normal ones you get stuck with one or the other.  Not sure why, except that the standard voices run on a 32 bit service and the "Pro" voices run on a 64 bit service and somehow they crap on each other.  "Thanks Microsoft!"
