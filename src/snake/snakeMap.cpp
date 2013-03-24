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
#include <snake/snake.hpp>
#include <snake/snakeMap.hpp>

/**
 * @brief Coloca uno de los 3 tipos de comida en el mapa.
 * @param food Tipo de comida (FOOD_NORMAL, FOOD_BONUS, FOOD_WARP).
 *
 * Esta funci�n no hace comprobaci�n de falta de inicializaci�n de los atributos de la clase.
 */
void snakeMap_t::locateFood(SnakeFood food){
	int posX, posY;
	do {
		posX = rand() % (background->width() / tileSize);
		posY = rand() % (background->height() / tileSize);
	} while(checkCollision(posX, posY));

	switch(food){
	case FOOD_NORMAL:
		foodPos.x = posX;
		foodPos.y = posY;
		break;
	case FOOD_BONUS:
		bonusPos.x = posX;
		bonusPos.y = posY;
		break;
	case FOOD_WARP:
		warpPos.x = posX;
		warpPos.y = posY;
		break;
	}
}

/**
 * @brief Detecta si hay alguna colisi�n en la posici�n indicada o si no se indica ninguna posici�n, detecta si la cabeza de la serpiente ha chocado contra un muro del mapa.
 * @param posX Posici�n en el eje X del punto que se quiere comprobar.
 * @param posY Posici�n en el eje Y del punto que se quiere comprobar.
 * @return true si hay alguna colisi�n y false si no la hay.
 */
bool snakeMap_t::checkCollision(int posX, int posY){
	if(posX < 0 || posY < 0){
		if(snake != NULL && wallPos != NULL){
			int X, Y;
			snake->headPos(&X, &Y);
			for(int i = 0; i < nWalls; i++){
				if(wallPos[i].x == X && wallPos[i].y == Y)
					return true;
			}
		}
	}
	else if(snake->checkCollision(posX, posY))
		return true;
	else {
		for(int i = 0; i < nWalls; i++){
			if(wallPos[i].x == posX && wallPos[i].y == posY)
				return true;
		}
	}
	return false;
}

/**
 * @brief Constructor por defecto. Inicializa los valores de las variables de la clase.
 */
snakeMap_t::snakeMap_t(): wallPos(NULL), nWalls(0), background(NULL), special(NULL), tileSize(-1), foodLimit(-1), timeLimit(-1), snake(NULL) {
	foodPos.x = foodPos.y = bonusPos.x = bonusPos.y = warpPos.x = warpPos.y = -1;
	snake = new snake_t;
	setPos(0, 0);
}

/**
 * @brief Contructor. Inicializa la clase y coloca el mapa en la pantalla.
 * @param posX Posici�n en el eje X en la que se colocar� el comienzo del mapa en la pantalla.
 * @param posY Posici�n en el eje Y en la que se colocar� el comienzo del mapa en la pantalla.
 */
snakeMap_t::snakeMap_t(int posX, int posY): wallPos(NULL), nWalls(0), background(NULL), special(NULL), tileSize(-1), foodLimit(-1), timeLimit(-1), snake(NULL) {
	foodPos.x = foodPos.y = bonusPos.x = bonusPos.y = warpPos.x = warpPos.y = -1;
	snake = new snake_t;
	setPos(posX, posY);
}

/**
 * @brief Constructor. Inicializa la clase, coloca el mapa en la pantalla y le asigna una imagen de fondo.
 * @param posX Posici�n en el eje X en la que se colocar� el comienzo del mapa en la pantalla.
 * @param posY Posici�n en el eje Y en la que se colocar� el comienzo del mapa en la pantalla.
 * @param path Nombre de la imagen de fondo del mapa.
 */
snakeMap_t::snakeMap_t(int posX, int posY, string path): wallPos(NULL), nWalls(0), background(NULL), special(NULL), tileSize(-1), foodLimit(-1), timeLimit(-1), snake(NULL) {
	foodPos.x = foodPos.y = bonusPos.x = bonusPos.y = warpPos.x = warpPos.y = -1;
	snake = new snake_t;
	setPos(posX, posY);
	setBackground(path);
}

/**
 * @brief Destructor. Libera la memoria asignada a los atributos de la clase.
 */
snakeMap_t::~snakeMap_t(){
	if(wallPos != NULL)
		delete [] wallPos;
	if(background != NULL)
		delete background;
	if(special != NULL)
		delete special;
	if(snake != NULL)
		delete snake;
}

