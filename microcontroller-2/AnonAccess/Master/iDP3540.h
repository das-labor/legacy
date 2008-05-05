#ifndef IDP3540_H_
#define IDP3540_H_
/*
    Function code    Hex. Code                              Functions
       FF + n           0C + n     "n" -lines paper feed command
         SO              0E        Enlarged character command
         SI               0F       Normal character command
         LF              0A        Paper feed command
         CR              0D        Print command
        DC1               11       Initial set command
        DC2               12       Inverted character command
        DC3               13       Red color print command
        CAN               18       Clear command
     ESC + P + 0      1B, 50, 00   Paper full cut command
     ESC + P + 1      1B, 50, 01   Paper partial cut command
     ESC + - + n      1B, 2D, n    Underline command
  ESC + * + n1 + n2 1B, 2A, n1, n2 Graphic command
      ESC + 1           1B, 31     1 / 9 inch paper feed preset command
      ESC + 2           1B, 32     2 / 9 inch paper feed preset command
    ESC + C + n        1B, 43, n   Page length set command
     ESC + f + 1      1B, 66, 01   From feed command
    ESC + N + n       1B, 4E, n    "n line" skip perforation command
      ESC + O           1B, 4F     Skip perforation cancel command
         RS              1E        Buzzer command
        SUB              1A        Second drawer drive command
         FS              1C        First drawer quick drive command
ESC + BEL + N1 + N2 1B, 07, n1, n2 Drive pulse setting command for the first drawer
        BEL                7       First drawer drive command

*/

#define NLINEFEED_CMD  0x0C
#define LARGECHAR_CMD  0x0E
#define NORMALCHAR_CMD 0x0F
#define LINEFEED_CMD   0x0A
#define PRINT_CMD      0X0D
#define INITALISE_CMD  0x11
#define INVERTCHAR_CMD 0x12
#define REDPRINT_CMD   0x13
#define CLEAR_CMD      0x18
#define FULLCUT_CMDS   "\x1B\x50\x00"
#define PARTCUT_CMDS   "\x1B\x50\x01"
#define UNDERLINE_CMDS "\x1B\x2D"
#define GRAPHIC_CMDS   "\x1B\x2A"
#define PAPERFEED19_CMDS "\x1B\x31"
#define PAPERFEED29_CMDS "\x1B\x32"
#define PAGELENGTH_CMDS "\x1B\x43"
#define FORMFEED_CMDS "\x1B\x66\x01"
#define NSKIPPERFORATION_CMDS "\x1B\x4E"
#define SKIPPERFORATIONCANCEL_CMDS "\x1B\x4F"
#define BUZZER_CMD     0x1E
#define SECDRAWER_CMD  0x1A
#define FRSTDRAWERQUICK_CMD 0x1C
#define DRIVEPULSSETTING_CMDS "\x1B\x07"
#define FRSTDRAWER     0x07





void cutpaper(void);
void setred(void);






#endif /*IDP3540_H_*/
