#ifndef imageLoad_H
#define imageLoad_H
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#define bgrConversion true

typedef unsigned char BYTE;
typedef unsigned short WORD;

#pragma pack(1)
typedef struct{
	BYTE	IDLength;		// velikost obrazov�ho identifik�toru
	BYTE	ColorMapType;	// typ barevn� mapy
	BYTE	Imagetype;		// typ obr�zku
	WORD	CMapStart;		// po��tek barevn� palety
	WORD	CMapLength;		// d�lka barevn� palety
	BYTE	CMapDepth;		// bitov� hloubka polo�ek barevn� palety
	WORD	XOffset;		// X-ov� sou�adnice po��tku obr�zku
	WORD	YOffset;		// Y-ov� sou�adnice po��tku obr�zku
	WORD	Width; 			// ���ka obr�zku uveden� v pixelech
	WORD	Height; 		// v��ka obr�zku uveden� v pixelech
	BYTE	PixelDepth; 	// po�et bit� na jeden pixel (bitov� hloubka)
	BYTE	ImageDescriptor;//	popisova� obr�zku
} TGAheader;

typedef struct                       
{
	unsigned short bfType;           /* Magic number for file */
	unsigned int   bfSize;           /* Size of file */
	unsigned short bfReserved1;      /* Reserved */
	unsigned short bfReserved2;      /* ... */
	unsigned int   bfOffBits;        /* Offset to bitmap data */
	unsigned int   biSize;           /* Size of info header */
	int            biWidth;          /* Width of image */
	int            biHeight;         /* Height of image */
	unsigned short biPlanes;         /* Number of color planes */
	unsigned short biBitCount;       /* Number of bits per pixel */
	unsigned int   biCompression;    /* Type of compression to use */
	unsigned int   biSizeImage;      /* Size of image data */
	int            biXPelsPerMeter;  /* X pixels per meter */
	int            biYPelsPerMeter;  /* Y pixels per meter */
	unsigned int   biClrUsed;        /* Number of colors used */
	unsigned int   biClrImportant;   /* Number of important colors */
} BMPheader;

#define BF_TYPE 0x4D42             /* "MB" */

#pragma pack()
using namespace std;

// Definice funkc� pro r�zn� barvy
void red() {
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
}

void green() {
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green
}

void blue() {
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue
}

void cyan() {
	glColor4f(0.0f, 1.0f, 1.0f, 0.5f); // Cyan
}

void magenta() {
	glColor4f(1.0f, 0.0f, 1.0f, 0.5f); // Magenta
}

void yellow() {
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow
}

void black() {
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Black
}

void white() {
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // White
}


// funkce na��t� jen bitmapov� tga, nekomprimovan�
unsigned char* loadTGA(const char* name, int& width, int& height, unsigned short& channels){

	ifstream f(name,ios::in|ios::binary|ios::ate);
	if (!f.is_open()) {
		cout<< "File "<< name <<" does not exist" << endl;
		return 0;
	}

	TGAheader header;
	f.seekg (0, ios::beg);

	f.read((char*)&header,sizeof(TGAheader));
	if(((BYTE)!header.ImageDescriptor)&0x20 | header.ImageDescriptor&0xC0 | ((BYTE)!header.Imagetype)&0x0E) {
		cout<< "Format is not supported"<< endl;
		f.close();
		return 0;
	}
	width = header.Width;
	height = header.Height;

	int bytu = header.PixelDepth/8;
	std::cout<<"Image "<< name <<" found!! Loading...";

	f.seekg (18 + header.IDLength + header.CMapLength*header.CMapDepth/8, ios::beg);
	// alokace obrazu u� i s okrajem


	unsigned char* result = new unsigned char[header.Width*header.Height*bytu];

	f.read((char*)result,header.Width*header.Height*bytu);



	for(int j=0; j<width*height*bytu; j+=bytu) {
		char backup = result[j+2];
		result[j+2] = result[j];
		result[j] = backup;
	}

	f.close();
	if(bytu==3){
		cout<< "loaded!" << "Size: [" <<height <<","<<width << "], Channels: " << bytu << " Poradi: BGR" << endl;
		channels = GL_RGB;
	}else{
		cout<< "loaded!" << "Size: [" <<height <<","<<width << "], Channels: " << bytu << " Poradi: BGRA" << endl;
		channels = GL_RGBA;
	}
	return result;
}

// funkce pro nahrani BMP obrazku do textury
// filename - nazev souboru
// texture - pointer na jeden prvek typu int, kde se ulozi generovany nazev textury
unsigned char* loadBMP(const char* filename, int& width, int& height, unsigned short& channels) {
	std::ifstream file;

	// otevreni souoboru
	file.open(filename, std::ios::binary);
	if(!file.is_open()) {
		std::cout << "Nemohu otevrit soubor \"" << filename << "\"" << std::endl; 
		return 0;
	}

	// BMP header
	BMPheader bmpHeader;

	// ziskani infa z headeru
	file.read((char *)&bmpHeader, 54);
	if(file.gcount() < 54 || bmpHeader.bfType != BF_TYPE) {
		std::cout << "BMP header neni spravny" << std::endl; 
		file.close();
		return 0;
	}

	int sizex, sizey, bpp=0;
	sizex = bmpHeader.biWidth;
	sizey = bmpHeader.biHeight;
	bpp = bmpHeader.biBitCount;
	width = sizex;
	height = sizey;

	if(!(sizex > 0 && sizey > 0 && bpp == 24)) {
		std::cout << "BMP nema spravne parametry (velikost, neni 24-bit)" << std::endl; 
		file.close();
		return 0;
	}


	// vytvoreni zasobniku pro obsah obrazu
	unsigned char * buffer = new unsigned char[sizex * sizey * 3];
	// nacteni obrazu
	file.read((char*)buffer, sizex * sizey * 3);
	if(file.gcount() < sizex * sizey * 3) {
		std::cout << "BMP obrazova data bud poskozeny, nebo nekompletni" << std::endl; 
		delete []buffer;
		file.close();
		return 0;
	}

	// BGR konverze na RGB
	if(bgrConversion) {
		for(int j=0; j<sizex*sizey*3; j+=3) {
			char backup = buffer[j+2];
			buffer[j+2] = buffer[j];
			buffer[j] = backup;
		}
	}


	std::cout << "Soubor \"" << filename << "\" nacten... OK" << std::endl;

	// close file
	channels = GL_RGB;
	file.close();
	return buffer;
}


bool setTexture(const char* name, unsigned int * texture, bool mipmap)
{
	unsigned char* data = 0;
	int width,height;
	unsigned short channels;
	if(strstr( name, ".tga") != NULL){
		data = loadTGA(name, width, height, channels);
		if(data==0) return false;
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		if(mipmap) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, channels, GL_UNSIGNED_BYTE, data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
		}
		delete [] data;
		return true;
	}

	if(strstr( name, ".bmp") != NULL){
		data = loadBMP(name, width, height, channels);
		if(data==0) return false;
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		if(mipmap) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, channels, GL_UNSIGNED_BYTE, data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0,channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
		}
		delete [] data;
		return true;
	}


	std::cout << "Neznama pripona souboru" << std::endl;
	return false;
}

#endif 
