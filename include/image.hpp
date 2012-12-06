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

#ifndef __IMAGE_T__
#define __IMAGE_T__

/**
 * @class image_t
 * @brief Clase encargada de representar las im�genes que se utilizan en el programa.
 */
class image_t {
	private:
		SDL_Surface* img;
	public:
		image_t();
		image_t(string path);
		~image_t();
		void open(string path);
		void setAlpha(int alpha);
		void blit(int x, int y, SDL_Surface* screen, SDL_Rect* imZone = NULL);
		void blit(int x, int y, SDL_Surface* screen, int imX, int imY, int imW, int imH);
		int width();
		int height();
};

#endif
