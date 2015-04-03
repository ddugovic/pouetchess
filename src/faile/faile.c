/*
MIT License

Copyright (c) 2000 Adrien M. Regimbald

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**************************************************
 * Faile version 1.4                              *
 * Author: Adrien Regimbald                       *
 * E-mail: adrien@ee.ualberta.ca                  *
 * Web Page: http://www.ee.ualberta.ca/~adrien/   *
 *                                                *
 * File: faile.c                                  *
 * Purpose: main program.                         *
 **************************************************/

#include "faile.h"
#include "protos.h"
#include <SDL/SDL.h> /* for SDL_Delay */

char divider[50] = "-------------------------------------------------";
move_s dummy = {0,0,0,0,0,0,0};

int board[144], board_to_display[144], moved[144], ep_square, white_to_move, wking_loc,
bking_loc, white_castled, black_castled, result, ply, pv_length[PV_BUFF],
history_h[144][144], pieces[33], squares[144], num_pieces, i_depth, fifty,
fifty_move[PV_BUFF], game_ply;

long int nodes, raw_nodes, qnodes, piece_count, killer_scores[PV_BUFF],
killer_scores2[PV_BUFF], moves_to_tc, min_per_game, inc, time_left,
opp_time, time_cushion, time_for_move, cur_score, start_piece_count,
last_root_score;

bool xb_mode, captures, searching_pv, post, time_exit, time_failure,
allow_more_time, bad_root_score, 
  /* Added by Frederic MARTIN for pouetChess */
  pouetChess_asks_FAILE_to_exit;

move_s pv[PV_BUFF][PV_BUFF], killer1[PV_BUFF], killer2[PV_BUFF],
killer3[PV_BUFF];

rtime_t start_time;

d_long h_values[14][144], ep_h_values[144], wck_h_values[2], wcq_h_values[2],
bck_h_values[2], bcq_h_values[2], color_h_values[2], cur_pos, 
rep_history[PV_BUFF];

hash_s *hash_table;

unsigned long int hash_mask, hash_max_mb = 0;

move_s move, comp_move;
int depth = 4, comp_color;


void AIInit(unsigned long int hash_max_megabytes)
{
	hash_max_mb=hash_max_megabytes;
	
	init_hash_values ();
	init_hash_tables ();
	init_game ();
	init_book ();
	xb_mode = FALSE;
	comp_color = 0;
	memcpy (board_to_display, board, sizeof (board));
	
}

void AINew()
{
	init_game ();
	refresh_hash (); /* refresh our hash tables: */
	comp_color = 0;
	memcpy (board_to_display, board, sizeof (board));
}


void AIExit()
{
#ifdef DEBUG
	int i = 1;
 	unsigned long int max_hash = 1, element_size = sizeof (hash_s);
	/* compute the maximum hash element, based upon size desired: */
	while (max_hash * element_size <= hash_max_mb<<19)  max_hash = 1 << i++;
	printf("Free hash_table : %lu kb of RAM\n", (max_hash*element_size)>>10);
#endif
	free (hash_table);
}



int AIComputeMove(void* data)
{
	void** thread_data=(void**) data;
	
	bool* thread_is_finished=(bool*)(thread_data[0]);
	(*thread_is_finished)=FALSE;
	
	char* move_to_do=(char*)(thread_data[1]);	
	
	

	clock_t cpu_start = 0, cpu_end = 0;
	nodes = 0;
	qnodes = 0;
	ply = 0;
	pouetChess_asks_FAILE_to_exit=FALSE;
			
	start_time = rtime ();
	cpu_start = clock ();
	comp_move = think ();
	cpu_end = clock ();
	
	clock_t elapsed_time = (cpu_end - cpu_start)/1000;
	
#ifdef DEBUG
	printf("think time = %dms\n",elapsed_time);
#endif
	
	if (elapsed_time<1600)
		SDL_Delay(1600-elapsed_time);

	/* check for a game end: */
	if (((comp_color == 1 && result != white_is_mated)  ||
		(comp_color == 0 && result != black_is_mated)) &&
		result != stalemate && result != draw_by_fifty &&
		result != draw_by_rep)
	{
		
		comp_to_coord (comp_move, move_to_do);
		
		make (&comp_move, 0);

		/* check to see if we draw by rep/fifty after our move: */
		if (is_draw ()) 
		{
			result = draw_by_rep;
		}
		else if (fifty > 100) 
		{
			result = draw_by_fifty;
		}

		reset_piece_square ();
		
#ifdef DEBUG
		/* check to see if we mate our opponent with our current move: */
		if (!result) 
		{
			if (xb_mode) 
			{
				printf ("move %s\n", move_to_do);
			}
			else 
			{
				printf ("\n%s\n", move_to_do);
			}
		}
		else 
		{
			if (xb_mode) 
			{
				printf ("move %s\n", move_to_do);
			}
			else 
			{
				printf ("\n%s\n", move_to_do);
			}
			if (result == white_is_mated) 
			{
				printf ("0-1 {Black Mates}\n");
			}
			else if (result == black_is_mated) 
			{
				printf ("1-0 {White Mates}\n");
			}
			else if (result == draw_by_fifty) 
			{
				printf ("1/2-1/2 {Fifty move rule}\n");
			}
			else if (result == draw_by_rep) 
			{
				printf ("1/2-1/2 {3 fold repetition}\n");
			}
			else 
			{
				printf ("1/2-1/2 {Draw}\n");
			}
		}
#endif
	}
#ifdef DEBUG
	/* we have been mated or there is a draw: */
	else 
	{
		if (result == white_is_mated) 
		{
			printf ("0-1 {Black Mates}\n");
		}
		else if (result == black_is_mated) 
		{
			printf ("1-0 {White Mates}\n");
		}
		else if (result == stalemate) 
		{
			printf ("1/2-1/2 {Stalemate}\n");
		}
		else if (result == draw_by_fifty) 
		{
			printf ("1/2-1/2 {Fifty move rule}\n");
		}
		else if (result == draw_by_rep) 
		{
			printf ("1/2-1/2 {3 fold repetition}\n");
		}
		else 
		{
			printf ("1/2-1/2 {Draw}\n");
		}
	}
#endif
		
	/*
	 inform the game we just finished AI computations
	 NOTE: MUST always be here (ie: before "return 0")
	*/
	(*thread_is_finished)=TRUE;
	
	
	return 0;
}







bool Player_do_a_move(char* input)
{
		
	if (verify_coord (input, &move)) 
	{
		make (&move, 0);
		reset_piece_square ();
#ifdef DEBUG
		printf ("\n");
		display_board (stdout, 1-comp_color);
#endif		
		
		/* update the board to display */
		memcpy (board_to_display, board, sizeof (board));

		return TRUE;
	}

	
	printf ("Illegal move: %s\n", input);
	return FALSE;
}




