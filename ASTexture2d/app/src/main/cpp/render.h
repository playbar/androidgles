/*
 * render.h
 *
 *  Created on: Aug 12, 2015
 *      Author: maxa
 */

#ifndef RENDER_H_
#define RENDER_H_

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "png.h"

struct Shader {
  const char* vertex;
  const char* fragment;
};

struct GLESEnv {
  GLuint m_program;

  GLuint m_texture_id;
  GLint m_format;
  GLint m_type;
  uint32_t m_width;
  uint32_t m_height;

  GLfloat* m_bg_vertex_buffer;
  GLfloat* m_bg_color_buffer;
  GLfloat* m_tri_vertex_buffer;
  GLushort* m_rectangle_index_buffer;
  GLfloat* m_rectangle_texCoord_buffer;
};

class Render {
public:
  Render();
  virtual ~Render();

  void setSurface(ANativeWindow* window);
  inline void setTextureFilename(const char* filename) { strcpy(m_texture_filename, filename); }
  void draw();

private:
  /** @defgroup WindowSurface Rendering surface stuff.
   * @see https://www.khronos.org/registry/egl/sdk/docs/man/html/eglIntro.xhtml
   * @{
   */
  /// Pointer to a window associated with the rendering surface.
  ANativeWindow* m_window;
  EGLDisplay m_egl_display;  //!< Connection between EGL API and native display.
  EGLSurface m_egl_surface;  //!< Native window extension with auxiliary buffers.
  EGLContext m_egl_context;  //!< Bind client API rendering to an EGL surface.
  EGLint m_width, m_height;  //!< Surface sizes.
  GLfloat m_aspect;  //!< Surface aspect ratio.
  EGLConfig m_config;  //!< Selected frame buffer configuration, best one.
  EGLint m_num_configs;  //!< Number of available frame buffer configurations.
  EGLint m_format;  //!< Frame buffer native visual type (actual configuration).
  /** @} */  // end of WindowSurface group

  GLESEnv glenv;
  char* m_texture_filename;

  void drawTriangle();
  void drawTexturedTriangle();

  void init();
  bool initConfig();
  void destroyDisplay();

  void prepareColorShaderSource(struct Shader* shader);
  void prepareTextureShaderSource(struct Shader* shader);
  GLuint prepareProgram(struct Shader* shader);

  GLuint loadShader(GLenum type, const char* shader_src);
  const uint8_t* loadImage(struct GLESEnv* glenv);
  static void callback_read_file(png_structp io, png_bytep data, png_size_t size);
};

#endif /* RENDER_H_ */
