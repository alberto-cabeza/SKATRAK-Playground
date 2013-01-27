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

/**
 * @brief Constructor por defecto. Inicializa los valores de la clase.
 */
menu_t::menu_t(): nOpt(0), selIndex(0), textPos(NULL), optName(NULL), selImage(NULL), backImage(NULL), clickSound(NULL), selectSound(NULL), callback(NULL)
{
}

/**
 * @brief Constructor. Inicializa todos los valores y reserva espacio para todos los botones de opciones.
 * @param optNumber N�mero de opciones que van a haber en el men�.
 */
menu_t::menu_t(int optNumber): nOpt(0), selIndex(0), textPos(NULL), optName(NULL), selImage(NULL), backImage(NULL), clickSound(NULL), selectSound(NULL), callback(NULL) {
	setOpts(optNumber);
}

/**
 * @brief Destructor. Libera la memoria ocupada por el men�.
 */
menu_t::~menu_t(){
	if(textPos != NULL)
		delete [] textPos;
	if(optName != NULL){
		for(int i = 0; i < nOpt; i++){
			if(optName[i] != NULL)
				delete optName[i];
		}
		delete [] optName;
	}
	if(callback != NULL)
		delete [] callback;
	if(selImage != NULL)
		delete selImage;
	if(backImage != NULL)
		delete backImage;
	if(clickSound != NULL)
		delete clickSound;
	if(selectSound != NULL)
		delete selectSound;
}

/**
 * @brief Establece el n�mero de opciones que hay en el men�.
 * @param optNumber Nuevo n�mero de opciones del men�.
 * @note Si llamas esta funci�n se van a liberar todos los callbacks y los textos.
 */
void menu_t::setOpts(int optNumber){
	// Eliminamos todo lo que dependa del n�mero de opciones
	if(textPos != NULL){
		delete [] textPos;
		textPos = NULL;
	}
	if(optName != NULL){
		for(int i = 0; i < nOpt; i++){
			if(optName[i] != NULL){
				delete optName[i];
				optName[i] = NULL;
			}
		}
		delete [] optName;
		optName = NULL;
	}
	if(callback != NULL){
		delete [] callback;
		callback = NULL;
	}

	nOpt = 0; // Acabamos de borrar todas las opciones
	selIndex = 0;

	if(optNumber > 0){
		textPos = new SDL_Rect[optNumber];
		if(textPos == NULL){
			fprintf(stderr, "menu_t::setOpts: No se ha podido reservar memoria para las posiciones de los textos.\n");
			return;
		}

		callback = new MenuCallbackFunc[optNumber];
		if(callback == NULL){
			fprintf(stderr, "menu_t::setOpts: No se ha podido reservar memoria para las funciones de callback.\n");
			delete [] textPos;
			textPos = NULL;
			return;
		}
		for(int i = 0; i < optNumber; i++)
			callback[i] = NULL;

		optName = new font_t*[optNumber];
		if(optName == NULL){
			fprintf(stderr, "menu_t::setOpts: No se ha podido reservar memoria para las fuentes de los textos de opciones.\n");
			delete [] textPos;
			delete [] callback;
			textPos = NULL;
			callback = NULL;
			return;
		}
		for(int i = 0; i < optNumber; i++)
			optName[i] = NULL;

		nOpt = optNumber;	// Todo se ha cargado correctamente, as� que actualizamos el n�mero de opciones que tenemos
	}
	else
		fprintf(stderr, "menu_t::setOpts: No se ha especificado un n�mero v�lido de opciones.\n");
}

/**
 * @brief Establece una funci�n de callback para un elemento del men�.
 * @param index �ndice del elemento.
 * @param func Funci�n de callback de la forma: 'returnVal func(void* datos);'.
 */
void menu_t::setOpt(int index, MenuCallbackFunc func){
	if(callback != NULL && index >= 0 && index < nOpt)
		callback[index] = func;
	else
		fprintf(stderr, "menu_t::setOpt: No se puede asignar un callback al elemento %d porque no se ha reservado memoria para �l.\n", index);
}

/**
 * @brief Establece las propiedades de los textos del men�.
 * @param fontName Nombre de la fuente TTF que se va a cargar.
 * @param fontSize Tama�o de la fuente.
 */
void menu_t::setTexts(string fontName, int fontSize){
	if(optName != NULL){
		for(int i = 0; i < nOpt; i++){
			optName[i] = new font_t(fontName);
			if(optName[i] != NULL){
				optName[i]->setSize(fontSize);
				optName[i]->setText("[UNDEFINED]");
			}
			else
				fprintf(stderr, "menu_t::setTexts: No se ha podido reservar memoria para la fuente n� %d.\n", i);
		}
	}
	else
		fprintf(stderr, "menu_t::setTexts: No se pueden especificar las caracter�sticas de los textos sin haber decidido cu�ntos van a haber.\n");
}

/**
 * @brief Establece las propiedades de los textos del men� a partir de una fuente ya configurada.
 * @param fontStyle Puntero a un objeto de la clase font_t.
 */
