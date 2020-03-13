#include <stdio.h>
#include <tradingcore2/csv.h>

CR2BEGIN

void CSVLoader::release() {
  if (this->m_buff != NULL) {
    free(this->m_buff);
  }

  if (this->m_head != NULL) {
    free(this->m_head);
  }

  if (this->m_data != NULL) {
    free(this->m_data);
  }

  this->m_buff = NULL;
  this->m_width = 0;
  this->m_height = 0;
  this->m_head = NULL;
  this->m_data = NULL;
}

void CSVLoader::load(const char* fn) {
  this->release();

  FILE* fp;

  fp = fopen(fn, "rb");
  if (fp == NULL) {
    return;
  }

  fseek(fp, 0, SEEK_END);
  long fs = ftell(fp);

  fseek(fp, 0, SEEK_SET);

  this->m_buff = (char*)malloc(fs + 1);
  fread(this->m_buff, fs, 1, fp);
  fclose(fp);
  this->m_buff[fs] = '\0';

  int w = 0, h = 0;
  int ll = 0;
  for (int i = 0; i < fs; ++i) {
    if (this->m_buff[i] != '\r' && this->m_buff[i] != '\n') {
      ++ll;
    }

    if (h == 0 && this->m_buff[i] == ',') {
      w++;
    }

    if (this->m_buff[i] == '\r' && this->m_buff[i + 1] == '\n') {
      if (h == 0) {
        w++;
      }

      if (ll > 0) {
        h++;
      }

      i++;

      ll = 0;
    }
  }

  h--;

  this->m_width = w;
  this->m_height = h;
  this->m_head = (char**)malloc(w * sizeof(char*));
  void* data = malloc(w * h * sizeof(char*) + h * sizeof(char**));
  this->m_data = (char***)data;

  char* start = NULL;
  int x = 0, y = -1;
  for (int i = 0; i < fs; ++i) {
    if (start == NULL) {
      start = this->m_buff + i;
    }

    if (y == -1) {
      if (this->m_buff[i] == ',') {
        this->m_buff[i] = '\0';
        this->m_head[x] = start;
        start = NULL;

        ++x;
      }
    } else {
      if (this->m_buff[i] == ',') {
        this->m_buff[i] = '\0';
        this->m_data[y][x] = start;
        start = NULL;

        ++x;
      }
    }

    if (this->m_buff[i] == '\r' && this->m_buff[i + 1] == '\n') {
      if (y == -1) {
        this->m_buff[i] = '\0';
        this->m_head[x] = start;
        start = NULL;
      } else {
        this->m_buff[i] = '\0';
        this->m_data[y][x] = start;
        start = NULL;
      }

      y++;
      x = 0;

      if (y < h) {
        this->m_data[y] =
            (char**)((char*)data + h * sizeof(char**) + w * y * sizeof(char*));
      }

      i++;
    }
  }
}

CR2END