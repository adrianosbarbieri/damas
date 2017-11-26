#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

void setup();
void copy_matrix();
int scan_matrix();
void loop();
void print_matrix();
void threat_square(int x, int y);
void convert_to_queen();
void convert_input_matrix();
void start_game();
void print_leds();
bool check_threat_pp(int x, int y);
bool check_threat_pn(int x, int y);
bool check_threat_nn(int x, int y);
bool check_threat_np(int x, int y);

enum PECA
{
    E,
    A,
    B,
    AT,
    BT,
    AK,
    BK,
    AKT,
    BKT
};
int matrix_old[8][8] = {{A, E, A, E, E, E, B, E}, {E, A, E, E, E, B, E, B}, {A, E, A, E, E, E, B, E},
                        {E, A, E, E, E, B, E, B}, {A, E, A, E, E, E, B, E}, {E, A, E, E, E, B, E, B},
                        {A, E, A, E, E, E, B, E}, {E, A, E, E, E, B, E, B}};

int matrix[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

int input_matrix[8][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
                          {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

int led_matrix[8][8] = {{-1, 2, -1, 4, -1, 6, -1, 8},
                        {13, -1, 15, -1, 17, -1, 19, -1},
                        {-1, 23, -1, 25, -1, 27, -1, 29},
                        {33, -1, 35, -1, 37, -1, 39, -1},
                        {-1, 43, -1, 45, -1, 47, -1, 49},
                        {53, -1, 55, -1, 57, -1, 59, -1},
                        {-1, 63, -1, 65, -1, 67, -1, 69},
                        {73, -1, 75, -1, 77, -1, 79, -1}};

bool curr_turn; /* true = A, false = B */
bool last_turn; /* true = A, false = B */
const int col[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int lin[4] = {10, 11, 12, 13}; // 12, 13};
const int turn_read = 0;
const int turn_write = 1;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(80, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    curr_turn = true;
    Serial.begin(9600);
    Serial.println();
    for(int i = 0; i < 8; i++)
    {
        Serial.print("coluna: ");
        Serial.print(col[i]);
        Serial.println();
        pinMode(col[i], OUTPUT);
    }

    for(int i = 0; i < 4; i++)
    {
        Serial.print("linha: ");
        Serial.print(lin[i]);
        Serial.println();
        pinMode(lin[i], INPUT);
    }
    pinMode(turn_write, OUTPUT);
    pinMode(turn_read, INPUT);
    digitalWrite(turn_write, HIGH);
    #if defined (__AVR_ATtiny85__)
      if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();
}

void print_leds()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(led_matrix[i][j] > 0)
            {
                switch(led_matrix[i][j])
                {
                    case A:
                      pixels.setPixelColor(led_matrix[i][j], pixels.Color(0, 0, 25));
                      break;
                    case B:
                      pixels.setPixelColor(led_matrix[i][j], pixels.Color(25, 0, 0));
                      break;
                    case E:
                      pixels.setPixelColor(led_matrix[i][j], pixels.Color(25, 25, 25));
                      break;
                }
            }
        }
    }
}

void copy_matrix()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            matrix_old[i][j] = matrix[i][j];
}

void clear_input_matrix()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 4; j++)
            input_matrix[i][j] = 0;
}

int scan_matrix()
{
    digitalWrite(turn_write, HIGH);

    last_turn = curr_turn;
    curr_turn = digitalRead(turn_read) == HIGH;
    curr_turn ? Serial.println("A") : Serial.println("B");

    clear_input_matrix();
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(col[i], HIGH);
        for(int j = 0; j < 4; j++)
        {
            if(digitalRead(lin[j]) == HIGH)
                input_matrix[i][j] = 1;
            else
                input_matrix[i][j] = 0;
        }
        digitalWrite(col[i], LOW);
    }

    convert_input_matrix();
    int sumiu = -1;
    int apareceu_i = -1;
    int apareceu_j = -1;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(curr_turn == true)
            {
                if(matrix[i][j] != 0)
                {
                    if(matrix_old[i][j] == A)
                        matrix[i][j] = A;
                    else if(matrix_old[i][j] == AK)
                        matrix[i][j] = AK;
                    else if(matrix_old[i][j] == BK)
                        matrix[i][j] = BK;
                    else if(matrix_old[i][j] == B)
                        matrix[i][j] = B;
                    else if(matrix_old[i][j] == E)
                    {
                        apareceu_i = i;
                        apareceu_j = j;
                    }
                }
                else
                {
                    if(matrix_old[i][j] == A)
                    {
                        sumiu = A;
                        matrix[i][j] = E;
                    }
                    else if(matrix_old[i][j] == AK)
                    {
                        sumiu = AK;
                        matrix[i][j] = E;
                    }
                    else if(matrix_old[i][j] == B)
                        matrix[i][j] = E;
                    else if(matrix_old[i][j] == BK)
                        matrix[i][j] = E;
                    else if(matrix[i][j] == E)
                        matrix[i][j] = E;
                }
            }
            else
            {
                if(matrix[i][j] != 0)
                {
                    if(matrix_old[i][j] == A)
                        matrix[i][j] = A;
                    else if(matrix_old[i][j] == AK)
                        matrix[i][j] = AK;
                    else if(matrix_old[i][j] == B)
                        matrix[i][j] = B;
                    else if(matrix_old[i][j] == BK)
                        matrix[i][j] = BK;
                    else if(matrix_old[i][j] == E)
                    {
                        apareceu_i = i;
                        apareceu_j = j;
                    }
                }
                else
                {
                    if(matrix_old[i][j] == B)
                    {
                        sumiu = B;
                        matrix[i][j] = E;
                    }
                    if(matrix_old[i][j] == BK)
                    {
                        sumiu = BK;
                        matrix[i][j] = E;
                    }
                    if(matrix_old[i][j] == A)
                        matrix[i][j] = E;
                    if(matrix_old[i][j] == AK)
                        matrix[i][j] = E;
                    if(matrix_old[i][j] == E)
                        matrix[i][j] = E;
                }
            }
        }
    }
    if(sumiu != -1)
        matrix[apareceu_i][apareceu_j] = sumiu;
    convert_to_queen();
    if(curr_turn != last_turn)
        copy_matrix();
    print_leds();
}

