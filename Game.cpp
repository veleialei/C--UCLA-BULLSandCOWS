#include "Game.h"
#include <iostream>
#include <windows.h>
using namespace std;
Game::Game(const vector<string> &words) {
	SYSTEMTIME time;
	GetSystemTime(&time);
	m_time = time.wHour*3600000 + time.wMinute*60000 + time.wSecond * 1000 + time.wMilliseconds;
	wordList = words;
}

bool Game::setSecretWord(const string &sw) {
	typedef vector<string>::iterator it;
	it start = wordList.begin(), end = wordList.end();
	bool exist = binary_search(start, end, sw);
	if (exist) {
		m_size = sw.length() + 1;
		m_secretWord = (char*) malloc(m_size);
		memcpy(m_secretWord, sw.c_str(), m_size);
	}
	return exist;
}

int Game::secretWordLength() const {
	return m_size - 1;
}

Game::~Game() {
	free(m_secretWord);
}

void Game::probe(const string &probeWord, int &nInRightPlace, int &nInWrongPlace) {
	int Bull = 0, Cow = 0;
	string probe = probeWord;
	string secret = m_secretWord;
	for (int j = 0; j < secret.length(); j++)
		for (int i = 0; i < probe.length(); i++) {
			if (probe[i] == secret[j] && islower(secret[j])) {
				if (i == j) 
					Bull++;
				else 
					Cow++;
				//cerr << probe << "\t" << secret << endl;
				probe[i] = ' ';
				secret[j] = ' ';
				j++;
				i = 0;
			}
		}
	//cerr << probe << "\t" << secret << endl;
	nInRightPlace = Bull;
	nInWrongPlace = Cow;
	cerr << "Bulls:\t" << nInRightPlace << "\tCows:" << nInWrongPlace << "\tExpected Bulls:" << secretWordLength() << endl;
	ShowTime();
}

void Game::ShowTime()
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	int nowTime = time.wHour * 3600000 + time.wMinute * 60000 + time.wSecond * 1000 + time.wMilliseconds;
	cerr << "Time lapsed (ms): " << nowTime - m_time << endl;
	m_time = nowTime;
}