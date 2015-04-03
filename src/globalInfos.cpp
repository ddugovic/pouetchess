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
 
 
#include "globalInfos.h"
#include "data.h"
#include <fstream>
#include <sstream>


/* result flags (from faile.h): */
#define no_result      0
#define stalemate      1
#define white_is_mated 2
#define black_is_mated 3
#define draw_by_fifty  4
#define draw_by_rep    5


GlobalInfos::GlobalInfos()
{
	game_is_running				= true;
	has_focus					= true;
	
	full_screen					= true;
	on_left_mouse_button_down	= false;
	on_left_mouse_button_up		= false;
	is_left_mouse_button_down	= false;
	on_right_mouse_button_down	= false;
	wheel_up					= false;
	wheel_down					= false;
			
	screen_width				= DEFAULT_WIDTH;
	screen_height				= DEFAULT_HEIGHT;
	new_screen_width			= screen_width;
	new_screen_height			= screen_height;
	last_time					= SDL_GetTicks();
	
	ai_level					= 3;
	selection_arrow				= false;
	display_move_possibilities	= true;
}


GlobalInfos::~GlobalInfos()
{
	
}


void GlobalInfos::Update()
{

	elapsed_time=SDL_GetTicks()-last_time;
	last_time=SDL_GetTicks();
	
	on_left_mouse_button_down	= false;
	on_left_mouse_button_up		= false;
	is_left_mouse_button_down	= false;
	on_right_mouse_button_down	= false;
	wheel_up					= false;
	wheel_down					= false;
			
	gui_key	= 0;
	
	while( SDL_PollEvent(&event) )
	{
		switch( event.type )
		{
			// -------------------------------------- // 
			case SDL_ACTIVEEVENT:
				if ( event.active.gain == 0 )
					has_focus = false;
				else
					has_focus = true;
				break;
				
			// -------------------------------------- // 
			case SDL_KEYDOWN:
				if (event.key.keysym.sym<1024)
				{
					key_state[event.key.keysym.sym]=true;
					if (event.key.keysym.sym<256)
						last_pressed_key=event.key.keysym.sym;
				}
				if ( (event.key.keysym.unicode & 0xFF80) == 0 ) 
				{
					gui_key = event.key.keysym.unicode & 0x7F;
				}
				else
					gui_key = 0;
				break;
				
			// -------------------------------------- // 
			case SDL_KEYUP:
				if (event.key.keysym.sym<1024)
					key_state[event.key.keysym.sym]=false;
				gui_key = 0;
				break;
				
			// -------------------------------------- // 
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button==SDL_BUTTON_LEFT)
				{
					on_left_mouse_button_down=true;
					is_left_mouse_button_down=true;
				}
				if (event.button.button==SDL_BUTTON_RIGHT)
				{
					on_right_mouse_button_down=true;
					is_right_mouse_button_down=true;
				}
				if (event.button.button==SDL_BUTTON_WHEELUP)
					wheel_up=true;
				if (event.button.button==SDL_BUTTON_WHEELDOWN)
					wheel_down=true;
				break;
				
			// -------------------------------------- // 
			case SDL_MOUSEBUTTONUP:
				if (event.button.button==SDL_BUTTON_LEFT)
				{
					on_left_mouse_button_up=true;
					is_left_mouse_button_down=false;
				}
				if (event.button.button==SDL_BUTTON_RIGHT)
				{
					on_right_mouse_button_up=true;
					is_right_mouse_button_down=false;
				}
				break;
				
			// -------------------------------------- // 	
			case SDL_MOUSEMOTION:
				mouse_x=event.motion.x;
				mouse_y=event.motion.y;
				break;
				
			// -------------------------------------- // 
			case SDL_QUIT:
				game_is_running=false;
				break;

			// -------------------------------------- // 
			default:
				break;
		}
	}
}


void GlobalInfos::SetCurrentScene(Game_scene current_scene_)
{
	current_scene=current_scene_;
}



bool GlobalInfos::KeyDown(SDLKey aKey)
{
	if (aKey<1024)
		return key_state[aKey];
	else
		return false;
}



void GlobalInfos::ReleaseKeys()
{
	for(int i=0;i<1024;i++)
		key_state[i]=0;
}


