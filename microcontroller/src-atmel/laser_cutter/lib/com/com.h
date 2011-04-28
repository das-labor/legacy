//Command byte 1:
#define MSK_MAIN_CMD       0x01
#define MSK_AUTO_CMD       0x02
#define MSK_500V_PSU_CMD   0x08
#define MSK_ZUEND_CMD      0x10
#define MSK_SIMMER_CMD     0x20

//Command byte 2:
#define MSK_PUMPE_CMD      0x01
#define MSK_POWER_PSU_CMD  0x02
#define MSK_CHARGE_CMD     0x04
#define MSK_DUMP_CMD       0x08
#define MSK_FIRE_CMD       0x10

//Simmer Slave response byte
#define MSK_500V_PSU_STATE 0x08
#define MSK_ZUEND_STATE    0x10
#define MSK_SIMMER_STATE   0x20

//Power slave response byte
#define MSK_POWER_PSU_STATE 0x01
#define MSK_PUMPE_STATE     0x02
#define MSK_CHARGE_STATE    0x04
