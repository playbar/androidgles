#include <cstdlib>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include "sample/EGLConfigChooser.h"
#include "logger.h"
#include "render.h"

Render::Render() {
  m_texture_filename = new char[256];
}

Render::~Render() {
  free(glenv.m_bg_vertex_buffer);
  free(glenv.m_rectangle_texCoord_buffer);
  glDeleteProgram(glenv.m_program);
  delete [] m_texture_filename;  m_texture_filename = nullptr;
}

void Render::setSurface(ANativeWindow* window) {
  m_window = window;
  initConfig();
  init();
}

void Render::draw() {
  if (m_egl_display != EGL_NO_DISPLAY) {
//    drawTriangle();
    drawTexturedTriangle();
  }
  eglSwapBuffers(m_egl_display, m_egl_surface);
}

// ----------------------------------------------------------------------------
void Render::drawTriangle() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(-4, -4, m_width + 4, m_height + 4);
  glUseProgram(glenv.m_program);

  GLint a_position = glGetAttribLocation(glenv.m_program, "a_position");
  GLint a_color = glGetAttribLocation(glenv.m_program, "a_color");
  //NSLog(@"CADisplayLink::renderColorSample() position=%i color=%i", a_position, a_color);

  glVertexAttribPointer(a_position, 4, GL_FLOAT, GL_FALSE, 0, &glenv.m_tri_vertex_buffer[0]);
  glVertexAttribPointer(a_color, 4, GL_FLOAT, GL_FALSE, 0, &glenv.m_bg_color_buffer[0]);

  glEnableVertexAttribArray(a_position);
  glEnableVertexAttribArray(a_color);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(a_position);
  glDisableVertexAttribArray(a_color);
}

void Render::drawTexturedTriangle() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(-4, -4, m_width + 4, m_height + 4);
  glUseProgram(glenv.m_program);

  GLint a_position = glGetAttribLocation(glenv.m_program, "a_position");
  GLint a_texCoord = glGetAttribLocation(glenv.m_program, "a_texCoord");

  glVertexAttribPointer(a_position, 4, GL_FLOAT, GL_FALSE, 0, &glenv.m_tri_vertex_buffer[0]);
  glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, 0, &glenv.m_rectangle_texCoord_buffer[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glenv.m_texture_id);
  GLint sampler = glGetUniformLocation(glenv.m_program, "s_texture");
  glUniform1i(sampler, 0);

  glEnableVertexAttribArray(a_position);
  glEnableVertexAttribArray(a_texCoord);

  glEnable(GL_TEXTURE_2D);
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(a_position);
  glDisableVertexAttribArray(a_texCoord);
}

