#ifndef __SHARED_ATTRIBUTES__
#define __SHARED_ATTRIBUTES__

/* Valores de retorno de los men�s */
enum returnVal	{	MAIN,			// Volver al men� principal
					ACTUAL_GAME,	// Volver al men� del juego actual
					EXIT,			// Salir del juego
					ERROR			// Salir con error
				};

extern system_t* sistema;
extern music_t* musica;

#endif
