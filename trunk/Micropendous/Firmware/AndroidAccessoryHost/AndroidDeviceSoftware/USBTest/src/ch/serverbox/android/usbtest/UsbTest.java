/*
 * USBTest.java
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

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;

import com.android.future.usb.UsbAccessory;
import com.android.future.usb.UsbManager;


import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.ParcelFileDescriptor;
import android.os.Debug;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class UsbTest extends Activity {
	private UsbManager mUsbManager;
	private UsbAccessory mAccessory = null;
	private Button mBtSend = null;
	FileOutputStream mFout = null;
	FileInputStream mFin = null;
	ParcelFileDescriptor mFileDescriptor;
	private PendingIntent mPermissionIntent = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {

/*
    	super.onCreate(savedInstanceState);
		l("USBTest onCreate Started");
		mUsbManager = UsbManager.getInstance(this);
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent("android.hardware.usb.action.USB_ACCESSORY_ATTACHED"), 0);
		IntentFilter filter = new IntentFilter("android.hardware.usb.action.USB_ACCESSORY_ATTACHED");
		filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
		registerReceiver(mUsbReceiver, filter);

		if (getLastNonConfigurationInstance() != null) {
			mAccessory = (UsbAccessory) getLastNonConfigurationInstance();
			l("USBTest opening Accessory");
			FileDescriptor fd = null; 
			try{
        		fd = mFileDescriptor.getFileDescriptor();
        	}catch(IllegalArgumentException e){
        		finish();
        	}catch(NullPointerException e){
        		finish();
        	}
			mFin = new FileInputStream(fd);
			l("FileInputStream for accessory opened");
			mFout = new FileOutputStream(fd);
			l("FileOutputStream for accessory opened");
		} else {
			l("accessory open fail");
		}

		setContentView(R.layout.main);
*/
		
//Debug.startMethodTracing("USBTest.trace");
    	
    	super.onCreate(savedInstanceState);
        IntentFilter i = new IntentFilter();
        i.addAction(UsbManager.ACTION_USB_ACCESSORY_ATTACHED);
        i.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
        i.addAction("ch.serverbox.android.usbtest.USBPERMISSION");
        registerReceiver(mUsbReceiver,i);
        
        l("onCreate Started");

    	// TODO: if an ADK Accessory is not connected, this will simply exit
        if(getIntent().getAction().equals("android.hardware.usb.action.USB_ACCESSORY_ATTACHED")){
        	l("Action is usb");
        	UsbAccessory accessory = UsbManager.getAccessory(getIntent());
        	mAccessory = accessory;
        	FileDescriptor fd = null;
        	try{
        		fd = UsbManager.getInstance(this).openAccessory(accessory).getFileDescriptor();
        	}catch(IllegalArgumentException e){
        		finish();
        	}catch(NullPointerException e){
        		finish();
        	}
			mFin = new FileInputStream(fd);
			l("FileInputStream for accessory opened");
			mFout = new FileOutputStream(fd);
			l("FileOutputStream for accessory opened");
        } else {
        	UsbAccessory[] accessories = UsbManager.getInstance(this).getAccessoryList();
        	// TODO: should this be causing an exit or waiting for a device to connect?
        	try {
	        	for(UsbAccessory a : accessories){
	        		l("accessory: " + a.getManufacturer());
	        		// Note that Manufacturer is used to match devices and must equal what is set in firmware 
	        		if(a.getManufacturer().equals("Opendous Inc.")){
	        			mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent("ch.serverbox.android.usbtest.USBPERMISSION"),0);
	        			UsbManager.getInstance(this).requestPermission(a,mPermissionIntent);
	        			l("permission requested");
	        			break;
	        		}
	        	}
        	} catch (Exception e) {
        		finish();
        	}
        }
//Debug.stopMethodTracing();
        setContentView(R.layout.main);

        mBtSend = (Button)(findViewById(R.id.btSebd));
        mBtSend.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				String s = ((EditText)findViewById(R.id.editText1)).getText().toString();
				queueWrite(s);
			}
		});
    }
    
    @Override
    protected void onDestroy() {
    	unregisterReceiver(mUsbReceiver);
    	super.onDestroy();
    }
    
    public void queueWrite(final String data){
    	if(mAccessory == null){
    		return;
    	}
    	new Thread(new Runnable() {
			@Override
			public void run() {
				try {
/*					l("Writing length "+data.length());
					mFout.write(new byte[]{(byte)data.length()});
					l("Writing data: "+data);
					mFout.write(data.getBytes());
					l("Done writing");
*/
					String text = "BufferData";
					byte[] mDataBuffer = text.getBytes();

					// first byte will be the number of bytes to send and the rest will be the bytes
					l("Writing data: " + (((Integer.toString(data.length())) + data.toString()).getBytes()));
					mFout.write(((Integer.toString(data.length())) + data.toString()).getBytes());
					l("Done writing");
					//mFin.read(mDataBuffer);
					//l("Data Read=" + (new String(mDataBuffer)));

				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}).start();
    }
    
	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
					UsbAccessory accessory = UsbManager.getAccessory(intent);
					l("Attached!");
					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
						//openAccessory(accessory);
						mAccessory = accessory;
			        	FileDescriptor fd = null;
			        	try{
			        		fd = UsbManager.getInstance(getApplicationContext()).openAccessory(accessory).getFileDescriptor();
			        	}catch(IllegalArgumentException e){
			        		finish();
			        	}catch(NullPointerException e){
			        		finish();
			        	}
			        	mFout = new FileOutputStream(fd);
			        	mBtSend.setEnabled(true);
					} else {
						l("permission denied for accessory " + accessory);
					}
			} else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
				UsbAccessory accessory = UsbManager.getAccessory(intent);
				if (accessory != null && accessory.equals(mAccessory)) {
					if(mFout != null)
						try {
							mFout.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
					mAccessory = null;
					mBtSend.setEnabled(false);
				}
			}else if("ch.serverbox.android.usbtest.USBPERMISSION".equals(action)){
				l("permission answered");
				if(intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)){
		        	UsbAccessory[] accessories = UsbManager.getInstance(getApplicationContext()).getAccessoryList();
		        	for(UsbAccessory a : accessories){
		        		l("accessory: "+a.getManufacturer());
		        		if(a.getManufacturer().equals("Opendous Inc.")){
		        			mAccessory = a;
		                	FileDescriptor fd = null;
		                	try{
		                		fd = UsbManager.getInstance(getApplicationContext()).openAccessory(a).getFileDescriptor();
		                	}catch(IllegalArgumentException e){
		                		finish();
		                	}catch(NullPointerException e){
		                		finish();
		                	}
		                	mFout = new FileOutputStream(fd);
		        			l("added accessory");
		        			break;
		        		}
		        	}
				}
			}
		}
	};
	
	private void l(String l){
		Log.d("USBTest=-=", l);
	}
}