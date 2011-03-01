/*
 * userial -- a USB to I2C converter
 *
 * Copyright (c) 2008-2009, Thomas Pircher <tehpeh@gmx.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/
#include "hardware.h"
#include "i2c.h"
#include "cbuf.h"
#include "spi.h"
#include "adc.h"
#include "command.h"

#include "cdc.h"
#include "cdc_descriptors.h"
#include <LUFA/Version.h>
#include <LUFA/Drivers/USB/USB.h>


/**
 * Contains the current baud rate and other settings of the virtual serial
 * port. While this demo does not use the physical USART and thus does not use
 * these settings, they must still be retained and returned to the host upon
 * request or the host will assume the device is non-functional.
 *
 * These values are set by the host via a class-specific request, however they
 * are not required to be used accurately.  It is possible to completely ignore
 * these value or use other settings as the host is completely unaware of the
 * physical serial link characteristics and instead sends and receives data in
 * endpoint streams.
 *****************************************************************************/
CDC_Line_Coding_t LineEncoding = {
    .BaudRateBPS = 0,
    .CharFormat  = OneStopBit,
    .ParityType  = Parity_None,
    .DataBits    = 8
};


cbf_t rx_buf;       //!< The receiver buffer
cbf_t tx_buf;       //!< The transmission buffer
cbf_t cmd_buf;      //!< The command buffer


/**
 * The main function.
 *
 * \return This function does never return.
 *****************************************************************************/
int main(void)
{
    hw_init();
    i2c_init();
    spi_init();
    adc_init();
    cbf_init(&rx_buf);
    cbf_init(&tx_buf);

    // indicate USB not ready
    led_set(HW_LEDMASK_USB_NOTREADY);

    // initialize USB Subsystem
    USB_Init();

    // main loop
    for (;;)
    {
        CDC_Task();
        USB_USBTask();
    }
}

/**
 * Event handler for the USB_Connect event. This indicates that the device is
 * enumerating via the status LEDs and starts the library USB task to begin the
 * enumeration and USB management process.
 *****************************************************************************/
void EVENT_USB_Device_Connect(void)
{
    // indicate USB enumerating
    led_set(HW_LEDMASK_USB_ENUMERATING);
}

/**
 * Event handler for the USB_Disconnect event. This indicates that the device
 * is no longer connected to a host via the status LEDs and stops the USB
 * management and CDC management tasks.
 *****************************************************************************/
void EVENT_USB_Device_Disconnect(void)
{
    // Indicate USB not ready
    led_set(HW_LEDMASK_USB_NOTREADY);
}

/**
 * Event handler for the USB_ConfigurationChanged event. This is fired when the
 * host set the current configuration of the USB device after enumeration - the
 * device endpoints are configured and the CDC management task started.
 *****************************************************************************/
void EVENT_USB_Device_ConfigurationChanged(void)
{
    // indicate USB connected and ready
    led_set(HW_LEDMASK_USB_READY);

    // setup CDC Notification, Rx and Tx Endpoints
    if (!(Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT,
                    ENDPOINT_DIR_IN, CDC_NOTIFICATION_EPSIZE,
                    ENDPOINT_BANK_SINGLE)))
    {
        led_set(HW_LED_ALL);
    }

    if (!(Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK,
                    ENDPOINT_DIR_IN, CDC_TXRX_EPSIZE,
                    ENDPOINT_BANK_SINGLE)))
    {
        led_set(HW_LEDMASK_USB_ERROR);
    }

    if (!(Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK,
                    ENDPOINT_DIR_OUT, CDC_TXRX_EPSIZE,
                    ENDPOINT_BANK_SINGLE)))
    {
        led_set(HW_LEDMASK_USB_ERROR);
    }

    // reset line encoding baud rate so that the host knows to send new values
    LineEncoding.BaudRateBPS = 0;
}

/**
 * Event handler for the USB_UnhandledControlRequest event. This is used to
 * catch standard and class specific control requests that are not handled
 * internally by the USB library (including the CDC control commands, which are
 * all issued via the control endpoint), so that they can be handled
 * appropriately for the application.
 *****************************************************************************/
void EVENT_USB_Device_UnhandledControlRequest(void)
{
    // Process CDC specific control requests
    switch (USB_ControlRequest.bRequest)
    {
        case REQ_GetLineEncoding:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                // Acknowledge the SETUP packet, ready for data transfer
                Endpoint_ClearSETUP();

                // Write the line coding data to the control endpoint
                Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_Line_Coding_t));

                // Finalize the stream transfer to send the last packet or clear the host abort
                Endpoint_ClearOUT();
            }

            break;
        case REQ_SetLineEncoding:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                // Acknowledge the SETUP packet, ready for data transfer
                Endpoint_ClearSETUP();

                // Read the line coding data in from the host into the global struct
                Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_Line_Coding_t));

                // Finalize the stream transfer to clear the last packet from the host
                Endpoint_ClearIN();
            }

            break;
        case REQ_SetControlLineState:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                // Acknowledge the SETUP packet, ready for data transfer */
                Endpoint_ClearSETUP();

                /* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
                   lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
                   CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
                 */

                Endpoint_ClearStatusStage();
            }

            break;
    }
}

/**
 * The CDC task.
 * Function to manage CDC data transmission and reception to and from the host.
 *****************************************************************************/
void CDC_Task(void)
{
    bool endpoint_full;

	// Device must be connected and configured for the task to run
	if (USB_DeviceState != DEVICE_STATE_Configured) {
	  return;
    }

    // Select the Serial Rx Endpoint
    Endpoint_SelectEndpoint(CDC_RX_EPNUM);

    if (Endpoint_IsOUTReceived()) {
        led_set(0);
        while (Endpoint_BytesInEndpoint()) {
            cbf_put(&rx_buf, Endpoint_Read_Byte());
        }
        // Check to see if all bytes in the current packet have been read
        if (!Endpoint_BytesInEndpoint()) {
            // Clear the endpoint buffer
            Endpoint_ClearOUT();
        }
        led_set(HW_LED1);
    }

    led_set(HW_LED2);
    while (cmd_scan(&rx_buf, &tx_buf)) {
        if (cmd_parse(&rx_buf, &cmd_buf, &tx_buf)) {
            if (!cbf_isempty(&cmd_buf)) {
                cmd_exec(&cmd_buf, &tx_buf);
            }
        }
    }
    led_set(HW_LEDMASK_USB_READY);


    // Select the Serial Tx Endpoint
    Endpoint_SelectEndpoint(CDC_TX_EPNUM);

    if (!cbf_isempty(&tx_buf)) {
        led_set(0);

        // Wait until Serial Tx Endpoint Ready for Read/Write
        while (!Endpoint_IsReadWriteAllowed());

        while (!cbf_isempty(&tx_buf) && (Endpoint_BytesInEndpoint() < CDC_TXRX_EPSIZE)) {
            Endpoint_Write_Byte(cbf_get(&tx_buf));
        }

        // Send the data
        endpoint_full = Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE;
        Endpoint_ClearIN();

        // If the last packet filled the endpoint, send an empty packet to release the buffer on
        // the receiver (otherwise all data will be cached until a non-full packet is received)
        if (endpoint_full)
        {
            // Wait until the endpoint is ready for another packet
            Endpoint_WaitUntilReady();

            // Send an empty packet to terminate the transfer
            Endpoint_ClearIN();
        }
    }
    led_set(HW_LEDMASK_USB_READY);
}
