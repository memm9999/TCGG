/*

						Date of release: 21/5/2003


		Microsoft(R) Windows(R) 1-bit(monochrome) and 4-bit(16 color)

							 Bitmap Reader

							 Version 2.00

								By Yas

						realyas2001@hotmail.com

							SZABIST Karachi

								PAKISTAN



		This is the header file used to display Microsoft(R) Windows(R) 1-bit
	(2 color also known as monochrome) and 4-bit(16 color) bitmap files in C
	language with the help of EGAVGA.BGI driver. This code has been compiled
	with the Borland Turbo C++ v3.0.



	NOTE:
		The code is a freeware only for evaluation purposes. If you wish to
	use it commertially than contact the author.


	The bitmap reader is a library header file which has the ability to display
1-bit(monochrome or 2 color) and 4-bit(16 color) images in C language. This
library has been written on the Borland Turbo C++ v3.0 .

	All the limitations reguarding the bitmaps are removerd in this version.

	The limitations removed are as follows:

1)  Various programmers had a problem displaying bitmaps(Even I did although I
	had traced the error in my previous version) properly i.e. the width of the
	bitmap had to be calculated using an error correction. This calculation is
	fully corrected and now bitmaps of any resolution can be displayed thus now
	the width of the bitmap does not have to be a multiple of 8. It can be of
	any width and height.

2)  Better control over the function which displays the bitmap is given.

3)  More technical help is given which will help to learn how the code works.




	The only problem faced now is that one of the colors is not displayed correctly
	in the bitmap.



	NEXT TARGET:

		My next target is to add compression algorigthm(Run Length Encoding)
		in the bitmap reader.

*/



#ifndef __BITMAP_H
#define __BITMAP_H

#ifndef __STDIO_H
#include<stdio.h>
#endif

#ifndef __GRAPHICS_H
#include<graphics.h>
#endif

#ifndef FILE_READING_ERROR
#define FILE_READING_ERROR 255
#endif

#ifndef BM_FILE_ERROR
#define BM_FILE_ERROR 254
#endif

#ifndef BM_FILE_ERROR
#define BM_FILE_ERROR 253
#endif

#ifndef COMPRESSION_ERROR
#define COMPRESSION_ERROR 252
#endif

#ifndef BITS_PER_PIXEL_ERROR
#define BITS_PER_PIXEL_ERROR 251
#endif

#ifndef FILE_HEADER_READING_ERROR
#define FILE_HEADER_READING_ERROR 250
#endif

#ifndef INFO_HEADER_READING_ERROR
#define INFO_HEADER_READING_ERROR 249
#endif




/* These two following structures are the Microsoft(R) Windows(R) 3.x bitmap

	structures:




   The Bitmap File Header */

typedef struct
{
	unsigned char type[2];  /* This specifies the type of file. This should be
									equal to BM */
	unsigned long file_size;  /* This gives the bitmap size in bytes */
	unsigned int reserved_1;  /* This is reserved and must be 0 */
	unsigned int reserved_2;  /* This is also reserved and must also be 0 */
	unsigned long offset;  /* This specifies the number of bytes from the
								starting of the bitmap to where the actual
								data part starts */
}BITMAP_FILE_HEADER;


/* The Bitmap Information Header */

typedef struct
{
	unsigned long structure_size;  /* This specifies in bytes the size of the
									header. This must be equal to 40. */
	long width;  /* This specifies the width of bitmap in pixels */
	long height;  /* This specifies the height of bitmap in pixels */
	unsigned int number_of_color_planes;  /* This specifies the number of
											color planes */
	unsigned int bits_per_pixel;  /* This specifies the bits per pixel i.e.
									the total number of colors in the bitmap */
	unsigned long compression;  /* This specifies the type of RLE compression in
								the bitmap. No compression means that the
								value will be 0. */
	unsigned long image_size;  /* This specifies the size of image in pixels
								i.e. width x height */
	long horizontal_pixels_per_meter;  /* This specifies the horizontal
											resolution of image as pixels per
											meter */
	long vertical_pixels_per_meter;  /* This specifies the vertical resolution
										of image as pixels per meter */
	unsigned long colors_used;  /* This specifies the colors used in the
									bitmap. For 4-bit image this value is 0 */
	unsigned long important_colors;  /* This specifies the minimum number of
										important colors in the bitmap */
}BITMAP_INFORMATION_HEADER;



/*	Since the colors are read as EGA colors therefore the colors must be
	converted into their respective VGA colors and so that the colors
	could be displayed as they are present in the bitmap. This is because
	the colors in a bitmap are stored in the form of EGA color palette.

	NOTE: There is one color in Microsoft's(R) EGA palette which is different
	from the one present with the palette given with the EGAVGA driver so it
	will always be displayed as an alternate color. */

unsigned char ega2vga(unsigned char color)
{
	switch(color)
	{
		case 11:
			return(YELLOW);

		case 14:
			return(LIGHTCYAN);

		case 12:
			return(LIGHTBLUE);

		case 1:
			return(RED);

		case 9:
			return(LIGHTRED);

		case 3:
			return(BROWN);

		case 4:
			return(BLUE);

		case 6:
			return(CYAN);

		case 7:
			return(DARKGRAY);

		case 8:
			return(LIGHTGRAY);

	}
	return(color);
}


