#pragma once
#include "Stream.h"
#include "SerialWombat.h"
/*! \file SerialWombatUART.h
*/

/*! \brief A class for the Serial Wombat 4B or SW18AB chips which creates an I2C to UART Bridge

This class allows use of the Serial Wombat 4B chips's internal UART hardware to send
and receive data at standard baud rates in 8-N-1 format.

A Tutorial video is avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/C1FjcaiBYZs" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/C1FjcaiBYZs


The class inherits from the Arduino Sream class, so functions such as println()
can be used once the UART is initialized.

The Serial Wombat 4B chip has a 64 byte transmit buffer and 128 byte receive buffer.
Therefore, up to 64 bytes can be sent to the SerialWombatUART at a time.
Attempts to send more than that will result in the write, print, etc command
blocking until space is available on the SerialWombatUART to buffer the data.

Received data is buffered on the Serial Wombat chip until it is retreived from the
host.  

\warning Due to the overhead of querying and retreiving data from the SerialWombatUART,
data loss is likely when receiving streams of data greater than the buffer size at higher
baud rates.

To minimize this possiblity, read data frequently from the Serial Wombat chip, and set I2C frequency
to 400kHz to maximize throughput (SW4B only) .

This class can Send, Receive, or both.  A single instance of this class is allowed per Serial Wombat 4B chip
due to the fact that it relies on the PIC16F15214's UART module, of which there is only one on the micro.

Two instances of this class can be used on the Serial Wombat 18AB chip by using the begin() call
which takes a hardware indicator of 0 or 1.

A full Serial Wombat packet send / receive sequence (8 bytes in each direction) over I2C is necessary to
query the status of the queues or to read or receive a byte of data.  

The protocol becomes more efficient if multiple bytes are read or written using the readBytes or 
write(const uint8_t* buffer, size_t size) interfaces rather than read() or write(uint8_t data).

The class must be assigned to a pin.  This may be either the receive or transmit pin.

Pin 0 on the Serial Wombat 4B is suggested as a receive pin as it has only input capability.

Serial Wombat 18AB pins must be enhanced digital performance pins.

Available baud rates are:
 - 300  
 - 1200 
 - 2400  
 - 4800  
 - 9600  
 - 19200 
 - 38400  
 - 57600  
 - 115200
*/

class SerialWombatUART :
    public Stream, public SerialWombatPin
{
public:
    /// \brief Constructor for the SerialWombatUART class.  Only one instance is allowed per SerialWombatChip 4B.
    /// \param serialWombat The Serial Wombat chip on which the SerialWombatUART instance will run.
    SerialWombatUART(SerialWombatChip& serialWombat);
   

    /// \brief Initalize the SerialWombatUART.  
/// \param baudRate  300, 1200, 2400, 4800, 9600,  19200,  38400,  57600,  115200
/// \param pin  The pin that will host the state machine.  This can be either the rxPin or txPin
/// \param rxPin The pin that will receive.  All 4 pins on the SW4B may be used.  255 if no receive function is needed
/// \param txPin The pin that will transmit.  Valid values for the SW4B are 1-3.  255 if no transmit function is needed
/// \param HWinterface  1 or 2 for HW UART 1 or 2
    int16_t begin(uint32_t baudRate, uint8_t pin, uint8_t rxPin, uint8_t txPin,uint8_t HWinterface = 1 );
    /// \brief Queries the SerialWombatUART for number bytes available to read
    /// \return Number of bytes available to read.
    int available();
    /// \brief Reads a byte from the SerialWombatUART
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int read();
    /// \brief  Discard all received bytes
    void flush();
    /// \brief Query the SerialWombatUART for the next avaialble byte, but don't remove it from the queue
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int peek();
    /// \brief Write a byte to the SerialWombatUART for Transmit
    /// \param data  Byte to write
    /// \return Number of bytes written
    /// 
    /// This does not check to see if space is avaialble in order to improve perfomance
    /// .  This isn't an issue at high baud rates, as
    /// overhead to transmit one byte at a time allows sufficent time for queuing data to be sent
    /// by the UART.  This could
    /// be a problem at very low baud rates and high I2C bus speeds.
    size_t write(uint8_t data);

    /// \brief Write bytes to the SerialWombatUART for Transmit
    /// \param buffer  An array of uint8_t bytes to send
    /// \param size the number of bytes to send
    /// \return the number of bytes sent
    /// 
    /// This function queries the SerialWombatUART for avaialble TX
    /// buffer space, and sends bytes as buffer space is avaialble.
    /// If avaialable buffer space is not sufficient to send the entire
    /// array then the function will block and continue trying until the
    /// entire message has been sent to the SerialWombatUART transmit queue.
    size_t write(const uint8_t* buffer, size_t size);

    /// \brief Queries the SerialWombatUART for the amount of free TX queue space
    /// \return A value between 0 and 64 for the SW4B
    int availableForWrite();

    /// \brief Reads a specified number of bytes from the SerialWombatUART RX queue
    /// \param buffer  An array into which to put received bytes
    /// \param length  The maximum number of bytes to be received
    /// \return the number of bytes written to buffer
    /// 
    /// This function will read bytes from the SerialWombatUART RX queue into buffer.
    /// If 'length' characters are not available to read then the value returned
    /// will be less than length.
    size_t readBytes(char* buffer, size_t length);


    void setTimeout(long timeout_mS);
protected:
     uint8_t _rxPin  = 255;
     uint8_t _txPin = 255;
     uint8_t _baudMarker = 0;
     uint32_t timeout = 5000;
     uint8_t _pinMode = PIN_MODE_UART_RX_TX;
     uint8_t _tx7Command = (uint8_t)SerialWombatCommands::COMMAND_UART0_TX_7BYTES;
     uint8_t _rx7Command = (uint8_t)SerialWombatCommands::COMMAND_UART0_RX_7BYTES;
};



