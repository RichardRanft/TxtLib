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
	m_registryList->clear();
	delete(m_registryList);
	m_registryList = nullptr;

	m_tokenMap->clear();
	delete(m_tokenMap);
	m_tokenMap = nullptr;
	
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
	m_tokenMap = new std::map<std::string*, std::string*>();

	m_registryList = new std::map<std::string*, std::string*>();
	getVoicesInRegistry();

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
		HRESULT hr;
		if (async)
			hr = cpVoice->Speak(wstrTo, SPF_ASYNC, 0);
		else
			hr = cpVoice->Speak(wstrTo, SPF_DEFAULT, 0);
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

	CComPtr<IEnumSpObjectTokens> cpIEnumeration;

	for (std::map<std::string*, std::string*>::iterator mit = m_tokenMap->begin(); mit != m_tokenMap->end(); mit++)
	{
		if (strcmp((*mit).first->c_str(), name->c_str()) == 0)
		{
			// Enumerate voice tokens
			const char* text = (*mit).second->c_str();
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
			TCHAR tokenId[1024] = { 0 };
			MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &tokenId[0], size_needed);
			CComPtr<ISpObjectToken> cpTokenObj;
			HRESULT hr = SpGetTokenFromId(tokenId, &cpTokenObj, false);
			if (SUCCEEDED(hr))
			{
				result = resetVoice((*mit).second);
			}
			break;
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

void CSpeechSynthesizer::getVoicesInRegistry()
{
	HKEY currentKey;
	TCHAR name[1024] = { 0 };
	DWORD dwSize = 1024, dwIdx = 0;
	long result;
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Speech Server\\v11.0\\Voices\\Tokens"), 0, KEY_ENUMERATE_SUB_KEYS, &currentKey);

	if (result != ERROR_SUCCESS)
	{
		// fail
	}
	else
	{
		DWORD index = 0;
		std::string* regKeyName = new std::string("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Speech Server\\v11.0\\Voices\\Tokens");
		while (ERROR_SUCCESS == RegEnumKeyEx(currentKey, index, name, &dwSize, NULL, NULL, NULL, NULL)) {
			char keyname[1024] = { 0 };
			wcstombs(keyname, name, 1024);
			m_registryList->insert(std::pair<std::string*, std::string*>(new std::string(regKeyName->c_str()), new std::string(keyname)));
			dwSize = 1024; // restore dwSize after is is set to key's length by RegEnumKeyEx
			++index; // increment subkey index
		}
		delete(regKeyName);
		regKeyName = nullptr;
	}
	//HKEY_LOCAL_MACHINE\SOFTWARE\\Wow6432Node\\Microsoft\\Speech\\Voices\\Tokens
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Speech\\Voices\\Tokens"), 0, KEY_ENUMERATE_SUB_KEYS, &currentKey);

	if (result != ERROR_SUCCESS)
	{
		// fail
	}
	else
	{
		DWORD index = 0;
		std::string* regKeyName = new std::string("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Speech\\Voices\\Tokens");
		while (ERROR_SUCCESS == RegEnumKeyEx(currentKey, index, name, &dwSize, NULL, NULL, NULL, NULL)) {
			char keyname[1024] = { 0 };
			wcstombs(keyname, name, 1024);
			m_registryList->insert(std::pair<std::string*, std::string*>(new std::string(regKeyName->c_str()), new std::string(keyname)));
			dwSize = 1024; // restore dwSize after is is set to key's length by RegEnumKeyEx
			++index; // increment subkey index
		}
		delete(regKeyName);
		regKeyName = nullptr;
	}
}

std::list<std::string*>* CSpeechSynthesizer::GetAvailableVoices()
{
	std::list<std::string*>* namelist = new std::list<std::string*>();
	// if we're not initialized return an empty list
	if (!m_initialized)
		return namelist;

	if (m_tokenMap->size() < 1)
	{
		for (std::map<std::string*, std::string*>::iterator mit = m_registryList->begin(); mit != m_registryList->end(); mit++)
		{
			std::string regToken = (*mit).first->c_str();
			regToken.append("\\");
			regToken.append((*mit).second->c_str());
			// Enumerate voice tokens
			const char* text = regToken.c_str();
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
			TCHAR tokenId[1024] = { 0 };
			MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &tokenId[0], size_needed);
			CComPtr<ISpObjectToken> cpTokenObj;
			HRESULT hr = SpGetTokenFromId(tokenId, &cpTokenObj, false);
			if (SUCCEEDED(hr))
			{
				wchar_t* desc = new wchar_t[256];
				SpGetDescription(cpTokenObj, &desc);
				char desctext[256] = { 0 };
				wcstombs(desctext, desc, 256);
				std::string* voiceName = new std::string(desctext);
				namelist->push_back(voiceName);
				m_tokenMap->insert(std::pair<std::string*, std::string*>(voiceName, new std::string(regToken.c_str())));
			}
			if (cpTokenObj != NULL)
				cpTokenObj.Release();
		}
	}
	else
	{
		for (std::map<std::string*, std::string*>::iterator mit = m_tokenMap->begin(); mit != m_tokenMap->end(); mit++)
		{
			namelist->push_back((*mit).first);
		}
	}

	return namelist;
}

bool CSpeechSynthesizer::resetVoice(std::string* name)
{
	cpVoice.Release();
	cpToken.Release();
	cpIEnum.Release();

	CoInitialize(cpIEnum);

	bool isServerVoice = name->find("Server");
	// Create a voice and set its token to the one we just found.
	const char* text = name->c_str();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), NULL, 0);
	TCHAR tokenId[1024] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, &text[0], (int)strlen(text), &tokenId[0], size_needed);
	HRESULT hr = SpGetTokenFromId(tokenId, &cpToken, false);
	if (SUCCEEDED(hr))
	{
		if (isServerVoice)
			hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&cpVoice);
		else
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

	return SUCCEEDED(hr);
}
