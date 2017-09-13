#include <string>
#include "AssetStorage.h"
#include "nativeInterface.h"

JNIEXPORT jlong JNICALL Java_com_orcchg_openglsample_NativeInterface_init
  (JNIEnv *, jobject) {
  auto ptr = new NativeInterface();
  jlong descriptor = (jlong)(intptr_t) ptr;
  return descriptor;
}

JNIEXPORT void JNICALL Java_com_orcchg_openglsample_NativeInterface_prepareAssets
  (JNIEnv *jenv, jobject, jlong descriptor, jobject assetsJava, jstring internal_storage) {
  AssetStorage* assets = new AssetStorage(jenv, assetsJava);
  assets->open("tile_floor.png");
  off_t asset_length = assets->length();
  char* asset_buffer = new (std::nothrow) char[asset_length];
  bool read_result = assets->read(asset_buffer);
  assets->close();
  delete assets;  assets = nullptr;

  const char* filename = jenv->GetStringUTFChars(internal_storage, 0);
  char filenameAux[256];
  strcpy(filenameAux, filename);
  strcat(filenameAux, "/texture.png");
  FILE* out = fopen(filenameAux, "w");
  fwrite(asset_buffer, sizeof(char), asset_length, out);
  fclose(out);
  delete [] asset_buffer;  asset_buffer = nullptr;
  jenv->ReleaseStringUTFChars(internal_storage, filename);
  jenv->DeleteLocalRef(internal_storage);

  NativeInterface* ptr = (NativeInterface*) descriptor;
  ptr->render->setTextureFilename(filenameAux);
}

JNIEXPORT void JNICALL Java_com_orcchg_openglsample_NativeInterface_setSurface
  (JNIEnv *jenv, jobject, jlong descriptor, jobject surface) {
  NativeInterface* ptr = (NativeInterface*) descriptor;
  if (surface == nullptr) {
    ANativeWindow_release(ptr->window);
    ptr->window = nullptr;
  } else {
    ptr->window = ANativeWindow_fromSurface(jenv, surface);
    ptr->render->setSurface(ptr->window);
    ptr->render->draw();
  }
}

JNIEXPORT void JNICALL Java_com_orcchg_openglsample_NativeInterface_destroy
  (JNIEnv *, jobject, jlong descriptor) {
  NativeInterface* ptr = (NativeInterface*) descriptor;
  delete ptr;
  ptr = nullptr;
}

NativeInterface::NativeInterface()
  : render(new Render()) {
}

NativeInterface::~NativeInterface() {
  delete render;
  render = nullptr;
}
