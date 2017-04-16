// Info450 Assignment 4.cpp : My favorite things.
// Topic: My favorite Video Game
// Andrew Nolen

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;


class VideoGame 
{
	string title;
	string developer;
	string platform;
	string rating;

public:

	friend class GameList;

	VideoGame() 
	{
		title = ""; developer = ""; platform = ""; rating = "";
	}
	VideoGame(string t, string dev, string p, string r) 
	{
		title = t;
		developer = dev;
		platform = p;
		rating = r;
	}

	void CaptureGame() 
	{
		cout << "What is the title of the game? --> ";
		cin.ignore();
		getline(cin, title);

		cout << "Who made the Game? --> ";
		getline(cin, developer);

		cout << "What console or consoles is the game on? --> ";
		getline(cin, platform);

		cout << "What do you rate the game? --> ";
		getline(cin, rating);
	}

	void ShowGame() 
	{
		cout << "------------------------" << endl;
		cout << "Title: " << title << endl;
		cout << "Developer: " << developer <<  endl;
		cout << "Platforms: " << platform << endl;
		cout << "Rating: " << rating << endl;
	}

	int SaveGame(ofstream &outfile) 
	{
		if (outfile.is_open())
		{
			outfile << title << "|" << developer << "|" << platform << "|" << rating << endl;
		}
		else
			return WRITEERROR;
	}

	bool CheckValue(VideoGame* t) 
	{
		if (title == t->title && developer == t->developer) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
};

class GameList
{
	VideoGame **list;
	int numgames;
	int listsize;
	int reallocateArray() 
	{
		VideoGame **temp;
		temp = new VideoGame *[listsize + ARRAYSIZE];
		listsize = listsize + ARRAYSIZE;
		for (int i = 0; i < numgames; i++) 
		{
			temp[i] = list[i];
		}
		delete[]list;
		list = temp;
		return 0;
	}

public: 

	friend class VideoGame;

	GameList()
	{
		list = new VideoGame*[ARRAYSIZE];
		numgames = 0;
		listsize = ARRAYSIZE;
	}
	~GameList() 
	{
		for (int i = 0; i < numgames; i++) 
		{
			delete list[i];
		}
		delete[]list;
	}

	void CheckGame() 
	{
		VideoGame* myGame;
		myGame = new VideoGame();
		bool passCheck = true;

		myGame->CaptureGame();

		for (int i = 0; i < numgames; i++) 
		{
			if (list[i]->CheckValue(myGame)) 
			{
				cout << "This Game already exists, Please enter a new game!" << endl;
				passCheck = false;
			}
		}
		if (passCheck == true) 
		{
			list[numgames] = myGame;
			numgames++;
		}
	}

	void GetUserInput() 
	{
		char answer;
		cout << "Enter a new game? Y/N?" << endl;
		cin >> answer;

		while (answer == 'Y' || answer == 'y') 
		{
			VideoGame* myGame = new VideoGame();
			CheckGame();
			cout << "Enter another game? Y/N?" << endl;
			cin >> answer;
		}
	}

	void ShowGameList() 
	{
		for (int i = 0; i < numgames; i++)
		{
			list[i]->ShowGame();
		}
	}

	int SaveGameList(string filename) 
	{
		ofstream output(filename, ios::trunc);
		if (output)
		{
			for (int i = 0; i < numgames; i++) 
			{
				list[i]->SaveGame(output);
			}
		}
		else
		{
			return WRITEERROR;
		}
		output.close();
		return 0;
	}

	int ReadGameList(string filename) 
	{
		string it, idev, ip, ir;
		

		ifstream infile(filename, ios::in);
		if (!infile) 
		{
			cout << "Your file could not be read!" << endl;
			return READERROR;
		}
		while (infile.good()) 
		{
			if (numgames == listsize) 
			{
				reallocateArray();
			}
			getline(infile, it, '|');
			if (!it.empty()) 
			{
				getline(infile, idev, '|');
				getline(infile, ip, '|');
				getline(infile, ir);
				list[numgames] = new VideoGame(it, idev, ip, ir);
				numgames++;
			}
		}
		infile.close();
		return 0;
	}

	int DeleteGame(string filename)
	{
		//Extra Credit 

		string it, idev, ip, ir;
		string erase;


		ifstream infile(filename, ios::in);
		cout << "Warning! please start a new session before continuing" << endl;
		cout << "Please enter the title of the game you want to delete: ";
		cin.ignore();
		getline(cin, erase);

		while (infile.good())
		{
			if (numgames == listsize)
			{
				reallocateArray();
			}
				getline(infile, it, '|');
				getline(infile, idev, '|');
				getline(infile, ip, '|');
				getline(infile, ir);
					if (!it.empty() && it != erase)
					{
						list[numgames] = new VideoGame(it, idev, ip, ir);
						numgames++;
					}
		}
		infile.close();
		return 0;
	}

	void EmptyGameList() 
	{
		memset(&list[0], 0, numgames);
	}
};

int main()
{
	string filename;
	GameList myVideoGame;
	char answer;
	char erase;
	cout << "Welcome to the my Favorite Video Game App!" << endl;
	cout << "Please enter the full-path filename: ";
	getline(cin, filename);

	do 
	{
		cout << "Would you like to (C)reate a new entry, (D)elete an existing one, or (Q)uit?" << endl;
		cin >> answer;

		if (answer == 'C' || answer == 'c')
		{
			myVideoGame.GetUserInput();
			myVideoGame.ReadGameList(filename);
			myVideoGame.SaveGameList(filename);
			myVideoGame.ShowGameList();
		}
		else if (answer == 'D' || answer == 'd')
		{
			//Extra Credit

			myVideoGame.DeleteGame(filename);
			myVideoGame.SaveGameList(filename);
			myVideoGame.ShowGameList();
		}
		else if (answer == 'Q' || answer == 'q') 
		{
			return 0;
		}
	} while (answer != 'Q' || answer != 'q');
}

