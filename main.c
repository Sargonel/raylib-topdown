#include "raylib.h"
#define WIDTH 2000
#define HEIGHT 1500
#define PLAYERSIZE 100
#define MAX_BULLET 100
#define dt GetFrameTime()

typedef struct player_st{
    float x,y;
    float w,h;
    float speed;
    float health;
    int count;
    bool kill;
    bool active;
}player_st;
typedef struct bullet_st{
    float x,y;
    float dx,dy;
    float r;
    float speed;
    bool active;
}bullet_st;
typedef struct enemy_st{
    float x,y;
    float w,h;
    float speed;
    bool active;
}enemy_st;

void draw_player(player_st p){
    if(p.active){DrawRectangle(p.x,p.y,p.w,p.h,BLUE);}
}
void update_player(player_st *p){
    if(IsKeyDown(KEY_W)){p->y -= p->speed *dt;}
    if(IsKeyDown(KEY_S)){p->y += p->speed *dt;}
    if(IsKeyDown(KEY_A)){p->x -= p->speed *dt;}
    if(IsKeyDown(KEY_D)){p->x += p->speed *dt;}
}
void draw_bullet(bullet_st bullet[MAX_BULLET]){
    for(int i=0;i < MAX_BULLET;i++){
        if(bullet[i].active){
            DrawCircle(bullet[i].x,bullet[i].y,bullet[i].r,WHITE);
        }
    }
}
void update_bullet(bullet_st bullet[MAX_BULLET]){
    for(int i=0;i < MAX_BULLET;i++){
        if(bullet[i].active){
            bullet[i].x += bullet[i].dx * bullet[i].speed * dt;
            bullet[i].y += bullet[i].dy * bullet[i].speed * dt;

            if (bullet[i].x < 0 || bullet[i].x > WIDTH || bullet[i].y < 0 || bullet[i].y > HEIGHT) {
                bullet[i].active = false;
            }
        }
    }
}
void shoot_bullet(bullet_st bullet[MAX_BULLET],player_st p){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        for(int i=0;i < MAX_BULLET;i++){
            if(!bullet[i].active){
                bullet[i].active = true;
                bullet[i].x = p.x + p.w/2;
                bullet[i].y = p.y + p.h/2;

                bullet[i].dx = 0;
                bullet[i].dy = 0;

                if (IsKeyDown(KEY_W)) bullet[i].dy = -1;
                if (IsKeyDown(KEY_S)) bullet[i].dy = 1;
                if (IsKeyDown(KEY_A)) bullet[i].dx = -1;
                if (IsKeyDown(KEY_D)) bullet[i].dx = 1;

                if (bullet[i].dx == 0 && bullet[i].dy == 0) bullet[i].dy = -1;

                break;

            }
        }
    }
}
void draw_enemy(enemy_st *e){
    if(e->active){DrawRectangle(e->x,e->y,e->w,e->h,RED);}
}
void update_enemy(enemy_st *e,player_st p){
    if(e->x < p.x){e->x += e->speed *dt;}
    if(e->x > p.x){e->x -= e->speed *dt;}
    if(e->y < p.y){e->y += e->speed *dt;}
    if(e->y > p.y){e->y -= e->speed *dt;}
}
void check_damage(enemy_st *e,player_st *p,bullet_st bullet[MAX_BULLET]){
    if(!e->active){return;}

    Rectangle e1 = {e->x,e->y,e->w,e->h};
    Rectangle p1 = {p->x,p->y,p->w,p->h};

    for(int i=0; i < MAX_BULLET; i++) {
    
    if (bullet[i].active) { 
        Vector2 center = {bullet[i].x, bullet[i].y};
        if(CheckCollisionCircleRec(center, bullet[i].r, e1)) {
            e->active = false;      
            bullet[i].active = false; 
            p->count++;             
            break;               
        }
    }
}

    if(CheckCollisionRecs(e1,p1)){p->health -= 10 * dt;}
    if(p->health <= 0){p->active = false;}
    
}
void draw_health(player_st p){
    DrawText(TextFormat("health: %.0f",p.health),0,0,100,WHITE);
}
void draw_kill(player_st p){
    int mes = MeasureText("kill:0000",100);
    DrawText(TextFormat("kill: %i",p.count),WIDTH-mes,0,100,WHITE);
}
void reset_game(player_st *p, enemy_st *e, bullet_st bullets[]) {
   
    p->x = WIDTH/2 - PLAYERSIZE/2;
    p->y = HEIGHT/2 - PLAYERSIZE/2;
    p->health = 100;
    p->count = 0;
    p->active = true;

   
    e->active = false;

   
    for(int i = 0; i < MAX_BULLET; i++) {
        bullets[i].active = false;
    }
}
int main(void)
{
    InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");

    float random_x = 0;
    float random_y = 0;

    int side = GetRandomValue(0, 3);
    if (side == 0) { random_x = -100; random_y = GetRandomValue(0, HEIGHT); } 
    if (side == 1) { random_x = WIDTH + 100; random_y = GetRandomValue(0, HEIGHT); } 
    if (side == 2) { random_x = GetRandomValue(0, WIDTH); random_y = -100; } 
    if (side == 3) { random_x = GetRandomValue(0, WIDTH); random_y = HEIGHT + 100; } 

    player_st player = {
        .x = WIDTH/2 - PLAYERSIZE/2,
        .y = HEIGHT/2 - PLAYERSIZE/2,
        .w = PLAYERSIZE,
        .h = PLAYERSIZE,
        .health = 100.0f,
        .kill = false,
        .active = true,
        .count = 0,
        .speed = 400.0f
    };
    enemy_st enemy = {
        .x = random_x,
        .y = random_y,
        .w = PLAYERSIZE,
        .h = PLAYERSIZE,
        .active = true,
        .speed = 200.0f
    };
    bullet_st bullet[MAX_BULLET];
    for(int i=0;i < MAX_BULLET;i++){
        bullet[i].speed = 800.0f;
        bullet[i].r = 15.0f;
        bullet[i].active = false;
    }
    while (!WindowShouldClose())
{
    if (player.active) {
      
        update_player(&player);
        update_bullet(bullet);
        shoot_bullet(bullet, player);
        
        if (enemy.active) {
            update_enemy(&enemy, player);
            check_damage(&enemy, &player, bullet);
        } else {
            
            int side = GetRandomValue(0, 3);
            if (side == 0) { enemy.x = -100; enemy.y = GetRandomValue(0, HEIGHT); } 
            if (side == 1) { enemy.x = WIDTH + 100; enemy.y = GetRandomValue(0, HEIGHT); } 
            if (side == 2) { enemy.x = GetRandomValue(0, WIDTH); enemy.y = -100; } 
            if (side == 3) { enemy.x = GetRandomValue(0, WIDTH); enemy.y = HEIGHT + 100; } 
            enemy.active = true;
        }
    } else {
        
        if (IsKeyPressed(KEY_R)) {
            reset_game(&player, &enemy, bullet);
        }
    }

    BeginDrawing();
        ClearBackground(BLACK);
        
        if (player.active) {
            draw_player(player);
            draw_enemy(&enemy);
            draw_bullet(bullet);
            draw_health(player);
            draw_kill(player);
        } else {
          
            DrawText("GAME OVER", WIDTH/2 - 300, HEIGHT/2 - 50, 100, RED);
            DrawText("Press 'R' to Restart", WIDTH/2 - 250, HEIGHT/2 + 50, 40, WHITE);
            draw_kill(player); 
        }
    EndDrawing();
}

    CloseWindow();

    return 0;
}