void menu_t::setTexts(font_t* fontStyle){
	if(optName != NULL){
		for(int i = 0; i < nOpt; i++){
			optName[i] = new font_t(*fontStyle);
			if(optName[i] == NULL)
				fprintf(stderr, "menu_t::setTexts: No se ha podido reservar memoria para la fuente n� %d.\n", i);
			else
				setText(i, "[UNDEFINED]");
		}
	}
	else
		fprintf(stderr, "menu_t::setTexts: No se pueden especificar las caracter�sticas de los textos sin haber decidido cu�ntos van a haber.\n");
}

/**
 * @brief Le da un nombre a la opci�n del men� indicada.
 * @param index �ndice de la opci�n.
 * @param text Nombre de la opci�n.
 */
void menu_t::setText(int index, string text){
	if(optName != NULL && textPos != NULL && index >= 0 && index < nOpt){
		if(optName[index] != NULL){
			optName[index]->setText(text);
			textPos[index].w = optName[index]->width();
			textPos[index].h = optName[index]->height();
		}
		else
			fprintf(stderr, "menu_t::setText: No se puede asignar un texto al elemento %d porque ha habido un error al asignarle propiedades.\n", index);
	}
	else
		fprintf(stderr, "menu_t::setText: No se puede asignar un texto al elemento %d porque no se ha reservado memoria para �l.\n", index);
}

/**
 * @brief Asigna una imagen de fondo del men�.
 * @param imageName Nombre del fichero de imagen.
 * @note �Cuidado! Es la imagen de fondo, as� que tiene que tener al menos el tama�o de la pantalla porque si no, se van a ver cosas feas
 * arrastr�ndose por ah�.
 */
void menu_t::setBackground(string imageName){
	if(backImage != NULL){
		delete backImage;
		backImage = NULL;
	}
	backImage = new image_t(imageName);
	if(backImage == NULL)
		fprintf(stderr, "menu_t::setBackground: No se ha podido cargar la imagen de fondo.\n");
}

/**
 * @brief Asigna una imagen para resaltar la opci�n activa del men�.
 * @param imageName Nombre del fichero de imagen.
 */
void menu_t::setImage(string imageName){
	if(selImage != NULL){
		delete selImage;
		selImage = NULL;
	}
	selImage = new image_t(imageName);
	if(selImage == NULL)
		fprintf(stderr, "menu_t::setImage: No se ha podido cargar la imagen para resaltar la opci�n activa.\n");
}

/**
 * @brief Asigna un sonido para reproducir cuando se pulsa un bot�n del men�.
 * @param soundName Nombre del fichero de sonido.
 */
void menu_t::setClickSFX(string soundName){
	if(clickSound != NULL){
		delete clickSound;
		clickSound = NULL;
	}
	clickSound = new sfx_t(soundName);
	if(clickSound == NULL)
		fprintf(stderr, "menu_t::setClickSFX: No se ha podido cargar el sonido de 'click'.\n");
}

/**
 * @brief Asigna un sonido para reproducir cuando se pasa el cursor por encima de un bot�n del men�.
 * @param soundName Nombre del fichero de sonido.
 */
void menu_t::setSelectSFX(string soundName){
	if(selectSound != NULL){
		delete selectSound;
		selectSound = NULL;
	}
	selectSound = new sfx_t(soundName);
	if(selectSound == NULL)
		fprintf(stderr, "menu_t::setSelectSFX: No se ha podido cargar el sonido de selecci�n.\n");
}

/**
 * @brief Distribuye los textos de opciones a lo largo de la pantalla seg�n las especificaciones del usuario.
 * @param flags Combinaci�n de los flags: MENU_RIGHT, MENU_LEFT, MENU_UP, MENU_DOWN o 0 para que est� en el centro de la pantalla.
 */
void menu_t::align(unsigned int flags){
	int posX = -1, posY = -1;
	int biggest = 0;

	if(textPos != NULL){
		// Establecemos la posici�n en X
		if(flags & MENU_ALIGN_LEFT){
			posX = MENU_MARGIN_H;
		}
		else {
			if(flags & MENU_ALIGN_RIGHT){
				for(int i = 0; i < nOpt; i++){
					if(textPos[i].w > biggest)
						biggest = textPos[i].w;
				}
				posX = sistema->width() - biggest - MENU_MARGIN_H;
				biggest = 0;	// Lo reseteamos para poder usarlo despu�s
			}
			else {
				for(int i = 0; i < nOpt; i++)
					textPos[i].x = (int)(sistema->width()/2 - textPos[i].w/2);
			}
		}
		if(posX != -1){
			for(int i = 0; i < nOpt; i++)
				textPos[i].x = posX;
		}
		// Establecemos la posici�n en Y
		for(int i = 0; i < nOpt; i++){
			if(textPos[i].h > biggest)
				biggest = textPos[i].h;
		}
		if(flags & MENU_ALIGN_UP)
			posY = MENU_MARGIN_V;
		else {
			int size = (biggest * nOpt) + (MENU_OPT_MARGIN * (nOpt - 1));
			if(flags & MENU_ALIGN_DOWN)
				posY = sistema->height() - size - MENU_MARGIN_V;
			else 
				posY = (int)((sistema->height() / 2) - (size / 2));
		}
		for(int i = 0; i < nOpt; i++)
			textPos[i].y = posY + (i * biggest) + ((i-1) * MENU_OPT_MARGIN);
	}
	else
		fprintf(stderr, "menu_t::align: No se han podido alinear las opciones porque los textos no se han cargado correctamente.\n");
}

