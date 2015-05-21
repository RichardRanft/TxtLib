// TxtLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TxtLib.h"


//// This is an example of an exported variable
//TXTLIB_API int nTxtLib=0;
//
//// This is an example of an exported function.
//TXTLIB_API int fnTxtLib(void)
//{
//	return 42;
//}
//
//// This is the constructor of a class that has been exported.
//// see TxtLib.h for the class definition
//CTxtLib::CTxtLib()
//{
//	return;
//}

CSpeechSynthesizer::CSpeechSynthesizer()
{
	m_initialized = false;
}

CSpeechSynthesizer::~CSpeechSynthesizer()
{
	cpVoice.Release();
	cpIEnum.Release();
	cpToken.Release();
}

bool CSpeechSynthesizer::IsInitialized()
{
	return m_initialized;
}

void CSpeechSynthesizer::Initialize()
{
	CoInitialize(cpIEnum);

	// Enumerate voice tokens that speak US English in a female voice.
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, L"Language=409", L"Gender=Female;", &cpIEnum);

	// Get the best matching token.
	if (SUCCEEDED(hr))
	{
		hr = cpIEnum->Next(1, &cpToken, NULL);
	}

	// Create a voice and set its token to the one we just found.
	if (SUCCEEDED(hr))
	{
		hr = cpVoice.CoCreateInstance(CLSID_SpVoice);
	}

	// Set the voice.
	if (SUCCEEDED(hr))
	{
		hr = cpVoice->SetVoice(cpToken);
	}

	// Set the output to the default audio device.
	if (SUCCEEDED(hr))
	{
		hr = cpVoice->SetOutput(NULL, TRUE);
	}

	m_initialized = true;
}

void CSpeechSynthesizer::SpeakText(const char* text, bool async = false)
{
	// Speak a string directly.
	if (m_initialized && text != NULL)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
		TCHAR wstrTo[MAX_PATH];
		MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &wstrTo[0], size_needed);
		if (async)
			cpVoice->Speak(wstrTo, SPF_ASYNC, 0);
		else
			cpVoice->Speak(wstrTo, SPF_DEFAULT, 0);
	}
}

// need to figure out how to do this specifically
void CSpeechSynthesizer::SetVoice(std::string* name)
{}

// need to work out a way of listing all available voices.
std::list<std::string*>* CSpeechSynthesizer::GetAvailableVoices()
{
	std::list<std::string*>* namelist = new std::list<std::string*>();

	CComPtr<IEnumSpObjectTokens> cpIEnumeration;

	// Enumerate voice tokens that speak US English in a female voice.
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, L"", L"", &cpIEnumeration);
	if (SUCCEEDED(hr))
	{
		unsigned long count = 0;
		unsigned long fetched = 0;
		cpIEnumeration->GetCount(&count);
		for (unsigned long i = 0; i < count; i++)
		{
			CComPtr<ISpObjectToken> cpTokenObj;
			cpIEnumeration->Next(1, &cpTokenObj, &fetched);
			wchar_t* desc = new wchar_t[256];
			SpGetDescription(cpTokenObj, &desc);
			char desctext[256] = { 0 };
			wcstombs(desctext, desc, 256);
			std::string* voiceName = new std::string(desctext);
			namelist->push_back(voiceName);
		}
	}

	return namelist;
}