/**
 * @brief Coloca el mapa en una posici�n de la pantalla.
 * @param posX Posici�n en el eje X en la que se colocar� el comienzo del mapa en la pantalla.
 * @param posY Posici�n en el eje Y en la que se colocar� el comienzo del mapa en la pantalla.
 */
void snakeMap_t::setPos(int posX, int posY){
	if(posX < sistema->width() && posX > 0)
		mapPos.x = posX;
	if(posY < sistema->height() && posY > 0)
		mapPos.y = posY;
}

/**
 * @brief Le asigna un fondo al mapa.
 * @param path Nombre de la imagen de fondo.
 * @note Es NECESARIO que un mapa tenga imagen para que funcione. Esto es porque la anchura y la altura del mismo se extraen de la imagen.
 */
void snakeMap_t::setBackground(string path){
	if(background != NULL){
		delete background;
		background = NULL;
	}

	background = new image_t(path);
	if(background == NULL)
		fprintf(stderr, "snakeMap_t::setBackground: No se ha podido cargar la imagen.\n");
}

/**
 * @brief Carga un archivo de texto con un determinado formato para crear un mapa.
 * @param path Nombre del archivo.
 *
 * El formato del archivo ser� el siguiente: 
 * El s�mbolo 'X' representar� las paredes. 
 * El s�mbolo 'H' representar� la posici�n de comienzo de la cabeza de la serpiente. 
 * El s�mbolo 'D' representar� la direcci�n de movimiento inicial de la serpiente y debe estar pegado al s�mbolo 'H'. 
 * Cualquier otro s�mbolo distinto de '\n' se interpretar� como espacio vac�o. 
 * Los s�mbolos se dispondr�n en la forma del mapa y el conjunto debe tener forma rectangular.
 */
void snakeMap_t::loadMapScheme(string path){
	// Borramos el array actual si lo hay
	if(wallPos != NULL){
		delete [] wallPos;
		wallPos = NULL;
	}

	// Apertura del fichero
	string temp = MAP_PATH;
	temp += path;
	std::ifstream input(temp.c_str());
	if(!input.is_open()){
		fprintf(stderr, "snakeMap_t::loadMapScheme: No se ha podido abrir el archivo.\n");
		return;
	}

	string buffer;
	bool headFound = false;
	int lineCharCount = -1, wallCount = 0, dirCount = 0;
	int x, y = 0;

	// Lectura del fichero
	while(!input.eof()){
		getline(input, buffer);
		if(lineCharCount == -1)
			lineCharCount = buffer.length();
		else if(lineCharCount != (int)buffer.length()){
			fprintf(stderr, "snakeMap_t::loadMapScheme: El archivo tiene l�neas de distinto tama�o (%d != %d).\n", lineCharCount, (int)buffer.length());
			input.close();
			return;
		}
		for(x = 0; x < lineCharCount; x++){
			// Las 'X' representan muros
			if(buffer[x] == 'X') wallCount++;

			// La 'H' representa la cabeza de la serpiente
			else if(buffer[x] == 'H'){
				if(headFound){
					fprintf(stderr, "snakeMap_t::loadMapScheme: El archivo contiene m�s de una cabeza.\n");
					input.close();
					return;
				}
				headFound = true;

				// La 'D' representa la direcci�n de la serpiente y debe estar pegada a la 'H', pero no diagonalmente
				// Comprobamos si la 'D' est� en la misma l�nea
				if(dirCount == 1 && x > 0 && buffer[x-1] == 'D')
					snake->setPos(x, y, MOVE_LEFT);
				else if(dirCount == 0 && x < lineCharCount-1 && buffer[x+1] == 'D')
					snake->setPos(x, y, MOVE_RIGHT);

				// Comprobamos si la 'D' est� arriba o abajo
				else {
					bool dirFound = false;
					std::streamoff readPos = input.tellg();

					// Si acabamos de leer al menos la segunda l�nea, volvemos a leer la l�nea anterior y buscamos la 'D'
					if(dirCount == 1 && readPos > std::ios::beg + 2*(lineCharCount + 2)){
						input.seekg(readPos - 2*(lineCharCount + 2));
						getline(input, temp);
						if(temp[x] == 'D'){
							dirFound = true;
							snake->setPos(x, y, MOVE_UP);
						}
					}

					// Leemos la l�nea siguiente para ver si la 'D' est� debajo. Si no est�, la 'D' no se ha encontrado y el mapa est� mal dise�ado
					if(!dirFound){
						input.seekg(readPos);
						getline(input, temp);
						if((int)temp.length() == lineCharCount && temp[x] == 'D')
							snake->setPos(x, y, MOVE_DOWN);
						else {
							fprintf(stderr, "snakeMap_t::loadMapScheme: El archivo no especifica correctamente la direcci�n de la serpiente (Utilizar 'D').\n");
							input.close();
							return;
						}
					}
					input.seekg(readPos);
				}
			}

			// Contamos el n�mero de direcciones que hay para no permitir que haya m�s de una y para evitar buscar hacia atr�s si sabemos que no hay nada
			else if(buffer[x] == 'D' && ++dirCount > 1){
				fprintf(stderr, "snakeMap_t::loadMapScheme: El archivo contiene m�s de un indicador de direcci�n.\n");
				input.close();
				return;
			}
		}
		y++;
	}

	if(!headFound){
		fprintf(stderr, "snakeMap_t::loadMapScheme: El archivo no especificaba la posici�n inicial de la serpiente (Utilizar 'H').\n");
		input.close();
		return;
	}

	if(tileSize > 0 && background != NULL && (((y+1)*tileSize != background->height()) || (lineCharCount*tileSize != background->width())))
		fprintf(stderr, "snakeMap_t::loadMapScheme: [WARNING] Las dimensiones del mapa cargado no coinciden con las dimensiones del fondo.\n");

	// Creamos el nuevo array de posiciones de los muros
	nWalls = wallCount;
	wallPos = new coord[nWalls];

	// Damos las posiciones a los muros
	if(wallPos != NULL){
		int i;
		input.seekg(std::ios::beg);
		for(i = 0, wallCount = 0, y = 0; wallCount < nWalls; y++){
			getline(input, buffer);
			for(x = 0; x < lineCharCount; x++){
				if(buffer[x] == 'X'){
					wallPos[i].x = x;
					wallPos[i].y = y;
					wallCount++; i++;
				}
			}
		}
	}
	else {
		fprintf(stderr, "snakeMap_t::loadMapScheme: No se ha podido reservar memoria para el array de muros.\n");
		snake->setPos(0, 0, MOVE_UP);
	}

	input.close();
}

