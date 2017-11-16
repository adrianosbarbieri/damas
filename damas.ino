void setup();
void copy_matrix();
int scan_matrix();
void loop();
void print_matrix();
void threat_square(int x, int y);
void convert_to_queen();
void convert_input_matrix();
void start_game();
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
bool curr_turn; /* true = A, false = B */
bool last_turn; /* true = A, false = B */
const int lin[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int col[4] = {10, 11, 12, 13}; // 12, 13};
const int turn_read = 0;
const int turn_write = 1;

void setup()
{
    curr_turn = true;
    Serial.begin(9600);
    Serial.println();
    for(int i = 0; i < 8; i++)
    {
        Serial.print("linha: ");
        Serial.print(lin[i]);
        Serial.println();
        pinMode(lin[i], OUTPUT);
    }

    for(int i = 0; i < 4; i++)
    {
        Serial.print("coluna: ");
        Serial.print(col[i]);
        Serial.println();
        pinMode(col[i], INPUT);
    }
    pinMode(turn_write, OUTPUT);
    pinMode(turn_read, INPUT);
    digitalWrite(turn_write, HIGH);
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
        digitalWrite(lin[i], HIGH);
        for(int j = 0; j < 4; j++)
        {
            if(digitalRead(col[j]) == HIGH)
                input_matrix[i][j] = 1;
            else
                input_matrix[i][j] = 0;
        }
        digitalWrite(lin[i], LOW);
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
}

void start_game()
{
    curr_turn = true;
    Serial.begin(9600);
    Serial.println();
    for(int i = 0; i < 8; i++)
    {
        Serial.print("linha: ");
        Serial.print(lin[i]);
        Serial.println();
        pinMode(lin[i], OUTPUT);
    }

    for(int i = 0; i < 4; i++)
    {
        Serial.print("coluna: ");
        Serial.print(col[i]);
        Serial.println();
        pinMode(col[i], INPUT);
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
            return true;
            matrix[x + 1][y + 1] = BKT;
        }
        if(matrix[x + 1][y + 1] == B && matrix[x + 2][y + 2] == E)
        {
            return true;
            matrix[x + 1][y + 1] = BT;
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
        digitalWrite(lin[i], LOW);
    for(int i = 0; i < 4; i++)
        digitalWrite(col[i], LOW);
    scan_matrix();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            threat_square(i, j);
    print_matrix();
    delay(500);
}
