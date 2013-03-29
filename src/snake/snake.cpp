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
// Cabecera de la clase
#include "snake/snake.hpp"
// Est�ndar
#include <string>
using std::string;
// SDL
#include "SDL/SDL.h"
// Propias
#include "snake/snakePiece.hpp"
#include "image.hpp"

/**
 * @brief Devuelve el �ndice en el eje X del tile que le corresponde a la cabeza de la serpiente.
 * @return �ndice en el eje X del tile o -1 en caso de error.
 *
 * Lo que hace realmente es calcular el estado de giro que tiene la cabeza de la serpiente en un
 * momento determinado y devolver el �ndice en el eje X donde se encuentra este giro.
 */
int snake_t::getHeadTilePos(){
	if(head == NULL) return -1;

	int aPosX = head->posX();
	int aPosY = head->posY();
	int relX, relY;

	// S�lo calculamos la posici�n si es la cabeza. Todos los dem�s deben obtener sus posiciones a partir del movimiento de la cabeza.
	if(head->prev == NULL){
		if(head->next == NULL)
			return 3*direction;
		else {
			relX = aPosX - head->next->posX();
			relY = aPosY - head->next->posY();
			switch(direction){
			case MOVE_UP:
				if(relX == 0) return 0;
				if(relX == 1) return 10;
				if(relX == -1) return 7;
				break;
			case MOVE_DOWN:
				if(relX == 0) return 3;
				if(relX == 1) return 11;
				if(relX == -1) return 8;
				break;
			case MOVE_LEFT:
				if(relY == 0) return 6;
				if(relY == 1) return 4;
				if(relY == -1) return 1;
				break;
			case MOVE_RIGHT:
				if(relY == 0) return 9;
				if(relY == 1) return 5;
				if(relY == -1) return 2;
				break;
			}
		}
	}

	// No se deber�a llegar hasta aqu�
	return -1;
}

/**
 * @brief Constructor. Crea la serpiente con el tama�o especificado.
 * @param pieces (OPCIONAL) N�mero de eslabones que se desea inicialmente.
 */
snake_t::snake_t(int pieces): pieceCount(0), head(NULL), tail(NULL), snake(NULL), tileSize(-1), turned(false) {
	addPiece(pieces);
	setPos(0, 0, MOVE_UP);
}

/**
 * @brief Destructor. Borra toda la lista enlazada de eslabones.
 */
snake_t::~snake_t(void){
	for(snakePiece_t* aux = head; aux != NULL; aux = head){
		head = aux->next;
		delete aux;
	}
	if(snake != NULL)
		delete snake;
}

/**
 * @brief Situa los eslabones alineados hacia una direcci�n con la cabeza en una posici�n.
 * @param posX Posici�n de la cabeza en el eje X.
 * @param posY Posici�n de la cabeza en el eje Y.
 * @param newDirection direcci�n hacia la que se va a mover la serpiente.
 */
void snake_t::setPos(int posX, int posY, Direction newDirection){
	// Si no hay serpiente no la podemos colocar
	if(head == NULL) return;

	direction = newDirection;
	head->setPos(posX, posY);

	int incX = 0, incY = 0;
	switch(direction){
	case MOVE_UP:
		incY = 1;
		break;
	case MOVE_DOWN:
		incY = -1;
		break;
	case MOVE_LEFT:
		incX = 1;
		break;
	case MOVE_RIGHT:
		incX = -1;
		break;
	}

	for(snakePiece_t* aux = head->next; aux != NULL; aux = aux->next){
		posX += incX;
		posY += incY;
		aux->setPos(posX, posY);
		aux->setTilePos(3 * direction);
	}
}

/**
 * @brief Abre una imagen que contenga los tiles de la serpiente.
 * @param path Nombre de la imagen.
 * @param newTileSize Tama�o de cada tile en la imagen.
 */
void snake_t::setImg(string path, int newTileSize){
	if(snake != NULL){
		delete snake;
		snake = NULL;
	}

	snake = new image_t(path);
	if(snake != NULL){
		tileSize = newTileSize;
		if((double)(snake->width() / (tileSize*12)) != 1.0)
			fprintf(stderr, "snake_t::setImg: [WARNING] La anchura de la imagen no concuerda con el tama�o de los tiles especificado (�Faltan tiles?).\n");
		if((double)(snake->height() / (tileSize*3)) != 1.0)
			fprintf(stderr, "snake_t::setImg: [WARNING] La altura de la imagen no concuerda con el tama�o de los tiles especificado (�Falta la Cabeza/Cuerpo/Cola?).\n");
	}
	else
		fprintf(stderr, "snake_t::setImg: Ha ocurrido un error al cargar la imagen.\n");
}

/**
 * @brief Devuelve la posici�n actual de la cabeza de la serpiente.
 * @param x Direcci�n de una posici�n de memoria ya reservada donde se escribir� la posici�n en el eje X de la serpiente.
 * @param y Direcci�n de una posici�n de memoria ya reservada donde se escribir� la posici�n en el eje Y de la serpiente.
 * @return -1 en caso de que no haya serpiente y 0 si s� la hay.
 */
int snake_t::headPos(int* x, int* y){
	if(head != NULL){
		if(x != NULL)
			*x = head->posX();
		if(y != NULL)
			*y = head->posY();
		return 0;
	}
	else
		return -1;
}