void GlobalInfos::GUI_init()
{
	SDL_EnableUNICODE(1);
	MediaPathManager::registerPath(PC_DATA_GUI_TEXTURES);
	MediaPathManager::registerPath(PC_DATA_GUI);
	guiFrame.GUIPanel::loadXMLSettings("pouetChess.xml");	
}

void GlobalInfos::GUI_init_language(const std::string& l)
{
	if (language=="")
		language=l;
	
	string path(PC_DATA_GUI);
	path+=language+"/";
	MediaPathManager::registerPath(path);
	
	string xml_file("pouetChess_");
	xml_file+=language+".xml";
	guiFrame.GUIPanel::loadXMLSettings(xml_file.c_str());
}


void GlobalInfos::GUI_render()
{
	guiFrame.render(elapsed_time/1000.0f);
}


void GlobalInfos::GUI_updateEvents()
{
	int x=(int) (1024.0f*mouse_x/(float)screen_width);
	int y=(int) (768.0f*mouse_y/(float)screen_height);
		
	MouseEvent event = MouseEvent(MB_UNKNOWN_BUTTON, x, y, 1024 - y);
	guiFrame.checkMouseEvents(event, ME_MOVED);
	if (on_left_mouse_button_down)
	{
		event = MouseEvent(MB_BUTTON1, x, y, 1024 - y);
		guiFrame.checkMouseEvents(event, ME_CLICKED);
	}
	if (on_left_mouse_button_up)
	{
		event = MouseEvent(MB_BUTTON1, x, y, 1024 - y);
		guiFrame.checkMouseEvents(event, ME_RELEASED);
	}
	if (gui_key)
		guiFrame.checkKeyboardEvents(KeyEvent(gui_key), KE_PRESSED);
}



void GlobalInfos::GUI_setScene(GUI_scene gui_scene)
{
	
	if (gui_scene == GUI_SCENE_LANGUAGE_SELECTION)
	{
		GUIRectangle* language_selection	= NULL;
		language_selection=guiFrame.getWidgetByCallbackString("language_selection");
		if (!language_selection)
		{
			Logger::writeErrorLog("BAD gui descritpion. Check language_selection XML file.");
			game_is_running=false;
			return;
		}
		language_selection->setVisible(true);
		return;
	}
				   

	GUIRectangle* main_menu_buttons		= NULL;
	GUIRectangle* main_menu_options		= NULL;	
	GUIRectangle* main_game_buttons		= NULL;
	GUIRectangle* pawn_promotion		= NULL;
	GUIRectangle* language_selection	= NULL;
	GUIRectangle* results				= NULL;
	GUIRectangle* about					= NULL;
	main_menu_buttons=guiFrame.getWidgetByCallbackString("main_menu_buttons");
	main_menu_options=guiFrame.getWidgetByCallbackString("main_menu_options");
	main_game_buttons=guiFrame.getWidgetByCallbackString("main_game_buttons");
	pawn_promotion=guiFrame.getWidgetByCallbackString("pawn_promotion");
	language_selection=guiFrame.getWidgetByCallbackString("language_selection");
	results=guiFrame.getWidgetByCallbackString("results");
	about=guiFrame.getWidgetByCallbackString("aboutScene");
	
	bool all_scenes_ok=(main_menu_buttons && 
						main_menu_options && 
						main_game_buttons &&
						language_selection&&
						pawn_promotion    &&
						results           &&
						about);
	
	if (!all_scenes_ok)
	{
		Logger::writeErrorLog("BAD gui descritpion. Check XML files.");
		game_is_running=false;
		return;
	}
	
	language_selection->setVisible(false);
	main_menu_options->setVisible(false);
	main_menu_buttons->setVisible(false);
	main_game_buttons->setVisible(false);
	pawn_promotion->setVisible(false);
	results->setVisible(false);
	about->setVisible(false);
	
	
	switch(gui_scene)
	{
		case GUI_SCENE_EMPTY:
			break;
			
		case GUI_SCENE_MAIN_MENU:
			main_menu_buttons->setVisible(true);
			break;
			
		case GUI_SCENE_MAIN_GAME:
			main_game_buttons->setVisible(true);
			break;
			
		case GUI_SCENE_OPTION_MENU:
			main_menu_options->setVisible(true);
			break;
			
		case GUI_SCENE_ABOUT:
			about->setVisible(true);
			break;
			
		default:
			break;
	}
}


