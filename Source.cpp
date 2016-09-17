#include <iostream>
#include <fstream>
#include "Player.h"
using namespace std;

const char* filename = "D:/Desktop/Summer 2016/CS32/Project 4/wordlist.txt";

void play(Game& g, Player &p)
{
	int secretLength = g.secretWordLength();
	int nTurns = 0;
	for (;;)
	{
		int nInRightPlace = 0, nInWrongPlace = 0;
		string probe = p.generateProbeWord();
		g.probe(probe, nInRightPlace, nInWrongPlace);
		// repeat probes until word is guessed
		nTurns++;
		if (nInRightPlace == secretLength)
			break;
		p.learn(probe, nInRightPlace, nInWrongPlace);
		cerr << "The Player probed " << nTurns << " times!" << endl << endl;
	}
}

int main()
{
	ifstream wordlistfile(filename);
	if (!wordlistfile)
	{
		cerr << "Cannot open " << filename << "!" << endl;
		return 1;
	}
	vector<string> words;

	string w;
	while (wordlistfile >> w)
		words.push_back(w);

	Player player(words);
	Game g(words);

	if (!g.setSecretWord("string"))  // Supply the secret word
	{
		cerr << "Secret word is not in word list!" << endl;
		return 1;
	}
	else
		play(g, player);
}