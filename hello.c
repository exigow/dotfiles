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

unsigned int compile_shader(const char *code, GLuint shader_type) {
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    int shaderCompiled = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled == GL_FALSE) {
        int length;
        char *error;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        glGetShaderInfoLog(shader, length, &length, error);
        fprintf(stderr, error);
        exit(EXIT_FAILURE);
    }
    return shader;
}

struct Config {
    char *fragment_filename;
    char *vertex_filename;
    char *texture_filename;
};

struct Config parse_config(int argc, char **argv) {
    char *previous_word = *argv;
    struct Config config = { NULL, NULL, NULL };
    while (argc--) {
        char *word = *argv++;
        if (!strcmp(previous_word, "-f"))
            config.fragment_filename = word;
        if (!strcmp(previous_word, "-v"))
            config.vertex_filename = word;
        if (!strcmp(previous_word, "-t"))
            config.texture_filename = word;
        previous_word = word;
    }
    return config;
}

int main(int argc, char **argv) {
    struct Config config = parse_config(argc, argv);
    Display *display = XOpenDisplay(NULL);
	Window window = RootWindow(display, DefaultScreen(display));
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* sdl_window = SDL_CreateWindowFrom((void*) window);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GLContext sdl_gl = SDL_GL_CreateContext(sdl_window);
    SDL_GL_SetSwapInterval(1);
    SDL_Event event;
    SDL_GL_MakeCurrent(sdl_window, sdl_gl);
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* original_surface = IMG_Load(config.texture_filename);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, original_surface->w, original_surface->h, 24, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    SDL_BlitSurface(original_surface, 0, surface, 0);
    SDL_FreeSurface(original_surface);
    glewInit();
    unsigned int programId = glCreateProgram();
    unsigned int vtxShaderId = compile_shader(read_text_file(config.vertex_filename), GL_VERTEX_SHADER);
    unsigned int fragShaderId = compile_shader(read_text_file(config.fragment_filename), GL_FRAGMENT_SHADER);
    glAttachShader(programId, vtxShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
    glValidateProgram(programId);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    unsigned int last_ticks = SDL_GetTicks();
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
        glUseProgram(programId);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1f(glGetUniformLocation(programId, "iTime"), time);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(sdl_window);
        SDL_Delay(1000 / 60);
    }
    return 0;
}
