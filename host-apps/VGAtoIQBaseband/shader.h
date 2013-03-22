#ifndef __SHADER_H
#define __SHADER_H

void setShaders(float width, float height, bool beVerbose, float max_freq, float sample_rate, int conv_depth);
void printProgramInfoLog(GLuint obj, bool beVerbose);
void printShaderInfoLog(GLuint obj, bool beVerbose);
int textFileWrite(char *fn, char *s) ;
char *textFileRead(char *fn) ;

#endif
