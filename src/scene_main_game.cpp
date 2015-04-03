/***************************************************************************
 *   Copyright (C) 2006 by Frederic MARTIN                                 *
 *   martin-frederic@users.sourceforge.net                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "scene_main_game.h"
#include "utils.h"
#include "faile/faile.h"
#include "faile/protos.h"
#include "faile/extvars.h"


//--------------------------------------------------//
//--------------------------------------------------//
//         **  E_scene_main_game **                 //
//--------------------------------------------------//
//--------------------------------------------------//

E_scene_main_game::E_scene_main_game()
{
	chessboard		= NULL;
}

E_scene_main_game::~E_scene_main_game()
{

}
		

bool E_scene_main_game::load()
{
	if (already_loaded)
		return true;
	
	clearScreen();
	pGlobalInfos->GUI_SetEvenListener(this);
	
	AINew();
	switch (pGlobalInfos->GetAILevel())
	{
		case 0:
			min_per_game=3;// Add 1 second for animation time ;)
			break;
			
		case 7:
			min_per_game=90;
			break;
			
		default:
			min_per_game=pGlobalInfos->GetAILevel()*10;
			break;
	}
#ifdef DEBUG
	printf("AI min_per_game is set to : %ld minutes\n",min_per_game);
	printf("A game is set to : %ld minutes\n",moves_to_tc);
#endif

	
	//TODO : check pGlobalInfos to see if the player want 2D or 3D interface
	chessboard = new C3DGraphicChessboard(); // 3D Chessboard
	chessboard->load();
	
	SquareXSelected=-1;
	SquareZSelected=-1;
	FutureXSquare=-1;
	FutureZSquare=-1;
	
	pGlobalInfos->GUI_setScene(GUI_SCENE_MAIN_GAME);
	pGlobalInfos->GUI_displayResult(no_result);

	
	player_color  = WHITE;
	current_color = player_color;
	game_mode = pGlobalInfos->GetGameMode();
					
	game_is_over=false;
	promotion=false;
	piece_for_promotion='q';
	
	FAILE_AI_thread=NULL;
	AI_comp_is_done=false;
	
	init_fade_out();
	fade_out=true;
	fade_in=false;
	
	finish_loading();
	return true;
}



void E_scene_main_game::unload()
{
	if (already_loaded==false)
		return;
	
	if (FAILE_AI_thread)
	{
		pouetChess_asks_FAILE_to_exit = TRUE;
		SDL_WaitThread(FAILE_AI_thread,NULL);
	}
	
	chessboard->unload();
	SAFE_DELETE(chessboard);
	
	already_loaded=false;
}





void E_scene_main_game::update()
{
	if (already_loaded==false)
	{
		if ( load()==false )
		{
			Logger::writeErrorLog("Loading of 'E_scene_main_game' failed: exiting...");
			pGlobalInfos->QuitGame();
			return;
		}
	}
	
	chessboard->display(board_to_display);
		
	click_on_chessboard(pGlobalInfos->MouseX(), pGlobalInfos->MouseY());
	
	// The escape Key cancel the selection
	// If no piece were selected when pressing escape, quit the the game
	if (pGlobalInfos->KeyDown(SDLK_ESCAPE))
	{
		pGlobalInfos->ReleaseKeys();
		
		if (SquareXSelected>-1 && SquareZSelected>-1)
		{
			SquareXSelected=-1;
			SquareZSelected=-1;
		}
		else
		{
			// go back to main menu
			scene_to_go=GAME_SCENE_MAIN_MENU;
			init_fade_in();
			fade_in=true;
		}
	}
	
	
	
	if (AI_comp_is_done)
	{
		AI_comp_is_done=false;
		FAILE_AI_thread=NULL;
		// update the board to display
		memcpy (board_to_display, board, sizeof (board));
		chessboard->Move_piece(('h'-move_to_do[0])+8*(move_to_do[1]-'1'),('h'-move_to_do[2])+8*(move_to_do[3]-'1'));
		
		if (result!=no_result)
		{
			game_is_over=true;
			pGlobalInfos->GUI_displayResult(result);
		}
		
		if (current_color == WHITE)
			current_color=BLACK;
		else
			current_color=WHITE;
	}
	

	go_2D();
	
	glDisable(GL_TEXTURE_2D);
	
	
	pGlobalInfos->GUI_updateEvents();
	pGlobalInfos->GUI_render();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (fade_out)
		fade_out=gl_fade_out(pGlobalInfos->ElapsedTime()/200.0f);
	

	if (fade_in)
	{
		if (!gl_fade_in(pGlobalInfos->ElapsedTime()/200.0f))
		{
			// At the end of the fade_in we have to load the next scene
			fade_in=false;
			unload();
			pGlobalInfos->SetCurrentScene(scene_to_go);
			clearScreen();
		}
	}

	exit_2D();
	
	
	SDL_Delay(5);
	

}




void E_scene_main_game::click_on_chessboard(int mouseX, int mouseY)
{
	// if it's not the player's turn, don't manage click on chessboard
	if ((game_mode == PLAYER_VS_AI) && (current_color != player_color))
		return;
	
	// promotion is a special state in which the player must answer
	// to the question of what is the piece he wants for the promotion
	if (promotion)
		return;
	
	// if the game is over, there is no reason to play anymore :)
	if (game_is_over)
		return;
	
		
	int sqX, sqY;
	
	
	if (pGlobalInfos->OnLeftMouseButtonDown())
	{	
		if (chessboard->clickOnChessBoard(mouseX,mouseY,&sqX,&sqY));
		{
			// if a piece was already selected, this means the player want to move this piece
			if (SquareXSelected>-1 && SquareZSelected>-1)
			{
				FutureXSquare=sqX;
				FutureZSquare=sqY;
				
				// check if the player move is legal (ie: move is in possible_moves array)
				if (move_is_legal())
				{
					// check if the move will be a promotion
					check_for_promotion();
					
					// if so, wait for the player to answer to which piece 
					// he wants for the promotion
					if (promotion)
						return;
					
					player_do_a_move(0);
								
					
					if (game_is_over)
						return;
											
					// ------- MODE PLAYER_VS_PLAYER -------
					if (game_mode == PLAYER_VS_PLAYER)
						chessboard->changeSideToPlay(current_color);
					

					// ------- MODE PLAYER_VS_AI -------
					if (game_mode == PLAYER_VS_AI)
					{		
						thread_data[0]=&AI_comp_is_done;
						thread_data[1]=move_to_do;
						
						FAILE_AI_thread=SDL_CreateThread(AIComputeMove,thread_data);
					}
					return;
				}
			}
			// if no piece were previously selected	
			SquareXSelected=-1;
			SquareZSelected=-1;
			
			int pos=24 + (sqY+1)*12 -(sqX+3);
			if (pos>=0 && pos<145)
			{
				bool color_selected=board_to_display[pos]%2;
				
				if (current_color!=WHITE)
					color_selected=!color_selected;
							
				if (color_selected && board_to_display[pos]!=npiece && board_to_display[pos]!=frame)
				{
					SquareXSelected=sqX;
					SquareZSelected=sqY;
					UpdatePossibleMoves(SquareXSelected,SquareZSelected);
				}
			}
			
		}
	}
	
	if (pGlobalInfos->display_possible_moves() && SquareXSelected>-1 && SquareZSelected>-1)
		chessboard->displayPossibleMoves(SquareXSelected,SquareZSelected,possible_moves);
}






void E_scene_main_game::check_for_promotion()
{
	// First check dest 
	if (current_color == WHITE && FutureZSquare != 7)
		return;
	if (current_color == BLACK && FutureZSquare != 0)
		return;	
	
	
	// now check if the selected piece is a pawn
	// and also check for the right color
	int pos=24 + (SquareZSelected+1)*12 -(SquareXSelected+3);
	if (pos>=0 && pos<145)
	{
		bool color_selected=board_to_display[pos]%2;
		
		if (current_color!=WHITE)
			color_selected=!color_selected;
					
		if (color_selected) // if the selected piece is owned by the current player
		{
			// test if the selected piece is a pawn or not. If not, return
			if (board_to_display[pos]!=wpawn && board_to_display[pos]!=bpawn)
				return;
		}
		
	}
	
	
	// here we are sure that a pawn is selected and its
	// destination deserves a promotion
	// So enable promotion !
	promotion = true;
	pGlobalInfos->GUI_displayPromotion(true);
		
}


void E_scene_main_game::player_do_a_move(char promotion)
{
	// Inform FAILE of the move
	char piece_pos[6];

	piece_pos[0]=(char)('h'-SquareXSelected);	// from
	piece_pos[1]=(char)(SquareZSelected+'1');	// from
	piece_pos[2]=(char)('h'-FutureXSquare);	// target
	piece_pos[3]=(char)(FutureZSquare+'1');	// target
	piece_pos[4]=promotion; // promotion
	piece_pos[5]=0;

	Player_do_a_move(piece_pos);
	// Update the graphical interface
	chessboard->Move_piece(SquareXSelected+8*SquareZSelected,FutureXSquare+8*FutureZSquare);

	if (game_mode == PLAYER_VS_PLAYER)
	{
		update_result();
		
		if (result!=no_result)
		{
			game_is_over=true;
			pGlobalInfos->GUI_displayResult(result);
		}
	}
					
	
	SquareXSelected=-1;
	SquareZSelected=-1;
	FutureXSquare=-1;
	FutureZSquare=-1;
	
	if (current_color == WHITE)
		current_color = BLACK;
	else
		current_color = WHITE;
}



void E_scene_main_game::update_result()
{
	
	/* check for draw by the 50 move rule: */
	if (fifty > 100) 
	{
		result = draw_by_fifty;
		return;		
	}
	else if (is_draw ()) 
	{
		result = draw_by_rep;
		return;
	}
	
	
	move_s moves[MOVE_BUFF];
	int num_moves, i, ep_temp;
	d_long temp_hash;

	ep_temp = ep_square;
	temp_hash = cur_pos;
	num_moves = 0;
	gen (&moves[0], &num_moves);
	
	// for all possible moves for all pieces
	for (i = 0; i < num_moves; i++) 
	{
		// if the move is from the side of the current player
		bool color=moves[i].from%2;
		if (current_color==WHITE)
			color=!color;
		
		// if the selected piece is not owned by the current player, continue
		if (!color) 
			continue;
		
		// test if the move is legal
		make (&moves[0], i);
		bool legal=check_legal (&moves[0], i); 
		
		unmake (&moves[0], i);
		ep_square = ep_temp;
		cur_pos = temp_hash;
		
		// if we find a legal move, game can continue
		if (legal)
			return;
	}	
	
	
	// if we are here, it means that there is no
	// move avaible for the current player
	if (in_check ())
	{
		if (white_to_move == 1)
			result = white_is_mated;
		else 
			result = black_is_mated;
	}
	else 
	{
		result = stalemate;
	}
	
}


