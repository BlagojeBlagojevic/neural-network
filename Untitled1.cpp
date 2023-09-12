#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<winbgim.h>



#define width 209 
#define height 242

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
            putpixel(x,y,bytes[0]);
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
		printf("0x%X\n",image[y][x]);
		//setpalette(image[y][x]&0xFF000000,image[y][x]&0x00FFFFFF);
		//putpixel(x,y,image[y][x]&0x00FFFFFF);
		//putpixel(x,y,image[y][x]);
		x++;
		//void arrow;
		
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
	
    //print(&nn);
    
    int gdriver = DETECT, gmode, errorcode;
	
    initwindow(height,width,"image");
	uint32_t image[height][width],image1[height][width];
	read_image("asd.ppm",image);
	save_image_as_ppm("saved.ppm",image);
	//system("saved.ppm");
	//while(1);
	
    //printout(&nn);
    return 0;
}
