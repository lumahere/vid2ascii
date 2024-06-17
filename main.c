#include "./raylib/include/raylib.h"
#include "cpm.h"
#include <stdio.h>
#include <unistd.h>
#define PIPE_READ 0
#define PIPE_WRITE 1

int main(void) {
  int vw = 1600;
  int vh = 800;
  int framesize = vw * vh;
  unsigned char *buff = malloc(framesize);

  int pipefd[2];
  pipe(pipefd);
  int pid = fork();
  if (pid == 0) {
    dup2(pipefd[PIPE_WRITE], STDOUT_FILENO);
    close(pipefd[PIPE_WRITE]);
    Cmd cmd = {0};
    cpm_cmd_append(&cmd, "ffmpeg");
    cpm_cmd_append(&cmd, "-i", "lagtrain.mp4", "-f", "rawvideo", "-pix_fmt",
                   "gray", "-s", "1600x800", "-");
    cpm_cmd_exec(cmd);
  } else {
    FILE *pipestrm = fdopen(pipefd[PIPE_READ], "rb");
    if (!pipestrm) {
      perror("fdopen");
      return 1;
    }
    InitWindow(vw, vh, "til");
    SetTargetFPS(24);
    while (fread(buff, 1, framesize, pipestrm) == framesize &&
           !WindowShouldClose()) {
      BeginDrawing();
      Image tis = {.width = vw,
                   .height = vh,
                   .data = buff,
                   .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
                   .mipmaps = 1};
      Texture2D tas = LoadTextureFromImage(tis);
      DrawTexture(tas, 0, 0, WHITE);
      EndDrawing();
    }
    CloseWindow();
  }
}
