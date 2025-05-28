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

static QCliCmd _cmd1, _cmd2, _cmd3;

int _cmd1_cb(int argc, char **argv) {
  QSH("cmd1 called\r\n");
  return 0;
}

int _cmd2_cb(int argc, char **argv) {
  QSH("cmd2 %s\r\n", argv[1]);
  return 0;
}

int _cmd3_cb(int argc, char **argv) {
  if (argc < 3) {
    QSH("cmd3 requires two arguments\r\n");
    return 1;
  }
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  QSH("cmd3: %d + %d = %d\r\n", a, b, a + b);
  return 0;
}

int main() {
  qcli_init(&cli, printf);

  qcli_add(&cli, &_cmd1, "cmd1", _cmd1_cb, "cmd1");
  qcli_add(&cli, &_cmd2, "cmd2", _cmd2_cb, "echo: cmd2 <arg>");
  qcli_add(&cli, &_cmd3, "cmd3", _cmd3_cb, "adder: cmd3 <arg1> <arg2>");
  int ret;
  ret = qcli_exec_str(&cli, "cmd1");
  QSH("cmd1_exec: %d\r\n", ret);
  ret = qcli_exec_str(&cli, "cmd2 ddd");
  QSH("cmd2_exec1: %d\r\n", ret);
  ret = qcli_exec_str(&cli, "cmd3 2 10");
  QSH("cmd2_exec2: %d\r\n", ret);

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