void start_game()
{
    curr_turn = true;
    Serial.begin(9600);
    Serial.println();
    for(int i = 0; i < 8; i++)
    {
        Serial.print("coluna: ");
        Serial.print(col[i]);
        Serial.println();
        pinMode(col[i], OUTPUT);
    }

    for(int i = 0; i < 4; i++)
    {
        Serial.print("linha: ");
        Serial.print(lin[i]);
        Serial.println();
        pinMode(lin[i], INPUT);
    }
}

void convert_input_matrix()
{
    for(int i = 0; i < 8; i += 2)
    {
        matrix[0][i] = input_matrix[i][0];
        matrix[2][i] = input_matrix[i][1];
        matrix[4][i] = input_matrix[i][2];
        matrix[6][i] = input_matrix[i][3];
        matrix[1][i + 1] = input_matrix[i + 1][0];
        matrix[3][i + 1] = input_matrix[i + 1][1];
        matrix[5][i + 1] = input_matrix[i + 1][2];
        matrix[7][i + 1] = input_matrix[i + 1][3];
    }
}

void convert_to_queen()
{
    for(int j = 0; j < 8; j++)
    {
        if(matrix[0][j] == B)
            matrix[0][j] = BK;
        if(matrix[7][j] == A)
            matrix[0][j] == AK;
    }
}

bool check_threat_pp(int x, int y)
{
    if(x + 2 < 8 && y + 2 < 8)
    {
        if(matrix[x + 1][y + 1] == BK && matrix[x + 2][y + 2] == E)
        {
            matrix[x + 1][y + 1] = BKT;
            return true;
        }
        if(matrix[x + 1][y + 1] == B && matrix[x + 2][y + 2] == E)
        {
            matrix[x + 1][y + 1] = BT;
            return true;
        }
    }
    return false;
}

bool check_threat_pn(int x, int y)
{
    if(x + 2 < 8 && y - 2 >= 0)
    {
        if(matrix[x + 1][y - 1] == BK && matrix[x + 2][y - 2] == E)
        {
            matrix[x + 1][y - 1] = BKT;
        }
        if(matrix[x + 1][y - 1] == B && matrix[x + 2][y - 2] == E)
        {
            matrix[x + 1][y - 1] = BT;
        }
    }
}

bool check_threat_np(int x, int y)
{
    if(x - 2 >= 0 && y + 2 < 8)
    {
        if(matrix[x - 1][y + 1] == BK && matrix[x - 2][y + 2] == E)
        {
            matrix[x - 1][y + 1] = BKT;
            return true;
        }
        if(matrix[x - 1][y + 1] == B && matrix[x - 2][y + 2] == E)
        {
            matrix[x - 1][y + 1] = BT;
            return true;
        }
    }
    return false;
}

bool check_threat_nn(int x, int y)
{
    if(x - 2 >= 0 && y - 2 >= 0)
    {
        if(matrix[x - 1][y - 1] == BK && matrix[x - 2][y - 2] == E)
        {
            matrix[x - 1][y - 1] = BKT;
            return true;
        }
        if(matrix[x - 1][y - 1] == B && matrix[x - 2][y - 2] == E)
        {
            matrix[x - 1][y - 1] = BT;
            return true;
        }
    }
    return false;
}

void threat_square(int x, int y)
{
    if(matrix[x][y] == A && curr_turn == true)
    {
        check_threat_pp(x, y);
        check_threat_pn(x, y);
        check_threat_np(x, y);
        check_threat_nn(x, y);
    }
    else if(matrix[x][y] == AK && curr_turn == true)
    {
        for(int i = 0; i < 8; i++)
            if(check_threat_pp(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_pn(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_nn(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_pn(x, y) == true)
                break;
    }
    else if(matrix[x][y] == B && curr_turn == false)
    {
        check_threat_pp(x, y);
        check_threat_pn(x, y);
        check_threat_np(x, y);
        check_threat_nn(x, y);
    }
    else if(matrix[x][y] == AK && curr_turn == true)
    {
        for(int i = 0; i < 8; i++)
            if(check_threat_pp(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_pn(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_nn(x, y) == true)
                break;
        for(int i = 0; i < 8; i++)
            if(check_threat_pn(x, y) == true)
                break;
    }
}

void print_matrix()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Serial.print(matrix[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println();
}


void print_input_matrix()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            Serial.print(input_matrix[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println();
}

void loop()
{
    for(int i = 0; i < 8; i++)
        digitalWrite(col[i], LOW);
    for(int i = 0; i < 4; i++)
        digitalWrite(lin[i], LOW);
    scan_matrix();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            threat_square(i, j);
    print_matrix();
    delay(500);
}
