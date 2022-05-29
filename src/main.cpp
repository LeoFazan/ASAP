#include <Keyboard.h> //Incluindo a biblioteca de comandos do teclado
#include <Mouse.h>    //Incluindo a biblioteca de comandos do mouse
#include <Arduino.h>

#define MC00 0
#define MC01 MOUSE_MIDDLE
#define MC02 0
#define MC10 MOUSE_LEFT
#define MC11 0
#define MC12 0
#define MC20 MOUSE_RIGHT
#define MC21 0
#define MC22 0

int horzPin = A2;  // Output do módulo horizontal do joystick 2
int vertPin = A3;  // Output do módulo vertical do joystick 2
int selPin = 8;    // Botão do joystick 2 (talvez não usado)
int horzPin2 = A1; // Output do módulo horizontal do joystick 1
int vertPin2 = A0; // Output do módulo vertical do joystick 1
int selPin2 = 9;   // Botão do joystick 2 (talvez não usado)

int vertZero, horzZero, vertZero2, horzZero2;                             // Armazena os valores iniciais de cada eixo (geralmente em torno de 512)
int vertValue, horzValue, vertValue2, horzValue2;                         // Armazena os outputs numéricos de cada eixo
int vertValueassist, horzValueassist, vertValue2assist, horzValue2assist; // Declara os assistentes para a tradução dos números dos eixos entre 410 e 530 para apenas 400
int mouseClickFlag = 0;                                                   // Auxilia no processo de clique no botão(talvez não seja usado)
int mouseClickFlag2 = 0;

int sensitivity = 250; // Quantomaior o valor = mais lento o mouse (em torno de 250)
int invertMouse = -1;  // Inversor de orientação do joystick, -1 para não inverter, 1 para inverter

// Declaração doscomandos da matriz de controle do mouse

const int mouse_com[3][3] = { // Matriz de controle do mouse
    {MC00, MC01, MC02},
    {MC10, MC11, MC12},
    {MC20, MC21, MC22}};

void setup()
{                                 // Inicialização dos pinos e variáveis
  pinMode(horzPin, INPUT);        // Torna os pinos como entradas (horizontal) (joystick 1)
  pinMode(vertPin, INPUT);        // Torna os pinos como entradas (vertical) (joystick 1)
  pinMode(selPin, INPUT);         // Torna o botão como entrada (talvez não será usado) (joystick 1)
  digitalWrite(selPin, HIGH);     // Coloca "high" na seleção do botão (joystick 1)
  delay(1000);                    // Delay para evitar erros das outputs (joystick 1)
  vertZero = analogRead(vertPin); // Armazena os valores iniciais dos joysticks, eles devem estar em posição NEUTRA quando inicializados (joystick 1)
  horzZero = analogRead(horzPin);

  pinMode(horzPin2, INPUT);         // Torna os pinos como entradas (horizontal) (joystick 1)
  pinMode(vertPin2, INPUT);         // Torna os pinos como entradas (vertical) (joystick 1)
  pinMode(selPin2, INPUT);          // Torna o botão como entrada (talvez não será usado) (joystick 1)
  digitalWrite(selPin2, HIGH);      // Coloca "high" na seleção do botão (joystick 1)
  delay(1000);                      // Delay para evitar erros das outputs (joystick 1)
  vertZero2 = analogRead(vertPin2); // Armazena os valores iniciais dos joysticks, eles devem estar em posição NEUTRA quando inicializados (joystick 1)
  horzZero2 = analogRead(horzPin2);

  Mouse.begin();    // Inicia a simulação do mouse
  Keyboard.begin(); // Inicia a simulação do teclado
}

void joystick1()
{                                             // Movimento do ponteiro do mouse
  vertValue = analogRead(vertPin) - vertZero; // Lê o deslocamento vertical
  horzValue = analogRead(horzPin) - horzZero; // Lê o deslocamento horizontal

  vertValueassist = vertValue % 400; // Traduz o valor de 420 a 530 para simplesmente 400
  vertValue = vertValue - vertValueassist;

  horzValueassist = horzValue % 400;
  horzValue = horzValue - horzValueassist;

  if (vertValue != 0)
    Mouse.move(0, (-1 * (vertValue / sensitivity)), 0); // Move o mouse no eixo y
  if (horzValue != 0)
    Mouse.move(((horzValue / sensitivity)), 0, 0); // Move o mouse no eixo x

  if ((digitalRead(selPin) == 0) && (!mouseClickFlag)) // Se o botão está pressionado (não sei se será usado)
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT); // Clica o botão esquerdo (não sei se será usado)
  }
  if ((digitalRead(selPin)) && (mouseClickFlag))
  { // Se o botão não está pressionado (não sei se será usado)
    mouseClickFlag = 0;
    Mouse.release(MOUSE_RIGHT); // Solta o botão esquerdo (não sei se será usado)
  }
}

void joystick2()
{

  {
    vertValue2 = analogRead(vertPin2) - vertZero2; // Lê o deslocamento vertical
    horzValue2 = analogRead(horzPin2) - horzZero2; // Lê o deslocamento horizontal

    vertValue2assist = vertValue2 % 400;
    vertValue2 = vertValue2 - vertValue2assist;
    if (vertValue2 == 400)
      vertValue2 = 1;
    if (vertValue2 == -400)
      vertValue2 = 2;

    horzValue2assist = horzValue2 % 400;
    horzValue2 = horzValue2 - horzValue2assist;
    if (horzValue2 == 400)
      horzValue2 = 1;
    if (horzValue2 == -400)
      horzValue2 = 2;
  }
  if (mouse_com[horzValue2][vertValue2] != 0)         // Manda os comandos do mouse
  {                                                   // Garante que haverá navegação sem atraso do joystick que controle o mouse enquanto não houver nada sendo aplicado no joystick comando
    Mouse.press(mouse_com[horzValue2][vertValue2]);   // Pressiona o comando desejado armazenado em aux
    delay(200);                                       // Delay para evitar que haja uma repetição excessiva do comando
    Mouse.release(mouse_com[horzValue2][vertValue2]); // Solta o comando (senão fica pressionado para sempre)
    delay(200);                                       // Mais um delay para evitar cliques múltiplos acidentais
  }
}

void loop()
{
  joystick1();
  joystick2();
}
