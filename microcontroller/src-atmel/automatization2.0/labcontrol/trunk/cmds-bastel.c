/* -*- Mode: C; tab-width: 2 -*- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "lib-host/can.h"
#include "lib/lap.h"

#include "cmds-bastel.h"
#include "Bastelcmd.h"


#define NDEF 0xff

typedef struct {
uint8_t value;
char *name;
char *desc;
} name_value_t;

name_value_t nametovalue2[] = {
	{C_SW, "SW","\t(C) Klasse der Switch-Objekte auswaehlen"},
	{C_PWM,"PWM","\t(C) Klasse der PWM-Objekte auswaehlen"},

	{F_REG_BANNER,"LAMP_BANNER","(O) wenn SW, dann Lampe an dem Banner"},
	{F_REG_FENSTER,"LAMP_FENSTER","(O) wenn SW, dann Lampe am Fenster"},

	{F_REG_ORGATISCH,"LAMP_ORGATISCH","(O) wenn SW, dann Lampe am Orgatisch"},
	{F_REG_DRUCKER_1,"LAMP_DRUCKER_1","(O) wenn SW, dann Lampe am Drucker 1"},
	{F_REG_DRUCKER_2,"LAMP_DRUCKER_2","(O) wenn SW, dann Lampe am Drucker 2"},
	{F_REG_HELMER_1,"LAMP_HELMER_1","(O) wenn SW, dann Lampe über Helmer 1"},
	{F_REG_HELMER_2,"LAMP_HELMER_2","(O) wenn SW, dann Lampe über Helmer 2"},

	{F_PWM_BANNER,"BANNER","\t(O) wenn PWM, dann Helligkeit der Bannerlampe 0x00=hell, 0xff=dunkel"},
	{F_PWM_FENSTER,"FENSTER","\t(O) wenn PWM, dann Helligkeit der Fensterlampe 0x00=hell, 0xff=dunkel"},
	{F_PWM_ORGATISCH,"ORGATISCH","(O) wenn PWM, dann Helligkeit der Orgatischlampe 0x00=hell, 0xff=dunkel"},

	{NDEF,"NDEF","\tDummyObjekt -  nicht verwenden macht nur sorgen ;)"},
};


void cmd_bastel_help() {
	int i = 0;
	printf("Das ganze setzt sich wie folgt zusammen:\n\n");
	printf("lapcontrol powercommander <class> <object> <function> <value>\n\n");
	printf("im folgendem sind alle Klassen mit (C) , alle Objekte mit (O) und alle Funktionen mit (F) markiert\n\n");
	printf("es findet keine logische ueberpruefung statt... d.h. man kann das auch falsch zusammenstellen, also bitte die eigene logik ueberpruefen. Es macht beispielsweise keinen Sinn die Klasse PWMs auszuwaehlen und dann ein Switchobjekt zu definiern und dieses am ende setzen zu wollen auf einen wert von 0xff oder so.\n\n");
	printf("Was aber geht ist in etwa folgendes: alle switche haben ein, aus, status; alle pwms haben set, get; virtuelle Objekte sind eigenstaendig definiert\n\n");

	for (i = 0; strcmp(nametovalue2[i].name, "NDEF") !=0; i++) {
		printf("%s \t%s\n", nametovalue2[i].name, nametovalue2[i].desc);
	}
	printf("\n");
	printf("Beispiel: 'labcontrol powercommander SW LAMP_TAFEL OFF 0x00'\n");
	printf("\t- schalte die tafellampe aus - das 0x00 am ende muss sein\n");
	printf("Beispiel: 'labcontrol powercommander SW LAMP_FLIPPER ON 0x00'\n");
	printf("\t- schalte die flipperlampe ein - das 0x00 am ende muss sein\n");
	printf("Beispiel: 'labcontrol powercommander PWM FLIPPER SET 0x00'\n");
	printf("\t- schalte die flipperlampe auf volle Helligkeit\n");
	printf("Beispiel: 'labcontrol powercommander PWM SCHRANK SET 0xff'\n");
	printf("\t- schalte die Helligkeit der Schranklampe auf niedrigste Stufe\n");
	printf("");
}
void cmd_bastel(int argc, char **argv) {
	
	pdo_message *msg;

	int i = 0;
	int k = 0;

	if (argc == 4) {
		msg = (pdo_message *)can_buffer_get();
		msg->addr_src = BASTELCMD_SENDER_ADDR;
		msg->addr_dst = BASTELCMD_IFACE_ADDR;
		msg->port_src = BASTELCMD_SENDER_PORT;
		msg->port_dst = BASTELCMD_IFACE_PORT;
		msg->dlc = 3;
		msg->cmd = NDEF;
		msg->data[0] = NDEF;
		msg->data[1] = NDEF;
		for (k = 0; strcmp(nametovalue2[k].name, "NDEF") !=0 ; k++) {
			if (strcmp(nametovalue2[k].name, argv[1]) == 0) {
				msg->cmd = nametovalue2[k].value;
			}
		}

		for (k = 0;  strcmp(nametovalue2[k].name, "NDEF") !=0 ; k++) {
				//printf("%s %s - res %d \n\n", argv[2], nametovalue2[k].name, strcmp(nametovalue2[k].name, argv[2]));
			if (strcmp(nametovalue2[k].name, argv[2]) == 0) {
				msg->data[0] = nametovalue2[k].value;
					//		printf("%s %d - %d\n", argv[i + 2], i, msg->data[i]);
			}
		}
		printf("bla\n\n");
		if (sscanf(argv[3], "%x", &i) != 1) {
			cmd_bastel_help();
			return;
		}
		msg->data[1] = i;
		printf("translated: %s - %s - %s\n",argv[1],argv[2],argv[3]);
		printf("to:         %d - %d - %d\n",msg->cmd,msg->data[0],msg->data[1]);

		if (msg->cmd == NDEF ||
				 msg->data[0] == NDEF) {
			printf("%d - %d - %d : ", msg->cmd, msg->data[0], msg->data[1]);
			cmd_bastel_help();
			return;
		}

		can_transmit((can_message*)msg);
	} else {
		cmd_bastel_help();
	}
	return;

}
