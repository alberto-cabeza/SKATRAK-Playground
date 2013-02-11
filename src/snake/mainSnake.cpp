/*
 *  This file is part of SKATRAK Playground.
 *
 *  SKATRAK Playground is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/> or
 *  write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 *  Floor, Boston, MA 02110-1301 USA
 *
 *  Sergio M. Afonso Fumero <theSkatrak@gmail.com>
 */

#include <SKATRAK_PLAYGROUND.hpp>
#include <shared_attributes.hpp>
#include <menu.hpp>

/* Declaraciones de los submen�s del men� del Snake */
returnVal gameSnake(void*);

returnVal mainSnake(void*){
	// Fuente del men�
	font_t menuFont("AlteHaasGroteskRegular.ttf");
	menuFont.setColor(255, 128, 0);
	menuFont.setSize(48);

	// Men�
	menu_t menu(2);
	menu.setTexts(&menuFont);

	menu.setText(0, "Jugar");
	menu.setOpt(0, &gameSnake);
	menu.setText(1, "Atr�s");
	menu.setOpt(1, &backCallback);

	menu.setBackground("snake/menu_prueba.png");
	menu.setImage("marcador_prueba.png");
	menu.align(MENU_ALIGN_RIGHT | MENU_ALIGN_DOWN);

	SDL_Event event;
	SDL_Surface* screen = sistema->scr();
	timekeeper_t timer;

	while(true){
		timer.refresh();
		while(SDL_PollEvent(&event)){
			switch(menu.update(&event)){
			case ERROR:
				return ERROR;
			case EXIT:
				return EXIT;
			case ACTUAL_MENU:
				break;
			case MAIN:
			case PREV_MENU:
				return ACTUAL_MENU;
			}
		}
		menu.blit(screen);
		sistema->update();
		timer.waitFramerate(30);
	}
	return ERROR;
}