/**
 * @brief Le asigna una imagen (tileset) a la serpiente.
 * @param path Nombre de la imagen.
 * @param size Tama�o de los tiles.
 */
void snakeMap_t::setSnakeImg(string path, int size){
	tileSize = size;
	if(snake != NULL)
		snake->setImg(path, size);
}

/**
 * @brief Le asigna una imagen (tileset) a las comidas.
 * @param path Nombre de la imagen.
 * @note El tama�o de las comidas debe ser igual al de los eslabones de la serpiente.
 */
void snakeMap_t::setFoodImg(string path){
	if(special != NULL){
		delete special;
		special = NULL;
	}

	special = new image_t(path);
	if(special == NULL)
		fprintf(stderr, "snakeMap_t::setFoodImg: No se ha podido cargar la imagen.\n");
}

/**
 * @brief Devuelve la anchura del mapa en la pantalla.
 * @return Anchura del mapa � 0 si no se ha cargado un fondo.
 */
int snakeMap_t::width(void) const {
	return (background != NULL)? background->width() : 0;
}

/**
 * @brief Devuelve la altura del mapa en la pantalla.
 * @return Altura del mapa � 0 si no se ha cargado un fondo.
 */
int snakeMap_t::height(void) const {
	return (background != NULL)? background->height() : 0;
}

/**
 * @brief Gira la serpiente a una direcci�n.
 * @param direction Nueva direcci�n.
 */
void snakeMap_t::turnSnake(Direction direction){
	if(snake != NULL)
		snake->turn(direction);
}

/**
 * @brief Actualiza la posici�n de la serpiente.
 * @return Un valor de los posibles objetos con los que puede chocar la serpiente al moverse.
 * @note Esta funci�n debe llamarse en cada fotograma.
 */
