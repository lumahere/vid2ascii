#include "./raylib/include/raylib.h"
#include "cpm.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#define PIPE_READ 0
#define PIPE_WRITE 1

typedef struct Video {
  const char *filepath;
  size_t total_frames;
  int width;
  int height;
  int pixel_fmt;
} Video;

Video LoadVideo(const char *filepath) {
  Video res = {0};
  res.filepath = filepath;

  return res;
}

void frame_extract(Video *vid) {
  int vw = 1600;
  int vh = 800;
  const char *res = "1600x800";
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
    cpm_cmd_append(&cmd, "-i", vid->filepath, "-f", "rawvideo", "-pix_fmt",
                   "gray", "-s", res, "-");
    cpm_cmd_exec(cmd);
  } else {
    FILE *pipestrm = fdopen(pipefd[PIPE_READ], "rb");
    if (!pipestrm) {
      perror("fdopen");
      exit(1);
    }
    while (fread(buff, 1, framesize, pipestrm) == framesize) {
      Image tis = {.width = vw,
                   .height = vh,
                   .data = buff,
                   .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
                   .mipmaps = 1};
      Texture2D tas = LoadTextureFromImage(tis);
    }
  }
}

int main(void) {}
