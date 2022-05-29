
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// DEFINICION FUNCIONES
unsigned char USART_read(void);				// Recepción de caracteres
void USART_write(unsigned char);			// Transmisión de caracteres
void USART_msg(char*);						// Transmision de cadenas
uint16_t ADC_read(uint8_t canal);			// Leer ADC en un canal especifico


int main(void){
	
	// CONFIGURACION DE USART
    DDRB |= (1<<5);		//PB5	COMO SALIDA		
	DDRD |= (1<<1);		//PD1	COMO SALIDA TXa
	DDRD &= ~(1<<0);	//PD0	COMO ENTRADA RX
	UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0); 
	UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	UBRR0 = 103;		//TRANSMISION 9600
	
	// CONFIGURACION DE ADC
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);	// SELECCIONAMOS 5v
	ADMUX &= ~(1<<ADLAR);	// AJUSTAMOSRESULTADO A LA DERECHA
	
	ADCSRA |= (1<<ADPS0);	// PREESCALADOR A 128 
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
	ADCSRA |= (1<<ADEN);	//SENAL DE ENCENDER ADC
	
	//USART_write_txt("Digite 1 para prender el led y 2 para apagarlo\n\r");
	//uint8_t opcion = 0 ; 
	
	char dest[10];
	uint16_t dato_adc;
	float voltaje;
	
    while (1) {
		dato_adc = ADC_read(0);
		voltaje= (float) (5.0/1023)*dato_adc;
		dtostrf(voltaje,5,1,dest);
		strcat(dest,"~");
		USART_write_txt(dest);
		_delay_ms(500);
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

uint16_t ADC_read(uint8_t canal){
	canal &= 0b00001111;	//limpiamos entrada a 5
	ADMUX = (ADMUX & 0xF0) | canal;	// limpiamos los ultimos 4 bits de ADMUX
	ADCSRA |= (1<<ADSC);	//Inicia la conversion
	while ((ADCSRA)&(1<<ADSC));	// se termina hasta que complete la conversion
	return(ADC);
}