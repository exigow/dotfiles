#include <GL/glew.h>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

typedef struct Config {
    char *fragmentFile;
    char *vertexFile;
    char *textureFile;
} Config;

static char* readFile(char *path) {
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *string = malloc(size + 1);
    fread(string, 1, size, file);
    fclose(file);
    return string;
}

static Config parseConfig(int argc, char **argv) {
    char *previous_word = *argv;
    Config config;
    while (argc--) {
        char *word = *argv++;
        if (!strcmp(previous_word, "-f"))
            config.fragmentFile = word;
        else if (!strcmp(previous_word, "-v"))
            config.vertexFile = word;
        else if (!strcmp(previous_word, "-t"))
            config.textureFile = word;
        previous_word = word;
    }
    return config;
}

static int compileShader(const char *code, int shaderType) {
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
        exit(EXIT_FAILURE);
    return shader;
}

int main(int argc, char **argv) {
    Config config = parseConfig(argc, argv);
    char *vertexText = readFile(config.vertexFile);
    char *fragmentText = readFile(config.fragmentFile);
    Display *display = XOpenDisplay(NULL);
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindowFrom((void*) RootWindow(display, DefaultScreen(display)));
    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_GL_MakeCurrent(window, SDL_GL_CreateContext(window));
    SDL_Surface* imageSurface = IMG_Load(config.textureFile);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, imageSurface->w, imageSurface->h, 24, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    SDL_BlitSurface(imageSurface, 0, surface, 0);
    glewInit();
    int program = glCreateProgram();
    int vertexShader = compileShader(vertexText, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(fragmentText, GL_FRAGMENT_SHADER);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(EXIT_SUCCESS);
            }
        }
        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1f(glGetUniformLocation(program, "iTime"), SDL_GetTicks() / 1000.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        SDL_Delay(1000 / 60);
    }
    return 0;
}