/*	When reading the monochrome bitmaps the colors must be converted to their
	respective colors for the VGA driver */

unsigned char monochrome2vga(unsigned char color)
{
	if(color==1)
		return(WHITE);
	return(color);
}



unsigned char displaybitmap(int x,int y,unsigned char *file_name)
{
	FILE *bitmap_file;
	BITMAP_FILE_HEADER bitmap_header;
	BITMAP_INFORMATION_HEADER bitmap_information;
	int screen_x,screen_y;
	unsigned int check=0;
	unsigned char color,pixel_color,correction,type;

	if((bitmap_file=fopen(file_name,"rb"))==NULL) /* Opening bitmap file in
												read-only mode */
		return(FILE_READING_ERROR);

	/* Reading the bitmap file header */
	if(fread(&bitmap_header,sizeof(BITMAP_FILE_HEADER),1,bitmap_file)!=1)
		return(FILE_HEADER_READING_ERROR);

	/* Reading the bitmap information header */
	if(fread(&bitmap_information,sizeof(BITMAP_INFORMATION_HEADER),1,bitmap_file)!=1)
		return(INFO_HEADER_READING_ERROR);

	if((bitmap_header.type[0]!='B')||(bitmap_header.type[1]!='M'))
									/* Checking whether the file is a bitmap
									file i.e. checking if it is BM(BitMap) */
	{
		fclose(bitmap_file);
		return(BM_FILE_ERROR);
	}

	if(bitmap_information.compression!=0) 	/* Checking whether there is a
											RLE compression or not because
											this code does not support RLE
											compression */
	{
		fclose(bitmap_file);
		return(COMPRESSION_ERROR);
	}

	fseek(bitmap_file,bitmap_header.offset,SEEK_SET);
	/* Setting the file pointer to the offset part, where the actual data of
		the bitmap starts */


	if(bitmap_information.bits_per_pixel==1) /* Checking whether the bitamp is
												a 1-bit(monochrome) bitamp */
	{

		/* This is an error correction so that images of different resolution
		can be displayed. This correction has to be checked for every bitmap */
		correction=bitmap_information.width%32;
		if(correction!=0)
			correction=32-correction;

		/* Now there is one more issue that the bitmap must be read from the
			left bottom to right top so we are going to do exactly that. */
		for(screen_y=y+bitmap_information.height-1;screen_y>y-1;screen_y--)
		{
			for(screen_x=x;screen_x<x+bitmap_information.width+correction;screen_x++)
			{

				/* The data in the bitamp is stored in such a way theat each
				pixel will give its color value.

				Reading one byte of color data.

				NOTE: One byte of color data will contain eight bits of color
				values and these values must be displayed individually */
				pixel_color=fgetc(bitmap_file);

				if(++check<=bitmap_information.width)
					/* Displaying the first color data */
					putpixel(screen_x,screen_y,monochrome2vga(pixel_color>>7));

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the second color data */
					color=pixel_color<<1;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)

				{
					/* Displaying the third color data */
					color=pixel_color<<2;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the forth color data */
					color=pixel_color<<3;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the fifth color data */
					color=pixel_color<<4;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the sixth color data */
					color=pixel_color<<5;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the seventh color data */
					color=pixel_color<<6;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

				screen_x++;

				if(++check<=bitmap_information.width)
				{
					/* Displaying the eighth color data */
					color=pixel_color<<7;
					putpixel(screen_x,screen_y,monochrome2vga(color>>7));
				}

			}
			check=0;
		}
		type=1;
	}
	else
		if(bitmap_information.bits_per_pixel==4)  /* Checking whether the
												bitmap is a 4-bit(16 color)
												bitmap file */
		{

			correction=bitmap_information.width%8;
			if(correction!=0)
				correction=8-correction;

			/* Reading the bitmap from left bottom to right top */
			for(screen_y=y+bitmap_information.height-1;screen_y>y-1;screen_y--)
			{
				for(screen_x=x;screen_x<x+bitmap_information.width+correction;screen_x++)
				{
					/*	The data of the bitmap is stored in such a way
						that each pixel will give its color.

						NOTE: Since in the bitmap each color data is written
						in the form of a nibble(half of a byte) and we are
						reading the data in the form of a byte therefore the
						function will read exactly two color data and we are
						are going to have to get the two color data and use it
						seperatly to display each pixel.


						Reading one byte of color data. */
					pixel_color=fgetc(bitmap_file);

					if(++check<=bitmap_information.width)

						/* Displaying the first color data */
						putpixel(screen_x,screen_y,ega2vga(pixel_color>>4));

					screen_x++;

					if(++check<=bitmap_information.width)
					{
						/* Displaying the second color data */
						color=pixel_color<<4;
						putpixel(screen_x,screen_y,ega2vga(color>>4));
					}
				}
				check=0;
			}
			type=4;
		}
		else

		/* If the bitmap is not a 1-bit or a 4-bit image then an error message
		will be displayed */
		{
			fclose(bitmap_file);
			return(BITS_PER_PIXEL_ERROR);
		}

	/* Closing the bitmap file */
	fclose(bitmap_file);
	return(type);
}


/* End of macro defination __BITMAP_H */
#endif