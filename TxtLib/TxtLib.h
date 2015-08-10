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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TXTLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TXTLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TXTLIB_EXPORTS
#define TXTLIB_API __declspec(dllexport)
#include <Windows.h>
#include <sapi.h>
#include <sperror.h>
#include <sphelper.h>
#include <string>
#include <list>
#include <map>
#else
#define TXTLIB_API __declspec(dllimport)
#include <string>
#include <list>
#include <map>
#include <tchar.h>
#endif

//// This class is exported from the TxtLib.dll
//class TXTLIB_API CTxtLib {
//public:
//	CTxtLib(void);
//	// TODO: add your methods here.
//};
//
//extern TXTLIB_API int nTxtLib;
//
//TXTLIB_API int fnTxtLib(void);

class TXTLIB_API CSpeechSynthesizer
{
private:
#ifdef TXTLIB_EXPORTS
	CComPtr<IEnumSpObjectTokens> cpIEnum;
	CComPtr<ISpObjectToken> cpToken;
	CComPtr<ISpVoice> cpVoice;
	bool m_initialized;
#endif
	std::map<std::string*, std::string*>* m_registryList;
	std::map<std::string*, std::string*>* m_tokenMap;

	bool resetVoice(std::string* name);
	void getVoicesInRegistry();
public:
	CSpeechSynthesizer();
	~CSpeechSynthesizer();

	bool IsInitialized();

	void Initialize();

	void SpeakText(const char* text, bool async);

	bool SetVoice(std::string* name);

	std::string GetVoice();

	std::list<std::string*>* GetAvailableVoices();
};
