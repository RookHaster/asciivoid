#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define H 40
#define W 120
#define K 0.05
#define STEP 0.2
typedef struct blackhole {
	int x, y, z;
	float radius;
} blackhole;

typedef struct ring {
	int x, y, z;
	float iradius, eradius, vol;
} ring;

typedef struct ray {
	float x, y, z;
	float dx, dy, dz;
	char alive, impact;
	char ascii;
} ray;

// --------------------------- FUNCTIONS -----------------------------

void init_bh(blackhole* bh, int x, int y, int z, float radius){
	bh->x = x;
	bh->y = y;
	bh->z = z;
	bh->radius = radius;
}

void init_ring(ring* r, blackhole* bh, float ir, float er, float vol){
	r->x = bh->x;
	r->y = bh->y;
	r->z = bh->z;
	r->iradius = ir;
	r->eradius = er;
}

ray** init_rays(){
	ray** rays = malloc(sizeof(struct ray*)*H);
	for (int i = 0; i < H; i++){
		rays[i] = malloc(sizeof(struct ray)*W);
		for (int j = 0; j < W; j++){
			rays[i][j].x = -20.0;
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

int hit_bh(float x, float y, float z, blackhole* hole){
	float xx = (x-hole->x)*(x-hole->x);
	float yy = (y-hole->y)*(y-hole->y);
	float zz = (z-hole->z)*(z-hole->z);
	int rr = hole->radius*hole->radius;
	return xx+yy+zz <= rr;
}

int hit_ring(float x, float y, float z, ring* orbit){
	float xx = (x-orbit->x)*(x-orbit->x);
	float zz = (z-orbit->z)*(z-orbit->z);
	float ir2 = orbit->iradius * orbit->iradius;
	float er2 = orbit->eradius * orbit->eradius;
	return xx+zz <= er2 && xx+zz >= ir2 
		&& y <= orbit->vol - (orbit->vol / 2) && y >= orbit->vol - (orbit->vol * 1.5);
}

void step(ray* vector, blackhole* bh){
	float rx = bh->x - vector->x;
	float ry = bh->y - vector->y;
	float rz = bh->z - vector->z;
	float dist = sqrtf(rx*rx + ry*ry + rz*rz);
	float force = K / (dist * dist + 1.0);
	rx /= dist;
	ry /= dist;
	rz /= dist;
	vector->dx += rx * force;
	vector->dy += ry * force;
	vector->dz += rz * force;
	float mag = sqrtf(vector->dx*vector->dx + vector->dy*vector->dy + vector->dz*vector->dz);
	vector->dx /= mag;
	vector->dy /= mag;
	vector->dz /= mag;
	vector->x += vector->dx * STEP;
	vector->y += vector->dy * STEP;
	vector->z += vector->dz * STEP;
}

void print_matrix(char matrix[H][W]){	
	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			putchar(matrix[i][j]);
		}
		putchar('\n');
	}
}

// -------------------------- MAIN -------------------------------

int main(void){
	blackhole hole;
	init_bh(&hole, 0, 0, 0, 5.0);
	ring orbit;
	init_ring(&orbit, &hole, 5.0, 12.0, 0.5);
	char matrix[H][W];
	ray** rays = init_rays();
	int sent = 1;
	while (sent){
	sent = 0;
	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			if (rays[i][j].alive){
				sent = 1; 
				step(&rays[i][j], &hole);
				if (hit_bh(rays[i][j].x, rays[i][j].y, rays[i][j].z, &hole)){
					rays[i][j].ascii = ' ';
					rays[i][j].alive = 0;
				}
				if (hit_ring(rays[i][j].x, rays[i][j].y, rays[i][j].z, &orbit)){
					rays[i][j].ascii = 'o';
					rays[i][j].alive = 0;
				}
				if (rays[i][j].x >= 20){
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
