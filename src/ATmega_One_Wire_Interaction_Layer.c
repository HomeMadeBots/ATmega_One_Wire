#include "ATmega_One_Wire_Interaction_Layer.h"

#include <avr/interrupt.h> /* sei, cli */
#include "Bits_Management.h"
#include "Delay.h"


/*============================================================================*/
/* Macros */
/*============================================================================*/
#define READ()             Test_Bit_Uint8(*(Me->Pin_Address),Me->Bit_Index)
#define SET_MODE_INPUT()   Reset_Bit_Uint8(*((Me->Pin_Address)+1),Me->Bit_Index)
#define SET_MODE_OUTPUT()  Set_Bit_Uint8(*((Me->Pin_Address)+1),Me->Bit_Index)
#define WRITE_LOW()        Reset_Bit_Uint8(*((Me->Pin_Address)+2),Me->Bit_Index)
#define WRITE_HIGH()       Set_Bit_Uint8(*((Me->Pin_Address)+2),Me->Bit_Index)


/*============================================================================*/
/* Private methods declaration */
/*============================================================================*/
static void Write_Bit(
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t bit_value );
static uint8_t Read_Bit( const ATmega_One_Wire_Interaction_Layer* Me );
static void Reset( const ATmega_One_Wire_Interaction_Layer* Me );
static void Write( 
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t byte_value );
static uint8_t Read( const ATmega_One_Wire_Interaction_Layer* Me );
static void Write_Bytes( 
    const ATmega_One_Wire_Interaction_Layer* Me,
    const uint8_t* bytes_array,
    uint8_t nb_bytes );
static void Read_Bytes(
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t* bytes_array,
    uint8_t nb_bytes );
static void Select(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const uint8_t address[8] );


/*============================================================================*/
/* Realized interfaces */
/*============================================================================*/
void ATmega_OneWire_IL__Communication__Send_Simple_Command(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const T_One_Wire_Device_Address* slave_address,
    uint8_t command )
{
    Reset( Me );
    Select( Me, *slave_address );
    Write( Me, command );
}
/*----------------------------------------------------------------------------*/
void ATmega_OneWire_IL__Communication__Send_Write_Command(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const T_One_Wire_Device_Address* slave_address,
    uint8_t command,
    const uint8_t* message,
    uint8_t nb_bytes )
{
    Reset( Me );
    Select( Me, *slave_address );
    Write( Me, command );
    Write_Bytes( Me, message, nb_bytes );
}
/*----------------------------------------------------------------------------*/
void ATmega_OneWire_IL__Communication__Send_Read_Command(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const T_One_Wire_Device_Address* slave_address,
    uint8_t command,
    uint8_t* message,
    uint8_t nb_bytes )
{
    Reset( Me );
    Select( Me, *slave_address );
    Write( Me, command );
    Read_Bytes( Me, message, nb_bytes );
}


/*============================================================================*/
/* Private methods definition */
/*============================================================================*/
static void Write_Bit( 
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t bit_value )
{
    if(bit_value & 1)
    {
        cli();
        WRITE_LOW();
        SET_MODE_OUTPUT();
        Wait_Microsecond(10);
        WRITE_HIGH();
        sei();
        Wait_Microsecond(50);
    }
    else
    {
        cli();
        WRITE_LOW();
        SET_MODE_OUTPUT();
        Wait_Microsecond(55);
        WRITE_HIGH();
        sei();
        Wait_Microsecond(5);
    }    
}
/*----------------------------------------------------------------------------*/
static uint8_t Read_Bit( const ATmega_One_Wire_Interaction_Layer* Me )
{
    uint8_t bit_value;

    cli();
    SET_MODE_OUTPUT();
    WRITE_LOW();
    Wait_Microsecond(3);
    SET_MODE_INPUT();
    Wait_Microsecond(10);
    bit_value = READ();
    sei();
    Wait_Microsecond(47);
    return bit_value;    
}
/*----------------------------------------------------------------------------*/
static void Reset( const ATmega_One_Wire_Interaction_Layer* Me )
{
    uint8_t retries = 0;

    cli();
    SET_MODE_INPUT();
    sei();

    /* Wait for the bus is HIGH */
    do
    {
        retries++;
        Wait_Microsecond(2);
    } while ( !READ() && retries < 100 );

    if( retries < 100 )
    {
        cli();
        WRITE_LOW();
        SET_MODE_OUTPUT();
        sei();
        Wait_Microsecond(480);
        cli();
        SET_MODE_INPUT();
        Wait_Microsecond(60);
        sei();
        Wait_Microsecond(240);
    }
}
/*----------------------------------------------------------------------------*/
static void Write(
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t byte_value )
{
    uint8_t bitmask;

    for (bitmask = 0x01; bitmask; bitmask <<= 1)
    {
        Write_Bit( Me, (bitmask & byte_value) ? 1 : 0 );
    }
    
}
/*----------------------------------------------------------------------------*/
static uint8_t Read( const ATmega_One_Wire_Interaction_Layer* Me )
{
    uint8_t bitmask;
    uint8_t byte_value = 0;

    for (bitmask = 0x01; bitmask; bitmask <<= 1)
    {
        if ( Read_Bit( Me ) )
        {
            byte_value |= bitmask;
        }
    }
    return byte_value;    
}
/*----------------------------------------------------------------------------*/
static void Write_Bytes(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const uint8_t* bytes_array,
    uint8_t nb_bytes )
{
    for ( uint8_t i = 0 ; i < nb_bytes ; i++ )
    {
        Write( Me, bytes_array[i] );
    }
}
/*----------------------------------------------------------------------------*/
static void Read_Bytes(
    const ATmega_One_Wire_Interaction_Layer* Me,
    uint8_t* bytes_array,
    uint8_t nb_bytes )
{
    for( uint8_t i = 0 ; i < nb_bytes ; i++ )
    {
        bytes_array[i] = Read( Me );
    }
}
/*----------------------------------------------------------------------------*/
static void Select(
    const ATmega_One_Wire_Interaction_Layer* Me,
    const uint8_t address[8] )
{
    uint8_t i;

    Write( Me, 0x55 );

    for( i = 0; i < 8; i++ )
    {
        Write( Me, address[i] );
    }
}