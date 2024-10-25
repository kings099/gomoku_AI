#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <vector>
#include <utility>

#include <string>
#include <time.h>

//VERSION1.0:����ʵ��alphabeta��֦Ҷ�㷨��������ǰ��ҵʹ�ù���unordered_set���Ż��������̣�ԭ���ǰ������ù�ϣ����ʽ�洢�����͸��Ӷȣ�ͬʱ��������������ʣ��Ѽ�����Χ�۽�������λ�õ�
//����4�񣨱����볢��2�񣬵�Ч���ܲ��ʵ��4���Ѿ��ӽ���ȫ�����ˡ��������ù�ϣ�洢�ܻ��㡣�����Ż�����1.�Ż��洢�ķ�����Ӧ�ô��ڱ����з������õı���������2.�Ż����ֱ�׼��ע�⵽
//������ʱ���ͷ��������ս����Ӱ��ܴ󣬳����뷨����Բ�ͬ������Բ�ͬ���ͽ��ж�̬�ĸ��֣��������ú������ȹ������д������Ż���3.������̱�Ե���㷨�д��Ż�����ǰ�ų�����ȫ�����ܵ�ֵ
//����ӱ���Ĩȥ��
//VERSION1.1��1.�۲���ľ����̣������Լ����㷨����Եط�����ʱ��ѡ����ض���׷������������ӷ���ϵ��2���ڵз���3ʱ����3���ı�����2.ͬʱ���������x_xx������ʱ���ᱻ��Ϊ��2���󸳷֡�

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

// ���ͷ���
const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 100000;//��ͷ�޶µ�4��
const int SCORE_BLOCKED_FOUR = 10000;//һ���޶µ�4��
const int SCORE_THREE = 5000;//��ͷ�޶µ�3��
const int SCORE_BLOCKED_THREE = 3000;//����һ�ߵ�3��
const int SCORE_TWO = 1000;//��ͷ�޶µ�2��


const double TIME_LIMIT = 1.8;
const int MAX_DEPTH = 3;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

const int DEFENSE_MULTIPLIER = 2; // �������ȼ���Ȩ

// ���巽�����������ڼ���ĸ���������������ˮƽ����ֱ���Խ���1���Խ���2
const int DIRECTIONS[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

struct Coordinate{
    int x;
    int y;
    //Ĭ�Ϲ��캯��
    Coordinate() :x(-1), y(-1) {}
    // ���ι��캯��
    Coordinate(int row, int col) :x(row), y(col) {}
};
// �Զ����ϣ������ԭ���ǲ���std::hashΪ�����������ɹ�ϣֵ��Ȼ�󽫵ڶ���ֵ����һλ��Ϊ��������ֵ�����֣���������ֵ���õ����
struct PairHash {
    int operator()(const std::pair<int, int>& position) const {
        return std::hash<int>()(position.first) ^ (std::hash<int>()(position.second) << 1);
    }
};
// ����uorderedserȫ�ֱ������洢������֦��Ҫ���Ե�λ�ã��Դﵽ�������������
std::unordered_set<std::pair<int, int>, PairHash> CheckPositionSet;

struct Board {
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int state = 0;
    Board() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
    }
    void UndoMove(int x, int y) {
        board[x][y] = EMPTY;
    }
};
Board A;

// ���������Χ��Ȧ����Чλ�ü���
void AddSurroundingPositions(int row, int col) {
    for (int i = -4; i <= 4; ++i) {
        for (int j = -4; j <= 4; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && CheckPositionSet.find({ newRow, newCol }) == CheckPositionSet.end() && A.board[newRow][newCol] == EMPTY) {
                CheckPositionSet.insert({ newRow, newCol });
            }
        }
    }
    // �Ƴ���ǰ����λ��
    CheckPositionSet.erase({ row, col });
}
bool judgeInRange(Coordinate temp) { //�ж�λ���Ƿ񳬳�����
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

// ���̾�����������
// �������������ͷ�����Ϊһ����������
int calculateLineScore(int count, bool blockedStart, bool blockedEnd, bool gapPattern,int currentPlayer, int player) {
    if (count >= 5) return SCORE_FIVE;  // ����
    if (count == 4) {
        if (!blockedStart && !blockedEnd) return SCORE_FOUR;         // ����
        if (!blockedStart || !blockedEnd) return SCORE_BLOCKED_FOUR; // ����
    }
    if (count == 3) {
        if ((!blockedStart && !blockedEnd)||gapPattern) {
            // ����Ƕ��ֵĻ���������߷���Ȩ��
            return (currentPlayer != player) ? SCORE_THREE * DEFENSE_MULTIPLIER : SCORE_THREE;
        }
        if (!blockedStart || !blockedEnd) {
            return (currentPlayer != player) ? SCORE_BLOCKED_THREE : SCORE_BLOCKED_THREE;
        }
    }
    if (count == 2) {
        if (!blockedStart && !blockedEnd) return SCORE_TWO;  // ���
    }
    return 0;
}


// ���һ�������ϵ��������������ͷ�����
int countConsecutive(int player, int row, int col, int dRow, int dCol, bool& blockedStart, bool& blockedEnd, bool& gapPattern) {
    int count = 1;  // ��ǰ���Ӽ�������
    blockedStart = blockedEnd = gapPattern = false;

    // ��һ������ͳ������
    int r = row + dRow, c = col + dCol;
    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && A.board[r][c] == player) {
        count++;
        r += dRow;
        c += dCol;
    }
    if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
        if (A.board[r][c] == EMPTY) {
            // ����Ƿ���� XX_X ��ʽ�Ļ���
            int r2 = r + dRow, c2 = c + dCol;
            if (r2 >= 0 && r2 < BOARD_SIZE && c2 >= 0 && c2 < BOARD_SIZE && A.board[r2][c2] == player) {
                count++;        // �������ʽ�ĵ���������
                gapPattern = true; // XX_X��ʽ�Ļ�3
            }
        }
        else {
            blockedEnd = true;
        }
    }
    // ������ͳ��
    r = row - dRow;
    c = col - dCol;
    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && A.board[r][c] == player) {
        count++;
        r -= dRow;
        c -= dCol;
    }
    if (A.board[r][c] == EMPTY) {
        int r2 = r - dRow, c2 = c - dCol;
        if (r2 >= 0 && r2 < BOARD_SIZE && c2 >= 0 && c2 < BOARD_SIZE && A.board[r2][c2] == player) {
            count++;
            gapPattern = true;// ��� X_XX ��ʽ�Ļ�3
        }
    }
    else {
        blockedStart = true;
    }

    return count;
}