/**
 * @brief Gestiona los eventos dentro del men�.
 *
 * Activa los sonidos, cambia el elemento activo del men�, detecta cuando el usuario intenta entrar en una de las opciones
 * y activa la funci�n de callback asociada.
 *
 * @param event Evento que hay que procesar.
 * @return Uno de los valores de returnVal. Devuelve por defecto MAIN si la funci�n de callback no est� definida para alguno de los submen�s.
 * @note Hay que llamar a esta funci�n cada vez que se llame a SDL_PollEvent con la misma variable para que se actualice el men� adecuadamente.
 */
returnVal menu_t::update(SDL_Event* event){
	static int pressed = -1;
	int prevIndex = selIndex;	// Para controlar el n�mero de veces que se reproduce 'selectSound'

	switch(event->type){
		case SDL_MOUSEMOTION:
			if(textPos != NULL){
				for(int i = 0; i < nOpt; i++){
					if(event->motion.x >= textPos[i].x && event->motion.x <= textPos[i].x + textPos[i].w && event->motion.y >= textPos[i].y && event->motion.y <= textPos[i].y + textPos[i].h){
						selIndex = i;
						break;
					}
				}
				if(prevIndex != selIndex && selectSound != NULL)
					selectSound->play();
			}
			break;
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym){
				case SDLK_UP:
				case SDLK_LEFT:
					selIndex--;
					if(selIndex < 0) selIndex = nOpt - 1;
					break;
				case SDLK_DOWN:
				case SDLK_RIGHT:
					selIndex++;
					if(selIndex >= nOpt) selIndex = 0;
					break;
				case SDLK_RETURN:
					if(callback != NULL && callback[selIndex] != NULL)
						return callback[selIndex](NULL);
					else
						return MAIN;
					break;
				case SDLK_ESCAPE:
					return PREV_MENU;
				case SDLK_F12:
					sistema->toggleFullscreen();
				default: return ACTUAL_MENU;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event->button.button == SDL_BUTTON_LEFT){
				if(textPos != NULL){
					for(int i = 0; i < nOpt; i++){
						if(event->button.x >= textPos[i].x && event->button.x <= textPos[i].x + textPos[i].w && event->button.y >= textPos[i].y && event->button.y <= textPos[i].y + textPos[i].h){
							pressed = i;
							if(clickSound != NULL)
								clickSound->play();
							return ACTUAL_MENU;
						}
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event->button.button == SDL_BUTTON_LEFT){
				if(textPos != NULL && pressed != -1){
					if(event->button.x >= textPos[pressed].x && event->button.x <= textPos[pressed].x + textPos[pressed].w && event->button.y >= textPos[pressed].y && event->button.y <= textPos[pressed].y + textPos[pressed].h){
						if(callback != NULL && callback[pressed] != NULL)
							return callback[pressed](NULL);
						else
							return MAIN;
					}
				}
				pressed = -1;
			}
			break;
		case SDL_QUIT:
			return EXIT;
		default: break;
	}
	return ACTUAL_MENU;
}

/**
 * @brief Imprime todos los elementos del men� por pantalla.
 * @param screen Superficie sobre la que hacer el blitting.
 */
void menu_t::blit(SDL_Surface* screen){
	if(backImage != NULL)
		backImage->blit(0, 0, screen);
	else
		fprintf(stderr, "menu_t::blit: No se puede mostrar el fondo de pantalla porque no se ha cargado.\n");
	if(textPos != NULL && optName != NULL){
		if(selImage != NULL){
			selImage->blit(textPos[selIndex].x, textPos[selIndex].y + (int)((textPos[selIndex].h - selImage->height()) / 2), screen);
		}
		else
			fprintf(stderr, "menu_t::blit: No se puede imprimir el resaltador de opciones porque no se ha cargado en memoria.\n");
		for(int i = 0; i < nOpt; i++){
			if(optName[i] != NULL)
				optName[i]->blit(textPos[i].x, textPos[i].y, screen);
			else
				fprintf(stderr, "menu_t::blit: No se puede imprimir por pantalla la opci�n %d porque la imagen no est� cargada en memoria.\n", i);
		}
	}
	else
		fprintf(stderr, "menu_t::blit: No se pueden imprimir las opciones porque no est�n cargadas en memoria.\n");
}
