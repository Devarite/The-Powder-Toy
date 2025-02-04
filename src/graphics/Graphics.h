#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Config.h"

#include "common/String.h"
#if defined(OGLI)
#include "OpenGLHeaders.h"
#endif
#include "common/tpt-inline.h"
#include "Pixel.h"
#include "Icons.h"

//"Graphics lite" - slightly lower performance due to variable size,
class VideoBuffer
{
public:
	pixel * Buffer;
	int Width, Height;

	VideoBuffer(const VideoBuffer & old);
	VideoBuffer(VideoBuffer * old);
	VideoBuffer(pixel * buffer, int width, int height, int pitch = 0);
	VideoBuffer(int width, int height);
	void Resize(float factor, bool resample = false);
	void Resize(int width, int height, bool resample = false, bool fixedRatio = true);
	void Crop(int width, int height, int x, int y);
	TPT_INLINE void BlendPixel(int x, int y, int r, int g, int b, int a)
	{
	#ifdef PIX32OGL
		pixel t;
		if (x<0 || y<0 || x>=Width || y>=Height)
			return;
		if (a!=255)
		{
			t = Buffer[y*(Width)+x];
			r = (a*r + (255-a)*PIXR(t)) >> 8;
			g = (a*g + (255-a)*PIXG(t)) >> 8;
			b = (a*b + (255-a)*PIXB(t)) >> 8;
			a = a > PIXA(t) ? a : PIXA(t);
		}
		Buffer[y*(Width)+x] = PIXRGBA(r,g,b,a);
	#else
		pixel t;
		if (x<0 || y<0 || x>=Width || y>=Height)
			return;
		if (a!=255)
		{
			t = Buffer[y*(Width)+x];
			r = (a*r + (255-a)*PIXR(t)) >> 8;
			g = (a*g + (255-a)*PIXG(t)) >> 8;
			b = (a*b + (255-a)*PIXB(t)) >> 8;
		}
		Buffer[y*(Width)+x] = PIXRGB(r,g,b);
	#endif
	}

	TPT_INLINE void SetPixel(int x, int y, int r, int g, int b, int a)
	{
		if (x<0 || y<0 || x>=Width || y>=Height)
				return;
	#ifdef PIX32OGL
		Buffer[y*(Width)+x] = PIXRGBA(r,g,b,a);
	#else
		Buffer[y*(Width)+x] = PIXRGB((r*a)>>8, (g*a)>>8, (b*a)>>8);
	#endif
	}

	TPT_INLINE void AddPixel(int x, int y, int r, int g, int b, int a)
	{
		pixel t;
		if (x<0 || y<0 || x>=Width || y>=Height)
			return;
		t = Buffer[y*(Width)+x];
		r = (a*r + 255*PIXR(t)) >> 8;
		g = (a*g + 255*PIXG(t)) >> 8;
		b = (a*b + 255*PIXB(t)) >> 8;
		if (r>255)
			r = 255;
		if (g>255)
			g = 255;
		if (b>255)
			b = 255;
		Buffer[y*(Width)+x] = PIXRGB(r,g,b);
	}
	int SetCharacter(int x, int y, String::value_type c, int r, int g, int b, int a);
	int BlendCharacter(int x, int y, String::value_type c, int r, int g, int b, int a);
	int AddCharacter(int x, int y, String::value_type c, int r, int g, int b, int a);
	~VideoBuffer();

	void CopyData(pixel * buffer, int width, int height, int pitch);
};

class Graphics
{
public:
	pixel *vid;
	int sdl_scale;
#ifdef OGLI
	//OpenGL specific instance variables
	GLuint vidBuf, textTexture;
	void Reset();
	void LoadDefaults();
	void InitialiseTextures();
	void DestroyTextures();
 #endif

	//Common graphics methods in Graphics.cpp
	static char * GenerateGradient(pixel * colours, float * points, int pointcount, int size);

	//PTIF methods
	static void *ptif_pack(pixel *src, int w, int h, int *result_size);
	static pixel *ptif_unpack(void *datain, int size, int *w, int *h);
	static pixel *resample_img_nn(pixel *src, int sw, int sh, int rw, int rh);
	static pixel *resample_img(pixel *src, int sw, int sh, int rw, int rh);
	static pixel *rescale_img(pixel *src, int sw, int sh, int *qw, int *qh, int f);
	static pixel *render_packed_rgb(void *image, int width, int height, int cmp_size);

	//Font/text metrics
	static int CharWidth(String::value_type c);
	static int textnwidth(String s, int n);
	static void textnpos(String s, int n, int w, int *cx, int *cy);
	static int textwidthx(String s, int w);
	static int textwrapheight(String s, int width);
	static int textwidth(String s);
	static void textsize(String s, int & width, int & height);

	VideoBuffer DumpFrame();

	void blendpixel(int x, int y, int r, int g, int b, int a);
	void addpixel(int x, int y, int r, int g, int b, int a);

	void draw_icon(int x, int y, Icon icon, unsigned char alpha = 255, bool invert = false);

	void Clear();
	void Finalise();
	//
	int drawtext_outline(int x, int y, String s, int r, int g, int b, int a);
	int drawtext(int x, int y, String s, int r, int g, int b, int a);
	int drawchar(int x, int y, String::value_type c, int r, int g, int b, int a);
	int addchar(int x, int y, String::value_type c, int r, int g, int b, int a);

	void xor_pixel(int x, int y);
	void xor_line(int x, int y, int x2, int y2);
	void xor_rect(int x, int y, int width, int height);
	void xor_bitmap(unsigned char * bitmap, int x, int y, int w, int h);

	void draw_line(int x, int y, int x2, int y2, int r, int g, int b, int a);
	void drawrect(int x, int y, int width, int height, int r, int g, int b, int a);
	void fillrect(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawcircle(int x, int y, int rx, int ry, int r, int g, int b, int a);
	void fillcircle(int x, int y, int rx, int ry, int r, int g, int b, int a);
	void clearrect(int x, int y, int width, int height);
	void gradientrect(int x, int y, int width, int height, int r, int g, int b, int a, int r2, int g2, int b2, int a2);

	void draw_image(pixel *img, int x, int y, int w, int h, int a);
	void draw_image(const VideoBuffer & vidBuf, int x, int y, int a);
	void draw_image(VideoBuffer * vidBuf, int x, int y, int a);
	void draw_rgba_image(const unsigned char *data, int x, int y, float alpha);

	Graphics();
	~Graphics();
};

#endif
