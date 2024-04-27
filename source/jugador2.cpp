#include "gameEngine.h"

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
int numGroundChecks2 = 0;
const int ticksJumpDelay = 10;
int actJumpTicks2 = 0;

/*
Metodo para cargar una textura y devolverla como resultado
*/
SDL_Texture* Jugador2::CargarTextura2(const char* ruta)
{
	// Cargamos la textura y declaramos la textura
	SDL_Surface* img = IMG_Load(ruta);
	SDL_Texture* tex = NULL;
	
	if (img)
	{
		// Creamos la textura y liberamos la imagen
		tex = SDL_CreateTextureFromSurface(renderer, img);
		SDL_FreeSurface(img);
	}
	
	// Devolvemos la textura
	return tex;
}

/*
Metodo para iniciar las propiedades fisicas del jugador
*/
void Jugador2::IniciarCuerpoFisico2(b2World* world)
{
	// Definicion del cuerpo fisico
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // Indicamos que es dinamico
	
	// Colocamos al jugador en la posici�n indicada teniendo en cuenta que SDL_Rect tiene como origen la esquina
	// superior izquierda, y la posicion del cuerpo f�sico debe tener el origen en el centro del objeto. Ademas,
	// lo escalamos multiplicando por 0.01
	bodyDef.position.Set((posicion.x + posicion.w / 2.0f) * 0.01f, (posicion.y + posicion.h / 2.0f) * 0.01f);
	
	// Creamos el cuerpo cuerpo fisico en el mundo
	cuerpoFisico = world->CreateBody(&bodyDef);
	
	// Definimos un circulo para que quede centrado en el jugador con el tamanyo adequado
	b2CircleShape circle;
	circle.m_radius = (posicion.w / 2.0f - 1.0f) * 0.01f;
	
	// Creamos la definicion de la colision
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;		// Asignamos el circulo
	fixtureDef.density = 1.0f;		// Indicamos su densidad
	fixtureDef.friction = 0.0f;		// Indicamos su friccion
	
	// Asginamos la colision al cuerpo
	cuerpoFisico->CreateFixture(&fixtureDef);
	
	// Fijamos su rotacion
	cuerpoFisico->SetFixedRotation(true);
	
	// Creamos una caja para la comprobacion de si toca suelo
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.05, 0.05, b2Vec2(0, 0.12), 0);
	
	// Creamos la informacion necesario
	FixtureData* data = new FixtureData;
	data->tipo = TIPO_GROUND_CHECK;
	
	// Asignamos todo lo necesario a la nueva colision
	fixtureDef.shape = &polygonShape;		// Asignamos la caja
	fixtureDef.isSensor = true;				// Es un trigger
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(data); 	// Asignamos la informacion
	
	// Asignamos la colision
	cuerpoFisico->CreateFixture(&fixtureDef);
	
	// Fijamos su rotacion (probablemente innecesario ya que esta hecho anteriormente)
	cuerpoFisico->SetFixedRotation(true);
}