void Render::init() {
  struct Shader shader;
//  prepareColorShaderSource(&shader);
  prepareTextureShaderSource(&shader);
  glenv.m_program = prepareProgram(&shader);

  /* Prepare rendering ROI and texture buffer */
  // --------------------------------------------
  glenv.m_bg_vertex_buffer = (GLfloat*) malloc(sizeof(GLfloat) * 16);
  glenv.m_bg_vertex_buffer[0] = -1.0f;
  glenv.m_bg_vertex_buffer[1] = -1.0f;
  glenv.m_bg_vertex_buffer[2] = 0.0f;
  glenv.m_bg_vertex_buffer[3] = 1.0f;

  glenv.m_bg_vertex_buffer[4] = 1.0f;
  glenv.m_bg_vertex_buffer[5] = -1.0f;
  glenv.m_bg_vertex_buffer[6] = 0.0f;
  glenv.m_bg_vertex_buffer[7] = 1.0f;

  glenv.m_bg_vertex_buffer[8] = -1.0f;
  glenv.m_bg_vertex_buffer[9] = 1.0f;
  glenv.m_bg_vertex_buffer[10] = 0.0f;
  glenv.m_bg_vertex_buffer[11] = 1.0f;

  glenv.m_bg_vertex_buffer[12] = 1.0f;
  glenv.m_bg_vertex_buffer[13] = 1.0f;
  glenv.m_bg_vertex_buffer[14] = 0.0f;
  glenv.m_bg_vertex_buffer[15] = 1.0f;


  glenv.m_bg_color_buffer = (GLfloat*) malloc(sizeof(GLfloat) * 16);
  glenv.m_bg_color_buffer[0] = 0.55f;
  glenv.m_bg_color_buffer[1] = 0.0f;  //0.67f;
  glenv.m_bg_color_buffer[2] = 0.43f;
  glenv.m_bg_color_buffer[3] = 1.0f;

  glenv.m_bg_color_buffer[4] = 0.55f;
  glenv.m_bg_color_buffer[5] = 0.0f;  //0.67f;
  glenv.m_bg_color_buffer[6] = 0.43f;
  glenv.m_bg_color_buffer[7] = 1.0f;

  glenv.m_bg_color_buffer[8] = 0.55f;
  glenv.m_bg_color_buffer[9] = 0.0f;  //0.67f;
  glenv.m_bg_color_buffer[10] = 0.43f;
  glenv.m_bg_color_buffer[11] = 1.0f;

  glenv.m_bg_color_buffer[12] = 0.55f;
  glenv.m_bg_color_buffer[13] = 0.0f;  //0.67f;
  glenv.m_bg_color_buffer[14] = 0.43f;
  glenv.m_bg_color_buffer[15] = 1.0f;


  glenv.m_tri_vertex_buffer = (GLfloat*) malloc(sizeof(GLfloat) * 12);
  glenv.m_tri_vertex_buffer[0] = 0.0f;
  glenv.m_tri_vertex_buffer[1] = 0.5f;
  glenv.m_tri_vertex_buffer[2] = 0.0f;
  glenv.m_tri_vertex_buffer[3] = 1.0f;

  glenv.m_tri_vertex_buffer[4] = -0.5f;
  glenv.m_tri_vertex_buffer[5] = -0.5f;
  glenv.m_tri_vertex_buffer[6] = 0.0f;
  glenv.m_tri_vertex_buffer[7] = 1.0f;

  glenv.m_tri_vertex_buffer[8] = 0.5f;
  glenv.m_tri_vertex_buffer[9] = -0.5f;
  glenv.m_tri_vertex_buffer[10] = 0.0f;
  glenv.m_tri_vertex_buffer[11] = 1.0f;


  glenv.m_rectangle_index_buffer = (GLushort*) malloc(sizeof(GLushort) * 6);
  glenv.m_rectangle_index_buffer[0] = 0;
  glenv.m_rectangle_index_buffer[1] = 3;
  glenv.m_rectangle_index_buffer[2] = 2;
  glenv.m_rectangle_index_buffer[3] = 0;
  glenv.m_rectangle_index_buffer[4] = 1;
  glenv.m_rectangle_index_buffer[5] = 3;


  glenv.m_rectangle_texCoord_buffer = (GLfloat*) malloc(sizeof(GLfloat) * 8);
  glenv.m_rectangle_texCoord_buffer[0] = 1.0f;
  glenv.m_rectangle_texCoord_buffer[1] = 1.0f;
  glenv.m_rectangle_texCoord_buffer[2] = 0.0f;
  glenv.m_rectangle_texCoord_buffer[3] = 1.0f;

  glenv.m_rectangle_texCoord_buffer[4] = 1.0f;
  glenv.m_rectangle_texCoord_buffer[5] = 0.0f;
  glenv.m_rectangle_texCoord_buffer[6] = 0.0f;
  glenv.m_rectangle_texCoord_buffer[7] = 0.0f;

  /* Prepare texture */
  // --------------------------------------------
  const uint8_t* image_buffer = loadImage(&glenv);
  glGenTextures(1, &glenv.m_texture_id);
  glBindTexture(GL_TEXTURE_2D, glenv.m_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, glenv.m_format, glenv.m_width, glenv.m_height, 0, glenv.m_format, glenv.m_type, image_buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

bool Render::initConfig() {
  eglBindAPI(EGL_OPENGL_ES_API);

  if ((m_egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
    ERR("eglGetDisplay() returned error %d", eglGetError());
    return false;
  }

  if (!eglInitialize(m_egl_display, 0, 0)) {
    ERR("eglInitialize() returned error %d", eglGetError());
    return false;
  }

  {
    game::EGLConfigChooser eglConfigChooser(5, 6, 5, 0, 16, 0);
    m_config = eglConfigChooser.chooseConfig(m_egl_display);
    m_num_configs = eglConfigChooser.getNumberConfigs();
    DBG("Number of EGL display configs: %i", m_num_configs);
  }

  if (!eglGetConfigAttrib(m_egl_display, m_config, EGL_NATIVE_VISUAL_ID, &m_format)) {
    ERR("eglGetConfigAttrib() returned error %d", eglGetError());
    destroyDisplay();
    return false;
  }

  if (!(m_egl_surface = eglCreateWindowSurface(m_egl_display, m_config, m_window, 0))) {
    ERR("eglCreateWindowSurface() returned error %d", eglGetError());
    destroyDisplay();
    return false;
  }

  {
    const EGLint attributes[] = {
          EGL_CONTEXT_CLIENT_VERSION, 3,
          EGL_NONE
      };
    if (!(m_egl_context = eglCreateContext(m_egl_display, m_config, 0, attributes))) {
      ERR("eglCreateContext() returned error %d", eglGetError());
      destroyDisplay();
      return false;
    }
  }

  if (!eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface, m_egl_context)) {
    ERR("eglMakeCurrent() returned error %d", eglGetError());
    destroyDisplay();
    return false;
  }

  if (!eglQuerySurface(m_egl_display, m_egl_surface, EGL_WIDTH, &m_width) ||
      !eglQuerySurface(m_egl_display, m_egl_surface, EGL_HEIGHT, &m_height)) {
    ERR("eglQuerySurface() returned error %d", eglGetError());
    destroyDisplay();
    return false;
  }
  m_aspect = (GLfloat) m_width / m_height;
  DBG("Surface width = %i, height = %i, aspect = %lf", m_width, m_height, m_aspect);

  /// @see http://android-developers.blogspot.kr/2013_09_01_archive.html
  ANativeWindow_setBuffersGeometry(m_window, 0, 0, m_format);
  return true;
}

void Render::destroyDisplay() {
  if (m_egl_display != EGL_NO_DISPLAY) {
    eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (m_egl_context != EGL_NO_CONTEXT) {
      eglDestroyContext(m_egl_display, m_egl_context);
      m_egl_context = EGL_NO_CONTEXT;
    }
    if (m_egl_surface != EGL_NO_SURFACE) {
      eglDestroySurface(m_egl_display, m_egl_surface);
      m_egl_surface = EGL_NO_SURFACE;
    }
    eglTerminate (m_egl_display);
    m_egl_display = EGL_NO_DISPLAY;
  }
}

void Render::prepareColorShaderSource(struct Shader* shader) {
  shader->vertex =
      "  attribute vec4 a_position;   \n"
      "  attribute vec4 a_color;      \n"
      "                               \n"
      "  varying vec4 v_color;        \n"
      "                               \n"
      "  void main() {                \n"
      "    v_color = a_color;         \n"
      "    gl_Position = a_position;  \n"
      "  }                            \n";
  shader->fragment =
      "  precision mediump float;     \n"
      "                               \n"
      "  varying vec4 v_color;        \n"
      "                               \n"
      "  void main() {                \n"
      "    gl_FragColor = v_color;    \n"
      "  }                            \n";
}

void Render::prepareTextureShaderSource(struct Shader* shader) {
  shader->vertex =
      "  attribute vec4 a_position;                                            \n"
      "  attribute vec2 a_texCoord;                                            \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    v_texCoord = a_texCoord;                                            \n"
      "    gl_Position = a_position;                                           \n"
      "  }                                                                     \n";
  shader->fragment =
      "  precision mediump float;                                              \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "  uniform sampler2D s_texture;                                          \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    gl_FragColor = texture2D(s_texture, v_texCoord);                    \n"
      "  }                                                                     \n";
}

GLuint Render::prepareProgram(struct Shader* shader) {
  //NSLog(@"CADisplayLink::prepareProgram() enter");
  GLuint vertex_shader = loadShader(GL_VERTEX_SHADER, shader->vertex);
  GLuint fragment_shader = loadShader(GL_FRAGMENT_SHADER, shader->fragment);

  GLuint m_program = glCreateProgram();
  if (m_program == 0) {
    const char* message = "Failed to create program object";
    //NSLog(@"%s", message);
    return 0;
  }

  glAttachShader(m_program, vertex_shader);
  glAttachShader(m_program, fragment_shader);

  // bind a_position to attribute with index m_vertex_location
//  glBindAttribLocation(m_program, m_vertex_location, "a_position");
//  glBindAttribLocation(m_program, m_color_location, "a_color");
//  glBindAttribLocation(m_program, m_texCoord_location, "a_texCoord");
  glLinkProgram(m_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLint linked = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint infoLen = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      char* infoLog = (char*) malloc(sizeof(char) * infoLen);
      glGetProgramInfoLog(m_program, infoLen, NULL, infoLog);
      //NSLog(@"Error linking program: %s", infoLog);
      if (strcmp(infoLog, "--From Vertex Shader:\n--From Fragment Shader:\nLink was successful.")) {
        //NSLog(@"No linker error !");
        free(infoLog);
        return 0;
      }
      free(infoLog);
    }
    glDeleteProgram(m_program);
//    throw ShaderException("Error linking program");
    return 0;
  }

  //NSLog(@"CADisplayLink::prepareProgram() exit");
  return m_program;
}

GLuint Render::loadShader(GLenum type, const char* shader_src) {
  //NSLog(@"enter ShaderHelper::loadShader()");
  //NSLog(@"GL version: %s", glGetString(GL_VERSION));
  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    //NSLog(@"Failed to create shader of type %i", type);
    return 0;
  }

  glShaderSource(shader, 1, &shader_src, NULL);  // load shader source
  glCompileShader(shader);

  GLint compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);  // get status
  if (compiled == 0) {
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      char* infoLog = (char*) malloc(sizeof(char) * infoLen);
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      //NSLog(@"Error compiling shader: %s", infoLog);
      free(infoLog);
    }
    glDeleteShader(shader);
    return 0;
  }
  //NSLog(@"exit ShaderHelper::loadShader()");
  return shader;
}

