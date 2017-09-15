//
// Created by houguoli on 2017/9/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>
#include "GraphicBuffer.h"
#include <dlfcn.h>
#include <jni.h>
#include "Graphics.h"
#include "my_log.h"

int Graphics::initGL(){

    const char* const driver_absolute_path = "/system/lib/egl/libEGL_mali.so";
    // On Gingerbread you have to load symbols manually from Mali driver because
    // Android EGL library has a bug.
    // From  ICE CREAM SANDWICH you can freely use the eglGetProcAddress function.
    // You might be able to get away with just eglGetProcAddress (no dlopen). Tr y it,  else revert to the following code
    void* dso = dlopen(driver_absolute_path, RTLD_LAZY);
    if (dso != 0)
    {
        LOGI("dlopen: SUCCEEDED");
        _eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)dlsym(dso, "eglCreateImageKHR");
        _eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) dlsym(dso, "eglDestroyImageKHR");
    }
    else
    {
        LOGI("dlopen: FAILED! Loading functions in common way!");
        _eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR");
        _eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR");
    }

    if(_eglCreateImageKHR == NULL)
    {
        LOGE("Error: Failed to find eglCreateImageKHR at %s:%in", __FILE__, __LINE__);
        exit(1);
    }
    if(_eglDestroyImageKHR == NULL)
    {
        LOGE("Error: Failed to find eglDestroyImageKHR at %s:%in", __FILE__, __LINE__);
        exit(1);
    }
    _glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) eglGetProcAddress("glEGLImageTargetTexture2DOES");
    if(_glEGLImageTargetTexture2DOES == NULL)
    {
        LOGI("Error: Failed to find glEGLImageTargetTexture2DOES at %s:%in", __FILE__, __LINE__);
        return 0;
    }

    graphicBuffer = new GraphicBuffer( emu_width,emu_height,
                                       PIXEL_FORMAT_RGBA_8888,
                                       GraphicBuffer::USAGE_HW_TEXTURE |
                                       GraphicBuffer::USAGE_HW_2D |
                                               GraphicBuffer::USAGE_SW_READ_OFTEN |
                                               GraphicBuffer::USAGE_SW_WRITE_OFTEN);

    status_t err = graphicBuffer->initCheck();
    if (err != NO_ERROR)
    {
        LOGI("Error: %sn", strerror(-err));
        return 0;
    }

    GGLSurface t;
    //   graphicBuffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN, &addr);
    graphicBuffer->lock( GraphicBuffer::USAGE_SW_WRITE_OFTEN, &t);

    memset(t.data,128,t.stride*t.height);
    graphicBuffer->unlock();


    // Retrieve andorid native buffer
    ANativeWindowBuffer* anb =graphicBuffer->getNativeBuffer();
    // create the new EGLImageKHR
    const EGLint attrs[] =
            {
                    EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
                    EGL_NONE, EGL_NONE
            };

    mEngine.mTexture.pEGLImage = _eglCreateImageKHR(eglGetCurrentDisplay(),
                                                    mEngine.nContext, EGL_NATIVE_BUFFER_ANDROID, (EGLClientBuffer)anb, attrs);
    if(mEngine.mTexture.pEGLImage == EGL_NO_IMAGE_KHR)
    {
        LOGI("Error: eglCreateImage() failed at %s:%in", __FILE__, __LINE__);
        return 0;
    }
    checkGlError("eglCreateImageKHR");
    LOGI("create Program.......");
    GLuint gProgram = createProgram(vertex_source, fragment_source);
    checkGlError("createProgram");
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glUseProgram(gProgram);
    vPosition = glGetAttribLocation(gProgram, "glVertex");
    checkGlError("glGetAttribLocation glVertex");
    glEnableVertexAttribArray(vPosition);

    a_texCoord0 = glGetAttribLocation(gProgram, "a_texCoord0");
    checkGlError("glGetAttribLocation texCoord0");
    glEnableVertexAttribArray(a_texCoord0);

    s_tex0 = glGetUniformLocation(gProgram, "s_texture0");
    checkGlError("glGetAttribLocation texture");



    LOGI("glGen Textures.......");
    glGenTextures(1, &texID);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DITHER);
    checkGlError("glDisable");
//  glEnable(GL_TEXTURE_2D);
//  checkGlError("glEnable(GL_TEXTURE_2D)");
    glGenTextures(1,&texID);
    checkGlError("glGenTextures");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
//  glPixelStorei(GL_PACK_ALIGNMENT, 1);
//      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST  );

//       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mEngine.mTexture.nTextureWidth, mEngine.mTexture.nTextureHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, NULL);

//  glFinish();
    _glEGLImageTargetTexture2DOES(GL_TEXTURE_2D,mEngine.mTexture.pEGLImage);

    float w = float(emu_width)  / float(mEngine.mTexture.nTextureWidth);
    float h = float(emu_height) / float(mEngine.mTexture.nTextureHeight);

    g_pos=initFloatBuffer(vertices,12);
    g_texvbo=initFloatBuffer(texCoords,8);

    glViewport(0, 0,  mEngine.nScreen_Width, mEngine.nScreen_Height);
    return 1;

}

void Graphics::renderFrame()
{

//  texture=(unsigned char*)ture;
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUniform1i(s_tex0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, g_pos);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, g_texvbo);
    glVertexAttribPointer(a_texCoord0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    eglSwapBuffers(mEngine.nDisplay, mEngine.nSurface);
｝

    void* Graphics::begin(){
        GGLSurface t;
        graphicBuffer->lock(&t,GRALLOC_USAGE_SW_WRITE_OFTEN);
        return t.data;
    }
    void Graphics::end(){
        graphicBuffer->unlock();
        renderFrame();
    }


    /*
     使用时如下：
Graphics  render;

void *buffer=render.begin();
把数据填充到buffer里
render.end();

编译时包含头文件
LOCAL_C_INCLUDES +=
    $(ANDROID_SRC_HOME)/frameworks/base/core/jni/android/graphics
    $(ANDROID_SRC_HOME)/frameworks/base/include/
    $(ANDROID_SRC_HOME)/hardware/libhardware/include
    $(ANDROID_SRC_HOME)/system/core/include
    $(ANDROID_SRC_HOME)/frameworks/base/native/include/
    $(ANDROID_SRC_HOME)/frameworks/base/opengl/include/

链接选项：
LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid  -lui -landroid_runtime  -ljnigraphics

     */