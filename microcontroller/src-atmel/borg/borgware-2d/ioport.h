
// Makros for simplified single pin io access.

#define PORT_(port) PORT ## port 
#define DDR_(port)  DDR  ## port 
#define PIN_(port)  PIN  ## port 

#define PORT(port) PORT_(port) 
#define DDRR(port)  DDR_(port) 
#define PINN(port)  PIN_(port)

#define SET_DDR(p)    DDRR(p##_PORT) |= (1<<p##_BIT)
#define CLEAR_DDR(p)  DDRR(p##_PORT) &= ~(1<<p##_BIT)
#define OUTPUT_ON(p)  PORT(p##_PORT) |= (1<<p##_BIT)
#define OUTPUT_OFF(p) PORT(p##_PORT) &= ~(1<<p##_BIT)
#define INPUT(p)    ((PINN(p##_PORT) & (1<<p##_BIT)) != 0)


/* 
	Use Like this:


#define LED_PORT C
#define LED_BIT  7

#define SWITCH_PORT B
#define SWITCH_BIT  0

int main(){
	SET_DDR(LED);       //set to output
	OUTPUT_ON(SWITCH);  //turn on pullup
	
	if(INPUT(SWITCH)){
		OUTPUT_ON(LED);
	}else{
		OUTPUT_OFF(LED);
	}
}

*/
