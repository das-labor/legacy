
// global variable that holds a shadow of the current output data.
// it is written to and read by I2C, and on write the content
// is transferred to the outputs with the set_outputs() function
extern t_outputdata outputdata;

// function for setting the outputs
extern void set_outputs(void);

// set all the outputs to output and init the PWM timers
extern void init_outputs(void);
