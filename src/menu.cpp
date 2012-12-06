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

#include "../include/menu.hpp"

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
menu_t::menu_t(int optNumber): nOpt(optNumber), selIndex(0), textPos(NULL), optName(NULL), selImage(NULL), backImage(NULL), clickSound(NULL), selectSound(NULL), callback(NULL) {
	setOpts(nOpt);
}

/**
 * @brief Destructor. Libera la memoria ocupada por el men�.
 */
menu_t::~menu_t(){

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
			fprintf(stderr, "No se ha podido reservar memoria para las posiciones de los textos del men�.\n");
			return;
		}

		callback = new MenuCallbackFunc[optNumber];
		if(callback == NULL){
			fprintf(stderr, "No se ha podido reservar memoria para las funciones de callback del men�.\n");
			delete [] textPos;
			textPos = NULL;
			return;
		}
		for(int i = 0; i < optNumber; i++)
			callback[i] = NULL;

		optName = new font_t*[optNumber];
		if(optName == NULL){
			fprintf(stderr, "No se ha podido reservar memoria para las fuentes de los textos de opciones del men�.\n");
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
	else {
		fprintf(stderr, "No se ha especificado un n�mero v�lido de opciones al men�.\n");
	}
}

/**
 * @brief Establece una funci�n de callback para un elemento del men�.
 * @param index �ndice del elemento.
 * @param func Funci�n de callback de la forma: 'returnVal func(void* datos);'.
 */
void menu_t::setOpt(int index, MenuCallbackFunc func){
	if(index >= 0 && index < nOpt){
		callback[index] = func;
	}
	else
		fprintf(stderr, "No se puede asignar un callback al elemento %d del men� porque no se ha reservado memoria para �l.\n", index);
}

/**
 * @brief Establece las propiedades de los textos del men�.
 * @param fontName Nombre de la fuente TTF que se va a cargar.
 * @param fontSize Tama�o de la fuente.
 */
void menu_t::setTexts(string fontName, int fontSize){
	for(int i = 0; i < nOpt; i++){
		optName[i] = new font_t(fontName);
		if(optName[i] != NULL)
			optName[i]->setSize(fontSize);
		else
			fprintf(stderr, "No se ha podido reservar memoria para la fuente n� %d.\n", i);
	}
}

/**
 * @brief Establece las propiedades de los textos del men� a partir de una fuente ya configurada.
 * @param fontStyle Puntero a un objeto de la clase font_t.
 */
void menu_t::setTexts(font_t* fontStyle){
	for(int i = 0; i < nOpt; i++){
		optName[i] = new font_t(*fontStyle);
		if(optName[i] == NULL)
			fprintf(stderr, "No se ha podido reservar memoria para la fuente n� %d.\n", i);
	}
}

/**
 * @brief Le da un nombre a la opci�n del men� indicada.
 * @param index �ndice de la opci�n.
 * @param text Nombre de la opci�n.
 */
void menu_t::setText(int index, string text){
	if(index >= 0 && index < nOpt){
		if(optName[index] != NULL)
			optName[index]->setText(text);
		else
			fprintf(stderr, "No se puede asignar un texto al elemento %d porque ha habido un error al asignarle propiedades.\n", index);
	}
	else
		fprintf(stderr, "No se puede asignar un texto al elemento %d del men� porque no se ha reservado memoria para �l.\n", index);
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
		fprintf(stderr, "No se ha podido cargar la imagen de fondo del men� desde %s.\n", imageName.c_str());
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
		fprintf(stderr, "No se ha podido cargar la imagen para resaltar la opci�n activa del men� desde %s.\n", imageName.c_str());
}

/**
 * @brief
 */
returnVal menu_t::update(SDL_Event* event){

}

/**
 * @brief Imprime todos los elementos del men� por pantalla.
 * @param screen Superficie sobre la que hacer el blitting.
 */
void menu_t::blit(SDL_Surface* screen){
	if(backImage != NULL)
		backImage->blit(0, 0, screen);
	else
		fprintf(stderr, "No se puede mostrar el fondo de pantalla porque no se ha cargado.\n");
	if(textPos != NULL && optName != NULL){
		if(selImage != NULL)
			selImage->blit(textPos[selIndex].x, textPos[selIndex].y - (int)(selImage->height()/2), screen);
		else
			fprintf(stderr, "No se puede imprimir el resaltador de opciones porque no se ha cargado en memoria.\n");
		for(int i = 0; i < nOpt; i++){
			if(optName[i] != NULL)
				optName[i]->blit(textPos[i].x, textPos[i].y, screen);
			else
				fprintf(stderr, "No se puede imprimir por pantalla la opci�n %d porque la imagen no est� cargada en memoria.\n", i);
		}
	}
	else
		fprintf(stderr, "No se pueden imprimir las opciones porque no est�n cargadas en memoria.\n");
}