SnakeHit snakeMap_t::update(){
	// Comprobaci�n de que se ha cargado correctamente el mapa
	if(background == NULL || snake == NULL || tileSize < 0){
		fprintf(stderr, "snakeMap_t::update: El mapa no est� cargado correctamente.\n");
		return HIT_NONE;
	}

	// La cuenta atr�s para que aparezca un bonus o desaparezca si ya hay uno
	static int countdown = -1;

	if(countdown < 0)
		countdown = BONUS_RESPAWN_TIME + (rand() % BONUS_RESPAWN_VARIATION);
	else if(countdown == 0){
		if((bonusPos.x < 0 || bonusPos.y < 0) && (warpPos.x < 0 || warpPos.y < 0)){
			locateFood(FOOD_BONUS);
			countdown = BONUS_RESPAWN_TIME + (rand() % BONUS_RESPAWN_VARIATION);	// Va a haber entre 10 y 15 movimientos de la serpiente de tiempo para coger el bonus
		}
		else
			bonusPos.x = bonusPos.y = -1;	// Hacemos desaparecer el bonus si no se recoge a tiempo
	}

	// Colocaci�n de la primera comida si no est�
	if(foodPos.x < 0 || foodPos.y < 0)
		locateFood(FOOD_NORMAL);

	// Variables de estado
	static int frames = 0, eaten = 0;
	frames++; countdown--;

	// Comprobaciones de si hay que colocar el warp
	if(frames == timeLimit || eaten == foodLimit)
		locateFood(FOOD_WARP);

	// Movemos la serpiente
	snake->step();

	// Comprobaciones de serpiente fuera del espacio de juego (se teletransporta)
	int headX, headY;
	if(snake->headPos(&headX, &headY) >= 0){
		if(headX >= (int)(background->width()/tileSize))
			snake->setHeadPos(0, headY);
		else if(headX < 0)
			snake->setHeadPos((int)(background->width()/tileSize)-1, headY);
		if(headY >= (background->height()/tileSize))
			snake->setHeadPos(headX, 0);
		else if(headY < 0)
			snake->setHeadPos(headX, (background->height()/tileSize)-1);
	}
	else {
		fprintf(stderr, "snakeMap_t::update: La serpiente no est� cargada.\n");
		countdown = -1;
		frames = eaten = 0;
		return HIT_DEATH;
	}

	// Comprobaci�n de choques
	if(snake->checkCollision() || checkCollision()){
		countdown = -1;
		frames = eaten = 0;
		return HIT_DEATH;
	}

	// Comprobaciones sobre si has comido algo
	if(headX == foodPos.x && headY == foodPos.y){
		eaten++;
		snake->addPiece(2);
		if(warpPos.x < 0 || warpPos.y < 0)	// S�lo se colocan nuevas comidas si no hay ningun warp activo
			locateFood(FOOD_NORMAL);
		else
			foodPos.x = foodPos.y = 1000000000;	// No uso '-1' para que no se detecte como que acaba de empezar la partida y se vuelva a colocar una comida
		return HIT_NORMAL;
	}
	else if(headX == bonusPos.x && headY == bonusPos.y){
		snake->addPiece();
		countdown += BONUS_RESPAWN_TIME + (rand() % BONUS_RESPAWN_VARIATION);	// Se le suma al tiempo que faltaba para que se quitara el bonus el tiempo para el siguiente
		bonusPos.x = bonusPos.y = -1;
		return HIT_BONUS;
	}
	else if(headX == warpPos.x && headY == warpPos.y){
		countdown = -1;
		frames = eaten = 0;
		return HIT_WARP;
	}
	else
		return HIT_NONE;
}

/**
 * @brief Imprime el mapa y la serpiente por pantalla.
 * @param screen Superficie sobre la que realizar el blitting.
 */
void snakeMap_t::blit(SDL_Surface* screen){
	if(background == NULL || snake == NULL || special == NULL)
		return;

	// Imprimimos el fondo y la serpiente
	background->blit(mapPos.x, mapPos.y, screen);
	snake->blit(mapPos.x, mapPos.y, screen);

	// Imprimimos las comidas que est�n presentes
	SDL_Rect zone = {0, 0, (Uint16)tileSize, (Uint16)tileSize};
	if(foodPos.x >= 0 && foodPos.y >= 0)
		special->blit(mapPos.x + (foodPos.x * tileSize), mapPos.y + (foodPos.y * tileSize), screen, &zone);
	if(bonusPos.x >= 0 && bonusPos.y >= 0){
		zone.x = FOOD_BONUS * tileSize;
		special->blit(mapPos.x + (bonusPos.x * tileSize), mapPos.y + (bonusPos.y * tileSize), screen, &zone);
	}
	if(warpPos.x >= 0 && warpPos.y >= 0){
		zone.x = FOOD_WARP * tileSize;
		special->blit(mapPos.x + (warpPos.x * tileSize), mapPos.y + (warpPos.y * tileSize), screen, &zone);
	}
	if(wallPos != NULL){
		zone.x = (FOOD_WARP + 1) * tileSize;
		for(int i = 0; i < nWalls; i++)
			special->blit(mapPos.x + (wallPos[i].x * tileSize), mapPos.y + (wallPos[i].y * tileSize), screen, &zone);
	}
}