// �����������̾���
int evaluateBoard(int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE :BLACK;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (A.board[row][col] == player || A.board[row][col] == opponent) {
                int currentPlayer = A.board[row][col];
                int scoreMultiplier = (currentPlayer == player) ? 1 : -1;

                // ��ÿ�������������
                for (int dir = 0; dir < 4; ++dir) {
                    bool blockedStart = false, blockedEnd = false, gapPattern = false;
                    int count = countConsecutive(currentPlayer, row, col, DIRECTIONS[dir][0], DIRECTIONS[dir][1], blockedStart, blockedEnd, gapPattern);
                    totalScore += scoreMultiplier * calculateLineScore(count, blockedStart, blockedEnd, gapPattern,currentPlayer, myFlag);
                }
            }
        }
    }

    return totalScore;
}

int AlphaBeta(int depth, int alpha, int beta, bool maximizingPlayer, int player, Coordinate& bestMove, clock_t startTime) {
    // �ж��Ƿ񳬹�ʱ������
    if ((clock() - startTime) / (double)CLOCKS_PER_SEC >= TIME_LIMIT) {
        return evaluateBoard(player);  // ��ʱ���ص�ǰ����ֵ
    }

    if (depth == 0) {
        return evaluateBoard(player);  // ����ݹ���ȷ�������ֵ
    }

    Coordinate currentBestMove;
    if (maximizingPlayer) {
        int maxEval = _INF;
        for (auto it = CheckPositionSet.begin(); it != CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta( depth - 1, alpha, beta, false, 3 - player, currentBestMove, startTime);

                if (eval > maxEval) {
                    maxEval = eval;
                    bestMove = Coordinate(row, col);  // �����������
                }

                alpha = std::max(alpha, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Beta ��֦
            }
        }
        return maxEval;
    }
    else {
        int minEval = INF;
        for (auto it = CheckPositionSet.begin(); it != CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta( depth - 1, alpha, beta, true, 3 - player, currentBestMove, startTime);

                if (eval < minEval) {
                    minEval = eval;
                    bestMove = Coordinate(row, col);  // �����������
                }

                beta = std::min(beta, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Alpha ��֦
            }
        }
        return minEval;
    }
}


void loop()
{
    char tag[10] = { 0 };
    int status;
    Coordinate command;
    while (1) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0) {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            printf("OK\n");
            fflush(stdout);
        }
        else
            if (strcmp(tag, PLACE) == 0) {
                scanf("%d %d", &command.x, &command.y);
                A.board[command.x][command.y] = enemyFlag;
                AddSurroundingPositions(command.x, command.y);
            }
            else if (strcmp(tag, TURN) == 0) {
                Coordinate BestMove;
                clock_t startTime = clock();
                AlphaBeta(MAX_DEPTH, _INF, INF, false, myFlag, BestMove, startTime);
                A.board[BestMove.x][BestMove.y] = myFlag;
                AddSurroundingPositions(BestMove.x, BestMove.y);
                printf("%d %d\n", BestMove.x, BestMove.y);
                fflush(stdout);
            }

            else if (strcmp(tag, END) == 0) {
                scanf("%d", &status);
                exit(0);
            }

    }
}
int main()
{
    AddSurroundingPositions(BOARD_MIDDLE_1, BOARD_MIDDLE_1);
    AddSurroundingPositions(BOARD_MIDDLE_2, BOARD_MIDDLE_2);
    AddSurroundingPositions(BOARD_MIDDLE_1, BOARD_MIDDLE_2);
    AddSurroundingPositions(BOARD_MIDDLE_2, BOARD_MIDDLE_1);
    loop();

    return 0;
}