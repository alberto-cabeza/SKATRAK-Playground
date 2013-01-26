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

/**
 * @brief Constructor. Inicializa la clase con sus valores por defecto.
 */
messagebox_t::messagebox_t(): nOpt(0), selIndex(1), text(NULL), box(NULL), msgPos(NULL)
{
}

/**
 * @brief Constructor. Inicializa la clase.
 * @param optNumber N�mero de posibles respuestas a la pregunta.
 */
messagebox_t::messagebox_t(int optNumber): nOpt(0), selIndex(1), text(NULL), box(NULL), msgPos(NULL) {
	setOpts(optNumber);
}

/**
 * @brief Destructor. Libera la memoria ocupada por el mensaje y todos sus elementos.
 */
messagebox_t::~messagebox_t(){
	if(text != NULL){
		for(int i = 0; i <= nOpt; i++){
			if(text[i] != NULL)
				delete text[i];
		}
		delete [] text;
	}
	if(box != NULL)
		delete box;
	if(msgPos != NULL)
		delete [] msgPos;
}

/**
 * @brief Asigna el n�mero de posibles respuestas y reserva espacio para estos elementos.
 * @param optNumber N�mero de posibles respuestas.
 */
void messagebox_t::setOpts(int optNumber){
	// Borramos todo antes de nada
	if(text != NULL){
		for(int i = 0; i <= nOpt; i++){
			if(text[i] != NULL)
				delete text[i];
		}
		delete [] text;
		text = NULL;
	}
	if(box != NULL){
		delete box;
		box = NULL;
	}
	if(msgPos != NULL){
		delete [] msgPos;
		msgPos = NULL;
	}

	// Esto viene siendo lo mismo de la clase menu_t
	nOpt = 0;
	selIndex = 1;	// Tener en cuenta que el �ndice 0 es el del enunciado

	if(optNumber > 0 && optNumber < 5){ // Como m�ximo 4 opciones para que quepa bien
		msgPos = new SDL_Rect[optNumber + 1]; // Reservamos uno m�s para el enunciado
		if(msgPos == NULL){
			fprintf(stderr, "No se ha podido reservar memoria para las posiciones de los textos del mensaje.\n");
			return;
		}

		text = new font_t*[optNumber + 1];
		if(text == NULL){
			fprintf(stderr, "No se ha podido reservar memoria para los textos del mensaje.\n");
			delete [] msgPos;
			msgPos = NULL;
			return;
		}
		for(int i = 0; i <= optNumber; i++)
			text[i] = NULL;

		nOpt = optNumber;
	}
	else
		fprintf(stderr, "No has especificado un n�mero v�lido de opciones para el mensaje.\n");
}

/**
 * @brief Le asigna una fuente al mensaje.
 * @param fontName Nombre de la fuente utilizada.
 * @param fontSize Tama�o de la fuente.
 */
void messagebox_t::setFont(string fontName, int fontSize){
	if(text != NULL){
		for(int i = 0; i <= nOpt; i++){
			text[i] = new font_t(fontName);
			if(text[i] != NULL){
				text[i]->setSize(fontSize);
				text[i]->setText("[UNDEFINED]");
			}
			else
				fprintf(stderr, "No se ha podido reservar memoria para la fuente n� %d.\n", i);
		}
	}
	else
		fprintf(stderr, "No se ha podido asignar una fuente al mensaje porque no se hab�a reservado memoria para ella.\n");
}

/**
 * @brief Le asigna una fuente al mensaje.
 * @param fontStyle Direcci�n a un objeto font_t con el estilo a utilizar.
 */
void messagebox_t::setFont(font_t* fontStyle){
	if(text != NULL){
		for(int i = 0; i <= nOpt; i++){
			text[i] = new font_t(*fontStyle);
			if(text[i] == NULL)
				fprintf(stderr, "No se ha podido reservar memoria para la fuente n� %d.\n", i);
			else
				setText(i, "[UNDEFINED]");
		}
	}
	else
		fprintf(stderr, "No se pueden especificar las caracter�sticas de los textos de los men�s sin haber decidido cu�ntos van a haber.\n");
}

