#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern Interfaz* interfaz;

/*
Metodo para cargar la imagen que se encuentra en ruta en la variable textura.
*/
void HongoVida::CargarTextura(const char* ruta)
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
Metodo para iniciar las propiedades fisicas del hongo de vida
*/
void HongoVida::IniciarCuerpoFisico(b2World* world)
{
	// Definicion del cuerpo fisico
	b2BodyDef groundBodyDef;
	
	// Colocamos el bloque en la posición indicada teniendo en cuenta que SDL_Rect tiene como origen la esquina
	// superior izquierda, y la posicion del cuerpo físico debe tener el origen en el centro del objeto. Ademas,
	// lo escalamos multiplicando por 0.01
	groundBodyDef.position.Set((posicion.x + posicion.w / 2.0f) * 0.01f, (posicion.y + posicion.h / 2.0f) * 0.01f);
	
	//  Definicion de un cuadrado
	b2PolygonShape groundBox;
	
	// La caja se crea a partir de la mitad del tamanyo del ancho y alto, y se multiplica por 0.01 para escalar
	// las fisicas a un tamanyo que funcione correctamente (si los objetos son muy grandes, se comporta de forma
	// extranya)
	groundBox.SetAsBox((posicion.w / 2.0f) * 0.01f, (posicion.h / 2.0f)  * 0.01f);
	
	// Creamos una definicion de colision
	b2FixtureDef fixtureDef;		
	fixtureDef.shape = &groundBox;		// Asignamos el cuadrado
	fixtureDef.isSensor = true;			// Se trata de un trigger
	
	// Creamos el bloque de informacion necesario
	FixtureData* data = new FixtureData;
	data->tipo = TIPO_HONGO_VIDA;
	data->hongoVida = this;
	
	// Asignamos la informacion
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(data);
	
	// Creamos el cuerpo cuerpo fisico en el mundo y agregamos la colision
	cuerpoFisico = world->CreateBody(&groundBodyDef);
	cuerpoFisico->CreateFixture(&fixtureDef);
}

/*
Constructor de un hongo de vida en una posicion dada. Necesita la camara y el mundo fisico actual
*/
HongoVida::HongoVida(float x, float y, Camara* camara, b2World* world)
{
	// Ajustamos la posicion del SDL_Rect
	posicion.x = x;
	posicion.y = y;
	
	// No se ha recogido
	recogido = false;
	
	// Guardamos la camara
	this->camara = camara;
	
	// Cargamos la texture
	CargarTextura(RUTA_HONGO_VIDA);
	
	// Iniciamos y creamos el cuerpo fisico
	IniciarCuerpoFisico(world);
}

/*
Metodo a ejecutar cuando se ha recogido el hongo de vida
*/
void HongoVida::Recoger()
{
	if (!recogido)
	{
		// Actualizamos la interfaz
		interfaz->CambiarNumeroVidas(1);
		recogido = true;
	}
}

/*
Metodo para dibujar en pantalla el hongo
*/
void HongoVida::Renderizar()
{
	// Si se ha recogido no lo renderizamos
	if (recogido) return;
	
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
void HongoVida::Destruir()
{
	// Destruimos la textura
	SDL_DestroyTexture(textura);
}
