void test_blinkenlights();


// Definition of states for the parsing state machine
enum state_t
{
	nowhere,
	comment,
	duration,
	inframe
};
