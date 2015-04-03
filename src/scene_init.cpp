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
 
 
#include "scene_init.h"
#include "globalInfos.h"
#include "utils.h"
#include "models.h"

//--------------------------------------------------//
//--------------------------------------------------//
//                 **  Chess_Scene **               //
//--------------------------------------------------//
//--------------------------------------------------//

Chess_Scene::Chess_Scene()
{
	already_loaded=false;
}

Chess_Scene::~Chess_Scene()
{

}

void Chess_Scene::finish_loading()
{
	SDL_Delay(100);
	

	pGlobalInfos->Update();
	pGlobalInfos->Update();
	
	already_loaded=true;	
}

//--------------------------------------------------//
//--------------------------------------------------//
//               **  E_scene_init **                //
//--------------------------------------------------//
//--------------------------------------------------//

E_scene_init::E_scene_init()
{

}


E_scene_init::~E_scene_init()
{

}


bool E_scene_init::load()
{
	if (already_loaded)
		return true;
	
	language="";
	language_selected = pGlobalInfos->LanguageExists();
	if (!language_selected)
	{
		// gui event listener
		pGlobalInfos->GUI_SetEvenListener(this);
		pGlobalInfos->GUI_setScene(GUI_SCENE_LANGUAGE_SELECTION);
		language = "english";// default language
	}
	
	finish_loading();
	return true;
}

void E_scene_init::unload()
{
	if (already_loaded==false)
		return;
	

	
	already_loaded=false;
}

void E_scene_init::update()
{
	if (already_loaded==false)
	{
		if ( load()==false )
		{
			Logger::writeErrorLog("Loading of 'E_scene_init' failed: exiting...");
			pGlobalInfos->QuitGame();
			return;
		}
	}
	
	if (language_selected)
	{
		pGlobalInfos->GUI_init_language(language);
		pGlobalInfos->GUI_setScene(GUI_SCENE_EMPTY);
		unload();
		pGlobalInfos->SetCurrentScene(GAME_SCENE_INTRO);
		return;
	}
	
	go_2D();
	
	pGlobalInfos->GUI_updateEvents();
	pGlobalInfos->GUI_render();

	exit_2D();

	SDL_Delay(10);

}


void E_scene_init::actionPerformed(GUIEvent &evt)
{
	const std::string &callbackString  = evt.getCallbackString();
	GUIRectangle      *sourceRectangle = evt.getEventSource();
	int                widgetType      = sourceRectangle->getWidgetType();
	
	if(widgetType == WT_BUTTON)
	{
		GUIButton   *button = (GUIButton*)sourceRectangle;
		if(button->isClicked())
		{
			if (callbackString == "okButton")
				language_selected=true;
		}
	}
	else if (widgetType == WT_RADIO_BUTTON)
	{
		GUIRadioButton *button = (GUIRadioButton*)sourceRectangle;
		
		if(button->isClicked())
			language=callbackString;	
	}
	
}



//--------------------------------------------------//
//--------------------------------------------------//
//              **  E_scene_intro **                //
//--------------------------------------------------//
//--------------------------------------------------//

E_scene_intro::E_scene_intro()
{

}

E_scene_intro::~E_scene_intro()
{

}

bool E_scene_intro::load()
{
	if (already_loaded)
		return true;
	

	
	
	
	finish_loading();
	return true;
}

void E_scene_intro::unload()
{
	if (already_loaded==false)
		return;
	

	
	already_loaded=false;
	
}

void E_scene_intro::update()
{
	if (already_loaded==false)
	{
		if ( load()==false )
		{
			Logger::writeErrorLog("Loading of 'E_scene_intro' failed: exiting...");
			pGlobalInfos->QuitGame();
			return;
		}
	}
	
	
	pGlobalInfos->SetCurrentScene(GAME_SCENE_MAIN_MENU);

	SDL_Delay(10);
}



