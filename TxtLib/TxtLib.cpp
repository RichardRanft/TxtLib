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
	m_voiceLangMap->clear();
	delete(m_voiceLangMap);
	m_voiceLangMap = nullptr;

	m_langList->clear();
	delete(m_langList);
	m_langList = nullptr;
	
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
	// Language=C09;Language=809;Language=409;Language=411;Language=412;Language=804;Language=C04;Language=404;
	m_langList = new std::list < std::string* >();
	m_langList->push_back(new std::string("Language=403"));		// Catalan Spain
	m_langList->push_back(new std::string("Language=406"));		// Danish Denmark
	m_langList->push_back(new std::string("Language=C07"));		// German Germany
	m_langList->push_back(new std::string("Language=C09"));		// English Australia
	m_langList->push_back(new std::string("Language=1009")); 	// English Canada
	m_langList->push_back(new std::string("Language=809"));		// English Great Britain
	m_langList->push_back(new std::string("Language=4009")); 	// English India
	m_langList->push_back(new std::string("Language=409"));		// English United States
	m_langList->push_back(new std::string("Language=2C0A")); 	// Spanish Spain
	m_langList->push_back(new std::string("Language=80A"));		// Spanish Mexico
	m_langList->push_back(new std::string("Language=40B"));		// Finnish Finland
	m_langList->push_back(new std::string("Language=C0C"));		// French Canada
	m_langList->push_back(new std::string("Language=40C"));		// French France
	m_langList->push_back(new std::string("Language=410"));		// Italian Italy
	m_langList->push_back(new std::string("Language=411"));		// Japanese Japan
	m_langList->push_back(new std::string("Language=412"));		// Korean Korea
	m_langList->push_back(new std::string("Language=414"));		// Norwegian Norway
	m_langList->push_back(new std::string("Language=813"));		// Dutch Netherlands
	m_langList->push_back(new std::string("Language=415"));		// Polish Poland
	m_langList->push_back(new std::string("Language=416"));		// Portuguese Brazil
	m_langList->push_back(new std::string("Language=816"));		// Portuguese Portugal
	m_langList->push_back(new std::string("Language=419"));		// Russian Russia
	m_langList->push_back(new std::string("Language=81D"));		// Swedish Sweden
	m_langList->push_back(new std::string("Language=804"));		// Chinese China
	m_langList->push_back(new std::string("Language=C04"));		// Chinese Hong Kong
	m_langList->push_back(new std::string("Language=404"));		// Chinese Taiwan

	m_voiceLangMap = new std::map<std::string*, std::string*>();

	CoInitialize(cpIEnum);

	// Enumerate voice tokens that speak US English in a female voice.
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, L"", L"Gender=Female;", &cpIEnum);

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
		TCHAR wstrTo[1024] = { 0 };
		MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &wstrTo[0], size_needed);
		if (async)
			cpVoice->Speak(wstrTo, SPF_ASYNC, 0);
		else
			cpVoice->Speak(wstrTo, SPF_DEFAULT, 0);
	}
}

// need to figure out how to do this specifically
bool CSpeechSynthesizer::SetVoice(std::string* name)
{
	// if we're not initialized return false
	if (!m_initialized)
		return false;
	bool result = false;

	TCHAR lang[13] = { 0 };

	// first, check m_voiceLangMap for this voice name and retrieve the correct language
	for (std::map<std::string*, std::string*>::iterator it = m_voiceLangMap->begin(); it != m_voiceLangMap->end(); it++)
	{
		if (strcmp((*it).first->c_str(), name->c_str()) == 0)
		{
			const char* text = (*it).second->c_str();
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
			MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &lang[0], size_needed);
			break;
		}
	}
	// now 
	CComPtr<IEnumSpObjectTokens> cpIEnumeration;

	// Enumerate voice tokens
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, lang, L"", &cpIEnumeration);
	if (SUCCEEDED(hr))
	{
		unsigned long count = 0;
		unsigned long fetched = 0;
		cpIEnumeration->GetCount(&count);
		for (unsigned long i = 0; i < count; i++)
		{
			CComPtr<ISpObjectToken> cpTokenObj;
			hr = cpIEnumeration->Next(1, &cpTokenObj, &fetched);

			wchar_t* desc = new wchar_t[256];
			SpGetDescription(cpTokenObj, &desc);
			char desctext[256] = { 0 };
			wcstombs(desctext, desc, 256);
			std::string* voiceName = new std::string(desctext);
			if (strcmp(voiceName->c_str(), name->c_str()) == 0)
			{
				result = resetVoice(name);
				break;
			}
		}
	}

	return result;
}

std::string CSpeechSynthesizer::GetVoice()
{
	wchar_t* desc = new wchar_t[256];
	SpGetDescription(cpToken, &desc);
	char desctext[256] = { 0 };
	wcstombs(desctext, desc, 256);
	std::string voiceName = std::string(desctext);
	return voiceName;
}

std::list<std::string*>* CSpeechSynthesizer::GetAvailableVoices()
{
	std::list<std::string*>* namelist = new std::list<std::string*>();
	// if we're not initialized return an empty list
	if (!m_initialized)
		return namelist;

	for (std::list<std::string*>::iterator it = m_langList->begin(); it != m_langList->end(); it++)
	{
		CComPtr<IEnumSpObjectTokens> cpIEnumeration;
		CoInitialize(cpIEnumeration);

		// Enumerate voice tokens
		const char* text = (*it)->c_str();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
		TCHAR lang[13] = { 0 };
		MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &lang[0], size_needed);
		HRESULT hr = SpEnumTokens(SPCAT_VOICES, lang, L"", &cpIEnumeration);
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
				m_voiceLangMap->insert(std::pair<std::string*, std::string*>(voiceName, (*it)));
				if (cpTokenObj != NULL)
					cpTokenObj.Release();
			}
		}
		if (cpIEnumeration != NULL)
			cpIEnumeration.Release();
	}

	return namelist;
}

bool CSpeechSynthesizer::resetVoice(std::string* name)
{
	cpVoice.Release();
	cpToken.Release();
	cpIEnum.Release();

	CoInitialize(cpIEnum);

	// Enumerate voice tokens that speak US English in a female voice.
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, L"", L"", &cpIEnum);

	// Get the best matching token.
	if (SUCCEEDED(hr))
	{
		unsigned long count = 0;
		cpIEnum->GetCount(&count);
		for (unsigned long i = 0; i < count; i++) 
		{
			if (cpToken != NULL)
				cpToken.Release();
			hr = cpIEnum->Next(1, &cpToken, NULL);

			wchar_t* desc = new wchar_t[256];
			SpGetDescription(cpToken, &desc);
			char desctext[256] = { 0 };
			wcstombs(desctext, desc, 256);
			std::string* voiceName = new std::string(desctext);
			if (strcmp(voiceName->c_str(), name->c_str()) == 0)
			{
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
				break;
			}
		}
	}

	return SUCCEEDED(hr);
}
