/*
 * USBReceiver.java part of USBTest
 *
 * Copyright (C) 2011 - Manuel Di Cerbo
 * Additional coding and bug fixes by Opendous Inc. www.opendous.org
 *
 * USBTest is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * USBTest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with USBTest; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


package ch.serverbox.android.usbtest;

import com.android.future.usb.UsbAccessory;
import com.android.future.usb.UsbManager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class UsbReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
				UsbAccessory accessory = UsbManager.getAccessory(intent);
				Log.d("USB","Attached!");
				if (intent.getBooleanExtra(
						UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
					//openAccessory(accessory);
				} else {
					Log.d("USB", "permission denied for accessory "
							+ accessory);
				}
		} else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
			UsbAccessory accessory = UsbManager.getAccessory(intent);
		}
	}		
}


