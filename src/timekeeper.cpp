#include "../include/SKATRAK_PLAYGROUND.hpp"

/**
 * @brief Actualiza el contador de tiempo.
 */
void timekeeper_t::refresh(){
	current = SDL_GetTicks();
}

/**
 * @return Tiempo transcurrido en milisegundos desde que se cre� el contador.
 */
int timekeeper_t::elapsed(){
	return SDL_GetTicks() - start;
}

/**
 * @brief Espera el tiempo necesario para controlar la velocidad a la que funciona el juego.
 * @note Esta funci�n aumenta el n�mero de frames que se supone que se han mostrado por pantalla.
 *
 * Es recomendable llamar a esta funci�n al final del game loop y llamar a timekeeper_t::refresh al principio del mismo.
 */
void timekeeper_t::waitFramerate(int fps){
	if(fps > 0){
		unsigned int tmp = (unsigned int)1000/fps;
		if(SDL_GetTicks() - current < tmp)
			SDL_Delay(tmp - (SDL_GetTicks() - current));
	}
	frames++;
}

/**
 * @return N�mero de fotogramas que se han imprimido por pantalla desde la �ltima vez que se llam�.
 * @note Esta funci�n resetea el n�mero de fotogramas que se han imprimido por pantalla.
 * @see timekeeper_t::waitFramerate
 */
int timekeeper_t::renderedFrames(){
	int tmp = frames;
	frames = 0;
	return tmp;
}
