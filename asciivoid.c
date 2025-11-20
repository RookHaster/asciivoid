#include <stdio.h>
#include <stdlib.h>

#define H 40
#define W 120

typedef struct blackhole {
	int x, y, z;
	int radius;
	float gravity;
} blackhole;

typedef struct ring {
	int x, y, z;
	int iradius, eradius;
} ring;

typedef struct ray {
	float x, y, z;
	float dx, dy, dz;
	char alive, impact;
	char ascii;
} ray;

ray** init_rays(){
	ray** rays = malloc(sizeof(struct ray*)*H);
	for (int i = 0; i < H; i++){
		rays[i] = malloc(sizeof(struct ray)*W);
		for (int j = 0; j < W; j++){
			rays[i][j].x = -7.0;
			rays[i][j].y = (float)(i-H/2);
			rays[i][j].z = (float)(j-W/2);
			rays[i][j].dx = 1.0;
			rays[i][j].dy = rays[i][j].dz = 0.0;
			rays[i][j].alive = 1;
			rays[i][j].impact = 0;
			rays[i][j].ascii = '.';
		}
	}
	return rays;
}

int hit(float x, float y, float z, blackhole* hole){
	float xx = (x-hole->x)*(x-hole->x);
	float yy = (y-hole->y)*(y-hole->y);
	float zz = (z-hole->z)*(z-hole->z);
	int rr = hole->radius*hole->radius;
	return xx+yy+zz <= rr;
}

void print_matrix(char matrix[H][W]){	
	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			putchar(matrix[i][j]);
		}
		putchar('\n');
	}
}

int main(void){
	struct blackhole hole;
	hole.x = hole.y = hole.z = 0;
	hole.radius = 8;
	char matrix[H][W];
	ray** rays = init_rays();
	int sent = 1;
	while (sent){
	sent = 0;
	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			if (rays[i][j].alive){
				sent = 1; 
				rays[i][j].x += rays[i][j].dx;
				if (hit(rays[i][j].x, rays[i][j].y, rays[i][j].z, &hole)){
					rays[i][j].ascii = '@';
					rays[i][j].alive = 0;
				}
				if (rays[i][j].x >= 10){
					rays[i][j].alive = 0;
				}
			}
		}
	}}
	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			matrix[i][j] = rays[i][j].ascii;
		}
	}
	print_matrix(matrix);
	return 0;
}
