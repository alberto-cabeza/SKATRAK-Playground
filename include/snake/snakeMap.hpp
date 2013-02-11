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

#ifndef __SNAKEMAP_T__
#define __SNAKEMAP_T__

/* Diferentes cosas con las que puede impactar la serpiente en cada movimiento */
enum SnakeHit {
	HIT_NORMAL,	// Se comi� una comida
	HIT_BONUS,	// Se comi� un bonus
	HIT_WARP,	// Salto de nivel
	HIT_NONE,	// No choc� con nada
	HIT_DEATH	// Se mordi� la cola o choc� contra una pared
};

/**
 * @class snakeMap_t
 * @brief Representa un rect�ngulo de juego compuesto por la serpiente, bloques opcionales y las comidas y bonus que se puede comer la serpiente.
 *
 * Permite configurar bonus warp para pasar a otro nivel.
 */
class snakeMap_t {
	private:
		struct coord {
			int x, y;
		} mapPos, foodPos, bonusPos, warpPos, *wallPos;
		int nWalls;				// N�mero de bloques de pared que hay
		image_t* background;	// Define el espacio de juego y le da un aspecto al fondo
		image_t* special;		// Imagen que contiene los tiles de la comida, los bonus, los warps y los muros
		int tileSize;			// Es el mismo para las comidas y los eslabones de la serpiente
		int moveFreq;			// N�mero de fotogramas que tienen que pasar para que se mueva la serpiente
		int foodLimit;			// N�mero de comidas necesarias para que aparezca un warp
		int timeLimit;			// N�mero de fotogramas necesarios para que aparezca un warp
		snake_t* snake;			// La serpiente
	protected:
		void init(void);
	public:
		// Constructores y destructor
		snakeMap_t(void);
		snakeMap_t(int posX, int posY);
		snakeMap_t(int posX, int posY, string path);
		~snakeMap_t(void);
		// Opciones del mapa
		void setPos(int posX, int posY);
		void setBackground(string path);
		void loadMapScheme(string path);
		// Im�genes
		void setSnakeImg(string path, int size = -1);
		void setFoodImg(string path, int size = -1);
		void setDelay(int delay);
		void setFoodLimit(int nFood);
		void setTimeLimit(int nFrames);
		SnakeHit update(SDL_Event* event);
};

#endif