void GlobalInfos::GUI_displayPromotion(bool yes_or_no)
{
	GUIRectangle* pawn_promotion		= NULL;
	pawn_promotion=guiFrame.getWidgetByCallbackString("pawn_promotion");
	if (!pawn_promotion)
	{
		Logger::writeErrorLog("BAD gui descritpion. Check pawn_promotion file.");
		game_is_running=false;
		return;
	}
	pawn_promotion->setVisible(yes_or_no);
}


void GlobalInfos::GUI_displayResult(int result)
{
	GUIRectangle* results				= NULL;
	GUIRectangle* string_result			= NULL;
	GUIRectangle* result_to_display		= NULL;
	results=guiFrame.getWidgetByCallbackString("results");
	if (!results)
	{
		Logger::writeErrorLog("BAD gui descritpion. Check results file.");
		game_is_running=false;
		return;
	}
	results->setVisible(false);
	
	
	if (result==no_result)
		return;
		
	results->setVisible(true);
	switch (result)
	{
		case stalemate:
			string_result=guiFrame.getWidgetByCallbackString("stalemate");
			break;
		case white_is_mated:
			string_result=guiFrame.getWidgetByCallbackString("white_is_mated");
			break;
		case black_is_mated:
			string_result=guiFrame.getWidgetByCallbackString("black_is_mated");
			break;
		case draw_by_fifty:
			string_result=guiFrame.getWidgetByCallbackString("draw_by_fifty");
			break;
		case draw_by_rep:
			string_result=guiFrame.getWidgetByCallbackString("draw_by_rep");
			break;
		
		default:
			string_result=guiFrame.getWidgetByCallbackString("draw");
			break;
		
	}
	
	if (!string_result)
	{
		Logger::writeErrorLog("BAD gui descritpion. Check Strings file.");
		game_is_running=false;
		return;
	}
	result_to_display=guiFrame.getWidgetByCallbackString("Result_string");
	if (!result_to_display)
	{
		Logger::writeErrorLog("BAD gui descritpion. Check for Result_string label in results.xml");
		game_is_running=false;
		return;
	}
	GUILabel* str_src=(GUILabel*)string_result;
	GUILabel* str_dest=(GUILabel*)result_to_display;
	str_dest->setLabelString(str_src->getLabelString());


}


void GlobalInfos::GUI_SetEvenListener(GUIEventListener* listener)
{
	guiFrame.setGUIEventListener(listener);
}



void GlobalInfos::GUI_updateOptions(bool save)
{
	
	GUICheckBox* gui_display_move_pos			= NULL;
	GUICheckBox* gui_display_selection_arrow	= NULL;		
	GUIComboBox* gui_language					= NULL;
	GUICheckBox* gui_fullscreen					= NULL;
	GUITextBox*	 gui_screen_width				= NULL;
	GUITextBox*	 gui_screen_height				= NULL;
	GUIComboBox* gui_level_ai					= NULL;		
	
	
	gui_display_move_pos=(GUICheckBox*)guiFrame.getWidgetByCallbackString("Display move possibilities");
	gui_display_selection_arrow=(GUICheckBox*)guiFrame.getWidgetByCallbackString("Display selection arrow");
	gui_language=(GUIComboBox*)guiFrame.getWidgetByCallbackString("cbLanguages");
	gui_fullscreen=(GUICheckBox*)guiFrame.getWidgetByCallbackString("Full screen");
	gui_screen_width=(GUITextBox*)guiFrame.getWidgetByCallbackString("scr_width");
	gui_screen_height=(GUITextBox*)guiFrame.getWidgetByCallbackString("scr_height");
	gui_level_ai=(GUIComboBox*)guiFrame.getWidgetByCallbackString("ai_level");
	
	
	bool all_ok=(gui_display_move_pos && 
			gui_display_selection_arrow && 
			gui_language &&
			gui_fullscreen&&
			gui_screen_width    &&
			gui_screen_height           &&
			gui_level_ai);
	
	if (!all_ok)
	{		
		Logger::writeErrorLog("BAD gui descritpion in main_menu_options.xml");
		game_is_running=false;
		return;
	}
			
	if (save)
	{
		display_move_possibilities	= gui_display_move_pos->isChecked();
		selection_arrow				= gui_display_selection_arrow->isChecked();
		language					= gui_language->getSelectedItem();
		full_screen					= gui_fullscreen->isChecked();
		new_screen_width			= atoi(gui_screen_width->getText().c_str());
		new_screen_height			= atoi(gui_screen_height->getText().c_str());
		ai_level					= gui_level_ai->getItemIndex(gui_level_ai->getSelectedItem());
	 
	}
	else
	{
		ostringstream oss_w, oss_h;
		oss_w << new_screen_width;
		oss_h << new_screen_height;
		
		gui_display_move_pos->setChecked(display_move_possibilities);
		gui_display_selection_arrow->setChecked(selection_arrow);
		gui_fullscreen->setChecked(full_screen);
		
		gui_screen_width->setText(oss_w.str());
		gui_screen_height->setText(oss_h.str());
		
		gui_level_ai->setItemIndex(ai_level);
		GUI_UpdateLevelInfos(ai_level);
		gui_language->setItemIndex(gui_language->getItemIndex(language));
	}
}