/*
Constructor del jugador
*/
Jugador2::Jugador2(Camara2* camara2, b2World* world)
{
	// Variable para la velocidad de las animaciones
	int nTickPorFrame = 4;
	
	// Asignamos la camara
	this->camara2 = camara2;
	
	// El estado es el de quieto
	estado = Quieto;
	
	// Iniciamos el resto de variables
	tamanyo = 0;
	direccion = 0;
	velocidad = 0;
	float velocidad_movimiento = 10.0f;
	posicion.x = 20;
	posicion.y = 20;
	posicion.w = 16;
	posicion.h = 24;
	
	// Iniciamos el cuerpo fisico
	IniciarCuerpoFisico2(world);
	
	// Cargamos todas las texturas y animaciones
	agachado[0] = CargarTextura2(RUTA_RUN_0);
	agachado[1] = CargarTextura2(RUTA_RUN_0);
	agachado[2] = CargarTextura2(RUTA_RUN_0);
	agachado[3] = CargarTextura2(RUTA_RUN_0);
	cambioDir[0] = CargarTextura2(RUTA_RUN_0);
	cambioDir[1] = CargarTextura2(RUTA_RUN_0);
	cambioDir[2] = CargarTextura2(RUTA_RUN_0);
	cambioDir[3] = CargarTextura2(RUTA_RUN_0);
	cayendo[0] = CargarTextura2(RUTA_RUN_0);
	cayendo[1] = CargarTextura2(RUTA_RUN_0);
	cayendo[2] = CargarTextura2(RUTA_RUN_0);
	cayendo[3] = CargarTextura2(RUTA_RUN_0);
	const char* corriendo0[] = {
		RUTA_IDLE_0,
		RUTA_IDLE_1
	};
	corriendo[0].CrearAnimacion(2, nTickPorFrame, corriendo0);
	const char* corriendo1[] = {
		RUTA_IDLE_0,
		RUTA_IDLE_1
	};
	corriendo[1].CrearAnimacion(2, nTickPorFrame, corriendo1);
	const char* corriendo2[] {
		RUTA_IDLE_0,
		RUTA_IDLE_1,
		RUTA_IDLE_2,
		RUTA_IDLE_3
	};
	corriendo[2].CrearAnimacion(4, nTickPorFrame, corriendo2);
	const char* corriendo3[] {
		RUTA_IDLE_0,
		RUTA_IDLE_1,
		RUTA_IDLE_2,
		RUTA_IDLE_3
	};
	corriendo[3].CrearAnimacion(4, nTickPorFrame, corriendo3);
	quieto[0] = CargarTextura2(RUTA_IDLE_0);
	quieto[1] = CargarTextura2(RUTA_IDLE_1);
	quieto[2] = CargarTextura2(RUTA_IDLE_2);
	quieto[3] = CargarTextura2(RUTA_IDLE_3);
	quietoArriba[0] = CargarTextura2(RUTA_IDLE_0);
	quietoArriba[1] = CargarTextura2(RUTA_IDLE_1);
	quietoArriba[2] = CargarTextura2(RUTA_IDLE_2);
	quietoArriba[3] = CargarTextura2(RUTA_IDLE_3);
	saltando[0] = CargarTextura2(RUTA_RUN_0);
	saltando[1] = CargarTextura2(RUTA_RUN_1);
	saltando[2] = CargarTextura2(RUTA_RUN_2);
	saltando[3] = CargarTextura2(RUTA_RUN_3);
	saltoCorriendo[0] = CargarTextura2(RUTA_RUN_0);
	saltoCorriendo[1] = CargarTextura2(RUTA_RUN_1);
	saltoCorriendo[2] = CargarTextura2(RUTA_RUN_2);
	saltoCorriendo[3] = CargarTextura2(RUTA_RUN_3);
	const char* sprint0[] = {
		RUTA_RUN_0,
		RUTA_RUN_1
	};
	sprint[0].CrearAnimacion(2, nTickPorFrame, sprint0);
	const char* sprint1[] = {
		RUTA_RUN_0,
		RUTA_RUN_1
	};
	sprint[1].CrearAnimacion(2, nTickPorFrame, sprint1);
	const char* sprint2[] = {
		RUTA_RUN_0,
		RUTA_RUN_1,
		RUTA_RUN_2,
		RUTA_RUN_3
	};
	sprint[2].CrearAnimacion(4, nTickPorFrame, sprint2);
	const char* sprint3[] = {
		RUTA_RUN_0,
		RUTA_RUN_1,
		RUTA_RUN_2,
		RUTA_RUN_3
	};
	sprint[2].CrearAnimacion(4, nTickPorFrame, sprint3);
	const char* muerte0[] = {
		RUTA_MUERTE,
	};

	muerte.CrearAnimacion(1, nTickPorFrame, muerte0);
}

	// Método para mover el jugador hacia la izquierda
	void Jugador2::MoverIzquierda() {
		// Actualizamos la posición del jugador hacia la izquierda
		// Se puede ajustar la velocidad según sea necesario
		posicion.x -= velocidad_movimiento;
	}

	// Método para mover el jugador hacia la derecha
	void Jugador2::MoverDerecha() {
		// Actualizamos la posición del jugador hacia la derecha
		// Se puede ajustar la velocidad según sea necesario
		posicion.x += velocidad_movimiento;
	}

	// Método para mover el jugador hacia arriba
	void Jugador2::MoverArriba() {
		// Actualizamos la posición del jugador hacia arriba
		// Se puede ajustar la velocidad según sea necesario
		posicion.y += 0.2f;
	}

	// Método para mover el jugador hacia abajo
	void Jugador2::MoverAbajo() {
		// Actualizamos la posición del jugador hacia abajo
		// Se puede ajustar la velocidad según sea necesario
		posicion.y -= 0.2f;
	}


