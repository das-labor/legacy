#ifndef __SHADER_H
#define __SHADER_H

void setShaders(float width, float height, int beVerbose, float max_freq, float sample_rate, int conv_depth);
void printProgramInfoLog(GLuint obj, int beVerbose);
void printShaderInfoLog(GLuint obj, int beVerbose);
int textFileWrite(char *fn, char *s) ;
char *textFileRead(char *fn) ;

#endif
