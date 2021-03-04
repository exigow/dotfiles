#include <GL/glew.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <math.h>

char* read_text_file(char *filename) {
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");
    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size] = '\0';
        if (string_size != read_size) {
            free(buffer);
            buffer = NULL;
        }
        fclose(handler);
    }
    return buffer;
}

GLuint compile_shader(const char *code, GLuint shaderType) {
    GLuint result = glCreateShader(shaderType);
    glShaderSource(result, 1, &code, NULL);
    glCompileShader(result);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv( result, GL_COMPILE_STATUS, &shaderCompiled );
    if (shaderCompiled != GL_TRUE) {
        GLint logLength;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar*)malloc(logLength);
            glGetShaderInfoLog(result, logLength, &logLength, log);
            printf("Compilation error! Message:\n%s\n", log);
            free(log);
        }
        glDeleteShader(result);
        return 0;
    }
    return result;
}

int main(int argc, char **argv) {
    Display *display = XOpenDisplay(NULL);
	const Window window = RootWindow(display, DefaultScreen(display));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	    printf("error %s\n", SDL_GetError());
	}
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("error %s\n", IMG_GetError());
    }
    SDL_Window* sdl_window = SDL_CreateWindowFrom((void*) window);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GLContext sdl_gl = SDL_GL_CreateContext(sdl_window);
    SDL_GL_SetSwapInterval(1);
    SDL_Event event;
    SDL_GL_MakeCurrent(sdl_window, sdl_gl);
    SDL_Surface* original_surface = IMG_Load("/home/exigo/image.png");
    if (original_surface == NULL) {
        printf("Unable to load image %s\n", SDL_GetError());
    }
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 1024, 1024, 24, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    if (surface == NULL) {
        printf("Unable to load image %s\n", SDL_GetError());
    }
    SDL_BlitSurface(original_surface, 0, surface, 0);
    SDL_FreeSurface(original_surface);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW error %s\n", glewGetString(err));
    }
    GLuint programId = glCreateProgram();
    GLuint vtxShaderId = compile_shader(read_text_file("/home/exigo/hello.vert"), GL_VERTEX_SHADER);
    GLuint fragShaderId = compile_shader(read_text_file("/home/exigo/hello.frag"), GL_FRAGMENT_SHADER);
    GLuint timeLoc;
    if (vtxShaderId && fragShaderId) {
        glAttachShader(programId, vtxShaderId);
        glAttachShader(programId, fragShaderId);
        glLinkProgram(programId);
        glValidateProgram(programId);
        timeLoc = glGetUniformLocation(programId, "iTime");
        if (timeLoc == -1) {
            printf("Uniform iTime not active\n");
        }
        GLint logLen;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char* log = (char*) malloc(logLen * sizeof(char));
            glGetProgramInfoLog(programId, logLen, &logLen, log);
            printf("Compilation error! Message:\n%s\n", log);
            free(log);
        }
    }
    if (vtxShaderId) {
        glDeleteShader(vtxShaderId);
    }
    if (fragShaderId) {
        glDeleteShader(fragShaderId);
    }
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    printf("Image size: %i, %i\nTexture ID: %i\n", surface->w, surface->h, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Uint32 last_ticks = SDL_GetTicks();
    float time = 0;
    while (1) {
        Uint32 ticks = SDL_GetTicks();
        float delta_time = (ticks - last_ticks) / 1000.0;
        last_ticks = ticks;
        time += delta_time;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    SDL_FreeSurface(surface);
                    SDL_DestroyRenderer(sdl_renderer);
                    SDL_DestroyWindow(sdl_window);
                    SDL_Quit();
                    return 1;
                }
            }
        }
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(programId);
        glUniform1f(timeLoc, time);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        SDL_GL_SwapWindow(sdl_window);
        SDL_Delay(1000 / 60);
    }
    return 0;
}