/**
 * @brief Le asigna un texto a alguno de los campos.
 * @param index �ndice del texto. Desde 0 (Enunciado) hasta nOpt.
 * @param fontText Texto a asignar.
 */
void messagebox_t::setText(int index, string fontText){
	if(text != NULL && msgPos != NULL && index >= 0 && index <= nOpt){
		if(text[index] != NULL){
			text[index]->setText(fontText);
			msgPos[index].w = text[index]->width();
			msgPos[index].h = text[index]->height();
		}
		else
			fprintf(stderr, "No se puede asignar un texto al elemento %d del mensaje porque ha habido un error al asignarle propiedades.\n", index);
	}
	else
		fprintf(stderr, "No se puede asignar un texto al elemento %d del mensaje porque no se ha reservado memoria para �l.\n", index);
}

/**
 * @brief Le da una imagen a utilizar para el fondo del mensaje.
 * @param path Nombre de la imagen a utilizar.
 */
void messagebox_t::setBackground(string path){
	if(box != NULL){
		delete box;
		box = NULL;
	}
	box = new image_t(path);
	if(box == NULL)
		fprintf(stderr, "Ha habido un error al crear la imagen de fondo del mensaje.\n");
}

/**
 * @brief Distribuye los textos por el recuadro.
 * @note No llamar a esta funci�n hasta que se haya especificado la cantidad de campos y los textos.
 * @return -1 En caso de error y 0 si no hay errores.
 *
 * S�lo hay que llamar a esta funci�n una vez. Una vez se hayan distribuido los mensajes, s�lo hay que imprimirlos por pantalla.
 */
int messagebox_t::locateTexts(){
	if(box == NULL || text == NULL || msgPos == NULL){
		fprintf(stderr, "No se pueden situar los textos porque la memoria no est� reservada.\n");
		return -1;
	}

	// La posici�n de la ventana es en el centro
	wndPos.x = (int)((sistema->width() - box->width()) / 2);
	wndPos.y = (int)((sistema->height() - box->height()) / 2);

	// Situamos el enunciado en la mitad superior del cuadro
	msgPos[0].x = wndPos.x + (int)((box->width() / 2) - (text[0]->width() / 2));
	msgPos[0].y = wndPos.y + (int)((box->height() / 4) - (text[0]->height() / 2));

	// La distribuci�n de los textos depende de la cantidad de ellos
	// Esto deber�a poder hacerse mejor
	switch(nOpt){
	case 1:
		msgPos[1].x = wndPos.x + (int)((box->width() / 2) - (text[1]->width() / 2));
		msgPos[1].y = wndPos.y + (int)((3 * box->height() / 4) - (text[1]->height() / 2));
		break;
	case 2:
		msgPos[1].x = wndPos.x + (int)((box->width() / 4) - (text[1]->width() / 2));
		msgPos[1].y = wndPos.y + (int)((3 * box->height() / 4) - (text[1]->height() / 2));
		msgPos[2].x = wndPos.x + (int)((3 * box->width() / 4) - (text[2]->width() / 2));
		msgPos[2].y = wndPos.y + (int)((3 * box->height() / 4) - (text[2]->height() / 2));
		break;
	case 3:
		msgPos[1].x = wndPos.x + (int)((box->width() / 4) - (text[1]->width() / 2));
		msgPos[1].y = wndPos.y + (int)((3 * box->height() / 4) - (text[1]->height() / 2));
		msgPos[2].x = wndPos.x + (int)((box->width() / 2) - (text[2]->width() / 2));
		msgPos[2].y = wndPos.y + (int)((3 * box->height() / 4) - (text[2]->height() / 2));
		msgPos[3].x = wndPos.x + (int)((3 * box->width() / 4) - (text[3]->width() / 2));
		msgPos[3].y = wndPos.y + (int)((3 * box->height() / 4) - (text[3]->height() / 2));
		break;
	case 4:
		msgPos[1].x = wndPos.x + (int)((box->width() / 4) - (text[1]->width() / 2));
		msgPos[1].y = wndPos.y + (int)((box->height() / 2) - (text[1]->height() / 2));
		msgPos[2].x = wndPos.x + (int)((3 * box->width() / 4) - (text[2]->width() / 2));
		msgPos[2].y = wndPos.y + (int)((box->height() / 2) - (text[2]->height() / 2));
		msgPos[3].x = wndPos.x + (int)((box->width() / 4) - (text[3]->width() / 2));
		msgPos[3].y = wndPos.y + (int)((3 * box->height() / 4) - (text[3]->height() / 2));
		msgPos[4].x = wndPos.x + (int)((3 * box->width() / 4) - (text[4]->width() / 2));
		msgPos[4].y = wndPos.y + (int)((3 * box->height() / 4) - (text[4]->height() / 2));
		break;
	}
	return 0;
}

