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
#else
#define TXTLIB_API __declspec(dllimport)
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
public:
	CSpeechSynthesizer();
	~CSpeechSynthesizer();

	bool IsInitialized();

	void Initialize();

	void SpeakText(const char* text, bool async);

	void SetVoice(std::string* name);

	std::list<std::string*>* GetAvailableVoices();
};
