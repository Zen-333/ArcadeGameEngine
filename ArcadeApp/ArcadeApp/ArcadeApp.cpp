#include <iostream>
#include "Color.h"
#include "SDL.h"
#include "Screen.h"
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

int main(int argc, char * argv[])
{
	// TESTING BRANCH MAIN //

	const int chessBoardLength = 8;
	const int squareLength = 12;
	const int squareCount = chessBoardLength * chessBoardLength;
	const Vec2D startPos(50, 50);

	std::vector<AARectangle> chessBoard;

	const float pawnRadius = 2.0f;
	std::vector<Vec2D> blackPawnsPos;
	std::vector<Vec2D> blackCastelPos;
	std::vector<Vec2D> blackHorsePos;
	std::vector<Vec2D> blackElephantPos;
	Vec2D blackQueen;
	Vec2D blackKing;

	std::vector<Vec2D> whitePawnsPos;
	std::vector<Vec2D> whiteCastelPos;
	std::vector<Vec2D> whiteHorsePos;
	std::vector<Vec2D> whiteElephantPos;
	Vec2D whiteQueen;
	Vec2D whiteKing;

	bool setWhitePiecePos = false;
	bool setBlackPiecePos = false;

	

	for(int i = 0; i != chessBoardLength; i++)
	{
		float x = 0;
		float y = 0;

		for(int j = 0; j != chessBoardLength; j++)
		{
			x = startPos.GetX() + (j * squareLength);
			y = startPos.GetY() + (i * squareLength);

			Vec2D newPos(x, y);

			AARectangle chessSquare(newPos, squareLength, squareLength);
			chessBoard.push_back(chessSquare);

			if(i % chessBoardLength == 6)
			{
				whitePawnsPos.push_back(chessSquare.GetCenterPoint());
			}
			else if(i % chessBoardLength == 1)
			{
				blackPawnsPos.push_back(chessSquare.GetCenterPoint());
			}
			
			if(!setBlackPiecePos)
			{
				const Vec2D startPosCenter(newPos.GetX() + (squareLength / 2), newPos.GetY() + (squareLength / 2));
				const float centerX = startPosCenter.GetX();
				const float centerY = startPosCenter.GetY();

				blackCastelPos.push_back(startPosCenter);
				blackCastelPos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 1)), centerY));

				blackHorsePos.push_back(Vec2D(centerX + squareLength, centerY));
				blackHorsePos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 2)), centerY));

				blackElephantPos.push_back(Vec2D(centerX + (squareLength * 2), centerY));
				blackElephantPos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 3)), centerY));

				blackQueen.SetX(centerX + (squareLength * 3));
				blackQueen.SetY(centerY);

				blackKing.SetX(centerX + (squareLength * 4));
				blackKing.SetY(centerY);

				setBlackPiecePos = true;
			}

			if(!setWhitePiecePos)
			{
				const Vec2D startPosCenter(newPos.GetX() + (squareLength / 2), newPos.GetY() + (squareLength / 2));
				const float centerX = startPosCenter.GetX();
				const float centerY = startPosCenter.GetY();

				whiteCastelPos.push_back(Vec2D(centerX, centerY + (squareLength * (chessBoardLength - 1))));
				whiteCastelPos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 1)), centerY + (squareLength * (chessBoardLength - 1))));

				whiteHorsePos.push_back(Vec2D(centerX + squareLength, centerY + (squareLength * (chessBoardLength - 1))));
				whiteHorsePos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 2)), centerY + (squareLength * (chessBoardLength - 1))));

				whiteElephantPos.push_back(Vec2D(centerX + (squareLength * 2), centerY + (squareLength * (chessBoardLength - 1))));
				whiteElephantPos.push_back(Vec2D(centerX + (squareLength * (chessBoardLength - 3)), centerY + (squareLength * (chessBoardLength - 1))));

				whiteQueen.SetX(centerX + (squareLength * 3));
				whiteQueen.SetY(centerY + (squareLength * (chessBoardLength - 1)));

				whiteKing.SetX(centerX + (squareLength * 4));
				whiteKing.SetY(centerY + (squareLength * (chessBoardLength - 1)));

				setWhitePiecePos = true;
			}
		}
		
	}



	Screen theScreen;
	theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);

	Vec2D MiddleScreen(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	AARectangle rect = { startPos, 12, 12};

	theScreen.SwapScreen();

	SDL_Event sdlEvent;
	bool running = true;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	float rotationSpeed = 6.283 / 5;

	bool c = false;

	Circle bKing(blackKing, pawnRadius);
	Circle bQueen(blackQueen, pawnRadius);

	Circle wKing(whiteKing, pawnRadius);
	Circle wQueen(whiteQueen, pawnRadius);

	Star2D star(2.0f, 1.0f, MiddleScreen, 3, true);

	while (running)
	{
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		//LAST = NOW;
		//NOW = SDL_GetPerformanceCounter();
		//deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

		//line.Rotate(rotationSpeed * deltaTime, MiddleScreen);

		
		//for(int i = 0; i < chessBoard.size(); i++)
		//{
		//	if(i % chessBoardLength == 0 && i != 0)
		//	{
		//		c = c;
		//	} else
		//	{
		//		c = !c;
		//	}

		//	const Color cc = c ? Color::Gray() : Color::Black();
		//	theScreen.Draw(chessBoard[i], Color::Gray(), true, cc);
		//	
		//}

		//for (Vec2D pos : whitePawnsPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::White(), true, Color::White());
		//}

		//for (Vec2D pos : blackPawnsPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::Green(), true, Color::Green());
		//}

		//for (Vec2D pos : blackCastelPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::Green(), true, Color::Green());
		//}

		//for (Vec2D pos : whiteCastelPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::White(), true, Color::White());
		//}

		//for (Vec2D pos : blackHorsePos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::Green(), true, Color::Green());
		//}

		//for (Vec2D pos : whiteHorsePos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::White(), true, Color::White());
		//}

		//for (Vec2D pos : blackElephantPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::Green(), true, Color::Green());
		//}

		//for (Vec2D pos : whiteElephantPos)
		//{
		//	Circle pawn(pos, pawnRadius);
		//	theScreen.Draw(pawn, Color::White(), true, Color::White());
		//}



		//theScreen.Draw(bKing, Color::Green(), true, Color::Green());
		//theScreen.Draw(bQueen, Color::Green(), true, Color::Green());

		//theScreen.Draw(wKing, Color::White(), true, Color::White());
		//theScreen.Draw(wQueen, Color::White(), true, Color::White());

		theScreen.Draw(star, Color::Cyan());

		c = false;

		theScreen.SwapScreen();


		// TESTING BRANCH RECTANGLE //
	}



	return 0;
}