void E_scene_main_game::UpdatePossibleMoves(unsigned char posX, unsigned char posZ)
{
	for(int i=0;i<32;i++)
		possible_moves[i]=99;
	
	int j=0;
	
	char piece_pos[2];
	
	// convert pouetChess coordinates to FAILE coordinates
	piece_pos[0]=(unsigned char)('h'-posX);
	piece_pos[1]=(unsigned char)(posZ+'1');

	
	move_s moves[MOVE_BUFF];
	int num_moves, i, ep_temp;
	char comp_move[6];
	d_long temp_hash;

	ep_temp = ep_square;
	temp_hash = cur_pos;
	num_moves = 0;
	gen (&moves[0], &num_moves);

	// for all possible moves for all pieces
	for (i = 0; i < num_moves; i++) 
	{
		comp_to_coord (moves[i], comp_move);
		
		// if the move is from the piece we want
		if ((comp_move[0]==piece_pos[0]) && (comp_move[1]==piece_pos[1]))
		{
			// test if the move is legal
			make (&moves[0], i);
			if (check_legal (&moves[0], i)) 
			{
				// convert from FAILE coords to pouetChess coords
				unsigned char new_pos=(unsigned char)(('h'-comp_move[2])+8*(comp_move[3]-'1'));
				// add this move to the possible moves array
				possible_moves[j++]=new_pos;	
			}
			unmake (&moves[0], i);
			ep_square = ep_temp;
			cur_pos = temp_hash;
		}
	}
}



