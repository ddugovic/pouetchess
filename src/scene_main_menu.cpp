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


#include "scene_main_menu.h"
#include "utils.h"
#include "data.h"

//--------------------------------------------------//
//--------------------------------------------------//
//         **  E_scene_main_menu **                 //
//--------------------------------------------------//
//--------------------------------------------------//



E_scene_main_menu::E_scene_main_menu()
{
	texture_background	= NULL;
}

E_scene_main_menu::~E_scene_main_menu()
{
	
}



bool E_scene_main_menu::load()
{
	if (already_loaded)
		return true;
	
	pGlobalInfos->GUI_SetEvenListener(this);
	
	texture_background = new CBasicTexture;
	if (false==texture_background->load(PC_DATA_TEXTURE_BACKGROUND,false))
	{
		Logger::writeErrorLog("Loading of :"PC_DATA_TEXTURE_BACKGROUND" failed");
		return false;
	}

	texture_background->SetTexParameter(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	texture_background->SetTexParameter(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	Initialize_scene(GUI_SCENE_MAIN_MENU);
	
	init_fade_out();
	fade_out=true;
	fade_in=false;

	finish_loading();
	return true;
}



void E_scene_main_menu::unload()
{
	if (already_loaded==false)
		return;
	
	SAFE_DELETE(texture_background);
	
	already_loaded=false;
}



void E_scene_main_menu::update()
{
	if (already_loaded==false)
	{
		if ( load()==false )
		{
			Logger::writeErrorLog("Loading of 'E_scene_main_menu' failed: exiting...");
			pGlobalInfos->QuitGame();
			return;
		}
	}
	
	switch(menu_to_display)
	{
		case GUI_SCENE_MAIN_MENU:
			display_main_menu();
			break;
			
		case GUI_SCENE_ABOUT:
			display_about();
			break;
			
		case GUI_SCENE_OPTION_MENU:
			display_options();
			break;
	}
	
	
	go_2D();
	
	glColor3f(1.0f,1.0f,1.0f);
	draw_image(0,0,1024,1024,texture_background);
	
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
}




void E_scene_main_menu::Initialize_scene(GUI_scene scene_to_init)
{
	
	// set the new game scene
	menu_to_display=scene_to_init;
	
	// set the new GUI scene
	pGlobalInfos->GUI_setScene(scene_to_init);
}





void E_scene_main_menu::display_main_menu()
{	
	SDL_Delay(15);
	if (pGlobalInfos->KeyDown(SDLK_ESCAPE))
	{
		pGlobalInfos->ReleaseKeys();
		pGlobalInfos->QuitGame();
		return;
	}
}


void E_scene_main_menu::display_options()
{	
	SDL_Delay(15);
}


void E_scene_main_menu::display_about()
{
	if (pGlobalInfos->KeyDown(SDLK_ESCAPE) || pGlobalInfos->KeyDown(SDLK_RETURN))
	{
		pGlobalInfos->ReleaseKeys();
		Initialize_scene( GUI_SCENE_MAIN_MENU );
	}

	SDL_Delay(15);
}





void E_scene_main_menu::actionPerformed(GUIEvent &evt)
{
	
	
	const std::string &callbackString  = evt.getCallbackString();
	GUIRectangle      *sourceRectangle = evt.getEventSource();
	int                widgetType      = sourceRectangle->getWidgetType();

	if(widgetType == WT_BUTTON)
	{
		GUIButton   *button = (GUIButton*)sourceRectangle;
		if(button->isClicked())
		{
			if (callbackString == "onePlayer")
			{
				pGlobalInfos->SetGameMode(PLAYER_VS_AI);
				scene_to_go=GAME_SCENE_MAIN_GAME;
				init_fade_in();
				fade_in=true;
			}
			
			if (callbackString == "twoPlayers")
			{
				pGlobalInfos->SetGameMode(PLAYER_VS_PLAYER);
				scene_to_go=GAME_SCENE_MAIN_GAME;
				init_fade_in();
				fade_in=true;			
			}
			if (callbackString == "options")
			{
				pGlobalInfos->GUI_updateOptions(false);
				Initialize_scene(GUI_SCENE_OPTION_MENU);
			}
			if (callbackString == "about")
				Initialize_scene(GUI_SCENE_ABOUT);
			if(callbackString == "quit")
				pGlobalInfos->QuitGame();
			
			if(callbackString == "okButton")
			{
				pGlobalInfos->GUI_updateOptions(true);
				Initialize_scene(GUI_SCENE_MAIN_MENU);
			}
			
			if(callbackString == "cancelButton")
				Initialize_scene(GUI_SCENE_MAIN_MENU);
			
		}
	}
	else if(widgetType == WT_COMBO_BOX)
	{
		if(callbackString == "ai_level")
			pGlobalInfos->GUI_UpdateLevelInfos(-1);
		
	}
}