const uint8_t* Render::loadImage(struct GLESEnv* glenv) {
  //NSLog(@"CADisplayLink::loadImage() enter");
  FILE* file_descriptor = NULL;
  png_byte header[8];
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_byte* image_buffer = NULL;
  png_bytep* row_ptrs = NULL;
  png_int_32 row_size = 0;
  bool transparency = false;
  int error_code = 0;

  const char* m_filename = m_texture_filename;
  size_t header_size = sizeof(header);
  file_descriptor = fopen(m_filename, "rb");
  if (file_descriptor == NULL) {
    //NSLog(@"Failed to fopen(), errno is: %s", strerror(errno));
    error_code = 10;
    goto ERROR_PNG;
  }
  if (header_size != fread(header, 1, header_size, file_descriptor)) { error_code = 11; goto ERROR_PNG; }
  if (png_sig_cmp(header, 0, 8) != 0) { error_code = 3; goto ERROR_PNG; }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) { error_code = 4; goto ERROR_PNG; }
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) { error_code = 5; goto ERROR_PNG; }

  png_set_read_fn(png_ptr, file_descriptor, callback_read_file);
  {
    int jump_code = 0;
    if ( (jump_code = setjmp(png_jmpbuf(png_ptr))) != 0) {
      //NSLog(@"PNGTexture: setjump() returned code %i", jump_code);
      error_code = 6; goto ERROR_PNG;
    }
  }

  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  int depth, color_type;
  png_uint_32 width, height;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &color_type, NULL, NULL, NULL);
  glenv->m_width = width;
  glenv->m_height = height;

  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png_ptr);
    transparency = true;
  }

  if (depth < 8) {
    png_set_packing(png_ptr);
  } else {
    png_set_strip_16(png_ptr);
  }

  switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(png_ptr);
      glenv->m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGB:
      glenv->m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGBA:
      glenv->m_format = GL_RGBA;
      break;
    case PNG_COLOR_TYPE_GRAY:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      glenv->m_format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
      break;
    case PNG_COLOR_TYPE_GA:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      glenv->m_format = GL_LUMINANCE_ALPHA;
      break;
  }
  glenv->m_type = GL_UNSIGNED_BYTE;
  png_read_update_info(png_ptr, info_ptr);

  row_size = png_get_rowbytes(png_ptr, info_ptr);
  if (row_size <= 0) { error_code = 7; goto ERROR_PNG; }
  image_buffer = (png_byte*) malloc(sizeof(png_byte) * row_size * height);
  if (image_buffer == NULL) { error_code = 8; goto ERROR_PNG; }

  row_ptrs = (png_bytep*) malloc(sizeof(png_bytep) * height);
  if (row_ptrs == NULL) { error_code = 9; goto ERROR_PNG; }
  for (int32_t i = 0; i < height; ++i) {
    row_ptrs[height - (i + 1)] = image_buffer + i * row_size;
  }
  png_read_image(png_ptr, row_ptrs);

  fclose(file_descriptor);
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  free(row_ptrs);
  //NSLog(@"CADisplayLink::loadImage() exit");
  return image_buffer;

  ERROR_PNG:
    //NSLog(@"Error while reading PNG file: %s, code %i", m_filename, error_code);
    if (file_descriptor != NULL) { fclose(file_descriptor); }
    free(image_buffer);  image_buffer = NULL;
    free(row_ptrs);  row_ptrs = NULL;
    if (png_ptr != NULL) {
      png_infop* info_ptr_p = info_ptr != NULL ? &info_ptr : NULL;
      png_destroy_read_struct(&png_ptr, info_ptr_p, NULL);
    }
    return NULL;
}

void Render::callback_read_file(png_structp io, png_bytep data, png_size_t size) {
  FILE* file = (FILE*) png_get_io_ptr(io);
  if (size != fread(data, 1, size, file)) {
    fclose(file);
    //NSLog(@"CADisplayLink::callback_read_file() error!");
    png_error(io, "Error while reading PNG file (from callback_read_file()) !");
  }
}
