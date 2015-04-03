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

#ifndef __GLOBAL_INFOS_H__
#define __GLOBAL_INFOS_H__



#include <SDL/SDL.h>
#include <string>

#include "sxmlgui/EasyGL.h"	// contains <SDL/SDL_opengl.h>
#include "scenes.h"


#define GAME_VERSION						"0.2.0"


#define	DEFAULT_WIDTH						1024
#define DEFAULT_HEIGHT						768


enum GUI_scene
{
	GUI_SCENE_EMPTY,
	GUI_SCENE_LANGUAGE_SELECTION,
	GUI_SCENE_MAIN_MENU,
	GUI_SCENE_MAIN_GAME,
	GUI_SCENE_OPTION_MENU,
	GUI_SCENE_ABOUT,
};

enum GameMode
{
	PLAYER_VS_PLAYER,
	PLAYER_VS_AI,
};



/**
 * @class GlobalInfos: used to manage game events and more.
 * It must be only one instance of this class in the game. This class manages all game events and more.
 */
class GlobalInfos
{
	public:
		GlobalInfos();
		~GlobalInfos();
		void Update();
		void SetCurrentScene(Game_scene current_scene_);
		void ReleaseKeys();
		void QuitGame()									{game_is_running=false;}
		void SetScreenWidth(int w)						{screen_width=w;}
		void SetHeightScreen(int h)						{screen_height=h;}
		void SetFullScreen(bool fullScreen)				{full_screen=fullScreen;}
		void WriteGameConfiguration();
		bool LoadGameConfiguration();
		bool KeyDown(SDLKey aKey);
		bool IsFullScreen()								{return full_screen;}
		bool GotFocus()									{return has_focus;}
		bool OnLeftMouseButtonDown()					{return on_left_mouse_button_down;}
		bool OnLeftMouseButtonUp()						{return on_left_mouse_button_up;}
		bool IsLeftMouseButtonDown()					{return is_left_mouse_button_down;}
		bool OnRightMouseButtonDown()					{return on_right_mouse_button_down;}
		bool OnRightMouseButtonUp()						{return on_right_mouse_button_up;}
		bool IsRightMouseButtonDown()					{return is_right_mouse_button_down;}
		bool OnMouseWheelUp()							{return wheel_up;}
		bool OnMouseWheelDown()							{return wheel_down;}
		bool IsGameRunning() 							{return game_is_running;}
		int MouseX()									{return mouse_x;}
		int MouseY()									{return mouse_y;}
		int CurrentScene()								{return current_scene;}
		int ScreenWidth()								{return screen_width;}
		int ScreenHeight()								{return screen_height;}
		unsigned char LastPressedKey()					{return last_pressed_key;}
		Uint32 ElapsedTime()							{return elapsed_time;}
		
		// ---------------------------------
		// Game specific
		// ---------------------------------

		void SetSelectionArrow(bool arrow)				{selection_arrow=arrow;}
		bool GetSelectionArrow()						{return selection_arrow;}
		void SetAILevel(unsigned short lvl)				{ai_level=lvl;}
		unsigned short GetAILevel()						{return ai_level;}
		void SetModelsList(GLuint ml)					{models_list=ml;}
		GLuint GetModelsList()							{return models_list;}
		void SetGameMode(GameMode mode)					{game_mode=mode;}
		GameMode GetGameMode()							{return game_mode;}
		bool LanguageExists()							{return language!="";}
		bool display_possible_moves()					{return display_move_possibilities;}
		
		// ---------------------------------
		// GUI
		// ---------------------------------
		void GUI_init();
		void GUI_render();
		void GUI_updateEvents();
		void GUI_setScene(GUI_scene gui_scene);
		void GUI_init_language(const std::string& l);
		void GUI_SetEvenListener(GUIEventListener* listener);
		void GUI_displayPromotion(bool yes_or_no);
		void GUI_displayResult(int result);
		void GUI_updateOptions(bool save);
		void GUI_UpdateLevelInfos(int lvl);
	
	private:
		bool full_screen;
		bool game_is_running;
		bool has_focus;
		bool on_left_mouse_button_down;
		bool on_left_mouse_button_up;
		bool is_left_mouse_button_down;
		bool on_right_mouse_button_down;
		bool on_right_mouse_button_up;
		bool is_right_mouse_button_down;
		bool wheel_up;
		bool wheel_down;
		bool key_state[1024];
		char gui_key;
		int mouse_x;
		int mouse_y;
		int screen_width, screen_height;
		int new_screen_width,new_screen_height;
		unsigned char last_pressed_key;
		Game_scene current_scene;
		Uint32 elapsed_time, last_time;
		SDL_Event event;
		
		// ---------------------------------
		// Game specific
		// ---------------------------------
		bool selection_arrow;
		bool display_move_possibilities;
		unsigned short ai_level;
		GLuint models_list;
		GameMode game_mode;
		std::string language;
		
		
		// ---------------------------------
		// GUI
		// ---------------------------------
		GUIFrame      guiFrame;
		
};


// One and only global info instance in the game
extern GlobalInfos* pGlobalInfos;


#endif
