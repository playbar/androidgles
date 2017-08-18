/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: G:\\projects\\MJ\\MojingSDK\\Core\\VrService\\src\\com\\baofeng\\mojing\\service\\MojingSDKAIDLService.aidl
 */
package com.baofeng.mojing.service;
public interface MojingSDKAIDLService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.baofeng.mojing.service.MojingSDKAIDLService
{
private static final java.lang.String DESCRIPTOR = "com.baofeng.mojing.service.MojingSDKAIDLService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.baofeng.mojing.service.MojingSDKAIDLService interface,
 * generating a proxy if needed.
 */
public static com.baofeng.mojing.service.MojingSDKAIDLService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.baofeng.mojing.service.MojingSDKAIDLService))) {
return ((com.baofeng.mojing.service.MojingSDKAIDLService)iin);
}
return new com.baofeng.mojing.service.MojingSDKAIDLService.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_clientResume:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
com.baofeng.mojing.service.IServiceResponse _arg1;
_arg1 = com.baofeng.mojing.service.IServiceResponse.Stub.asInterface(data.readStrongBinder());
this.clientResume(_arg0, _arg1);
reply.writeNoException();
return true;
}
case TRANSACTION_clientPause:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
this.clientPause(_arg0);
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.baofeng.mojing.service.MojingSDKAIDLService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public void clientResume(java.lang.String packageName, com.baofeng.mojing.service.IServiceResponse callback) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(packageName);
_data.writeStrongBinder((((callback!=null))?(callback.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_clientResume, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
@Override public void clientPause(java.lang.String packageName) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(packageName);
mRemote.transact(Stub.TRANSACTION_clientPause, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_clientResume = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_clientPause = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
}
public void clientResume(java.lang.String packageName, com.baofeng.mojing.service.IServiceResponse callback) throws android.os.RemoteException;
public void clientPause(java.lang.String packageName) throws android.os.RemoteException;
}
