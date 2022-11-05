/*

# Guitar Tuner
Copyright 2021 Chet Zema

This component reads the state of the Polytune LEDs using the FT4222 (SPI -> USB Reader).

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "ftd2xx.h"
#include "libft4222.h"

#include "display.h"
#include "leds.h"

volatile uint8_t running = 1;

static void ctrl_c_handler(int signum)
{
	(void)(signum);
	running = 0;
}

static void setup_handlers(void)
{
	struct sigaction sa =
	{
		.sa_handler = ctrl_c_handler,
	};

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

/// <summary>Queries USB interface for a suitable FT4222 and returns its location identifier.</summary>
/// <returns>location_id or -1</returns>
const char* spi_init() {
	FT_STATUS                 ftStatus;
	FT_DEVICE_LIST_INFO_NODE* devInfo = NULL;
	DWORD                     numDevs = 0;
	int                       i;
	char                      *description;
	int                       found4222 = 0;

	setup_handlers();

	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus != FT_OK) {
		printf("SPI: FT_CreateDeviceInfoList failed (error code %d)\n", (int)ftStatus);
		goto exit;
	}

	if (numDevs == 0) {
		printf("SPI: No devices connected.\n");
		goto exit;
	}

	/* Allocate storage */
	devInfo = calloc((size_t)numDevs, sizeof(FT_DEVICE_LIST_INFO_NODE));
	if (devInfo == NULL) {
		printf("SPI: Allocation failure.\n");
		goto exit;
	}

	/* Populate the list of info nodes */
	ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
	if (ftStatus != FT_OK) {
		printf("SPI: FT_GetDeviceInfoList failed (error code %d)\n",(int)ftStatus);
		goto exit;
	}

	// Search for compatible device.
	for (i = 0; i < (int)numDevs; i++) {
		if (devInfo[i].Type == FT_DEVICE_4222H_0 ||
			devInfo[i].Type == FT_DEVICE_4222H_1_2 ||
			devInfo[i].Type == FT_DEVICE_4222H_3) {
			// In mode 0, the FT4222H presents two interfaces: A and B.
			// In modes 1 and 2, it presents four interfaces: A, B, C and D.
			// In mode 3, the FT4222H presents a single interface.

			printf("SPI: FT4222H Device Found at %d: '%s'\n", i, devInfo[i].Description);

			description = devInfo[i].Description;
			found4222 = 1;
			break;
		}
	}

	if (found4222 == 0) {
		printf("No FT4222H detected.\n");
	}

exit:
	free(devInfo);
	return description;
}

int timeval_subtract(result, x, y) struct timeval* result, * x, * y;
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	   tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

/// <summary>Reads from the FT4222 SPI slave.</summary>
/// <param name="location_id"></param>
/// <returns></returns>
int spi_reader(char *description) {
	FT_STATUS                 ftStatus;
	FT_HANDLE                 ftHandle = (FT_HANDLE)NULL;
	FT4222_STATUS             ft4222Status;
	FT4222_Version            ft4222Version;
	int                       retCode;
	uint16                    bytesReceived = 0;
	uint8                     rxBuffer[65535];
	struct timeval            start, stop, diff;
	long int				  elapsed;

	printf("SPI: Connecting to %s.\n", description);
	ftStatus = FT_OpenEx(description, FT_OPEN_BY_DESCRIPTION, &ftHandle);
	if (ftStatus != FT_OK) {
		printf("SPI: FT_OpenEx failed (error %d)\n", (int)ftStatus);
		retCode = -901;
		goto exit;
	}

	// Configure the FT4222 as SPI slave.
	ft4222Status = FT4222_SPISlave_InitEx(ftHandle, SPI_SLAVE_NO_PROTOCOL);
	if (FT4222_OK != ft4222Status) {
		printf("SPI: FT4222_SPISlave_Init failed (error %d)\n", (int)ft4222Status);
		retCode = -903;
		goto exit;
	}
	else {
		(void)FT4222_UnInitialize(ftHandle);
		(void)FT_Close(ftHandle);
		ftHandle == (FT_HANDLE)NULL;

		ftStatus = FT_OpenEx(description, FT_OPEN_BY_DESCRIPTION, &ftHandle);
		if (ftStatus != FT_OK) {
			printf("SPI: FT_OpenEx failed (error %d)\n", (int)ftStatus);
			retCode = -901;
			goto exit;
		}

		// Configure the FT4222 as SPI slave.
		ft4222Status = FT4222_SPISlave_InitEx(ftHandle, SPI_SLAVE_NO_PROTOCOL);
		if (FT4222_OK != ft4222Status) {
			printf("SPI: FT4222_SPISlave_Init failed (error %d)\n", (int)ft4222Status);
			retCode = -903;
			goto exit;
		}
	}

	setuid(getuid());
	puts("SPI: Superuser relinquished.");

	printf("SPI: FT4222 SPI Slave OK\n");
	printf("SPI: Running.\n");
	printf("SPI: Press Ctrl+C to quit.\n");

	gettimeofday(&start, NULL);

	// Main loop.
	while (running == 1) {
		uint16 bytesAvailable = 0;
		uint16 bytesRead = 0;

		// Only render every 30ms (thats 33 fps).
		gettimeofday(&stop, NULL);
		timeval_subtract(&diff, &stop, &start);
	
		if (diff.tv_sec > 1 || diff.tv_usec > 30000) { // 33 frame/sec
			//printf("R %u.%u\n", diff.tv_sec, diff.tv_usec);
			// Send the virtual display to the LEDs.
			display_render();
			display_reset();
				
			gettimeofday(&start, NULL);
		}

		ft4222Status = FT4222_SPISlave_GetRxStatus(ftHandle, &bytesAvailable);

		if (FT4222_OK != ft4222Status) {
			printf("SPI: FT4222_SPISlave_GetRxStatus failed (error %d)\n", (int)ft4222Status);
			retCode = -904;
			goto exit;
		}

		if (bytesAvailable == 0) {
			continue;
		}

		ft4222Status = FT4222_SPISlave_Read(ftHandle, rxBuffer, bytesAvailable, &bytesRead);

		if (FT4222_OK != ft4222Status) {
			printf("SPI: FT4222_SPISlave_Read failed (error %d)\n", (int)ft4222Status);
			retCode = -905;
			goto exit;
		}
		
		if (bytesRead > 65500) puts("O"); // Overflow warning.
		if (bytesRead > 32767) bytesRead -= 4096; // Skip towards the end.
		if (bytesRead < 4) continue; // This doesn't really happen, but its a guard to make sure we have at least four bytes because the rest of the code assumes that.

		// Search for the first zero followed by a non-zero (to sync the stream).
		int i = 0;
		int synced = 0;

		for (; i < bytesRead - 1;i++) {
			if (rxBuffer[i] == 0 && rxBuffer[i + 1] != 0) {
				synced = 1;
				break;
			}
		}

		if (synced) {
			for (;i < bytesRead - 3;i += 4) {
				if (running == 0) break;

				// Read until zero byte found.
				if (rxBuffer[0] != 0) {
					i -= 3;
					continue;
				}

				if (rxBuffer[i + 1] == 0) {
					i -= 3;
					continue;
				}

				// Update the virtual display.
				display_update(rxBuffer[i + 3], rxBuffer[i + 2], rxBuffer[i + 1], rxBuffer[i + 0]);
			}
		}
	}

exit:
	if (ftHandle != (FT_HANDLE)NULL) {
		(void)FT4222_UnInitialize(ftHandle);
		(void)FT_Close(ftHandle);
		ftHandle == (FT_HANDLE)NULL;
	}

	leds_shutdown();

	return retCode;
}
