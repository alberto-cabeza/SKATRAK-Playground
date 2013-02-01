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
#include <snake/snakePiece.hpp>

/**
 * @brief Constructor por defecto. Inicializa las variables de la clase con un valor conocido.
 */
snakePiece_t::snakePiece_t(): x(-1), y(-1), next(NULL), prev(NULL)
{
}

/**
 * @brief Constructor. Inicializa la posici�n del eslab�n.
 * @posX Posici�n del eslab�n en el eje X.
 * @posY Posici�n del eslab�n en el eje Y.
 * @param (OPCIONAL) Puntero al elemento anterior de la lista.
 */
snakePiece_t::snakePiece_t(int posX, int posY, snakePiece_t* prevPiece): x(posX), y(posY), next(NULL), prev(prevPiece)
{
}

/**
 * @brief Constructor de copia.
 * @param copy Referencia al objeto snakePiece_t al que copiar.
 * @param prevPiece (OPCIONAL) Puntero al elemento anterior de la lista.
 */
snakePiece_t::snakePiece_t(snakePiece_t* copy, snakePiece_t* prevPiece): x(copy->x), y(copy->y), next(NULL), prev(prevPiece)
{
}

/**
 * @brief Sit�a el eslab�n en una posici�n.
 * @param posX Posici�n del eslab�n en el eje X.
 * @param posY Posici�n del eslab�n en el eje Y.
 */
void snakePiece_t::setPos(int posX, int posY){
	x = posX;
	y = posY;
}

/**
 * @brief Mueve el eslab�n una posici�n hacia una direcci�n.
 * @param direction Direcci�n hacia la que mover el eslab�n
 */
void snakePiece_t::move(Direction direction){
	switch(direction){
	case MOVE_UP:
		y--;
		break;
	case MOVE_DOWN:
		y++;
		break;
	case MOVE_LEFT:
		x--;
		break;
	case MOVE_RIGHT:
		x++;
		break;
	}
}

/**
 * @brief Imprime el eslab�n de la serpiente por pantalla.
 * @param screen Superficie sobre la que realizar el blitting.
 * @param zone Zona en la imagen donde se encuentra el tile.
 * @param tiles Imagen de tiles.
 */
void snakePiece_t::blit(SDL_Surface* screen, SDL_Rect* zone, image_t* tiles){
	tiles->blit(x, y, screen, zone);
}
