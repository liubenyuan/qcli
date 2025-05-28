/**
 * @ Author: luoqi
 * @ Create Time: 2024-08-02 03:24
 * @ Modified by: luoqi
 * @ Modified time: 2024-11-02 10:18
 * @ Description:
 */

#include "qcli/qcli.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static QCliObj cli;

#define ISARG(arg, str) (strcmp((arg), (str)) == 0)
#define QSH(...) cli.print(__VA_ARGS__)

/*
 type ? for help
 commands:
   cmd1
   cmd2 <arg>
   cmd2 test exec_str_test
 */
int main() {
  qcli_init(&cli, printf);
  char ch;
  for (;;) {
    if (system("stty raw -echo") < 0) {
      QSH(" #! system call error !\r\n");
    }
    ch = getchar();
    ch = (ch == 127) ? 8 : ch;
    if (ch != 3) {
      qcli_exec(&cli, ch);
    } else {
      if (system("stty -raw echo") < 0) {
        QSH(" #! system call error !\r\n");
      }
      QSH("\33[2K");
      QSH("\033[H\033[J");
      QSH(" \r\n#! QSH input thread closed !\r\n\r\n");
      return 0;
    }
  }
  return 0;
}
