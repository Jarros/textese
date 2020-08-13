
#include <iostream>
#include <sstream>
#include <fstream>

/*
#include "Windows.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <locale>  
#include <stack>
#include <list>
#include <vector>
#include <array>
#include <set>
*/

#include "dictionary.h"

using namespace std;

std::string txt;
typedef unsigned char byte;
#define skip if(false)

bool IsLetter(char c);

/*

	(*)		= optional (duplicate w n wo, wo s prioritized)
	*-		= open endin (input endin copied t output endin)
	-*		= open begginin (input begginin copied t output begginin)
	(column (input) priority: left>>right)
	(row (word) priority: up>>down) (so longer words/phrases t put in d beginin)

	TODO: parse titles / namins (eg words dat begin w uppercase / enclosed in brackets) n skip replacin dem


*/

//TODO: insert dummy symbol in d beginnin

void Replace(std::string& txt,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = txt.find(oldStr, pos)) != std::string::npos) {
		txt.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void ReplaceEndin(std::string& txt,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = txt.find(oldStr, pos)) != std::string::npos) {
		if (/*IsLetter(txt[pos - 1]) && */!IsLetter(txt[pos + oldStr.length()]))
		{
			txt.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		else
		{
			pos += oldStr.length();
		}
	}
}

void ReplaceBeginnin(std::string& txt,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = txt.find(oldStr, pos)) != std::string::npos) {
		if (!IsLetter(txt[pos - 1])/* && IsLetter(txt[pos + oldStr.length()])*/)
		{
			txt.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		else
		{
			pos += oldStr.length();
		}
	}
}

void ReplaceBeginninEndin(std::string& txt,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = txt.find(oldStr, pos)) != std::string::npos) {
		if (/*IsLetter(txt[pos - 1]) && */IsLetter(txt[pos + oldStr.length()]))
		{
			txt.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		else
		{
			pos += oldStr.length();
		}
	}
}


void ReplaceWord(std::string& txt,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = txt.find(oldStr, pos)) != std::string::npos) {
		if (!IsLetter(txt[pos - 1]) && !IsLetter(txt[pos + oldStr.length()]))
		{
			txt.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		else
		{
			pos += oldStr.length();
		}
	}
}



bool IsLetter(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	return false;
}

bool IsUppercaseLetter(char c)
{
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}
bool IsLowercaseLetter(char c)
{
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

void ParseInput()
{
	int l=txt.length();
	
	{
		int i = 0;
		int n = 0;

		while (i < l + n)
		{
			if (IsUppercaseLetter(txt[i]))
			{
				txt[i] -= ('A' - 'a');
				if (i == 0 || !IsLetter(txt[i - 1]))
				{
					txt.insert(i, "^");// txt[i] = '+';
					n++; i++;
				}
			}
			i++;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		if (dict[i][0])
		{

			std::string sO = dict[i][0];
			bool flag_begin = false;
			bool flag_end = false;
			int sO_endpos = sO.length() - 1;

			//TODO:use it
			skip
			{
				if (sO[0] == '-')
				{
					flag_begin = true;
					sO = sO.substr(1, sO_endpos);
				}
				if (sO[sO_endpos] == '-')
				{
					flag_end = true;
					sO = sO.substr(0, sO_endpos - 1);
				}
			}

			for (int k = 1; k < 8; k++)
			{
				if (dict[i][k])
				{
					std::string s = dict[i][k];
					int l = s.length() - 1;
					if (s[0] == '-')
					{
						if (s[l] == '-')// && !IsLetter(txt[i - 1]) && !IsLetter(txt[i + s.length() + 1]))
						{
							string _i = s.substr(1, l - 1);
							string o = sO.substr(1, sO.length() - 2);
							Replace/*BeginninEndin*/(txt, _i, o);
						}
						else
						{
							string _i = s.substr(1, l);
							string o = sO.substr(1, sO.length() - 1);
							ReplaceEndin(txt, _i, o);
						}
					}
					else if (s[l] == '-')
					{
						string _i = s.substr(0, l);
						string o = sO.substr(0, sO.length() - 1);
						ReplaceBeginnin(txt, _i, o);
					}
					else
					{
						ReplaceWord(txt, s, sO);
					}
				}
				else
					break;
			}
		}
	}
	
	skip
	for (int i = 0; i < txt.length(); i++)
		if (txt[i] == '^') {
			txt.erase(i);
			if (IsLowercaseLetter(txt[i])) {
				txt[i]+=('A' - 'a');
			}
		}

	l = txt.length();
	{
		int i = 0;
		int n = 0;

		while (i < l + n)
		{
			if (txt[i]=='^')
			{

				txt.erase(i, 1);
				txt[i] += ('A' - 'a');
				n--;
			}
			else
				i++;
		}
	}


	skip
		for (int i = 0; i < txt.length(); i++)
			if(!IsLetter(txt[i]))
				txt[i] = '+';
}


void Save()
{
	/*
	std::ofstream myFile("out.txt");

	myFile.close();
	*/
}

void Load()
{
	char* in;

	std::ifstream myFile("input.txt");

	std::string line;
	//while (
	//std::getline(myFile, line);
		//)
	{

	}

	std::stringstream strStream;
	strStream << myFile.rdbuf(); //read the file
	txt = strStream.str(); //txt holds the content of the file

	myFile.close();

}


int main()
{
	cout << "INPUT:" << endl;
	Load();
	int i = txt.length();
	cout << txt << endl << endl;

	ParseInput();
	int o = txt.length();
	float compression = round((float)o / (float)i * 1000.f)/10.f;

	cout << "OUTPUT:" << endl;
	cout << txt << endl << endl;
	cout << i << "->" << o << " (" << compression << "%)";
	int k; cin >> k;

}