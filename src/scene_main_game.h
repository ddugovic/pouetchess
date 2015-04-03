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


#ifndef __E_SCENE_MAIN_GAME_H__
#define __E_SCENE_MAIN_GAME_H__


#include "scenes.h"
#include "graphic_chessboard.h"
#include "texture.h"
#include "globalInfos.h"
#include <SDL/SDL_thread.h>



class E_scene_main_game: public Chess_Scene,GUIEventListener
{
	public:
		E_scene_main_game();
		virtual ~E_scene_main_game();
		virtual bool load();
		virtual void unload();
		virtual void update();
		
	private:
		int SquareXSelected;
		int SquareZSelected;
		int FutureXSquare;
		int FutureZSquare;
		
		bool promotion;
		bool game_is_over;
		bool fade_in, fade_out;
		
		char piece_for_promotion;
		unsigned char possible_moves[32];

		GameMode game_mode;
		Game_scene scene_to_go;
		
		CGraphicChessboard* chessboard;
		
		Piece_color player_color;
		Piece_color current_color;
		
				
		SDL_Thread* FAILE_AI_thread;
		bool AI_comp_is_done;		// true when AI computation is done
		char move_to_do[6];			// AI Move in coord format
		void* thread_data[2];		// data to send to the thread
		
	
		void display_main_menu();
		void display_main_game();
		void display_about();
		void display_options();
		void click_on_chessboard(int mouseX, int mouseY);
		void check_for_promotion();
		void player_do_a_move(char promotion);
		void update_result();
		void UpdatePossibleMoves(unsigned char posX, unsigned char posZ);
		bool move_is_legal();
		
		/**
		 * GUI event management
		 * @param evt the current event to manage
		 */
		virtual void actionPerformed(GUIEvent &evt);

};



#endif