void GlobalInfos::GUI_UpdateLevelInfos(int lvl)
{
	GUILabel*    gui_info_time					= NULL;
	GUIComboBox* gui_level_ai					= NULL;
		
	gui_info_time=(GUILabel*)guiFrame.getWidgetByCallbackString("Info_time");
	gui_level_ai=(GUIComboBox*)guiFrame.getWidgetByCallbackString("ai_level");
	
	if (!gui_info_time)
	{		
		Logger::writeErrorLog("BAD gui descritpion in main_menu_options.xml : Cannot find 'Info_time' CallbackString");
		game_is_running=false;
		return;
	}
	if (!gui_level_ai)
	{
		Logger::writeErrorLog("BAD gui descritpion in main_menu_options.xml : Cannot find 'ai_level' CallbackString");
		game_is_running=false;
		return;
	}
	
	if (lvl<0)
		lvl=gui_level_ai->getItemIndex(gui_level_ai->getSelectedItem());
	
	int time_to_think;
	
	if (lvl<0 || lvl>7)
		return;
		
	switch (lvl)
	{
		case 0:
			time_to_think=2;
			break;
			
		case 7:
			time_to_think=90;
			break;
			
		default:
			time_to_think=lvl*10;
			break;
	}
	std::string label=gui_info_time->getLabelString();
	// find the first non-whitespace character
	std::string::size_type pos;
	pos = label.find_first_of(":");
	
	ostringstream new_label;
	new_label << label.substr(0,pos) << ": " << time_to_think;
	
	gui_info_time->setLabelString(new_label.str());
}


bool GlobalInfos::LoadGameConfiguration()
{
	
#ifdef _WIN32
	std::string strFilename("config.ini");
#else
    std::string strFilename(getenv("HOME"));
	strFilename+="/.pouetChess";
#endif
	
	std::string error_msg;

	std::ifstream file(strFilename.c_str());
	if(!file)
		return true;

	// parse all lines from the configuration file
	for(int line = 1; ; line++)
	{
		// read the next configuration line
		std::string strBuffer;
		std::getline(file, strBuffer);

		// stop if we reached the end of file
		if(file.eof()) break;

		// check if an error happend while reading from the file
		if(!file)
		{
			error_msg="Error while reading configuration file : '" + strFilename;
			Logger::writeErrorLog(error_msg);
			return false;
		}

		// find the first non-whitespace character
		std::string::size_type pos;
		pos = strBuffer.find_first_not_of(" \t");

		// check for empty lines
		if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

		// check for comment lines
		if(strBuffer[pos] == '#') continue;

		// get the key
		std::string strKey;
		strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
		pos += strKey.size();

		// get the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos);
		if((pos == std::string::npos) || (strBuffer[pos] != '='))
		{
			error_msg="Syntax error in the configuration file : '" + strFilename;
			Logger::writeErrorLog(error_msg);
			return false;
		}

		// find the first non-whitespace character after the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos + 1);

		// get the data
		std::string strData;
		strData = strBuffer.substr(pos, strBuffer.find_first_of("\t\n\r #", pos) - pos);

		// width
		if(strKey == "Width")
		{
			screen_width = atoi(strData.c_str());
		}
		else if(strKey == "Height")
		{
			screen_height = atoi(strData.c_str());
		}
		else if(strKey == "Fullscreen")
		{
			if (strData=="yes")
				full_screen = true;
			else
				full_screen = false;
		}
		else if(strKey == "AI_level")
		{
			ai_level = atoi(strData.c_str());
		}
		else if(strKey == "Selection_arrow")
		{
			if (strData=="yes")
				selection_arrow=true;
			else
				selection_arrow=false;
		}
		else if(strKey == "Move_pos")
		{
			if (strData=="yes")
				display_move_possibilities=true;
			else
				display_move_possibilities=false;
		}	
		else if(strKey == "Language")
		{
			language = strData;
		}
		
	}
	
	new_screen_width			= screen_width;
	new_screen_height			= screen_height;

	return true;
}



