// Ver https://en.wikipedia.org/wiki/Q-learning

#define numEstados 32
#define numAcoes 4

// Numeros de pinos a serem ajustados:
const int pinoIvrm_1 = 2;
const int pinoIvrm_2 = 4;
const int pinoIvrm_3 = 7;
const int pino_mtr_1 = 9;
const int pino_mtr_2 = 10;
int estado = 0, estadoAnterior, estIvrm_1, estIvrm_2, estIvrm_3, mtr_1 = 0,
  mtr_2 = 0;
// Essas velocidades de PWM devem ser ajustadas:
int velocidade_0 = 44;
int velocidade_1 = 88;
float alfa = 1.0; // Valor inicial, a ser ajustado aqui.
const float alfaFinal = 0.5; // Valor final, a ser ajustado aqui.
const float fatorQuedaAlfa = 0.9; // A ser ajustado.
float gama = 0.5; // Valor inicial, a ser ajustado aqui.
const float gamaFinal = 0.8; // Valor final, a ser ajustado aqui.
const float fatorAumentoGama = 1.1; // A ser ajustado.
float alfaTmp, gamaTmp;
bool alfaMudando, gamaMudando;
float Q[ numEstados ][ numAcoes ];
float recompensa[ numEstados ];
int quantoEsperaParaSortearAcao = 33;
int contaParaSorteiosAcao = 1;
int acao = 0;

void setup() {
  pinMode( pinoIvrm_1, INPUT );
  pinMode( pinoIvrm_2, INPUT );
  pinMode( pinoIvrm_3, INPUT );
  pinMode( pino_mtr_1, OUTPUT );
  pinMode( pino_mtr_2, OUTPUT );
  // Iniciação de Q:
  for( int indEstado = 0; indEstado < numEstados; indEstado++ ) {
    for( int indAcao = 0; indAcao < numAcoes; indAcao++ ) {
      Q[ indEstado ][ indAcao ] = 0.0;
    }
  }
  // É medido o estado inicial do conjunto de sensores:
  if( digitalRead( pinoIvrm_1 ) == LOW ) estIvrm_1 = 0;
  else                                   estIvrm_1 = 1;
  if( digitalRead( pinoIvrm_2 ) == LOW ) estIvrm_2 = 0;
  else                                   estIvrm_2 = 1;
  if( digitalRead( pinoIvrm_3 ) == LOW ) estIvrm_3 = 0;
  else                                   estIvrm_3 = 1;
  mtr_1 = mtr_2 = 0;
  analogWrite( pino_mtr_1, velocidade_0 );
  analogWrite( pino_mtr_2, velocidade_0 );
  estadoAnterior = 2 * ( 2 * ( 2 * ( 2 * estIvrm_1 + estIvrm_2 ) + estIvrm_3 )
			 + mtr_1 ) + mtr_2;
  // O valor da recompensa depende do estado resultante da ação.
  // A seguir, o valor do índice é o número do estado resultante.
  recompensa[ 0 ] = -6;
  recompensa[ 1 ] = -5;
  recompensa[ 2 ] = -5;
  recompensa[ 3 ] = -8;
  recompensa[ 4 ] = -8;
  recompensa[ 5 ] = -10;
  recompensa[ 6 ] = 8;
  recompensa[ 7 ] = -8;
  recompensa[ 8 ] = 9;
  recompensa[ 9 ] = -5;
  recompensa[ 10 ] = -5;
  recompensa[ 11 ] = 10;
  recompensa[ 12 ] = 0;
  recompensa[ 13 ] = -8;
  recompensa[ 14 ] = -2;
  recompensa[ 15 ] = -5;
  recompensa[ 16 ] = -8;
  recompensa[ 17 ] = 8;
  recompensa[ 18 ] = -10;
  recompensa[ 19 ] = -8;
  recompensa[ 20 ] = 0;
  recompensa[ 21 ] = 2;
  recompensa[ 22 ] = 2;
  recompensa[ 23 ] = -5;
  recompensa[ 24 ] = 0;
  recompensa[ 25 ] = -2;
  recompensa[ 26 ] = -8;
  recompensa[ 27 ] = -5;
  recompensa[ 28 ] = -8;
  recompensa[ 29 ] = 3;
  recompensa[ 30 ] = 3;
  recompensa[ 31 ] = -10;
  if( alfa > alfaFinal ) alfaMudando = true;
  else                   alfaMudando = false;
  if( gama < gamaFinal ) gamaMudando = true;
  else                   gamaMudando = false;
}

void loop() {
  // Escolhe a ação sobre motores.
  // Caso em que sorteia:
  if( contaParaSorteiosAcao == quantoEsperaParaSortearAcao ) {
    acao = random( 0, numAcoes );
    contaParaSorteiosAcao = 1;
  }
  // Caso em que escolhe a melhor na tabela Q:
  else {
    float maxQNoEstado = Q[ estadoAnterior ][ 0 ];
    int indAcaoMax = 0;
    for( int indAcao = 1; indAcao < numAcoes; indAcao++ ) {
      if( Q[ estadoAnterior ][ indAcao ] > maxQNoEstado ) {
	maxQNoEstado = Q[ estadoAnterior ][ indAcao ];
	indAcaoMax = indAcao;
      }
    }
    acao = indAcaoMax;
    contaParaSorteiosAcao++;
  }
  // Traduz o número da ação para ações sobre motores:
  if( acao > 1 ) mtr_1 = 1;
  else           mtr_1 = 0;
  if( acao % 2 ) mtr_2 = 1;
  else           mtr_2 = 0;
  if( mtr_1 == 0 ) analogWrite( pino_mtr_1, velocidade_0 );
  else             analogWrite( pino_mtr_1, velocidade_1 );
  if( mtr_2 == 0 ) analogWrite( pino_mtr_2, velocidade_0 );
  else             analogWrite( pino_mtr_2, velocidade_1 );
  estIvrm_1 = digitalRead( pinoIvrm_1 );
  estIvrm_2 = digitalRead( pinoIvrm_2 );
  estIvrm_3 = digitalRead( pinoIvrm_3 );
  int estadoNovo = 2 * ( 2 * ( 2 * ( 2 * estIvrm_1 + estIvrm_2 ) + estIvrm_3 )
			 + mtr_1 ) + mtr_2;
  float maxQnovo = Q[ estadoNovo ][ 0 ];
  for( int indAcao = 1; indAcao < numAcoes; indAcao++ ) {
    if( Q[ estadoNovo ][ indAcao ] > maxQnovo ) {
      maxQnovo = Q[ estadoNovo ][ indAcao ];
    }
  }
  Q[ estadoAnterior ][ acao ] =
    ( 1 - alfa ) * Q[ estadoAnterior ][ acao ] +
    alfa * ( recompensa[ estadoNovo ] + gama * maxQnovo );
  estadoAnterior = estadoNovo;
  if( alfaMudando ) {
    alfaTmp = alfa * fatorQuedaAlfa;
    if( alfaTmp > alfaFinal ) {
      alfa = alfaTmp;
    }
    else {
      alfa = alfaFinal;
      alfaMudando = false;
    }
  }
  if( gamaMudando ) {
    gamaTmp = gama * fatorAumentoGama;
    if( gamaTmp < gamaFinal ) {
      gama = gamaTmp;
    }
    else {
      gama = gamaFinal;
      gamaMudando = false;
    }
  }
}
