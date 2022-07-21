#include "tipo1.h"
#include "tipo2.h"

void inicializa_cabecalho_tipo1(FILE *fp);
tipo1_t le_registro_csv_tipo1(FILE *fp);
void grava_registro_tipo1(FILE *fp, tipo1_t dados);

void inicializa_cabecalho_tipo2(FILE *fp);
tipo2_t le_registro_csv_tipo2(FILE *fp);
void grava_registro_tipo2(FILE *fp, tipo2_t dados);
