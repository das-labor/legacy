#define CONF_BLINK_ON   0x0000
#define CONF_BLINK_OFF  0x0000

/* define how many milliseconds to wait for a response */
#define CONF_RX_TIMEOUT 1000

/* set your tx callback function here */
int ((*rc_tx_cb) (int numchars, void *in_data));

int ((*rc_rx_cb) (void *out_data));
