
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "level.h"
#include "draw.h"
#include "state.h"

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;
    int contador=0; //Contador para el puntaje
    FILE *archivo;
    archivo = fopen("archivos/hiscore.txt","r"); //Abrimos el archivo hiscore.txt para ver la mayor puntuacion hasta el momento
    char linea[100];
    fgets(linea,100,archivo);
    int valor = atoi(linea);
    InitWindow(screen_width,screen_height,"Presente - the game");
    SetTargetFPS(60);
    InitAudioDevice();
    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);
    //insertar la música
    Music music = LoadMusicStream("archivos/Megalovania.mp3"); //Se cargan los archivos donde esta la musica
    Sound disparo = LoadSound("archivos/sable.mp3");
    PlayMusicStream(music);
    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);
    // Main loop
    while(!WindowShouldClose()){
        UpdateMusicStream(music);
        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) PlaySound(disparo);
        // Update the state
        state_update(lvl,sta,&contador);
        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);

            draw_state(lvl, sta);
            DrawText(FormatText("PUNTAJE: %i", contador), 10, 10, 20, MAROON);
            DrawText(FormatText("PUNTAJE MAXIMO: %i", valor), 10, 40, 20, BLUE);
            DrawText("Presente profe!",190,200,20,LIGHTGRAY);

        EndDrawing();

    }
    UnloadMusicStream(music);
    // Unload music stream buffers from RA
    //Se abre el nuevo archivo para sobreescribir el que tiene el puntaje maximo
    FILE *archivo2;
    char buffer[100];
    fclose(archivo);
    if (contador > valor){
      archivo2 = fopen("archivos/hiscore.txt","w");
      sprintf(buffer, "%d", contador);
      fputs(buffer,archivo2);
      fclose(archivo2);
    }
    // Closer window
    CloseWindow();
    CloseAudioDevice();
    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
