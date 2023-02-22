#include <iostream>
#include <stdio.h>
#include <math.h>
#include "VecFunctions.h"

int main() {
    int width = 240;
    int height = 60;
    float aspect = (float) width / height;
    float pixelAspect = 11.0f / 24.0f;
    char gradient[] = " .:!/r(l1Z4H9W8$@";
    int gradientSize = sizeof(gradient)/sizeof(gradient[0]) - 2;
    char *screen = new char[width * height];


    for (int t = 0; t < 10000; t++) {
        vec3 light = norm(vec3(-0.5, 0.5, -1.0));
        vec3 spherePos = vec3(0, 3, 0);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                int color = 0;
                vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
                uv.x *= aspect * pixelAspect;
                vec3 ro = vec3(-12, 0, 0);
                vec3 rd = norm(vec3(2, uv));
                ro = rotateY(ro, 0.25);
                rd = rotateY(rd, 0.25);
                ro = rotateZ(ro, t * 0.01);
                rd = rotateZ(rd, t * 0.01);
                float diff = 1;
                for (int k = 0; k < 5; k++) {
                    float minIt = 99999;

                    vec2 intersection = sphere(ro - spherePos, rd, 1);
                    vec3 n = 0;
                    float albedo = 1;
                    if (intersection.x > 0) {
                        vec3 itPoint = ro - spherePos + rd * intersection.x;
                        minIt = intersection.x;
                        n = norm(itPoint);
                    }
                    vec3 boxN = 0;
                    intersection = box(ro, rd, 1, boxN);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = boxN;
                    }
                    intersection = plane(ro, rd, vec3(0, 0, -1), 1);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = vec3(0, 0, -1);
                        albedo = 0.5;
                    }
                    if (minIt < 99999) {
                        diff *= dot(n, light) * 0.5 + 0.5;
                        ro = ro + rd * (minIt - 0.01);
                        rd = reflect(rd, n);
                        color = (int) (diff * 20);
                    } else break;
                }

                color = clamp(color, 0, gradientSize);
                char pixel = gradient[color];
                screen[i + j * width] = pixel;
            }
        }
        std::cout << "\x1B[2J\x1B[H";
        //std::printf(screen);
        puts(screen);
        //for(u_long i=1;i<sizeof(screen)/sizeof(screen[0]);i++) putchar_unlocked(screen[i]);
    }
}
