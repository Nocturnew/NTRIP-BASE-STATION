#include "./SerialPort.h"

/*
 * 'openSerialPort()' - Open serial port.
 *
 * Returns the file descriptor on success or -1 on error.
 * For more information: https://www.cmrr.umn.edu/~strupp/serial.html
 * https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#everything-is-a-file
 */
int openSerialPort(int *spDescriptor, char *portName, int baudRate) // "/dev/ttyACM0", B115200
{

	*spDescriptor = (int)open(portName, O_RDWR);//  | O_NOCTTY | O_NDELAY

	if (*spDescriptor == -1)
	{
		perror("open_port: Unable to open serial port.");
	}
	else
	{
		fcntl(spDescriptor, F_SETFL, FNDELAY);

		struct termios options;

		// Get the current options for the port...
		tcgetattr((int)(*spDescriptor), &options);

		// Set the baud rate
		cfsetispeed(&options, baudRate);
		cfsetospeed(&options, baudRate);

		// Set control modes
		options.c_cflag &= ~CSIZE; 			// Mask the character size bits
		options.c_cflag |= CS8;    			// Select 8 data bits
		options.c_cflag &= ~PARENB; 		// Clear parity bit, disabling parity (most common)
		options.c_cflag &= ~CSTOPB; 		// Clear stop field, only one stop bit used in communication (most common)
		options.c_cflag &= ~CRTSCTS; 		// Disable RTS/CTS hardware flow control (most common)
		options.c_cflag |= CREAD | CLOCAL; 	// Turn on READ & ignore ctrl lines (CLOCAL = 1)

		// Set local modes
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Disable canonical data (enable raw data), disable echo and signal interprets

		// Set input modes
		options.c_iflag &= ~(IXON | IXOFF | IXANY);
		options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

		// Set output modes
		options.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
		options.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

		// Set timeout value
		// For more information: http://unixwiz.net/techtips/termios-vmin-vtime.html
		options.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
		options.c_cc[VMIN] 	= 1;

		tcflush(spDescriptor, TCIFLUSH);

		// Save tty settings, also checking for error
		if (tcsetattr(*spDescriptor, TCSANOW, &options) != 0)
		{
			printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		}
	}
	return spDescriptor;
}

int writeSerialPort(int spDescriptor, unsigned char *string, int size)
{
	int res = 0;

	res = write(spDescriptor, string, size);
	if (res < 0)
	{
		printf("Error %i from write: %s\n", errno, strerror(errno));
	}

	return res;
}

int readSerialPort(int spDescriptor, unsigned char *string, int size)
{

	memset(string, '\0', size);

	// Read bytes. The behavior of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	int res = 0;

	res = read(spDescriptor, string, size);
	if (res < 0)
	{
		printf("Error %i from read: %s\n", errno, strerror(errno));
	}

	return res;
}

bool closeSerialPort(int spDescriptor)
{
	int res = 0;

	res = close(spDescriptor);
	if (res < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
