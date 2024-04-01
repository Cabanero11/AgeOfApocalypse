#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

/*
Metodo para cargar la imagen que se encuentra en ruta en la variable textura.
*/
void Tuberia::CargarTextura(const char* ruta)
{
	// Cargamos la imagen
	SDL_Surface* img = IMG_Load(ruta);
	
	if (img)
	{
		// Ajustamos el tamanyo del SDL_Rect al de la imagen
		posicion.w = img->w;
		posicion.h = img->h;
		
		// Creamos la textura a partir de la imagen
		textura = SDL_CreateTextureFromSurface(renderer, img);
		
		// Liberamos la imagen
		SDL_FreeSurface(img);
	}
}

/*
Metodo para iniciar las propiedades fisicas de la plataforma
*/
void Tuberia::IniciarCuerpoFisico(b2World* world)
{
	// Definicion del cuerpo fisico
	b2BodyDef groundBodyDef;
	
	// Colocamos el bloque en la posición indicada teniendo en cuenta que SDL_Rect tiene como origen la esquina
	// superior izquierda, y la posicion del cuerpo físico debe tener el origen en el centro del objeto. Ademas,
	// lo escalamos multiplicando por 0.01	
	groundBodyDef.position.Set((posicion.x + posicion.w / 2.0f) * 0.01f, (posicion.y + posicion.h / 2.0f) * 0.01f);
	
	// Creamos el cuerpo cuerpo fisico en el mundo
	cuerpoFisico = world->CreateBody(&groundBodyDef);
	
	//  Definicion de una colision cuadrada
	b2PolygonShape groundBox;
	
	// La caja se crea a partir de la mitad del tamanyo del ancho y alto, y se multiplica por 0.01 para escalar
	// las fisicas a un tamanyo que funcione correctamente (si los objetos son muy grandes, se comporta de forma
	// extranya)
	groundBox.SetAsBox((posicion.w / 2.0f) * 0.01f, (posicion.h / 2.0f)  * 0.01f);
	cuerpoFisico->CreateFixture(&groundBox, 0.0f);
}

/*
Constructor de una tuberia en una posicion dada. Necesita la camara, el mundo fisico actual y el tipo para
indicar el tile a usar
*/
Tuberia::Tuberia(int tipo, float x, float y, Camara* camara, b2World* world)
{	
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// Guardamos la camara
	this->camara = camara;
	
	// Cargamos la textura indica
	switch (tipo)
	{
		case 1:
			CargarTextura(RUTA_TUBERIA_GRANDE_AZUL_CUERPO);
			break;
		case 2:
			CargarTextura(RUTA_TUBERIA_GRANDE_AZUL_CABEZA);
			break;
		case 3:
			CargarTextura(RUTA_TUBERIA_GRANDE_GRIS_CUERPO);
			break;
		case 4:
			CargarTextura(RUTA_TUBERIA_GRANDE_GRIS_CABEZA);
			break;
		case 5:
			CargarTextura(RUTA_TUBERIA_GRANDE_VERDE_CUERPO);
			break;
		case 6:
			CargarTextura(RUTA_TUBERIA_GRANDE_VERDE_CABEZA);
			break;
		case 7:
			CargarTextura(RUTA_TUBERIA_PEQUENYA_VERDE_SUELO);
			break;
		case 8:
			CargarTextura(RUTA_TUBERIA_PEQUENYA_VERDE_CUERPO);
			break;
		case 9:
			CargarTextura(RUTA_TUBERIA_PEQUENYA_VERDE_CABEZA);
			break;
	}
	
	// Iniciamos las fisicas
	IniciarCuerpoFisico(world);
}

/*
Metodo para dibujar en pantalla la tuberia
*/
void Tuberia::Renderizar()
{
	// Calculamos la posicion donde se debe dibujar teniendo en cuenta la camara (funciona como un offset)
	int posicionDibujoX = posicion.x - camara->x;
	int posicionDibujoY = posicion.y - camara->y;
	
	// Si tenemos la textura y se encuentra dentro de los limites de la pantalla, lo dibujamos
	if (textura && 
		posicionDibujoX + posicion.w > -50 && posicionDibujoX < SCREEN_W + 50 && 
		posicionDibujoY > -50 && posicionDibujoY + posicion.h < SCREEN_H + 50)
	{
		SDL_Rect posicionDibujo;
		posicionDibujo = {posicionDibujoX, posicionDibujoY, posicion.w, posicion.h};
		SDL_RenderCopy(renderer, textura, NULL, &posicionDibujo);
	}
}

/*
Metodo para liberar el bloque de la memoria
*/
void Tuberia::Destruir()
{
	// Destruimos la textura
	SDL_DestroyTexture(textura);
}
