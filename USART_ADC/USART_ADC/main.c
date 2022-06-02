#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

// DEFINICION FUNCIONES
unsigned char USART_read(void);				// Recepción de caracteres
void USART_write(unsigned char);			// Transmisión de caracteres
void USART_msg(char*);						// Transmision de cadenas
uint16_t ADC_read(uint8_t canal);			// Leer ADC en un canal especifico

int pulso = 0;		// VALOR TIMER
int i=0;		//MULTIPLEXOR
int distancia = 0;		//DISTANCIA EN CM
char muestra[10];		//DISTANCIA EN STRING


int main(void){
	
	// CONFIGURACION DE USART
    DDRB |= (1<<5);		//PB5	COMO SALIDA		
	DDRD |= (1<<1);		//PD1	COMO SALIDA TXa
	DDRD &= ~(1<<0);	//PD0	COMO ENTRADA RX
	UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0); 
	UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	UBRR0 = 103;		//TRANSMISION 9600
	
	//CONFIGURACION DE INTERRUPCION Y SENSOR ULTRASONICO
	cli();		//Desactivamos interrupciones globales
	DDRD= 0b11111011;		//Configuramos el puerto D como entrada(echo) y salida (trigger)
	EICRA |= (1<<ISC00);		//Cuqluier cambio logico activa la interrupcion en int0
	EICRA &= ~(1<<ISC01);
	
	EIMSK |=(1<<INT0);		// Activamos interrupcion externa en int0
	sei();		//Activamos interrupciones globales
	
    while (1) {
		
		PORTD |= (1<<PIND1);	//ponemos en alto para que mande un pulso sonico
		_delay_us(10);
		PORTD &= ~(1<<PIND1);	//volvemos a poner en 0
		
		distancia = pulso/900;		//leemos el ancho del pulso
		itoa(distancia,muestra,10);
		strcat(muestra,"~");
		USART_write_txt(muestra);
		_delay_ms(100);
	}
}

unsigned char USART_read(void){
	if(UCSR0A&(1<<7))						//si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;						//devuelve el dato almacenado en el registro UDR0
	else
		return 0;
}

void USART_write(unsigned char caracter){
	while(!(UCSR0A&(1<<5)));				// mientras el registro UDR0 esté lleno espera
	UDR0 = caracter;						//cuando el el registro UDR0 está vacio se envia el caracter
}

void USART_write_txt(char* cadena){
	while(*cadena !=0x00){
		USART_write(*cadena);				//transmite los caracteres de cadena
		cadena++;							//incrementa la ubicación de los caracteres en cadena para enviar el siguiente caracter de cadena
	}
}

ISR(INT0_vect){
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);		//modo de operacion normal de la interrupcion
	
	if(i==1){
		pulso = TCNT1;		//registro que almacena el tiempo transcurrido por el timer
		TCNT1=0x0000;		// se hace 0 para no acumular valores
		i=0;
	}
	if(i==0){
		TCNT1 = 0x0000;
		
		// cada 16ms el periodo es 32ms
		TCCR1B |= (1<<CS10);
		TCCR1B &= ~(1<<CS11);
		TCCR1B &= ~(1<<CS12);
		i=1;
	}
	
}

