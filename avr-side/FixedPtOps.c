// Test fixed-point multiply     
             
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <stdlib.h>

//set up the debugging utility ASSERT
#define __ASSERT_USE_STDERR
#include <assert.h>
#include "uart.h"


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/********************************************************************************
Macros and Defines
********************************************************************************/

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// UART file descriptor
// putchar and getchar are in uart.c
FILE uart_str = FDEV_SETUP_STREAM(usart_putchar, usart_getchar, _FDEV_SETUP_RW);


//=== Macros for comparison to asm/c versions ===============
// multiply two fixed 8:8 numbers 
#define multfixSlow(a,b) ((int)((((long)(a))*((long)(b)))>>8)) 
// divide two fixed 8:8 numbers 
#define divfixSlow(a,b)  ((int)((((long)(a))<<8)/((long)(b))))

//========================================================
int divfix(int nn, int dd)
{
    int x, d ;
    signed char count, neg ;
    count = 0;
    neg = 0 ; 
    d = dd ;
    
    // only works with + numbers
    if (d & 0x8000)
    {
        neg = 1;
        d = -d ;
    }
    
    // range reduction
    while (d>0x0100)
    {
        --count ;
        d >>= 1 ;
    }
    
    while (d<0x0080)
    {
        ++count ;
        d <<= 1 ;
    }
     
    // Newton interation
    x = 0x02ea - (d<<1) ;
    x = multfix(x, 0x0200-multfix(d,x));
    x = multfix(x, 0x0200-multfix(d,x)); 

    
    // range expansion
    if (count>0)  x = x<<count ;
    else if (count<0) x = x>>(-count) ;  
    
    // fix sign
    if (neg==1) x=-x;
    
    //form ratio
    x = multfix(x,nn) ;  
     
    return x ;  
}

//========================================================
int sqrtfix(int aa)
{
   
    int a;
    char nextbit, ahigh;
    int root, p ; 
    a = aa; 
    ahigh = a>>8 ;
    //
    // range sort to get integer part and to
    // check for weird bits near the top of the range
    if (ahigh >= 0x40)     //bigger than 64?
    {
        if (a > 0x7e8f)    //>=126.562 = 11.25^2
        {
            root = 0x0b40;  // 11
            nextbit = 0x10 ;
        } 
        else if (ahigh >= 0x79)    //>=121
        {
            root = 0x0b00;  // 11
            nextbit = 0x40 ;
        }
        else if (ahigh >= 0x64)    //>=100
        {
            root = 0x0a00;  // 10
            nextbit = 0x80 ;
        }  
        else if (ahigh >= 0x51)    //>=81
        {
            root = 0x0900;  // 9
            nextbit = 0x80 ;
        } 
        else   //64
        {
            root = 0x0800;      //8
            nextbit = 0x80 ; 
        } 
    }
    else if  (ahigh >= 0x10)  //16    //smaller than 64 and bigger then 16
    { 
        if (ahigh >= 0x31)  //49
        {
            root = 0x0700;      //7
            nextbit = 0x80 ; 
        }
        else if (ahigh >= 0x24)  //36
        {
            root = 0x0600;      //6
            nextbit = 0x80 ; 
        }
        else if (ahigh >= 0x19)  //25
        {
            root = 0x0500;      //5
            nextbit = 0x80 ; 
        }   
        else //16
        {
            root = 0x0400;      //4
            nextbit = 0x80 ; 
        }
    }  
    else       //smaller than 16
    { 
         if (ahigh >= 0x09)  //9
        {
            root = 0x0300;      //3
            nextbit = 0x80 ; 
        } 
        else if (ahigh >= 0x04)  //4
        {
            root = 0x0200;      //2
            nextbit = 0x80 ; 
        }
        else if (ahigh >= 0x01)  //1
        {
            root = 0x0100;      //1
            nextbit = 0x80 ; 
        }  
        else     //less than one
        {
            root = 0;
            nextbit = 0x80 ;  
        }
    }
    // now get the low order bits  
    while (nextbit)
    {
		root = nextbit + root;
		p =  multfix(root,root);  
        if (p >= a) root = root - nextbit ;
    	nextbit = nextbit>>1 ;
    }
    return root ;  
}
  

//======================================================  

volatile int fix0, fix1, prod ; 

int main(void)
{  
  //init the UART -- uart_init() is in uart.c
  usart_init ( MYUBRR );
  sei();
  while(1)
  {
  
    // fixed input
    unsigned char f00 = usart_getchar();
    unsigned char f01 = usart_getchar();
    unsigned char f10 = usart_getchar();
    unsigned char f11 = usart_getchar();
    
    fix0 = ((int)f00 << 8) + ((int)f01);
    fix1 = ((int)f10 << 8) + ((int)f11);
    
    // test multiply 
    prod = multfix(fix0, fix1) ;
    unsigned char out0 = (unsigned char)((prod >> 8) & 0xFF);
    unsigned char out1 = (unsigned char)(prod & 0xFF);
    
    usart_putchar(out0);
    usart_putchar(out1);
    
  }
} 




