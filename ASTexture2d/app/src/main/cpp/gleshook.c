

#include <stdint.h>
#include "gleshook.h"
#include <GLES2/gl2.h>
#include "inlineHook.h"
#include <android/log.h>
#include <string.h>
#include <EGL/egl.h>

#define LOG_TAG "mjhook"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//egl
EGLSurface (*old_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) = NULL;
EGLSurface MJ_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    LOGI("MJ_eglCreateWindowSurface");
    return old_eglCreateWindowSurface(dpy, config, win, attrib_list);
}

EGLSurface (*old_eglCreatePbufferSurface)(EGLDisplay dpy, EGLConfig config,const EGLint *attrib_list) = NULL;
EGLSurface MJ_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    LOGI("MJ_eglCreatePbufferSurface");
    return old_eglCreatePbufferSurface(dpy, config, attrib_list);
}

EGLSurface (*old_eglCreatePixmapSurface)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list) = NULL;
EGLSurface MJ_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    LOGI("MJ_eglCreatePixmapSurface");
    return old_eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
}

EGLBoolean (*old_eglMakeCurrent)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) = NULL;
EGLBoolean MJ_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    LOGI("MJ_eglMakeCurrent");
    return old_eglMakeCurrent(dpy, draw, read, ctx);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface) = NULL;
EGLBoolean MJ_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    LOGI("MJ_eglSwapBuffers");
    return old_eglSwapBuffers(dpy, surface);
}

EGLBoolean (*old_eglCopyBuffers)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target) = NULL;
EGLBoolean MJ_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    LOGI("MJ_eglCopyBuffers");
    return MJ_eglCopyBuffers(dpy, surface, target);
}

/////////////////////////////
//gles
void (*old_glShaderSource) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) = NULL;
void MJ_glShaderSource (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    LOGI("MJ_glShaderSource");
    for(int i = 0; i < count; ++i){
        int len = strlen(*string);
        FILE *pfile = fopen("/sdcard/shader.txt", "wb");
        fwrite(*string, len, 1, pfile);
        fflush(pfile);
        fclose(pfile);
//        LOGI("shader: %s", *string);
    }
    return old_glShaderSource(shader, count, string, length);
}

void  (*old_glBindBuffer) (GLenum target, GLuint buffer) = NULL;
void MJ_glBindBuffer (GLenum target, GLuint buffer)
{
    LOGI("MJ_glBindBuffer, bufferid=%d", buffer);
    return old_glBindBuffer(target, buffer);
}

void (*old_glBindFramebuffer)(GLenum target, GLuint framebuffer) = NULL;
void MJ_glBindFramebuffer (GLenum target, GLuint framebuffer)
{
    LOGI("MJ_glBindFramebuffer, framebuffer=%d", framebuffer);
    return old_glBindFramebuffer(target, framebuffer);
}



void (*old_glBindRenderbuffer)(GLenum target, GLuint renderbuffer) = NULL;
void MJ_glBindRenderbuffer (GLenum target, GLuint renderbuffer)
{
    LOGI("MJ_glBindRenderbuffer");
    return old_glBindRenderbuffer(target, renderbuffer);
}

void (*old_glBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) = NULL;
void MJ_glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    LOGI("MJ_glBindBufferRange");
    return old_glBindBufferRange(target, index, buffer, offset, size);
}

void (*old_glBindBufferBase) (GLenum target, GLuint index, GLuint buffer) = NULL;
void MJ_glBindBufferBase (GLenum target, GLuint index, GLuint buffer)
{
    LOGI("MJ_glBindBufferBase");
    return old_glBindBufferBase(target, index, buffer);
}

void (*old_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) = NULL;
void MJ_glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    LOGI("MJ_glBufferData");
    FILE *pfile = fopen("/sdcard/bufferdata.txt", "wb");
    fwrite(data, size, 1, pfile);
    fflush(pfile);
    fclose(pfile);
    return old_glBufferData(target, size, data, usage);
}

void (*old_glDisableVertexAttribArray) (GLuint index) = NULL;
void MJ_glDisableVertexAttribArray (GLuint index)
{
    LOGI("MJ_glDisableVertexAttribArray, index=%d", index);
    return old_glDisableVertexAttribArray(index);
}

void (*old_glEnableVertexAttribArray) (GLuint index) = NULL;
void MJ_glEnableVertexAttribArray (GLuint index)
{
    LOGI("MJ_glEnableVertexAttribArray, index=%d", index);
    return old_glEnableVertexAttribArray(index);
}

void (*old_glVertexAttribPointer)(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) = NULL;
void MJ_glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
    LOGI("MJ_glVertexAttribPointer, indx=%d, size=%d, type=%d, stride=%d, ptr=%d", indx, size, type, stride, ptr);
    return old_glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

void (*old_glDrawArrays)(GLenum mode, GLint first, GLsizei count) = NULL;
void MJ_glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
    LOGI("MJ_glDrawArrays");
    return old_glDrawArrays(mode, first, count);
}

void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) = NULL;
void MJ_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    LOGI("MJ_glDrawElements");
    return old_glDrawElements(mode, count, type, indices);
}

void (*old_glUseProgram) (GLuint program) = NULL;
void MJ_glUseProgram (GLuint program)
{
    LOGI("MJ_glUseProgram, programid=%d", program);
    return old_glUseProgram(program);
}

void (*old_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = NULL;
void MJ_glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    LOGI("MJ_glRenderbufferStorage");
    return old_glRenderbufferStorage(target, internalformat, width, height);
}

