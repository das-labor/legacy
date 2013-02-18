#ifndef __SHADER_H
#define __SHADER_H

void setShaders(float width, float height) ;
void printProgramInfoLog(GLuint obj);
void printShaderInfoLog(GLuint obj);
int textFileWrite(char *fn, char *s) ;
char *textFileRead(char *fn) ;

#endif
