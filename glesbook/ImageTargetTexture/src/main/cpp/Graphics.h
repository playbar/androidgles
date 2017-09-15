//
// Created by houguoli on 2017/9/15.
//

#ifndef GLESBOOK_GRAPHICS_H
#define GLESBOOK_GRAPHICS_H

#include <EGL/eglext.h>
#include <GLES2/gl2ext.h>

class Graphics
{
public:
    int initGL();

private:
    GraphicBuffer *graphicBuffer;
    PFNEGLCREATEIMAGEKHRPROC _eglCreateImageKHR;
    PFNEGLDESTROYIMAGEKHRPROC _eglDestroyImageKHR;
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC _glEGLImageTargetTexture2DOES;
};

#endif //GLESBOOK_GRAPHICS_H
