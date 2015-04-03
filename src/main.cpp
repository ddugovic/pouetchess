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
/**
 * @mainpage pouetChess : A 3D and Open Source Chess game with an embedded AI
 * @section About
 * This program has been developed under Linux Fedora Core 4 with KDE and
 * written whith Kate (very good editor !).
 *
 * It compiles without warning with gcc 4.0.x and probably earlier versions as well.
 *
 * @section dependencies
 * pouetChess dependencies :
 *
 * - SDL
 * - SDL_image
 * - OpenGL
 *
 * @section Hardware-configuration
 * Hardware configuration required: ('may' work with less than that) 
 *
 * - Processor : 600Mhz
 * - Memory : 128 Mb
 * - Graphic card : GeForce 4 or Radeon 9600 with latest drivers properly installed
 *
 *
*/


#include "scenes.h"
#include "scene_init.h"
#include "scene_main_menu.h"
#include "scene_main_game.h"

#include "utils.h"
#include <SDL/SDL_opengl.h>

#include "globalInfos.h"
#include "models.h"
#include "data.h"

#include "faile/faile.h"
#include "faile/protos.h"

GlobalInfos* pGlobalInfos;



int main(int argc, char *argv[])
{
	// The one and only global Info instance is allocated here
	pGlobalInfos = new GlobalInfos;
	if (pGlobalInfos->LoadGameConfiguration()==false)
		return 1;
	
	// SDL Initialization
	if (SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		printf("SDL_Init(SDL_INIT_VIDEO) failed : %s\n",SDL_GetError());
		SDL_Quit();
		return 2;
	}
	
	SDL_WM_SetCaption("pouetChess version "GAME_VERSION,PC_DATA_ICON_POUETCHESS);
	
	SDL_Surface* game_icon=SDL_LoadBMP(PC_DATA_ICON_POUETCHESS);
	
	if (game_icon)
		SDL_WM_SetIcon(game_icon,NULL);
	
	
	
	// Get video informations in order to get the bpp
	const SDL_VideoInfo* info = NULL;
	info = SDL_GetVideoInfo();

	// Should never happen
	if (info == NULL) 
	{
		printf("SDL_GetVideoInfo() failed: %s\n",SDL_GetError());
		SDL_Quit();
		return 3;
	}
	
	
	// Window creation
	if (pGlobalInfos->IsFullScreen())
	{
		// we want fullscreen mode, if we can't have this mode, try in windowed mode
		if (NULL==SDL_SetVideoMode(pGlobalInfos->ScreenWidth(), pGlobalInfos->ScreenHeight(), info->vfmt->BitsPerPixel, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN))
		{
			printf("SDL_SetVideoMode failed : %s\n", SDL_GetError());
			printf("I try the window mode\n");
			if (NULL==SDL_SetVideoMode(pGlobalInfos->ScreenWidth(), pGlobalInfos->ScreenHeight(), info->vfmt->BitsPerPixel, SDL_OPENGL | SDL_HWSURFACE))
			{
				printf("SDL_SetVideoMode failed : %s\n", SDL_GetError());
				SDL_Quit();
				return 4;
			}
			else
				printf("%dx%d in window mode is OK\n",pGlobalInfos->ScreenWidth(),pGlobalInfos->ScreenHeight());
		}
	}
	else
	{
		if (NULL==SDL_SetVideoMode(pGlobalInfos->ScreenWidth(), pGlobalInfos->ScreenHeight(), info->vfmt->BitsPerPixel, SDL_OPENGL | SDL_HWSURFACE))
		{
			printf("SDL_SetVideoMode failed : %s\n", SDL_GetError());
			SDL_Quit();
			return 4;
		}
	}
	
	// OpenGL basic initialization
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glViewport (0, 0, (GLsizei)(pGlobalInfos->ScreenWidth()), (GLsizei)(pGlobalInfos->ScreenHeight()));
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0f, (GLfloat)(pGlobalInfos->ScreenWidth())/(GLfloat)(pGlobalInfos->ScreenHeight()),
					1.0f, 10000.0f);		
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	// clear screen
	SDL_GL_SwapBuffers();
	SDL_Delay(25);
	
	// load all the 3D models and put them into the Graphic card's memory
	if (load3DModels()==false)
	{
		printf("3D model loading failed\n");
		return 5;
	}
	
	createModelList();
	unload3DModels();// now all the models are stored in the graphic card we don't need them
	
	// Init Artificial Intelligence
	// with size of the hash table in MBytes
	// TODO: Add this in options menu and conf. file
#ifdef DEBUG
	// Debug mode is not for serious play so AI level doesn't matter
	AIInit(1);
#else
	// 24MB will give very good and strong AI
	AIInit(24);
#endif	
	
	// GUI initialization
	pGlobalInfos->GUI_init();
	
	// Allocate scene
	Chess_Scene* chess_scene[10]={0};
	
	chess_scene[GAME_SCENE_INIT]		= new E_scene_init;
	chess_scene[GAME_SCENE_INTRO]		= new E_scene_intro;
	chess_scene[GAME_SCENE_MAIN_MENU]	= new E_scene_main_menu;
	chess_scene[GAME_SCENE_MAIN_GAME]	= new E_scene_main_game;
	// add new scenes right here
	
	// first scene is SCENE_INIT.
	pGlobalInfos->SetCurrentScene(GAME_SCENE_INIT);

	// main game loop
	while( pGlobalInfos->IsGameRunning() )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear screen
		glLoadIdentity();											// Load Identity matrix
		
		pGlobalInfos->Update();										// update game events and other things
		chess_scene[pGlobalInfos->CurrentScene()]->update();		// update the game
		
		SDL_GL_SwapBuffers();									// swap buffers : the scene is now displayed
	}
	
	clearScreen();
	
	// write current options in the configuration file
	pGlobalInfos->WriteGameConfiguration();
	
	// Free the graphic card's memory
	deleteModelsList();
	
	// unload hash table
	AIExit();
	
	// Game is done : unload everything
	for(int i=0;i<10;i++)
	{
		if (chess_scene[i]!=NULL)
		{
			chess_scene[i]->unload();
			SAFE_DELETE(chess_scene[i]);
		}
	}
	
	TexturesManager::flushAllTextures();

#ifdef DEBUG
	printf("Game properly terminated\n");
#endif
			
	delete pGlobalInfos;
	pGlobalInfos=NULL;
	
	SDL_Quit();
	return(0);
}

