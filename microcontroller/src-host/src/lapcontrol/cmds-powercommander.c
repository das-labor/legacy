/* -*- Mode: C; tab-width: 2 -*- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <can.h>
#include <lap.h>

#include "cmds-powercommander.h"
#include "PowerCommander.h"


typedef struct {
uint8_t value;
char *name;
char *desc;
} name_value_t;

name_value_t nametovalue[] = { 
	{F_SW_OFF , "OFF","\t(F) einen Schalter ausschalten - funktioniert nur mit switch_objekten"},
	{F_SW_ON , "ON","\t(F) einen Schalter einschalten - funktioniert nur mit switch_objekten"},

	{F_SW_STATUS, "STATUS","\t(F) Status eines Schalters abfragen - funktioniert nur mit switch_objekten"},
	{F_PWM_SET, "SET","\t(F) Helligkeit setzen - funktioniert nur mit PWM-Objekten"},

	{F_PWM_GET, "GET","\t(F) Helligkeit abfragen - funktioniert nur mit PWM-Objekten"},
	{C_SW, "SW","\t(C) Klasse der Switch-Objekte auswaehlen"},

	{C_PWM,"PWM","\t(C) Klasse der PWM-Objekte auswaehlen"},
	{C_VIRT,"VIRT","\t(C) Klasse der virtuellen Objekte auswaehlen"},

	{SWL_TAFEL,"LAMP_TAFEL","(O) wenn SW, dann Lampe an der Tafel"},
	{SWL_BEAMER,"LAMP_BEAMER","(O) wenn SW, dann Lampe am Beamer"},

	{SWL_SCHRANK,"LAMP_SCHRANK","(O) wenn SW, dann Lampe bei den Schraenken"},
	{SWL_FLIPPER,"LAMP_FLIPPER","(O) wenn SW, dann Lampe am Flipper"},

	{SWL_LOUNGE,"LAMP_LOUNGE","(O) wenn SW, dann Lampe in der Lounge"},
	{SWL_KUECHE,"LAMP_KUECHE","(O) wenn SW, dann Lampe in der Kueche"},

	{SWL_VORTRAG,"LAMP_VORTRAG","(O) wenn SW, dann alle Lampen im Vortragsraum - wird sich aendern"},
	{SWL_KLO,"LAMP_KLO","(O) wenn SW, dann Lampe im Klo"},

	{SWA_HS,"HS","\t(O) wenn SW, dann Hauptschuetz"},
	{SWA_BEAMER,"PROJEKTOR","(O) wenn SW, den Beamer"},

	{SWA_FREE,"FREE","\t(O) wenn SW, dann ... z.Z. nicht angeschlossen"},
	{SWA_STECKDOSEN,"STECKDOSEN","(O) wenn SW, dann Steckdosen in der Lounge"},

	{SWA_230Haupt,"230HS","\t(O) wenn SW, dann 230V Optokoppler"},
	{PWM_TAFEL,"TAFEL","\t(O) wenn PWM, dann Helligkeit der Tafellampe 0x00=hell, 0xff=dunkel "},

	{PWM_BEAMER,"BEAMER","\t(O) wenn PWM, dann Helligkeit der Beamerlampe 0x00=hell, 0xff=dunkel "},
	{PWM_SCHRANK,"SCHRANK","(O) wenn PWM, dann Helligkeit der Schranklampe 0x00=hell, 0xff=dunkel "},

	{PWM_FLIPPER,"FLIPPER","(O) wenn PWM, dann Helligkeit der Flipperlampe 0x00=hell, 0xff=dunkel "},
	{PWM_LOUNGE,"LOUNGE","\t(O) wenn PWM, dann Helligkeit in der Lounge 0x00=hell, 0xff=dunkel "},

	{PWM_KUECHE,"KUECHE","\t(O) wenn PWM, dann Helligkeit in der Kueche 0x00=hell, 0xff=dunkel "},
	{VIRT_POWER,"POWER","\t(O) wenn Virtuelle Klasse ausgewaehlt, Hauptschalter... schaltet eine Menge von Schaltern - unterstuetzt ON/OFF"},
	{D_NDEF,"D_NDEF","\tDummyObjekt -  nicht verwenden macht nur sorgen ;)"},
};


void cmd_powercommander_help(){
	int i=0;
	printf("Das ganze setzt sich wie folgt zusammen:\n\n");
	printf("lapcontrol powercommander <class> <object> <function> <value>\n\n");
	printf("im folgendem sind alle Klassen mit (C) , alle Objekte mit (O) und alle Funktionen mit (F) markiert\n\n");
	printf("es findet keine logische ueberpruefung statt... d.h. man kann das auch falsch zusammenstellen, also bitte die eigene logik ueberpruefen. Es macht beispielsweise keinen Sinn die Klasse PWMs auszuwaehlen und dann ein Switchobjekt zu definiern und dieses am ende setzen zu wollen auf einen wert von 0xff oder so.\n\n");
	printf("Was aber geht ist in etwa folgendes: alle switche haben ein, aus, status; alle pwms haben set, get; virtuelle Objekte sind eigenstaendig definiert\n\n");
	
	for(i=0;i<29;i++){
		printf("%s \t%s\n",nametovalue[i].name,nametovalue[i].desc);
	}
	printf("\n");
	printf("Beispiel: 'labcontrol powercommander SW LAMP_TAFEL OFF 0x00'\n");
	printf("\t- schalte die tafellampe aus - das 0x00 am ende muss sein\n");
	printf("Beispiel: 'labcontrol powercommander SW LAMP_FLIPPER ON 0x00'\n");
	printf("\t- schalte die flipperlampe ein - das 0x00 am ende muss sein\n");
	printf("Beispiel: 'labcontrol powercommander PWM FLIPPER SET 0x00'\n");
	printf("\t- schalte die Helligkeit der flipperlampe auf 100%\n");
	printf("Beispiel: 'labcontrol powercommander PWM SCHRANK SET 0xff'\n");
	printf("\t- schalte die Helligkeit der Schranklampe auf niedrigste Stufe\n");
	printf("");
}
void cmd_powercommander(int argc, char **argv)
{
	char *arg = argv[1];
	
	pdo_message *msg;

	int i=0;
	int k=0;
	
	if (argc == 5){ 
		msg = (pdo_message *)can_buffer_get();
		msg->addr_src = 0x23;
		msg->addr_dst = 0x02;
		msg->port_src = 0x00;
		msg->port_dst = 0x01;
		msg->dlc= 4;
		msg->cmd= C_NDEF; // von PowerCommander.h
		msg->data[0]= O_NDEF;
		msg->data[1]= F_NDEF;
		msg->data[2]= D_NDEF;
		for(k=0;k<29;k++){
			if(strcmp(nametovalue[k].name,argv[1])==0) {
				msg->cmd=nametovalue[k].value;
			}
		}
		for(i=0;i<2;i++){
			for(k=0;k<29;k++){
				//	printf("%s %s - res %d \n\n",argv[i+2],nametovalue[k].name, strcmp(nametovalue[k].name,argv[i+2]));
				if(strcmp(nametovalue[k].name,argv[i+2])==0){
					msg->data[i]=nametovalue[k].value;
					//		printf("%s %d - %d\n",argv[i+2],i,msg->data[i]);
				}
			}
		}
		if (sscanf(argv[4], "%x", &i) != 1){
			cmd_powercommander_help();
			return;
		}
		msg->data[2]=i;
		printf("%d - %d - %d - %d : foobar\n",msg->cmd,msg->data[0],msg->data[1],msg->data[2]);
		printf("%s %s %s %s\n",argv[1],argv[2],argv[3],argv[4]);
		if ( msg->cmd == C_NDEF ||
				 msg->data[0] == O_NDEF ||
				 msg->data[1] == F_NDEF){
			printf("%d - %d - %d - %d : foobar",msg->cmd,msg->data[0],msg->data[1],msg->data[2]);
			cmd_powercommander_help();
			return;
		}
		
		can_transmit((can_message*)msg);
	} else {
		cmd_powercommander_help();
	}
	/* show help */
	/* 	printf( "\nUsage: lapcontrol [OPTIONS] powercommander <class> <object> <function> <data>\n\n" ); */
	/* 	printf( "Classes: Available Subcommands:\n\n" ); */
	
	
	return;

}