/**
 * @brief Cambia la posici�n de la cabeza de la serpiente.
 * @param x Posici�n en el eje X de la cabeza de la serpiente.
 * @param y Posici�n en el eje Y de la cabeza de la serpiente.
 *
 * Esta funci�n est� pensada para permitir a la serpiente aparecer por otro lado al salir de la zona de juego.
 * No se cambia la posici�n de ning�n otro elemento.
 */
void snake_t::setHeadPos(int x, int y){
	if(head == NULL || x < 0 || y < 0){
		fprintf(stderr, "snake_t::setHeadPos: No se ha podido recolocar la cabeza.\n");
		return;
	}
	head->setPos(x, y);
}

/**
 * @brief Gira la serpiente hacia una nueva direcci�n.
 * @param newDirection Direcci�n hacia la que se mover� la serpiente a partir de ahora.
 *
 * S�lo gira si no se ha girado desde la �ltima vez que se movi� y si el giro est� permitido.
 */
void snake_t::turn(Direction newDirection){
	if(!turned && newDirection != direction){
		switch(direction){
		case MOVE_UP:
			if(newDirection != MOVE_DOWN){
				direction = newDirection;
				turned = true;
			}
			break;
		case MOVE_DOWN:
			if(newDirection != MOVE_UP){
				direction = newDirection;
				turned = true;
			}
			break;
		case MOVE_LEFT:
			if(newDirection != MOVE_RIGHT){
				direction = newDirection;
				turned = true;
			}
			break;
		case MOVE_RIGHT:
			if(newDirection != MOVE_LEFT){
				direction = newDirection;
				turned = true;
			}
			break;
		}
	}
}

/**
 * @brief Mueve la serpiente una unidad de espacio.
 * @note No llamar a esta funci�n antes que a snake_t::addPiece.
 *
 * Lo que se hace en realidad es coger la cola y ponerla en la posici�n nueva a la que se mueve la serpiente,
 * cambiando los punteros de cola y cabeza y las posiciones.
 */
void snake_t::step(void){
	if(head == NULL) return;

	// Cuando se mueve, se resetea 'turned' para que pueda volver a girarse
	turned = false;
	snakePiece_t* aux = tail;

	// El tile del segundo segmento hay que calcularlo antes de mover la serpiente como si fuera la cabeza
	head->setTilePos(getHeadTilePos());

	// Cogemos el �ltimo segmento y lo ponemos en la posici�n de la cabeza
	tail = aux->prev;
	tail->next = NULL;
	head->prev = aux;
	aux->next = head;
	head = aux;
	head->prev = NULL;
	head->setPos(head->next->posX(), head->next->posY());

	// Ahora movemos la nueva cabeza y calculamos su estado de giro/posici�n
	head->move(direction);
	head->setTilePos(getHeadTilePos());
}

/**
 * @brief Le da un tama�o a la serpiente.
 * @param nPieces N�mero de piezas que va a tener la serpiente.
 *
 * Esta funci�n borra la serpiente actual, as� que luego hay que llamar a setPos para que se pongan los tiles correctamente.
 */
void snake_t::setPieces(int nPieces){
	for(snakePiece_t* aux = head; aux != NULL; aux = aux->next)
		delete aux;
	head = tail = NULL;
	pieceCount = 0;
	addPiece(nPieces);
}

/**
 * @brief A�ade m�s eslabones a la serpiente.
 * @param nPieces (OPCIONAL) N�mero de eslabones a a�adir.
 */
void snake_t::addPiece(int nPieces){
	pieceCount += nPieces;

	// Iniciamos la serpiente si no lo est� ya
	if(head == NULL){
		head = new snakePiece_t;
		tail = head;
		nPieces--;
	}

	for(int i = 0; i < nPieces; i++){
		tail->next = new snakePiece_t(tail, tail);
		tail = tail->next;
	}
}

/**
 * @brief Detecta si hay colisiones entre una posici�n y la serpiente. Si no se especifican argumentos, detecta si la cabeza est� chocando con otra parte del cuerpo.
 * @param posX Posici�n en el eje X del punto que se quiere comprobar.
 * @param posY Posici�n en el eje X del punto que se quiere comprobar.
 * @return true si hay colisi�n y false si no la hay.
 */
bool snake_t::checkCollision(int posX, int posY){
	if(head == NULL) return false;

	// Si no especificamos una posici�n, compara la posici�n de la cabeza
	if(posX < 0 || posY < 0){
		posX = head->posX();
		posY = head->posY();
	}
	else if(head->posX() == posX && head->posY() == posY)
		return true;

	// Empieza a comparar por el 2� eslab�n de la serpiente
	for(snakePiece_t* aux = head->next; aux != NULL; aux = aux->next)
		if(aux->posX() == posX && aux->posY() == posY)
			return true;

	return false;
}

/**
 * @brief Imprime toda la serpiente por pantalla.
 * @param screen Superficie sobre la que realizar el blitting.
 * @param x Posici�n en el eje X de comienzo de la zona de movimiento de la serpiente en la pantalla.
 * @param y Posici�n en el eje Y de comienzo de la zona de movimiento de la serpiente en la pantalla.
 */
void snake_t::blit(int x, int y, SDL_Surface* screen){
	if(head == NULL || snake == NULL || tileSize < 0) return;

	head->blit(screen, x, y, SNAKE_HEAD, snake, tileSize);
	for(snakePiece_t* aux = head->next; aux->next != NULL; aux = aux->next)
		aux->blit(screen, x, y, SNAKE_BODY, snake, tileSize);
	tail->blit(screen, x, y, SNAKE_TAIL, snake, tileSize);
}
