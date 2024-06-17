#include "cpm.h"

const char *CFLAGS = "-Wall -Wextra -g -I./raylib/include";
const char *LFLAGS = "-Wl,-rpath=./raylib/lib -L./raylib/lib -lraylib -lm";

int main(int argc, char **argv) {
  CPM_REBUILD_SELF(argc, argv);
  Cmd bld = {0};
  cpm_cmd_append(&bld, "gcc");
  cpm_cmd_append(&bld, CFLAGS);
  cpm_cmd_append(&bld, LFLAGS);
  cpm_cmd_append(&bld, "main.c", "-o", "main");

  cpm_compile(bld);

  Cmd run = {0};
  cpm_cmd_append(&run, "./main");
  cpm_cmd_exec(run);
}
