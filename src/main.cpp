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

/* Inclusi�n de cabeceras necesarias */
// Est�ndar
#include <time.h>	// Para time();
// SDL
#include "SDL/SDL.h"
// Propias
#include "SKATRAK_PLAYGROUND.hpp"
#include "shared_attributes.hpp"
#include "system.hpp"
#include "music.hpp"
#include "image.hpp"
#include "font.hpp"
#include "timekeeper.hpp"

/* Definiciones de las rutas por defecto de los recursos */
// Definimos manualmente _RELEASE_VER si queremos distribuir el ejecutable (Utilizando otra estructura de directorios)
//#define _RELEASE_VER
#ifdef _RELEASE_VER
const char* MUS_PATH = "./resources/sound/";
const char* SFX_PATH = "./resources/sound/SFX/";
const char* FONT_PATH = "./resources/fonts/";
const char* IMG_PATH = "./resources/images/";
const char* INI_PATH = "./resources/settings/";
const char* MAP_PATH = "./resources/maps/";
#else
const char* MUS_PATH = "../../resources/sound/";
const char* SFX_PATH = "../../resources/sound/SFX/";
const char* FONT_PATH = "../../resources/fonts/";
const char* IMG_PATH = "../../resources/images/";
const char* INI_PATH = "../../resources/settings/";
const char* MAP_PATH = "../../resources/maps/";
#endif

/* Prototipo del men� principal */
returnVal mainMenu();
void startScreen(void);

system_t* sistema = NULL;			// La variable sistema se comparte entre todos los minijuegos y permanece igual para todos ellos.
music_t* musica = NULL;				// La variable musica se comparte por todos los minijuegos y cada uno puede tener su propia lista de reproducci�n.

int main(int argc, char* argv[]){
	// Inicializamos la semilla generadora de n�meros aleatorios
	srand((unsigned int)time(NULL));

	// Creamos la variable sistema (Iniciamos SDL)
	sistema = new system_t(1024, 768, 32);
	sistema->setIcon("icono_prueba.png");

	// Cargamos la lista de reproducci�n del men� principal
	musica = new music_t;
	loadTracklist("menu_setlist.ini");

	// Activamos la repetici�n de teclas para los men�s (Los juegos que no usen esta funcionalidad la tienen que volver a activar al salir)
	SDL_EnableKeyRepeat(250, 75);

	// Im�genes para mostrar en el men� inicio
	if(sistema->scr() == NULL){
		fprintf(stderr, "La pantalla no se ha iniciado. Saliendo del programa...\n");
		return 1;
	}

	// Pantalla inicial
	startScreen();

	// Paramos la m�sica antes de salir y liberamos recursos
	musica->halt();
	if(musica != NULL)
		delete musica;
	if(sistema != NULL)
		delete sistema;
	return 0;
}

void startScreen(){
	// Pantalla
	SDL_Surface* screen = sistema->scr();

	// Elementos de la pantalla
	image_t fondo("fondo_inicio_prueba.png");
	font_t nombreJuego("BOOTERFF.ttf");
	nombreJuego.setSize(72);
	nombreJuego.setText("SKATRAK Playground");
	nombreJuego.setColor(89, 72, 101);
	font_t empezar("BOOTERFF.ttf");
	empezar.setSize(32);
	empezar.setText("Pulse una tecla para empezar");
	empezar.setColor(89, 72, 101);
	timekeeper_t temporizador;

	// Variables para controlar el game loop
	SDL_Event event;
	bool salir = false;

	// Comenzamos a reproducir la m�sica
	musica->play();

	int alpha = 0;
	while((int)alpha < SDL_ALPHA_OPAQUE){
		temporizador.refresh();
		nombreJuego.setAlpha((int)alpha);
		fondo.blit(0, 0, screen);
		nombreJuego.blit((int)(screen->w / 2) - (int)(nombreJuego.width() / 2), (int)(screen->h / 2) - (int)(nombreJuego.height() / 2), screen);
		sistema->update();
		temporizador.waitFramerate(30);
		alpha += 3;
		while(SDL_PollEvent(&event)); // Vaciamos la cola de eventos para que no se propaguen por otros men�s
	}
	nombreJuego.setAlpha(SDL_ALPHA_OPAQUE);

	bool alphaAdd = false;
	while(!salir){
		// Reiniciamos el temporizador en cada ciclo
		temporizador.refresh();
		// Gestionamos los eventos
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					if(mainMenu() == ERROR)
						fprintf(stderr, "Se ha salido del programa con un error.\n");
					salir = true;
					break;
				case SDL_QUIT:
					salir = true;
			}
		}
		// Para dar una sensaci�n de texto parpadeante
		alpha += (alphaAdd)? 5 : -5;

		if(alpha <= SDL_ALPHA_TRANSPARENT) alphaAdd = true;
		else if(alpha >= SDL_ALPHA_OPAQUE - 5) alphaAdd = false;
		empezar.setAlpha(alpha);

		// Imprimimos por pantalla todo lo que haga falta e intercambiamos los buffers de v�deo
		fondo.blit(0, 0, screen);
		nombreJuego.blit((int)(screen->w / 2) - (int)(nombreJuego.width() / 2), (int)(screen->h / 2) - (int)(nombreJuego.height() / 2), screen);
		empezar.blit((int)(screen->w / 2) - (int)(empezar.width() / 2), (int)(3 * screen->h / 4) - (int)(empezar.height() / 2), screen);
		if(!salir)
			sistema->update();	// Esto es para que no se muestre la pantalla de inicio al salir

		// Fijamos los FPS a 30
		temporizador.waitFramerate(30);
	}
}