/*
Primera version para el movimiento del jugador, ya no se utiliza
*/
void Jugador2::Movimiento2(int x, int y)
{
	cuerpoFisico->SetLinearVelocity(b2Vec2(x * maxVelocidad, y * maxVelocidad));
}

/*
Metodo para dibujar al jugador y actualizar su maquina de estados
*/
void Jugador2::Renderizar2(int x, int y)
{
	// Calculamos la posicion donde esta cuerpo fisico (desaplicando la escala multiplicando por 100)
	posicion.x = floor(cuerpoFisico->GetPosition().x * 100.0f) - posicion.w / 2.0f;
	posicion.y = floor(cuerpoFisico->GetPosition().y * 100.0f) - posicion.h / 2.0f;
	
	// Actualizamos la camara
	camara2->Actualizar2();
	Movimiento2(x, y);
		
	// Calculamos la posicion donde se debe dibujar teniendo en cuenta la camara (funciona como un offset)	
	int posicionDibujoX = posicion.x - camara2->x;
	int posicionDibujoY = posicion.y - camara2->y;
	SDL_Rect posicionDibujo;
	posicionDibujo = {posicionDibujoX, posicionDibujoY, posicion.w, posicion.h};
	
	if (abs(cuerpoFisico->GetLinearVelocity().x) < 0.01)
		cuerpoFisico->SetLinearVelocity(b2Vec2(0.0f, cuerpoFisico->GetLinearVelocity().y));
	
	
	switch (estado)
	{
		case Quieto:
			SDL_RenderCopy(renderer, quieto[direccion + tamanyo], NULL, &posicionDibujo);
			
			if (cuerpoFisico->GetLinearVelocity().x > 0 && x > 0)
			{
				direccion = 0;
				estado = Corriendo;
			}
			else if (cuerpoFisico->GetLinearVelocity().x < 0 && x < 0)
			{
				direccion = 1;
				estado = Corriendo;
			}
			velocidad = x;
			cuerpoFisico->SetLinearVelocity(b2Vec2(velocidad, cuerpoFisico->GetLinearVelocity().y));
			break;

		case Corriendo:
			for (int i = 0; i < 4; i++)
				corriendo[i].Renderizar();
			SDL_RenderCopy(renderer, corriendo[direccion + tamanyo].textura, NULL, &posicionDibujo);
			
			if (cuerpoFisico->GetLinearVelocity().x == 0)
			{
				estado = Quieto;
				break;
			}
			velocidad += accel * x;
			if (x != 0 && signbit(x) != direccion)
			{
				estado = CambioDir;
				direccion = (direccion + 1) % 2;
			}
			if (x == 0)
			{
				if (velocidad > 0)
				{
					velocidad -= accel;
					if (velocidad <= 0) 
					{
						velocidad = 0;
						estado = Quieto;
					}
				}
				else if (velocidad < 0)
				{
					velocidad += accel;
					if (velocidad >= 0)
					{
						velocidad = 0;
						estado = Quieto;
					}
				}
			}
			if (abs(velocidad) >= maxVelocidad)
			{
				if (velocidad > 0) velocidad = maxVelocidad;
				if (velocidad < 0) velocidad = -maxVelocidad;
			}
			cuerpoFisico->SetLinearVelocity(b2Vec2(velocidad, cuerpoFisico->GetLinearVelocity().y));
			break;

		case Muerte:
			break;
	}

	

	
}