/*! \brief A class for the Serial Wombat 4B or SW18AB chips which creates a software based UART on the SW18AB

A Tutorial video is avaialble:

\htmlonly
TODO
\endhtmlonly

TODO


The class inherits from the Arduino Sream class, so functions such as println()
can be used once the Software UART is initialized.


\warning Due to the overhead of querying and retreiving data from the SerialWombatUART,
data loss is likely when receiving streams of data greater than the buffer size at higher
baud rates.

Multiple instances of this class can be created on the Serial Wombat 18AB chip.  This pin mode requires 
more CPU time than most, particularly for higher baud rate ports.  Serial Wombat chip CPU usage should
be checked if multiple software uarts are used.  Exceeding the available SW18AB CPU resources will cause
bit errors in the UART.

A queue in the User Buffer area is allocated for RX and one for TX prior to as part of begin for this mode.
Size of these queues should be determined based on system needs.  The User needs to ensure that the created queues do
not overlap with other structures created in the User Buffer

A full Serial Wombat packet send / receive sequence (8 bytes in each direction) over I2C or the main UART is necessary to
query the status of the queues or to read or receive a byte of data.  

The protocol becomes more efficient if multiple bytes are read or written using the readBytes or 
write(const uint8_t* buffer, size_t size) interfaces rather than read() or write(uint8_t data).

The class must be assigned to a pin.  This may be either the receive or transmit pin.

Available baud rates are:
 - 300  
 - 1200 
 - 2400  
 - 4800  
 - 9600  
 - 19200 
 - 28800   (Transmit only, receive may be unreliable )
 - 57600  (Transmit only, receive may be unreliable )
*/
class SerialWombatSWUART : public SerialWombatUART
{

public:
  

    SerialWombatSWUART(SerialWombatChip& serialWombatChip);
    
/// \param baudRate  300, 1200, 2400, 4800, 9600,  19200,  38400,  57600,  115200
/// \param pin  The pin that will host the state machine.  This can be either the rxPin or txPin
/// \param rxPin The pin that will receive.  All 4 pins on the SW4B may be used.  255 if no receive function is needed
/// \param txPin The pin that will transmit.  Valid values for the SW4B are 1-3.  255 if no transmit function is needed
/// \param userMemoryoffset The offset into User Memory where the software storage queues begin
/// \param rxLength The length in bytes of the on-chip rx queue (can be 0 if rxPin == 255).  
/// \param txLength The length in bytes of the on-chip tx queue (can be 0 if txPin == 255).  
    int16_t begin(uint32_t baudRate, uint8_t pin, uint8_t rxPin, uint8_t txPin, uint16_t userMemoryOffset, uint16_t rxLength, uint16_t txLength);


    /// \brief This method can't be called for Software UART because it doens't initialize queues in User Data Area
    int16_t begin(uint32_t baudRate, uint8_t pin, uint8_t rxPin, uint8_t txPin, uint8_t HWinterface) = delete;

    /// \brief Write bytes to the SerialWombatUART for Transmit
    /// \param buffer  An array of uint8_t bytes to send
    /// \param size the number of bytes to send
    /// \return the number of bytes sent
    /// 
    /// This function queries the SerialWombatSWUART for avaialble TX
    /// buffer space, and sends bytes as buffer space is avaialble.
    /// If avaialable buffer space is not sufficient to send the entire
    /// array then the function will block and continue trying until the
    /// entire message has been sent to the SerialWombatUART transmit queue.
    size_t write(const uint8_t* buffer, size_t size);

    /// \brief SerialWombatQueue created on the Serial Wombat chip for data received by the SerialWombatSWUART
    SerialWombatQueue rxQueue{ _sw };
    /// \brief SerialWombatQueue created on the Serial Wombat chip for data to be sent by the SerialWombatSWUART
    SerialWombatQueue txQueue{ _sw };
private:
    using SerialWombatUART::begin;    //make parent class begin unavaialble

};