void (*old_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = NULL;
void MJ_glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    LOGI("MJ_glFramebufferRenderbuffer");
    return old_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void (*old_glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = NULL;
void MJ_glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    LOGI("MJ_glFramebufferTexture2D");
    return old_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void (*old_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
void MJ_glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    LOGI("MJ_glCopyTexSubImage2D");
    return old_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

int hook(uint32_t target_addr, uint32_t new_addr, uint32_t **proto_addr)
{
    if (registerInlineHook(target_addr, new_addr, proto_addr) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) target_addr) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook(uint32_t target_addr)
{
    if (inlineUnHook(target_addr) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

void hookAllFun()
{
    hook((uint32_t) eglCreateWindowSurface, (uint32_t)MJ_eglCreateWindowSurface, (uint32_t **) &old_eglCreateWindowSurface);
    hook((uint32_t) eglCreatePbufferSurface, (uint32_t)MJ_eglCreatePbufferSurface, (uint32_t **) &old_eglCreatePbufferSurface);
    hook((uint32_t) eglCreatePixmapSurface, (uint32_t)MJ_eglCreatePixmapSurface, (uint32_t **) &old_eglCreatePixmapSurface);
    hook((uint32_t) eglMakeCurrent, (uint32_t)MJ_eglMakeCurrent, (uint32_t **) &old_eglMakeCurrent);
    hook((uint32_t) eglSwapBuffers, (uint32_t)MJ_eglSwapBuffers, (uint32_t **) &old_eglSwapBuffers);
    hook((uint32_t) eglCopyBuffers, (uint32_t)MJ_eglCopyBuffers, (uint32_t **) &old_eglCopyBuffers);
    /////////////////
    hook((uint32_t) glShaderSource, (uint32_t)MJ_glShaderSource, (uint32_t **) &old_glShaderSource);
    hook((uint32_t) glBindFramebuffer, (uint32_t)MJ_glBindFramebuffer, (uint32_t **) &old_glBindFramebuffer);
    hook((uint32_t) glBindRenderbuffer, (uint32_t)MJ_glBindRenderbuffer, (uint32_t **) &old_glBindRenderbuffer);
    hook((uint32_t) glBindBuffer, (uint32_t)MJ_glBindBuffer, (uint32_t **) &old_glBindBuffer);
//    hook((uint32_t) glBindBufferRange, (uint32_t)MJ_glBindBufferRange, (uint32_t **) &old_glBindBufferRange);
//    hook((uint32_t) glBindBufferBase, (uint32_t)MJ_glBindBufferBase, (uint32_t **) &old_glBindBufferBase);
    hook((uint32_t) glBufferData, (uint32_t)MJ_glBufferData, (uint32_t **) &old_glBufferData);
    hook((uint32_t) glDisableVertexAttribArray, (uint32_t)MJ_glDisableVertexAttribArray, (uint32_t **) &old_glDisableVertexAttribArray);
    hook((uint32_t) glEnableVertexAttribArray, (uint32_t)MJ_glEnableVertexAttribArray, (uint32_t **) &old_glEnableVertexAttribArray);
    hook((uint32_t) glVertexAttribPointer, (uint32_t)MJ_glVertexAttribPointer, (uint32_t **) &old_glVertexAttribPointer);
    hook((uint32_t) glDrawArrays, (uint32_t)MJ_glDrawArrays, (uint32_t **) &old_glDrawArrays);
    hook((uint32_t) glDrawElements, (uint32_t)MJ_glDrawElements, (uint32_t **) &old_glDrawElements);
    hook((uint32_t) glUseProgram, (uint32_t)MJ_glUseProgram, (uint32_t **) &old_glUseProgram);
    hook((uint32_t) glRenderbufferStorage, (uint32_t)MJ_glRenderbufferStorage, (uint32_t **) &old_glRenderbufferStorage);
    hook((uint32_t) glFramebufferRenderbuffer, (uint32_t)MJ_glFramebufferRenderbuffer, (uint32_t **) &old_glFramebufferRenderbuffer);
    hook((uint32_t) glFramebufferTexture2D, (uint32_t)MJ_glFramebufferTexture2D, (uint32_t **) &old_glFramebufferTexture2D);
    hook((uint32_t) glCopyTexSubImage2D, (uint32_t)MJ_glCopyTexSubImage2D, (uint32_t **) &old_glCopyTexSubImage2D);

    return;
}

void unhookAllFun()
{
    inlineUnHookAll();
    unHook((uint32_t)glShaderSource);
//    unHook((uint32_t)glBindBuffer);
//    unHook((uint32_t)glBindBufferRange);
//    unHook((uint32_t)glBindBufferBase);
//    unHook((uint32_t)glBufferData);
//    unHook((uint32_t)glEnableVertexAttribArray);
//    unHook((uint32_t)glVertexAttribPointer);
}

JNIEXPORT void JNICALL Java_com_openglesbook_GLESHook_initHook(JNIEnv* env, jobject obj)
{
    LOGI("unhookAllFun begin");
//    unhookAllFun();
    LOGI("initHook begin");
    hookAllFun();
    LOGI("initHook after");
}

JNIEXPORT void JNICALL Java_com_openglesbook_GLESHook_hookTest(JNIEnv* env, jobject obj)
{

}

JNIEXPORT void JNICALL Java_com_openglesbook_GLESHook_unInitHook(JNIEnv* env, jobject obj)
{
    unhookAllFun();
}