void GlobalInfos::WriteGameConfiguration()
{
	
#ifdef _WIN32
	std::string strFilename("config.ini");
#else
    std::string strFilename(getenv("HOME"));
	strFilename+="/.pouetChess";
#endif

	std::ofstream cf(strFilename.c_str());
	if (!cf)
	{
		std::string error_msg("Unable to create configuration file : ");
		error_msg+=strFilename;
		Logger::writeErrorLog(error_msg);
		game_is_running=false;
		return;
	}
	
	std::string s_full_screen(full_screen?"yes":"no");
	std::string s_selection_arrow(selection_arrow?"yes":"no");
	std::string s_display_move_possibilities(display_move_possibilities?"yes":"no");
	
	cf << "##################################################################"	<< endl;
	cf << "#" 																	<< endl;
	cf << "#" 																	<< endl;
	cf << "#	Configuration file for pouetChess version " << GAME_VERSION 	<< endl;
	cf << "#" 																	<< endl;
	cf << "#" 																	<< endl;
	cf << "##################################################################"	<< endl;
	cf << endl << endl 															<< endl;
	cf << "##################################################################"	<< endl;
	cf << "#" 																	<< endl;
	cf << "#	Screen configuration"											<< endl;
	cf << "#" 																	<< endl;
	cf << endl																	<< endl;
	cf << "# Screen width "														<< endl;
	cf << "Width			=	" << new_screen_width							<< endl;
	cf << "# Screen Height "													<< endl;
	cf << "Height			=	" << new_screen_height	 						<< endl;
	cf << "# Full screen mode "													<< endl;
	cf << "Fullscreen		=	" << s_full_screen 								<< endl;
	cf << endl << endl 															<< endl;
	cf << "##################################################################"	<< endl;
	cf << "#" 																	<< endl;
	cf << "#	Artificial Intelligence configuration"							<< endl;
	cf << "#" 																	<< endl;
	cf << endl																	<< endl;
	cf << "# AI level"															<< endl;
	cf << "# From 0 (Newbie) to 7 (I'm Kasparov ;))"							<< endl;
	cf << "AI_level			=	" << ai_level 									<< endl;
	cf << endl << endl 															<< endl;
	cf << "##################################################################"	<< endl;
	cf << "#" 																	<< endl;
	cf << "#	Language"														<< endl;
	cf << "#" 																	<< endl;
	cf << endl																	<< endl;
	cf << "Language		=	" << language 										<< endl;
	cf << endl << endl 															<< endl;
	cf << "##################################################################"	<< endl;
	cf << "#" 																	<< endl;
	cf << "#	Other features configuration"									<< endl;
	cf << "#" 																	<< endl;
	cf << endl																	<< endl;
	cf << "# Draw a selection arrow above the current selected piece"			<< endl;
	cf << "# Some people may like it..."										<< endl;
	cf << "Selection_arrow	=	" << s_selection_arrow							<< endl;
	cf << endl																	<< endl;
	cf << "# Display move possibilites when a piece is selected"				<< endl;
	cf << "Move_pos	=	" << s_display_move_possibilities						<< endl;
	cf << endl << endl 															<< endl;
	cf << "#" 																	<< endl;
	cf << "#	End of the configuration file"									<< endl;
	cf << "#" 																	<< endl;
	cf << "#" 																	<< endl;
	cf << "##################################################################"	<< endl;

}




