#include <jni.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#define  LOG_TAG    "offscreen"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static EGLConfig eglConf;
static EGLSurface eglSurface;
static EGLContext eglCtx;
static EGLDisplay eglDisp;


const char vertex_shader_fix[]=
"attribute vec4 a_Position;\n"
"void main() {\n"
"	gl_Position=a_Position;\n"
"}\n";

const char fragment_shader_simple[]=
"precision mediump float;\n"
"void main(){\n"
"	gl_FragColor = vec4(0.0,1.0,0.0,1.0);\n"
"}\n";

const float tableVerticesWithTriangles[] = {
		// Triangle1
		-0.5f,-0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
		// Triangle2
		-0.5f,-0.5f,
		0.5f,-0.5f,
		0.5f, 0.5f,
		};

extern "C"
{
	JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_init(JNIEnv*env,jobject obj);
	JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_draw(JNIEnv*env,jobject obj);
	JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_release(JNIEnv*env,jobject obj);
}

JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_init(JNIEnv*env,jobject obj)
{
    // EGL config attributes
    const EGLint confAttr[] =
    {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,// very important!
            EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,//EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_ALPHA_SIZE, 8,// if you need the alpha channel
            EGL_DEPTH_SIZE, 8,// if you need the depth buffer
            EGL_STENCIL_SIZE,8,
            EGL_NONE
    };
    // EGL context attributes
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,// very important!
            EGL_NONE
    };
    // surface attributes
    // the surface size is set to the input frame size
    const EGLint surfaceAttr[] = {
             EGL_WIDTH,512,
             EGL_HEIGHT,512,
             EGL_NONE
    };
    EGLint eglMajVers, eglMinVers;
    EGLint numConfigs;

    eglDisp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(eglDisp == EGL_NO_DISPLAY)
    {
    	//Unable to open connection to local windowing system
    	LOGI("Unable to open connection to local windowing system");
    }
    if(!eglInitialize(eglDisp, &eglMajVers, &eglMinVers))
    {
    	// Unable to initialize EGL. Handle and recover
    	LOGI("Unable to initialize EGL");
    }
    LOGI("EGL init with version %d.%d", eglMajVers, eglMinVers);
    // choose the first config, i.e. best config
    if(!eglChooseConfig(eglDisp, confAttr, &eglConf, 1, &numConfigs))
    {
    	LOGI("some config is wrong");
    }
    else
    {
    	LOGI("all configs is OK");
    }
    // create a pixelbuffer surface
    eglSurface = eglCreatePbufferSurface(eglDisp, eglConf, surfaceAttr);
    if(eglSurface == EGL_NO_SURFACE)
    {
    	switch(eglGetError())
    	{
    	case EGL_BAD_ALLOC:
    	// Not enough resources available. Handle and recover
    		LOGI("Not enough resources available");
    		break;
    	case EGL_BAD_CONFIG:
    	// Verify that provided EGLConfig is valid
    		LOGI("provided EGLConfig is invalid");
    		break;
    	case EGL_BAD_PARAMETER:
    	// Verify that the EGL_WIDTH and EGL_HEIGHT are
    	// non-negative values
    		LOGI("provided EGL_WIDTH and EGL_HEIGHT is invalid");
    		break;
    	case EGL_BAD_MATCH:
    	// Check window and EGLConfig attributes to determine
    	// compatibility and pbuffer-texture parameters
    		LOGI("Check window and EGLConfig attributes");
    		break;
    	}
    }
    eglCtx = eglCreateContext(eglDisp, eglConf, EGL_NO_CONTEXT, ctxAttr);
    if(eglCtx == EGL_NO_CONTEXT)
    {
    	EGLint error = eglGetError();
    	if(error == EGL_BAD_CONFIG)
    	{
    		// Handle error and recover
    		LOGI("EGL_BAD_CONFIG");
    	}
    }
    if(!eglMakeCurrent(eglDisp, eglSurface, eglSurface, eglCtx))
    {
    	LOGI("MakeCurrent failed");
    }
    LOGI("initialize success!");
}

JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_draw(JNIEnv*env,jobject obj)
{
	const char*vertex_shader=vertex_shader_fix;
	const char*fragment_shader=fragment_shader_simple;
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glViewport(0,0,512,512);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertex_shader,NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragment_shader,NULL);
	glCompileShader(fragmentShader);
	GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
	glUseProgram(program);
	GLuint aPositionLocation =glGetAttribLocation(program, "a_Position");
	glVertexAttribPointer(aPositionLocation,2,GL_FLOAT,GL_FALSE,0,tableVerticesWithTriangles);
	glEnableVertexAttribArray(aPositionLocation);
	//draw something
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES,0,6);
	eglSwapBuffers(eglDisp,eglSurface);
}

JNIEXPORT void JNICALL Java_com_handspeaker_offscreentest_MyGles_release(JNIEnv*env,jobject obj)
{
    eglMakeCurrent(eglDisp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(eglDisp, eglCtx);
    eglDestroySurface(eglDisp, eglSurface);
    eglTerminate(eglDisp);

    eglDisp = EGL_NO_DISPLAY;
    eglSurface = EGL_NO_SURFACE;
    eglCtx = EGL_NO_CONTEXT;
}



