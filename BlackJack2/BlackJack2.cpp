// BlackJack

#include <iostream>
#include <random>
#include <ctime>
#include <conio.h>


char choice;
char input;
bool playing;

bool standing;
int temp;
int playerTotal;
int houseTotal;
int winner;

int pot;
int playerMoney = 100;
int houseMoney = 100;
int bettingAmount;
bool bet;

///		Using Mersenne instead of a proper card stack as my numbers. It generates numbers between 1-10
namespace MyRandom {
	std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}
int drawCard() {
	std::uniform_int_distribution<>die{ 1, 10 };
	return die(MyRandom::mersenne);
}

///		Draws a card for player and house/dealer
///		Turns values of 1 to 11 if it is beneficial
///		House/dealer will not draw cards if they show a number thats equal or bigger then 17
int playerHit() {
	temp = 0;
	temp = drawCard();
	if (temp == 1 && playerTotal < 11) {
		temp = 11;
		playerTotal = playerTotal + temp;
	}
	else {
		playerTotal = playerTotal + temp;
	}
	return playerTotal;
}
int houseHit() {
	if(houseTotal >= 17){
		return houseTotal;
	}
	else if (houseTotal <= playerTotal || houseTotal < 17) {
		temp = 0;
		temp = drawCard();
		if (temp == 1 && houseTotal < 11) {
			temp = 11;
			houseTotal = houseTotal + temp;
		}
		else {
			houseTotal = houseTotal + temp;
		}
		return houseTotal;

	}
}

///		Simple restart for when a round is over
void restart() {
	winner = 0;
	playerTotal = 0;
	houseTotal = 0;
	bettingAmount = 0;
	standing = false;

}

///		Checks how each player is doing
///		Uses int winner to check who has won
///			0 is nothing
///			1 is player
///			2 is house/dealer
///			3 is draw
///			4 is both lost
///		Prints out statement based on int winner and resets the pot if necessary
void winnerState(){
	if (!standing) {
		if (playerTotal == 21) {
			std::cout << "BLACKJACK!\n";
			playing = false;
			winner = 1;
			return;
		}
		else if (playerTotal > 21 && houseTotal > 21) {
			playing = false;
			winner = 3;
			return;
		}
		else if (playerTotal > 21) {
			playing = false;
			winner = 2;
			return;
		}
		else if (houseTotal > 21) {
			playing = false;
			winner = 1;
			return;
		}
		else if (houseTotal >= 17 && 21 > playerTotal && playerTotal > houseTotal) {
			playing = false;
			winner = 1;
			return;
		}
	}
	else if (standing) {
		if (houseTotal > 21) {
			playing = false; 
			winner = 1;
			return;
		}
		else if (houseTotal >= 17 && houseTotal < playerTotal && playerTotal <= 21) {
			playing = false;
			winner = 1;
			return;
		}
		else if (houseTotal > playerTotal && houseTotal <= 21) {
			playing = false;
			winner = 2;
			return;
		}
		else if (playerTotal == houseTotal && houseTotal >= 17) {
			playing = false;
			winner = 4;
			return;
		}
	}
	return;
}
void print(int num) {
	if (num == 0) {
		return;
	}
	else if (num == 1) {
		playerMoney = playerMoney + pot;
		std::cout << "Player Won and earned " << pot << "$ \n\n";
		std::cout << "PlayerTotal: " << playerTotal << "\n";
		std::cout << "HouseTotal: " << houseTotal << "\n";
		pot = 0;
		return;
	}
	else if (num == 2) {
		houseMoney = houseMoney + pot;
		std::cout << "The House Won and got "<< pot <<"$ back\n\n";
		std::cout << "PlayerTotal: " << playerTotal << "\n";
		std::cout << "HouseTotal: " << houseTotal << "\n";
		pot = 0;
		return;
	}
	else if (num == 3) {
		std::cout << "Both Lost\n\n";
		std::cout << "PlayerTotal: " << playerTotal << "\n";
		std::cout << "HouseTotal: " << houseTotal << "\n";
		std::cout << "The Pot is: " << pot << '\n';
		return;
	}
	else if (num == 4) {
		std::cout << "Draw\n\n";
		std::cout << "PlayerTotal: " << playerTotal << "\n";
		std::cout << "HouseTotal: " << houseTotal << "\n";
		std::cout << "The Pot is: " << pot << '\n';
		return;
	}

}

///		The player choses a betting amount and house/dealer must match that bet
///		Player cannot bet more than he has
///		If it is a draw or both lost. the pot is maintained to the next round with
///			a chance for additional bets
int main()
{
	do {
		system("cls");
		print(winner);
		restart();
		if (playerMoney == 0) {
			std::cout << "You're broke. You Lost\n\n";
			return 0;
		}
		if (houseMoney == 0) {
			std::cout << "The House is broke. You Won\n\n";
			return 0;
		}
		std::cout << "Press B to start\n";
		std::cout << "Press Q to quit\n";

		choice = _getch();

		switch (choice)
		{
		case'b':case'B':
			system("cls");
			std::cout << "The House has " << houseMoney << "$\n";
			std::cout << "You have " << playerMoney << "$\nHow much would you like to bet?\n";
			std::cout << "Current Pot: " << pot << '\n';
			std::cin >> bettingAmount;
			while (bettingAmount > playerMoney) {
				std::cout << "You don't have that much money. Enter a new amount\n";
				std::cout << "Current Pot: " << pot << '\n';
				std::cin >> bettingAmount;
			}
			while (bettingAmount > houseMoney) {
				std::cout << "The House cannot match that bet. Enter a new amount\n";
				std::cout << "Current Pot: " << pot << '\n';
				std::cin >> bettingAmount;
			}
			bet = true;
			system("cls");
			break;
		case 'q':case'Q':
			std::cout << "Goodbye\n";
			return 0;
			break;
		default:
			system("cls");
			break;
		}
		if (bet) {
			playerMoney = playerMoney - bettingAmount;
			houseMoney = houseMoney - bettingAmount;
			pot = pot + (bettingAmount * 2);
			playing = true;
		}
		

		while (playing) {
			if (!standing) {
				std::cout << "The Pot is " << pot << "$\n";
				std::cout << "Press H to Hit\n";
				std::cout << "Press S to Stand\n";
				std::cout << '\n';

				input = _getch();

				switch (input)
				{
				case'h':case'H':	// Hit
					playerHit();
					houseHit();
					break;
				case 's':case 'S':	// Stand
					standing = true;
					break;
				case 't':case'T':
					playerTotal = 17;
					houseTotal = 17;
					break;
				case 'q':case'Q':	// Quit
					playing = false;
					break;
				default:
					break;
				}
			}
			else {
				houseHit();
			}
			system("cls");
			std::cout << "PlayerTotal: " << playerTotal << "\n";
			std::cout << "HouseTotal: " << houseTotal << "\n";
			std::cout << '\n';
			winnerState();
		}
	} while (true);

	return 0;
}
