#include <stdio.h>

int main () {
	FILE *ftoc, *ftoh, *ffrom;
	int i = 0;
	int c;
	
	ftoc = fopen("zImage.c", "w");
	if (ftoc == NULL)
		return 1;

	ftoh = fopen("zImage.h", "w");
	if (ftoh == NULL)
		return 2;

	ffrom = fopen("zimage.zbin", "r"); 
	if (ffrom == NULL)
		return 3;

	fputs("unsigned char zImage[] = {", ftoc);
	while ((c = fgetc(ffrom)) != EOF) {
		if ((i % 8) == 0) 
			fprintf(ftoc, "\n\t\t");
		fprintf(ftoc, "0x%02X, ", c);	
		i++;
	}
	fprintf(ftoc, "};\n\n");
	fprintf(ftoh, "#ifndef Z_IMAGE_H\n#define Z_IMAGE_H\n\n");
	fprintf(ftoh, "#define Z_IMAGE_SIZE %d\n", i);
	fprintf(ftoh, "extern unsigned char zImage[];\n");
	fprintf(ftoh, "#endif // Z_IMAGE_H\n");
	
	fclose(ffrom);
	fclose(ftoh);
	fclose(ftoc);
	return 0;
}
