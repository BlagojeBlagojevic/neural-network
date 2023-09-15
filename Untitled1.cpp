#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDl_timer.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#undef main                //275 183
#define width 275
#define height 183
#define SEED   10


void randomnn(uint32_t weights[height][width])
{
		for(size_t y = 0; y < width; y++)
	{
		for(size_t x = 0; x < height; x++)
		{
				weights[y][x]=rand()%10000;
		}
	}
	
}

void nn(uint32_t image[height][width],uint32_t weights[height][width],uint32_t out[height][width])
{
	//forward
	
	for(size_t y = 0; y < height; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			if(weights[y][x]*image[y][x]>0xFFFFFFFF)
				out[y][x]=0xFFFFFFFF;
			else
				out[y][x]=weights[y][x]*image[y][x];
		}
	}
	
	//backwards
		for(size_t y = 0; y < height; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			if(weights[height][width] < 1)
				weights[y][x]++;
			if(out[y][x] > 1)
				weights[y][x]--;
		}
	}
	//system("pause");
	
	
}


//209 242


void save_image_as_ppm(const char *file_path,uint32_t image[height][width])
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: could write into file %s: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fprintf(f, "P6\n%d %d 255\n",width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            // 0xAABBGGRR
            uint32_t  pixel = image[y][x];
            uint8_t bytes[4] = {
                (pixel&0x000000FF)>>8*0,
                (pixel&0x0000FF00)>>8*1,
                //0,0,0,
                (pixel&0x00FF0000)>>8*2,
                (pixel&0xFF000000)>>8*3,
                //(pixel&0x00000000),
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            //assert(!ferror(f));
            printf("RED:%u  GREEN: %u  BLUE: %u\n",bytes[0],bytes[1],bytes[2]);
        }
    }

     fclose(f);
    //assert(ret == 0);
}


void read_image(const char *file_path,uint32_t image[height][width])
{
	FILE *f=fopen(file_path,"rb");
	char pom[23];
	fgets(pom,23,f);
	fgets(pom,23,f);
	fgets(pom,23,f);
	size_t x=0,y=0;
	while(!feof(f))
	{
		fread(&image[y][x],sizeof(uint32_t),1,f);
		
		x++;
		if(x==width)
		{
			x=0;
			y++;
		}
		if(y==height)
			{
				y=0;
				fclose(f);
				break;
			}
	}
		fclose(f);
}
int main(void)
{

	uint32_t image[height][width],weights[height][width],out[height][width];
	srand(100);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(800,640,NULL,&window,&renderer);
	SDL_Texture *texture;
	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,width,height);
	read_image("baa.ppm",image);
	randomnn(weights);
	size_t k = 0;
	
	while(k<30000)
	{
		k++;
		
		uint32_t *pixels;
		size_t i = 0;
		void *pixel1;
		int pitch;
		//system("pause");
		//SDL_Delay(10);
		size_t j = 0;
		//memset(&pixels,0,800*10);
		SDL_LockTexture(texture,NULL,(void**)&pixels,&pitch);
			nn(image,weights,out);
				for(size_t y = 0; y < height; y++)
					{				
						for(size_t x = 0; x < width; x++)
							{
								pixels[j++]=out[y][x];
							}
					}
		SDL_UnlockTexture(texture);
		SDL_RenderCopy(renderer,texture,NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
	system("pause");
	return 0;
}
