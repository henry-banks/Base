
#include "sfwdraw.h"
#include "GameState.h"
#include "MenuState.h"
#include "EndState.h"
#include <ctime>

/*
	The main function should be used for application state management.
	Currently, only one state is implemented here, you'll need to add additional
	and get them to behave properly.
*/
void main()
{
	//We're using randomization, so we gotta seed the randoms
	srand(time(0));

	sfw::initContext(1280, 800);

	int score = 0;

	states curState = MENU_START;
	states oldState = curState;	//used to call end functions

	GameState gs;
	MenuState menu;
	EndState end;

	gs.init(); // called once
	menu.init();
	end.init();

	//gs.play(); // Should be called each time the state is transitioned into

	while (sfw::stepContext())
	{
		if (oldState != curState)
		{
			switch (oldState)
			{
			case MENU:
				score = gs.getScore();
				menu.stop();
				break;
			case GAME:
				gs.stop();
				break;
			case END:
				end.stop();
				break;
			default:
				break;
			}
			oldState = curState;
		}

		
		switch (curState)
		{
		case MENU_START:
			menu.play();
		case MENU:
			menu.step();
			menu.draw();
			curState = menu.next_a();
			break;
		case GAME_START:
			gs.play();
		case GAME:
			gs.step();
			gs.draw();
			curState = gs.next_a();
			break;
		/*case END_START:
			end.play(score);
		case END:
			end.step();
			end.draw();
			curState = end.next_a();
			break;*/
		case TERMINATE:
			sfw::termContext();
		default:
			break;
		}

		//gs.next(); Determine the ID of the next state to transition to.
	}


	gs.stop(); // should be called each time the state is transitioned out of
	gs.term(); // called once


	sfw::termContext();

}
