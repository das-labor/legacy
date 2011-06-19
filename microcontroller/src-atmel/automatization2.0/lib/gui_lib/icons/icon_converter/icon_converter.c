
#include <stdio.h>

	

uint8_t reverse(uint8_t d){
	uint8_t o = 0;
	uint8_t x;
	for(x=0;x<8;x++){
		o <<= 1;
		o |= d & 0x01;
		d >>= 1;
	}
	return o;

}

struct bmp_header{
	uint16_t    bfType;         // ASCII-Zeichenkette "BM" (Dezimalwert 19778).
	uint32_t    bfSize;         // Größe der BMP-Datei in Byte. (unzuverlässig)
	uint32_t    bfReserved;     // 0
	uint32_t    bfOffBits;      // Offset der Bilddaten in Byte vom Beginn der Datei an.
	uint32_t    biSize;         // 40 (Größe der BITMAPINFOHEADER-Struktur in Byte)
	int32_t     biWidth;        // Breite der Bitmap in Pixel.
	int32_t     biHeight;       // Der Betrag gibt die Höhe der Bitmap in Pixel an. 
	uint16_t    biPlanes;       // 1 (Stand in einigen älteren Formaten wie PCX für die Anzahl der Farbebenen, wird aber für BMP nicht verwendet)
	uint16_t    biBitCount;     // Gibt die Farbtiefe der Bitmap in bpp an; muss einer der folgenden Werte sein: 1, 4, 8, 16, 24 oder 32. Bei 1, 4 und 8 bpp sind die Farben indiziert. 16 und 32 bpp sind ungebräuchlich.
	uint32_t    biCompression;  // Einer der folgenden Werte: 
	uint32_t    biSizeImage;    // Wenn biCompression=BI_RGB: Entweder 0 oder die Größe der Bilddaten in Byte.
	int32_t     biXPelsPerMeter;// Horizontale Auflösung des Zielausgabegerätes in Pixel pro Meter; wird aber für BMP-Dateien meistens auf 0 gesetzt.
	int32_t     biYPelsPerMeter;// Vertikale Auflösung des Zielausgabegerätes in Pixel pro Meter; wird aber für BMP-Dateien meistens auf 0 gesetzt.
	uint32_t    biClrUsed;      // Wenn biBitCount=1: 0.
	uint32_t    biClrImportant; // Wenn biBitCount=1, 4 oder 8: Die Anzahl sämtlicher im Bild verwendeten Farben; 0 bedeutet alle Farben der Farbtabelle.
};

int main(int argc, char * argv[]){
	if(argc < 4){
		printf("Usage: icon_converter <infile.bmp> <array_name> <outfile.c>\n");
		exit(1);
	}
	
	char * file_name  = argv[1];
	char * array_name = argv[2];
	char * outfile    = argv[3];
	
	FILE * fp = fopen(file_name, "r");
	if(fp == 0){
		printf("Error opening bmp file\r\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	uint8_t * buffer = malloc(size);
	fread(buffer, 1, size, fp);
	
	if(size < sizeof(struct bmp_header)){
		printf("No bmp file\n");
		exit(1);
	}
	
	struct bmp_header * hd = (struct bmp_header *)buffer;
	
	if(hd->bfType != 19778){
		printf("No bmp file\n");
		exit(1);
	}
	
	if(hd->biBitCount != 1){
		printf("Must have 1bpp Bitmap\n");
		exit(1);
	}

	if(hd->biCompression != 0){
		printf("Must have uncompressed Bitmap\n");
		exit(1);
	}
	
	printf("Width %d, Height %d\n", hd->biWidth, hd->biHeight);
	
	FILE * ofp = fopen(outfile, "w");
	if(ofp == 0){
		printf("Error opening outfile\n");
		exit(1);
	}
	
	uint32_t my_bytes_per_row = (hd->biWidth + 7) / 8;
	uint32_t bmp_bytes_per_row = ((hd->biWidth + 31) / 32) * 4;
	
	fprintf(ofp, "#include <stdint.h>\n");
	fprintf(ofp, "#include \"../icon.h\"\n");
	fprintf(ofp, "\n");
	fprintf(ofp, "icon_t %s = {\n", array_name);
	fprintf(ofp, "\t%d,\n", my_bytes_per_row * hd->biHeight + 6);
	fprintf(ofp, "\t%d,\n", hd->biWidth);
	fprintf(ofp, "\t%d,\n", hd->biHeight);
	fprintf(ofp, "\t{\n");
	
	uint32_t line = hd->biHeight - 1;
	
	uint32_t x, y;
	
	for(y=0;y<hd->biHeight;y++){
		fprintf(ofp, "\t\t");
		for(x=0;x<my_bytes_per_row;x++){
			uint32_t offset = hd->bfOffBits + (hd->biHeight - 1 - y) * bmp_bytes_per_row + x;
			uint8_t tmp = buffer[offset];
			fprintf(ofp, "0x%02x, ", reverse(tmp));
			//fprintf(ofp, "0x%02X, ", offset);
		}
		fprintf(ofp, "\n");
	}
	
	fprintf(ofp, "\t}\n");
	fprintf(ofp, "};\n");
	
	return 0;
}
