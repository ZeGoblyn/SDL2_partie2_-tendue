#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define LARGEUR_TILE 24
#define HAUTEUR_TILE 16
#define NOMBRE_BLOCS_LARGEUR 15
#define NOMBRE_BLOCS_HAUTEUR 13

const int FPS = 60;

void Afficher(SDL_Renderer* pRenderer, SDL_Texture* textuTil, char** table, int nombre_blocs_largeur, int nombre_blocs_hauteur, int xBase, int yBase);

int xBase;
int yBase;


int main(int argc, char* argv[]) 
{
	SDL_Window* pWindow = NULL;           //Pointeur sur la fenêtre
	SDL_Renderer* pRenderer = NULL;       //Pointeur sur le Rendus


	xBase = 0;
	yBase = 0;

	//Requis pour utiliser la SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{   //INIT_EVERYTHING = initialise l'audio, la vidéo, les contrôles,... cf; SDL wiki/API by name/SDL_Init
		pWindow = SDL_CreateWindow("TILESET_TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_TILE * NOMBRE_BLOCS_LARGEUR, HAUTEUR_TILE * NOMBRE_BLOCS_HAUTEUR, SDL_WINDOW_SHOWN);

		//si succès, créer window et renderer
		if (pWindow != NULL)
		{
			pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_PRESENTVSYNC);
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

			//------------------------------------------------------------------------------------------------//
			//Tilemapping

			char* table[] = {						//Disposition des tiles
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000000000000000000000000",
				"000000000555666000000000000000",
				"000000000000000000000560000000",
				"000000000000000000000000000000",
				"001000000000000000000000002222",
				"222200000000000000000000002222",
				"222200000000770000077700002222",
				"222200000000770000070000002222",
				"222200000000770000070000002222",
				"777777777777777777777777777777"
			};

			unsigned long int nTempsActuel = 0;
			unsigned long int nTempsPrecedent = 0;
			int nDeltaTime = div(1000, FPS).quot;
			/*int pixX = 0;
			int pixY = 0;
			int sens = 1;*/

			SDL_Surface* tileset = NULL;
			SDL_Texture* textuTil = NULL;
			SDL_Event event;
			int continuer = 1, tempsPrecedent = 0, tempsActuel = 0;
			int statut = EXIT_FAILURE;

			tileset = IMG_Load("tileset1.bmp");				//Insertion du tileset
			if (tileset == NULL)
			{
				fprintf(stderr, "Erreur chargement image droite : %s", SDL_GetError());
			}

			textuTil = SDL_CreateTextureFromSurface(pRenderer, tileset);
			if (textuTil == NULL)
			{
				fprintf(stderr, "Erreur SDL_CreateTexturetil : %s", SDL_GetError());
			}
			SDL_FreeSurface(tileset);

			Afficher(pRenderer, textuTil, table, NOMBRE_BLOCS_LARGEUR, NOMBRE_BLOCS_HAUTEUR, 0, 0);

			while (continuer)
			{
				SDL_PollEvent(&event);
				switch (event.type)
				{
				case SDL_QUIT:
					continuer = 0;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						continuer = 0;
						break;

					case SDLK_d:
						if (xBase < NOMBRE_BLOCS_LARGEUR)
							xBase = xBase + 1;
						break;

					case SDLK_q:
						if (xBase > 0)
							xBase = xBase - 1;
						break;
					}
				}

					nTempsActuel = SDL_GetTicks();
					if (nTempsActuel > nTempsPrecedent + nDeltaTime)  //Affichage des frames
					{
						Afficher(pRenderer, textuTil, table, NOMBRE_BLOCS_LARGEUR, NOMBRE_BLOCS_HAUTEUR, xBase, yBase);
						nTempsPrecedent = nTempsActuel;
					}

					SDL_RenderPresent(pRenderer);

			}

				//------------------------------------------------------------------------------------------------//

				SDL_DestroyTexture(textuTil);
				SDL_DestroyRenderer(pRenderer); //On supprime tout ce qui à été créer
				SDL_DestroyWindow(pWindow);
				SDL_Quit();

		}

	}

	return 0;
}


void Afficher(SDL_Renderer* pRenderer, SDL_Texture* textuTil, char** table, int nombre_blocs_largeur, int nombre_blocs_hauteur, int xBase, int yBase)
{
	int i, j;
	SDL_Rect Rect_dest;
	SDL_Rect Rect_source;
	Rect_source.w = LARGEUR_TILE;
	Rect_dest.w = LARGEUR_TILE;
	Rect_source.h = HAUTEUR_TILE;
	Rect_dest.h = HAUTEUR_TILE;
	for (i = xBase; i < NOMBRE_BLOCS_LARGEUR + xBase; i++) //i = x pour arriver à NOMBRE_BLOCS_LARGEUR + x
	{
		for (j = yBase; j < NOMBRE_BLOCS_HAUTEUR + yBase; j++)
		{
			Rect_dest.x = (i - xBase) * LARGEUR_TILE;
			Rect_dest.y = (j - yBase) * HAUTEUR_TILE;
			Rect_source.x = (table[j][i] - '0') * LARGEUR_TILE;
			Rect_source.y = 0;
			SDL_RenderCopy(pRenderer, textuTil, &Rect_source, &Rect_dest);
		}
	}
}
