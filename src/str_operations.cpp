#include "../include/str_operations.hpp"
#include <cstring>

/**
 * @brief Calcula el resultado de una potenciaci�n.
 * @param base Base
 * @param exponent Exponente
 * @return Base^Exponente
 */
double exp(int base, int exponent){
	double temp = 1;
	if(exponent > 0){
		for(int i = 0; i < exponent; i++)
			temp *= base;
	}
	else {
		for(int i = exponent; i < 0; i++)
			temp /= base;
	}
	return temp;
}

/**
 * @brief Transforma un car�cter ASCII del '0' al '9' a un entero con ese valor.
 * @param character Car�cter ASCII que se va a transformar
 * @return -1 si no es un n�mero o el n�mero.
 */
int str_op::chartoint(char character){
	if(character <= '9' && character >= '0')
		return character - '0';
	return -1;
}

/**
 * @brief Convierte una cadena de caracteres de n�meros a un n�mero.
 * @param str Puntero a una cadena de caracteres acabada en un car�cter nulo
 * @return ERROR_INT_VAL si la cadena no es de n�meros y el n�mero que representa en otro caso
 */
int str_op::strtoint(const char* str){
	int temp = 0, i = 0, length = strlen(str);
	bool negative = false;
	// Miramos si es un n�mero negativo
	if(str[i] == '-'){
		negative = true;
		i++;
	}
	// Recorremos la cadena de caracteres
	while(str[i] != '\0'){
		// Si es un n�mero se calcula su valor seg�n su posici�n y si no lo es devolvemos un valor de error
		if(chartoint(str[i]) != -1){
			temp += (chartoint(str[i])*(int)exp(10, length-i-1));
			i++;
		}
		else return ERROR_INT_VAL;
	}
	// Si el n�mero era negativo, hacemos un cambio de signo
	if(negative)
		return -temp;
	return temp;
}

/**
 * @brief Convierte una cadena de caracteres a un n�mero flotante con precisi�n doble
 * @param str Puntero a una cadena de caracteres acabada en un car�cter nulo
 * @return ERROR_INT_VAL si la cadena no representa a un n�mero decimal o el n�mero en otro caso
 */
double str_op::strtodouble(const char* str){
	double temp = 0.0;
	int i = 0, pointPos;
	bool negative = false;
	// Miramos si es un n�mero negativo
	if(str[i] == '-'){
		negative = true;
		i++;
	}
	// Localizamos el punto decimal si lo hay. Si no lo hay se sit�a al final de la cadena
	while(str[i] != '.' && str[i] != '\0') i++;
	pointPos = i;
	// Iniciamos un bucle para calcular la parte entera del n�mero decimal
	if(negative) i = 1;
	else i = 0;
	while(i < pointPos){
		if(chartoint(str[i]) != -1){
			temp += (chartoint(str[i])*exp(10, pointPos-i-1));
			i++;
		}
		else return (double)ERROR_INT_VAL;
	}
	// Calculamos la parte decimal
	if(str[pointPos] != '\0'){
		i = pointPos + 1;
		while(str[i] != '\0'){
			if(chartoint(str[i]) != -1){
				temp += (chartoint(str[i])*exp(10, -(i-pointPos)));
				i++;
			}
			else return (double)ERROR_INT_VAL;
		}
	}
	if(negative)
		return -temp;
	return temp;
}
