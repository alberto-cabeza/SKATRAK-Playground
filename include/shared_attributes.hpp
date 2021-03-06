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

#ifndef __SHARED_ATTRIBUTES__
#define __SHARED_ATTRIBUTES__

/* Inclusi�n de cabeceras necesarias */
#include <string>
using std::string;

/* Valores de retorno de los men�s */
enum returnVal {
	MAIN,			// Volver al men� principal
	ACTUAL_MENU,	// Volver al men� del juego actual
	PREV_MENU,		// Volver al men� anterior
	EXIT,			// Salir del juego
	ERROR			// Salir con error
};

/* Variables compartidas */
class music_t;
extern music_t* musica;

/* Funciones globales */
int loadTracklist(string path);

/* Funciones de callback comunes */
inline returnVal defaultCallback(void*){ return ACTUAL_MENU; }
inline returnVal exitCallback(void*){ return EXIT; }
inline returnVal backCallback(void*){ return PREV_MENU; }

#endif
