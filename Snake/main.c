#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* for the sh*t, WIN32 */
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <tgmath.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Windows �c Linux �ĺ�ʽ�����Q��΢��ͬ��
#define Sleep(msec) usleep(msec * 1000); // ��֧Ԯ s
#define _getch() getc(stdin)

// thanks to https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int _kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

#define LENGTH 40

double timePerFrame;
int numberOfRow = 12, numberOfColumn = 12;
int pulse = 0; //���ƶ���oO

double accelerate(int length);
int keyToQuaternary(char input, int quaternaryVector, int length); //���û����������ֵ����Ϊ����ֵ
void quaternaryVectorInterpreter(int quaternaryVector, int* currentRow, int* currentColumn); //������ֵ������λ�ñ仯��Ӧ
char coreToScreen(int number, int length, int quaternaryVector); //��core�����д洢�ĺ������ݿ��ӻ�

int main() {
    /*�������������*/
    srand((unsigned)time(NULL));

    /*����Ԥ����*/
    int core[LENGTH + 2][LENGTH + 2] = { 0 };
    char screen[2 * (LENGTH + 2) + 1][2 * (LENGTH + 2) + 1] = { 0 };
    int currentRow = numberOfRow / 2 + 1, currentColumn = numberOfColumn / 2 + 1;
    int quaternaryVector = rand() % 4;
    int length = 1;
    bool judgeSeed = false;

    /*����Ԥ����*/
    for (int i = 0; i < numberOfRow + 2; i++) {
        if (i == 0 || i == numberOfRow + 1) //�����к�ĩ��ȫ����'#'
            for (int j = 0; j < numberOfColumn + 2; j++)
                core[i][j] = 666;
        else //�������еĿ�ʼ��ĩβ����'#'
            for (int j = 0; j < numberOfColumn + 2; j++)
                if (j == 0 || j == numberOfColumn + 1)
                    core[i][j] = 666;
    }

    while (true) {
        /*����û��������룬�������������ͷ��һ���ƶ���λ��*/
        if (_kbhit())
            quaternaryVector = keyToQuaternary(_getch(), quaternaryVector, length);
        quaternaryVectorInterpreter(quaternaryVector, &currentRow, &currentColumn);

        /*��̽�롱̽����һ���ƶ�λ�õ����*/
        if (core[currentRow][currentColumn] == 777) {
            ++length;
            judgeSeed = false;
        }
        else if (core[currentRow][currentColumn] != 0)
            break;

        /*���ƶ�ģ��*/
        for (int i = 1; i <= numberOfRow; i++) {
            for (int j = 1; j <= numberOfColumn; j++)
                if (core[i][j] != 0 && core[i][j] != 777) {
                    ++core[i][j];
                    if (core[i][j] > length)
                        core[i][j] = 0;
                }
        }
        core[currentRow][currentColumn] = 1;

        /*�������̰����ʳ�ѭ��ֱ��ʳ�ﲻλ��������Ϊֹ*/
        while (judgeSeed == false) {
            int tempRow = rand() % numberOfRow + 1;
            int tempColumn = rand() % numberOfColumn + 1;
            if (core[tempRow][tempColumn] == 0) {
                core[tempRow][tempColumn] = 777;
                judgeSeed = true;
            }
        }

        /*��Ҫ���������ͳһ����screen[][]*/
        for (int i = 0; i < numberOfRow + 2; i++) {
            for (int j = 0; j < numberOfColumn + 2; j++) {
                screen[i][2 * j] = coreToScreen(core[i][j], length, quaternaryVector);
                screen[i][2 * j + 1] = ' '; //ע�⣬Ϊ��������ۣ����ǵ��ַ���ռ���أ�����ÿһ��Ԫ�غ��һ���ո�
            }
            screen[i][2 * (numberOfColumn + 1) + 1] = '\n';
        }

        /*��������*/
        // multiple unsequenced modifications to 'pulse'
        pulse++;
        pulse %= 2;

        /*ͳһ���*/
#ifdef _WIN32
        system("cls");
#else
        printf("\x1b[2J");
#endif
        for (int i = 0; i < numberOfRow + 2; i++)
            printf("%s", screen[i]);
        printf("Your current length is: %d\n", length);
        timePerFrame = accelerate(length);
        Sleep(timePerFrame);
    }

    /*�ڽ�����Ϸ����ʾ��Ӯ*/
    if (length != numberOfRow * numberOfColumn) {
        screen[numberOfRow / 2][numberOfColumn - 1] = 'G';
        screen[numberOfRow / 2][numberOfColumn] = 'A';
        screen[numberOfRow / 2][numberOfColumn + 1] = 'M';
        screen[numberOfRow / 2][numberOfColumn + 2] = 'E';
        screen[numberOfRow / 2 + 1][numberOfColumn - 1] = 'O';
        screen[numberOfRow / 2 + 1][numberOfColumn] = 'V';
        screen[numberOfRow / 2 + 1][numberOfColumn + 1] = 'E';
        screen[numberOfRow / 2 + 1][numberOfColumn + 2] = 'R';
    }
    else {
        screen[numberOfRow / 2][numberOfColumn - 1] = 'Y';
        screen[numberOfRow / 2][numberOfColumn] = 'O';
        screen[numberOfRow / 2][numberOfColumn + 1] = 'U';
        screen[numberOfRow / 2 + 1][numberOfColumn - 1] = 'W';
        screen[numberOfRow / 2 + 1][numberOfColumn] = 'I';
        screen[numberOfRow / 2 + 1][numberOfColumn + 1] = 'N';
    }
#ifdef _WIN32
    system("cls");
#else
    printf("\x1b[2J");
#endif
    for (int i = 0; i < numberOfRow + 2; i++)
        printf("%s", screen[i]);
    printf("Your final length is: %d\n", length);
    Sleep(10000);
    return 0;
}

double accelerate(int length) {
    return (5.0 * exp(3.0 - 0.05 * length) + 100);
}

void quaternaryVectorInterpreter(int quaternaryVector, int* currentRow, int* currentColumn) {
    switch (quaternaryVector) {
    case 0:
        ++ * currentColumn;
        break;
    case 2:
        -- * currentColumn;
        break;
    case 1:
        -- * currentRow;
        break;
    case 3:
        ++ * currentRow;
        break;
    }
}

char coreToScreen(int number, int length, int quaternaryVector) {
    if (number == 0)
        return ' ';
    else if (number <= length)
        if (number != 1)
            return '*';
        else
            switch (quaternaryVector) { //��ͬ�������ͷ
            case 0:
                return '>';
            case 1:
                return 'A';
            case 2:
                return '<';
            case 3:
                return 'V';
            }
    else if (number == 666)
        return '#';
    else if (number == 777) {
        if (pulse == 0)
            return 'o';
        else
            return 'O';
    }
    return '?'; // warning: control may reach end of non-void function
}

int keyToQuaternary(char input, int quaternaryVector, int length) {
    int temp = quaternaryVector;
    if (input == 'D' || input == 'd')
        temp = 0;
    else if (input == 'W' || input == 'w')
        temp = 1;
    else if (input == 'A' || input == 'a')
        temp = 2;
    else if (input == 'S' || input == 's')
        temp = 3;
    if (length > 1) //���߳�����1ʱ����ͷ����ֱ�ӷ���
        if ((quaternaryVector - temp) % 2 == 0)
            return quaternaryVector;
    return temp;
}