bool E_scene_main_game::move_is_legal()
{
	for(int i=0;i<32;i++)
		if (possible_moves[i]==FutureXSquare+8*FutureZSquare)
			return true;
	return false;
}



void E_scene_main_game::actionPerformed(GUIEvent &evt)
{
	
	
	const std::string &callbackString  = evt.getCallbackString();
	GUIRectangle      *sourceRectangle = evt.getEventSource();
	int                widgetType      = sourceRectangle->getWidgetType();

	if(widgetType == WT_BUTTON)
	{
		GUIButton   *button = (GUIButton*)sourceRectangle;
		if(button->isClicked())
		{
			
			if(callbackString == "QuitMainGame")
			{
				scene_to_go=GAME_SCENE_MAIN_MENU;
				init_fade_in();
				fade_in=true;
			}
			
			if (callbackString == "ResetCamera")
			{
				chessboard->resetCameraPos();
			}
			
			if (callbackString == "okButtonPromotion")
			{
				player_do_a_move(piece_for_promotion);
				pGlobalInfos->GUI_displayPromotion(false);
				promotion=false;
			}
		}
	}
	else if (widgetType == WT_RADIO_BUTTON)
	{
		GUIRadioButton *button = (GUIRadioButton*)sourceRectangle;
		
		if(button->isClicked())
		{
			
			if(callbackString == "queen")
				piece_for_promotion='q';
			
			if(callbackString == "rook")
				piece_for_promotion='r';
			
			if(callbackString == "bishop")
				piece_for_promotion='b';
			
			if(callbackString == "knight")
				piece_for_promotion='n';
		}
		
	}
}