/**
 * @brief Entra en un bucle y le da el control de la entrada del usuario al mensaje.
 * @param screen Superficie donde se har� el blitting del cuadro de mensaje.
 * @return El n�mero de �ndice de la opci�n seleccionada o 0 en caso de que el usuario env�e un evento SDL_QUIT.
 */
int messagebox_t::loop(SDL_Surface* screen){
	SDL_Surface* background = NULL;
	background = SDL_ConvertSurface(screen, screen->format, screen->flags);

	timekeeper_t timer;
	SDL_Event event;
	while(true){
		timer.refresh();
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_MOUSEMOTION:
				for(int i = 1; i <= nOpt; i++){
					if(event.motion.x >= msgPos[i].x && event.motion.x <= msgPos[i].x + msgPos[i].w && event.motion.y >= msgPos[i].y && event.motion.y <= msgPos[i].y + msgPos[i].h){
						selIndex = i;
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT){
					for(int i = 1; i <= nOpt; i++){
						if(event.motion.x >= msgPos[i].x && event.motion.x <= msgPos[i].x + msgPos[i].w && event.motion.y >= msgPos[i].y && event.motion.y <= msgPos[i].y + msgPos[i].h)
							return i;
					}
				}
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_RIGHT:
					selIndex++;
					break;
				case SDLK_LEFT:
					selIndex--;
					break;
				case SDLK_UP:
				case SDLK_DOWN:
					if(nOpt == 4){
						switch(selIndex){
						case 1:
						case 2:
							if(event.key.keysym.sym == SDLK_UP)
								selIndex += 2;
							else
								selIndex -= 2;
							break;
						case 3:
						case 4:
							if(event.key.keysym.sym == SDLK_UP)
								selIndex -= 2;
							else
								selIndex += 2;
							break;
						}
					}
					else {
						if(event.key.keysym.sym == SDLK_UP)
							selIndex++;
						else
							selIndex--;
					}
					break;
				default: break;
				}
				if(event.key.keysym.sym == SDLK_RETURN)
					return selIndex;
				break;
			case SDL_QUIT:
				return 0;
				break;
			}
		}
		// Evitamos que el �ndice se salga del rango
		if(selIndex > nOpt) selIndex = 1;
		else if(selIndex < 1) selIndex = nOpt;
		// Colocamos el resaltador en el texto seleccionado
		marker.x = msgPos[selIndex].x - 5;
		marker.y = msgPos[selIndex].y - 5;
		marker.h = msgPos[selIndex].h + 10;
		marker.w = msgPos[selIndex].w + 10;
		// Imprimimos todo por pantalla
		SDL_BlitSurface(background, NULL, screen, NULL);
		box->blit(wndPos.x, wndPos.y, screen);
		SDL_FillRect(screen, &marker, SDL_MapRGB(screen->format, 0, 255, 128));
		for(int i = 0; i <= nOpt; i++)
			text[i]->blit(msgPos[i].x, msgPos[i].y, screen);
		sistema->update();
		timer.waitFramerate(30);
	}
}
