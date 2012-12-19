# SKATRAK-Playground #

Peque�o programa que consiste en varios juegos t�picos de mesa o cl�sicos del PC. Incluir� multijugador para algunos de ellos.

## Licencia ##
Este juego estar� bajo la licencia **LGPL** (GNU Lesser General Public License). Leer m�s [aqu�](http://www.gnu.org/licenses/lgpl.html).

## Posibles juegos ##
- 3 en raya
- Conecta 4
- Hundir la flota
- Pong
- Encontrar las parejas
- Snake
- Comecocos

## Autor/es ##
Hasta el momento todo el c�digo ha sido escrito por m�.
De todas formas quien quiera participar en el proyecto, ya sea aportando **c�digo**, haciendo de **beta-tester** o haciendo el **dise�o gr�fico** del programa y los minijuegos bienvenido sea. Toda ayuda ser� reflejada aqu�, por supuesto.

## Contacto ##
Para contactar conmigo, [este](mailto:theSkatrak@gmail.com) es mi correo electr�nico.

Para los que quieran estar al d�a, este es el blog del proyecto, donde intentar� ir actualizando cada semana los cambios importantes que haya realizado en el proyecto: <http://skatrakplayground.wordpress.com/>.

## Construcci�n del proyecto ##
El juego estar� siempre con la �ltima versi�n compilada la mayor parte del tiempo, pero puede ser que por falta de tiempo a veces el c�digo fuente est� m�s actualizado que el ejecutable. Tambi�n puede ser que hagas modificaciones en el c�digo y desees probar el resultado. Para eso pongo este apartado.

### Linux ###
Aclaro que los sistemas operativos que he utilizado y que he comprobado que funcionan son **Ubuntu 10.04** (x86) y **Ubuntu 12.04** (x64). Avisen si tienen alg�n problema en otros sistemas operativos para intentar solucionarlos.

Compilar en Linux es muy f�cil. S�lo situarse con la terminal en la carpeta *project* y hacer:

    make

Y ya est�. Si no hubieron errores el ejecutable estar� en *bin/linux*.

### Windows ###
He preparado varias maneras de compilar en **Windows**. Al igual que con **Linux** s�lo he podido probar estos m�todos con algunos sistemas operativos; en este caso **Windows 7** (x64). Igualmente, si encuentran alg�n **problema** al compilar en otros sistemas operativos Windows h�ganmelo saber.

#### CMD ###
Para esto debes **descargar** e **instalar** antes [MinGW](http://downloads.sourceforge.net/project/mingw/Installer/mingw-get-inst/mingw-get-inst-20120426/mingw-get-inst-20120426.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fmingw%2Ffiles%2FInstaller%2Fmingw-get-inst%2F&ts=1353678569&use_mirror=ignum). Luego vas a la carpeta *project* y abres el archivo *"script compilador windows.bat"*. Te preguntar� por el directorio *bin* de tu instalaci�n de **MinGW**. Lo introduces y pulsas "Enter". Por defecto, la ruta de esta carpeta es *"C:\MinGW\bin"*. El proyecto se compilar� y si no hay errores el ejecutable se encontrar� en *bin\windows*.

#### wx-DevC++ ####
Este m�todo requiere haber instalado previamente el programa (disponible [aqu�](http://downloads.sourceforge.net/project/wxdsgn/wxDev-C%2B%2B/Version%207/wxdevcpp_7.4.2_full_setup.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fwxdsgn%2Ffiles%2FwxDev-C%252B%252B%2FVersion%25207%2F&ts=1353681832&use_mirror=heanet)). Lo �nico que hay que hacer es **abrir** el archivo *"project\SKATRAK Playground.dev"* y utilizar la opci�n de **compilar** del programa. El ejecutable estar� situado por supuesto en *"bin\windows"*.

#### Visual Studio ####
Para los que dispongan de este programa, tambi�n est� el archivo de proyecto en *"project\SKATRAK Playground.vcxproj"* en el que tan s�lo hay que **compilar** y el ejecutable aparece en *"bin\windows"*.

La ventaja de los proyectos de **wx-DevC++** y **Visual Studio** es que estos programas incluyen una **IDE** y ya tienen todos los archivos del proyecto incluidos, as� que es m�s f�cil explorarlo desde ah�. Para Linux a�n no tengo ning�n proyecto de programa de este estilo, aunque tengo previsto agregar uno de **Code::Blocks** en breve.