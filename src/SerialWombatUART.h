#pragma once
#include "Stream.h"
#include "SerialWombat.h"
/*! \file SerialWombatUART.h
*/

/*! \brief A class for the Serial Wombat 4B which creates an I2C to UART Bridge

This class allows use of the Serial Wombat 4B's internal UART hardware to send
and receive data at standard baud rates in 8-N-1 format.

A Tutorial video is avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/C1FjcaiBYZs" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/C1FjcaiBYZs


The class inherits from the Arduino Sream class, so functions such as println()
can be used once the UART is initialized.

The Serial Wombat 4B has a 64 byte transmit buffer and 128 byte receive buffer.
Therefore, up to 64 bytes can be sent to the Serial Wombat at a time.
Attempts to send more than that will result in the write, print, etc command
blocking until space is available on the Serial Wombat to buffer the data.

Received data is buffered on the Serial Wombat until it is retreived from the
host.  

\warning Due to the overhead of querying and retreiving data from the Serial Wombat,
data loss is likely when receiving streams of data greater than the buffer size at higher
baud rates.

To minimize this possiblity, read data frequently from the Serial Wombat, and set I2C frequency
to 400kHz to maximize throughput.

This class can Send, Receive, or both.  A single instance of this class is allowed per Serial Wombat 4B
due to the fact that it relies on the PIC16F15214's UART module, of which there is only one on the micro.

A full Serial Wombat packet send / receive sequence (8 bytes in each direction) over I2C is necessary to
query the status of the queues or to read or receive a byte of data.  

The protocol becomes more efficient if multiple bytes are read or written using the readBytes or 
write(const uint8_t* buffer, size_t size) interfaces rather than read() or write(uint8_t data).

The class must be assigned to a pin.  This may be either the receive or transmit pin.

Pin 0 on the Serial Wombat 4B is suggested as a receive pin as it has only input capability.

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
    public Stream
{
public:
    /// \brief Constructor for the SerialWombatUART class.  Only one instance is allowed per SerialWombat 4B.
    /// \param serialWombat The Serial Wombat on which the SerialWombatUART instance will run.
    SerialWombatUART(SerialWombat& serialWombat);
    /// \brief Initalize the SerialWombatUART.  
    /// \param baudRate  300, 1200, 2400, 4800, 9600,  19200,  38400,  57600,  115200
    /// \param pin  The pin that will host the state machine.  This can be either the rxPin or txPin
    /// \param rxPin The pin that will receive.  All 4 pins on the SW4B may be used.  255 if no receive function is needed
    /// \param txPin The pin that will transmit.  Valid values for the SW4B are 1-3.  255 if no transmit function is needed
    void begin(int baudRate,uint8_t pin, uint8_t rxPin, uint8_t txPin);
    /// \brief Queries the Serial Wombat for number bytes available to read
    /// \return Number of bytes available to read.
    int available();
    /// \brief Reads a byte from the Serial Wombat
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int read();
    /// \brief  Discard all received bytes
    void flush();
    /// \brief Query the Serial Wombat for the next avaialble byte, but don't remove it from the queue
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int peek();
    /// \brief Write a byte to the Serial Wombat UART for Transmit
    /// \param data  Byte to write
    /// \return Number of bytes written
    /// 
    /// This does not check to see if space is avaialble in order to improve perfomance
    /// .  This isn't an issue at high baud rates, as
    /// overhead to transmit one byte at a time allows sufficent time for queuing data to be sent
    /// by the UART.  This could
    /// be a problem at very low baud rates and high I2C bus speeds.
    size_t write(uint8_t data);

    /// \brief Write bytes to the Serial Wombat UART for Transmit
    /// \param buffer  An array of uint8_t bytes to send
    /// \param size the number of bytes to send
    /// \return the number of bytes sent
    /// 
    /// This function queries the Serial Wombat for avaialble TX
    /// buffer space, and sends bytes as buffer space is avaialble.
    /// If avaialable buffer space is not sufficient to send the entire
    /// array then the function will block and continue trying until the
    /// entire message has been sent to the Serial Wombat transmit queue.
    size_t write(const uint8_t* buffer, size_t size);

    /// \brief Queries the Serial Wombat for the amount of free TX queue space
    /// \return A value between 0 and 64 for the SW4B
    int availableForWrite();

    /// \brief Reads a specified number of bytes from the Serial Wombat RX queue
    /// \param buffer  An array into which to put received bytes
    /// \param length  The maximum number of bytes to be received
    /// \return the number of bytes written to buffer
    /// 
    /// This function will read bytes from the Serial Wombat RX queue into buffer.
    /// If 'length' characters are not available to read then the value returned
    /// will be less than length.
    size_t readBytes(char* buffer, size_t length);


    void setTimeout(uint32_t timeout_mS);
private:
     SerialWombat& _sw;
     uint8_t _rxPin  = 255;
     uint8_t _txPin = 255;
     uint8_t _pin = 255;
     uint8_t _baudMarker = 0;
     uint32_t timeout = 5000;
};

